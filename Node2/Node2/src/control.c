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
#include "CAN.h"
#include "PI.h"
#include "motor.h"
#include "control.h"
#include "solenoid.h"
#include "servo.h"


volatile int timer_flag;
volatile int solenoid_charge;

#define INTERRUPT_FREQUENCY ((double) 200)/3  // Hz
#define SOLENOID_CHARGE_TIME ((double) 1000)/1000  // s

#define SOLENOID_MAX_CHARGE (int)(INTERRUPT_FREQUENCY * SOLENOID_CHARGE_TIME)

// Set the timer_flag when the timer has counted to OCR0A
ISR(TIMER0_COMPA_vect){
	// Increment solenoid charge
	solenoid_charge = MIN(solenoid_charge + 1, SOLENOID_MAX_CHARGE);
	
	// Set timer flag
	timer_flag = 1;
}



// Initializes timer
void CONTROL_init(void) {
	// CTC mode (count until you reach a target clock count, then the interrupt is activated) [Table 16-8]
	set_bit(TCCR0A, WGM01);
		
	// Prescaler 1024 [Table 16-9]
	set_bit(TCCR0B, CS00);
	set_bit(TCCR0B, CS02);
		
	// 66.67 Hz timer, assuming 16MHz clock.
	OCR0A = 234;
		
	// Make sure the counter resets upon reaching its target (OCR0A). Enables interrupts [16.9.6]
	set_bit(TIMSK0, OCIE0A);
}


void CONTROL_run(control control_type) {
	while(1) {
		can_message control_inputs = receive_control_inputs();
		int8_t velocity_reference = control_inputs.data[JOYSTICK_X];
		uint8_t slider_left = control_inputs.data[SLIDER_LEFT];
		uint8_t slider_right = control_inputs.data[SLIDER_RIGHT];
		
		uint8_t buttons = control_inputs.data[BUTTONS];
		if ((solenoid_charge == SOLENOID_MAX_CHARGE) && (buttons & (1 << JOY_BUTTON))){
			solenoid_send_pulse();
			solenoid_charge = 0;
		}
		
		servo_set(slider_left);
		
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