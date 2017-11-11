/*
 * PI.c
 *
 * Created: 11.11.2017 15:34:11
 *  Author: ferdyow
 */ 
#include "defines.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "MAX520.h"
#include "motor.h"
#include "time.h"
#include "CAN.h"
#include "PI.h"

#define MAX_ERROR_SUM 1
#define K_P 1
#define K_I 0
#define min(a, b) ((a) < (b) ? (a) : (b))
#define sgn(a) ((a) >= 0 ? 1 : -1)

volatile int error_sum;

void PI_timer_interrupt(void);
void PI_timer_init(void);

//Set the timer_flag when the time has passed
ISR(TIMER4_COMPA_vect){
	PI_timer_interrupt();
}

void PI_timer_interrupt(void) {
	printf("INSIDE PI_timer_interrupt\n");
	// Find error
	can_message control_input = receive_control_inputs();
	int velocity_reference = control_input.data[0];
	int error  = velocity_reference - motor_get_velocity();
	
	// Calculate signal
	PI_regulate(error);
}

/**
 * Initialize the PI-regulator, setting the error_sum to zero.
 */
void PI_init(void) {
	error_sum = 0;
	// Enable interrupts
	set_bit(TCCR4A, COM4A1);
	PI_timer_init();
}


void PI_timer_init(void){
	//Use timer4
	//clear timer on compare (CTC) mode (16 bit, TOP in OCR4A) page 145 (mode 4 table 17-2)
	//clear_bit(TCCR4B, WGM43);
	set_bit(TCCR4B, WGM42);
	//clear_bit(TCCR4A, WGM41);
	//clear_bit(TCCR4A, WGM40);
		
	//resets at this value (output compare register) ~100 Hz when prescaler = 8
	OCR4A = 10000;
	
	//set prescaler 8
	clear_bit(TCCR4B, CS42);
	set_bit(TCCR4B, CS41);
	clear_bit(TCCR4B, CS40);
}

void PI_regulate(int error) {
	// Find the next error_sum.
	error_sum += error;
	
	// Saturate the error sum, such that it is on the interval [-MAX_ERROR_SUM, +MAX_ERROR_SUM]
	error_sum = sgn(error_sum) * min(MAX_ERROR_SUM, abs(error_sum));
	
	// Calculate the output signal of the PI-regulator.
	int output_signal = K_P * error + K_I * error_sum;
	
	motor_set_velocity(output_signal);
}
