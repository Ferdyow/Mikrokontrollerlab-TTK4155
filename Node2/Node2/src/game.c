/*
 * game.c
 *
 * Created: 19.11.2017 21:58:02
 *  Author: tobib
 */ 
#include "defines.h"
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "game.h"
#include "CAN.h"
#include "PI.h"
#include "motor.h"
#include "solenoid.h"
#include "servo.h"
#include "IR.h"

volatile control control_type;

volatile int timer_flag;
volatile int score_ms; //keeps the score
volatile int score_flag;

void GAME_timer_init(void);


// TIMER2 = GAME
// TIMER4 = SCORE

#define GAME_FREQUENCY (double) 100		        // Hz
#define TIMER0_PRESCALER 1024

#define SCORE_FREQUENCY (double) 10 			// Hz
#define TIMER4_PRESCALER 64



//Set the timer_flag 0.1s when the timer has counted to OCR4A
ISR(TIMER0_COMPA_vect) {  // Game timer interrupt
	timer_flag++;
}

ISR(TIMER4_COMPA_vect) {  // Score timer interrupt
	score_flag++;
	score_ms++;
}


void GAME_init(control c) {
	control_type = c;
	GAME_timer_init();
}


void GAME_send_score(int score_ms) {
	can_message highscore;
	highscore.id = 'h';
	highscore.length = 3;
	highscore.data[0] = score_ms >> 8;
	highscore.data[1] = score_ms & 0xFF;
	CAN_message_send(&highscore);
}


void GAME_timer_init(void) {
	// 16-bit interrupt
	set_bit(TCCR4A, COM4A1);
	set_bit(TCCR0A, COM0A1);
		
	// Use timer 1 for score, 4 for game timer
	// Clear timer on compare (CTC) mode (16 bit, TOP in OCR4A) page 145 (mode 4 table 17-2)
	set_bit(TCCR4B, WGM42);
	set_bit(TCCR0B, WGM02);
		
	// Resets at this value (output compare register) ~100 Hz when prescaler = 8
	OCR4A = (int)(F_CPU/(TIMER4_PRESCALER * SCORE_FREQUENCY));
	OCR0A = (int)(F_CPU/(TIMER0_PRESCALER * GAME_FREQUENCY));
		
	// Set prescaler 1024 (timer 2)
	set_bit(TCCR0B, CS02);
	set_bit(TCCR0B, CS00);
	
	// Set prescaler 64   (timer 4)
	set_bit(TCCR4B, CS41);
	set_bit(TCCR4B, CS40);
		
	// output compare A match interrupt enable (17.11.35)
	set_bit(TIMSK4, OCIE4A);
	set_bit(TIMSK0, OCIE0A);
	
	////////////////////////////////////
	
	score_flag = 0;
	timer_flag = 0;
}


void GAME_loop(void) {
	can_message control_inputs;
	int8_t velocity_reference = 0;
	uint8_t slider_left = 0;
	uint8_t slider_right = 0;
	uint8_t buttons = 0;
	while(1) {	
		control_inputs.id = 'e';  // Error/empty or something else starting with e
		
		CAN_message_receive(&control_inputs);
		if (control_inputs.id == 'e') {
			// No message
		} else if (control_inputs.id == 'c') {
			//printf("test\n");
			velocity_reference = control_inputs.data[JOYSTICK_X];
			slider_left = control_inputs.data[SLIDER_LEFT];
			slider_right = control_inputs.data[SLIDER_RIGHT];
			buttons = control_inputs.data[BUTTONS];	
			//servo_set(slider_right);
		} else if (control_inputs.id != 0) {
			printf("SOMETHING WENT SERIOUSLY WRONG WITH THE CAN BUS [%d]\n", control_inputs.id);
		}		
		
		if (buttons & (1 << JOY_BUTTON)) {
			solenoid_send_pulse();
		}
		
		if(timer_flag) {
			timer_flag = 0;
			switch (control_type) {
				case OPEN_LOOP:
				motor_set_velocity(velocity_reference);
				break;
				
				case CLOSED_LOOP:
				PI_control(velocity_reference);
			}
		}
		
		if(score_flag){
			score_flag = 0;
			GAME_send_score(score_ms);
			if(IR_disrupted()){
				score_ms = 0;
			}
		}
	}
}