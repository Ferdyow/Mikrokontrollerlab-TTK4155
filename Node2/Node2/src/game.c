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

#include "MCP2515.h"
#include "game.h"
#include "CAN.h"
#include "PI.h"
#include "motor.h"
#include "solenoid.h"
#include "servo.h"
#include "IR.h"

volatile control control_type;

volatile int timer_flag;
volatile int score_charge;
volatile int score_ms; //keeps the score
volatile int score_flag;

void GAME_timer_init(void);

#define INTERRUPT_FREQUENCY (double) 100		    // Hz
#define SCORE_TIME ((double) 100)/1000				// s
#define SCORE_MAX_CHARGE (int)(INTERRUPT_FREQUENCY * SCORE_TIME)


void print_can_message(can_message* msg) {
	if(msg->length == 0) return;
	printf("\n=== CAN MESSAGE BEGIN ===\n");
	printf("id: %d\tlength: %d\t DATA:\n", msg->id, msg->length);
	for(int i = 0; i < msg->length; i++) {
		printf("%x\t", msg->data[i]);
	}
	printf("\n=== CAN MESSAGE END ===\n");
}


//Set the timer_flag 0.1s when the timer has counted to OCR4A
ISR(TIMER4_COMPA_vect){
	score_charge++;
	
	if(score_charge >= SCORE_MAX_CHARGE){
		score_charge = 0;
		score_flag = 1;
		score_ms += 1;
	}
	
	// Set timer flag
	timer_flag = 1;
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
	set_bit(TCCR4A,COM4A1);
		
	// Use timer4
	// clear timer on compare (CTC) mode (16 bit, TOP in OCR4A) page 145 (mode 4 table 17-2)
	// clear_bit(TCCR4B, WGM43);
	set_bit(TCCR4B, WGM42);
	// clear_bit(TCCR4A, WGM41);
	// clear_bit(TCCR4A, WGM40);
		
	// resets at this value (output compare register) ~100 Hz when prescaler = 8
	OCR4A = 20000;
		
	// set prescaler 8
	clear_bit(TCCR4B, CS42);
	set_bit(TCCR4B, CS41);
	clear_bit(TCCR4B, CS40);
		
	// output compare A match interrupt enable (17.11.35)
	set_bit(TIMSK4, OCIE4A);
		
	score_flag = 0;
	score_ms = 0;
	score_charge = 0;
}


void GAME_loop(void) {
	can_message control_inputs;
	int8_t velocity_reference = 0;
	uint8_t slider_left = 0;
	uint8_t slider_right = 0;
	uint8_t buttons = 0;
	while(1) {
		do {
			CAN_message_receive(&control_inputs);
			print_can_message(&control_inputs);
		} while(control_inputs.id != 's');
		
		printf("yolo\n");
			
		score_ms = 0;
		while(control_inputs.id != 'q'){	
			printf("1\n");
			//control_inputs.id = 0;  // Error/empty or something else starting with e
			CAN_message_receive(&control_inputs);
			if (control_inputs.id == 0) {
				// No message
			} else if (control_inputs.id == 'c') {
				velocity_reference = control_inputs.data[JOYSTICK_X];
				slider_left = control_inputs.data[SLIDER_LEFT];
				slider_right = control_inputs.data[SLIDER_RIGHT];
				buttons = control_inputs.data[BUTTONS];	
			} else {
				printf("SOMETHING WENT SERIOUSLY WRONG WITH THE CAN BUS\n");
			}
		
			servo_set(slider_right);
		
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
}