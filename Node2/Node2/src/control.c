/*
 * control.c
 *
 * Created: 15.11.2017 08:29:13
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
#include "control.h"
#include "solenoid.h"
#include "servo.h"
#include "IR.h"


volatile int timer_flag;
volatile int solenoid_charge;
volatile int score_charge;
volatile int score_ms; //keeps the score
volatile int score_flag;

#define INTERRUPT_FREQUENCY (double) 100		    // Hz
#define SCORE_TIME ((double) 100)/1000				// s
#define SCORE_MAX_CHARGE (int)(INTERRUPT_FREQUENCY * SCORE_TIME)


//Set the timer_flag 0.1s when the timer has counted to OCR4A
ISR(TIMER4_COMPA_vect){
	// Increment solenoid charge
	score_charge++;
	
	if(score_charge == SCORE_MAX_CHARGE){
		score_charge = 0;
		score_flag = 1;
		score_ms += 1;
	}
	
	// Set timer flag
	timer_flag = 1;
}

// Initializes timer
void CONTROL_init(void) {	
	//16-bit interrupt
	set_bit(TCCR4A,COM4A1);
	
	//Use timer4
	//clear timer on compare (CTC) mode (16 bit, TOP in OCR4A) page 145 (mode 4 table 17-2)
	//clear_bit(TCCR4B, WGM43);
	set_bit(TCCR4B, WGM42);
	//clear_bit(TCCR4A, WGM41);
	//clear_bit(TCCR4A, WGM40);
	 
	//resets at this value (output compare register) ~100 Hz when prescaler = 8
	OCR4A = 20000;
	 
	//set prescaler 8
	clear_bit(TCCR4B, CS42);
	set_bit(TCCR4B, CS41);
	clear_bit(TCCR4B, CS40);
	 
	//output compare A match interrupt enable (17.11.35)
	set_bit(TIMSK4, OCIE4A);
	
	score_flag = 0;
	score_ms = 0;
	score_charge = 0;
}

void CONTROL_run(control control_type) {
	//printf("Inside CONTROL_run()\n");
	while(1) {
		can_message control_inputs = receive_control_inputs();
		int8_t velocity_reference = control_inputs.data[JOYSTICK_X];
		uint8_t slider_left = control_inputs.data[SLIDER_LEFT];
		uint8_t slider_right = control_inputs.data[SLIDER_RIGHT];
		uint8_t buttons = control_inputs.data[BUTTONS];
		
		if (buttons & (1 << JOY_BUTTON)){
			solenoid_send_pulse();
		}
		
		servo_set(slider_right);
		
		if(score_flag){
			score_flag = 0;
			GAME_send_highscore(score_ms);
			if(IR_disrupted()){
				score_ms = 0;
			}
		}
		
		if (timer_flag) {
			timer_flag = 0;
			switch (control_type) {
				case OPEN_LOOP: 
				motor_set_velocity(velocity_reference); 
				break;
				
				case CLOSED_LOOP: 
				PI_control(velocity_reference);
			}
		}
	}
}