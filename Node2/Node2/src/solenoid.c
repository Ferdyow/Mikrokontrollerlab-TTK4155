/*
 * solenoid.c
 *
 * Created: 15.11.2017 14:56:30
 *  Author: tobib
 */ 

#include "defines.h"

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/************************************************************************/
/* TIMER A: Wait for the solenoid to charge after firing.               */
/* TIMER B: Wait before retracting solenoid after ejecting.             */
/************************************************************************/
#define TIMER_A_DELAY ((double) 300)/1000  // s
#define TIMER_B_DELAY ((double) 30)/1000  // s
#define TIMER_PRESCALER 1024

#define interrupt_A_enable()     set_bit(TIFR5 , OCF5A ); set_bit(TIMSK5, OCIE5A)
#define interrupt_A_disable()  clear_bit(TIMSK5, OCIE5A)
#define interrupt_B_enable()     set_bit(TIFR5 , OCF5B ); set_bit(TIMSK5, OCIE5B)
#define interrupt_B_disable()  clear_bit(TIMSK5, OCIE5B)
#define solenoid_retract()       set_bit(PORTB , PB4   )
#define solenoid_eject()       clear_bit(PORTB , PB4   )

#define reset_timer() (TCNT5 = 0)

void solenoid_timer_init(void);


volatile int solenoid_charged_flag = 0;
ISR(TIMER5_COMPA_vect) {
	solenoid_charged_flag++;
	interrupt_A_disable();
}

// Retract the solenoid ~30 ms after shooting it out, and start recharging.
ISR(TIMER5_COMPB_vect) {
	solenoid_retract();
	interrupt_B_disable();
	interrupt_A_enable();
}


void solenoid_init(){
	//set output pin PB4
	set_bit(DDRB, DDB4);
	
	//set high to disconnect
	set_bit(PORTB, PB4);
	
	solenoid_timer_init();
}

void solenoid_timer_init(void) {
	// Timer mode 4, (CTC) [Table 17-2]
	set_bit(TCCR5B, WGM52);
		
	// Compare output mode. Clear counter on compare match [Table 17-3]
	set_bit(TCCR5A, COM5A1);
		
	// Use a prescaler of 1024 [Table 17-6]
	set_bit(TCCR5B, CS50);
	clear_bit(TCCR5B, CS51);
	set_bit(TCCR5B, CS52);
		
	// Set interrupt counter compare match values
	OCR5A = (int) (F_CPU * TIMER_A_DELAY / TIMER_PRESCALER);
	OCR5B = (int) (F_CPU * TIMER_B_DELAY / TIMER_PRESCALER);
	
	// Enable interrupt
	interrupt_B_enable();
}

void solenoid_send_pulse(){
	if(solenoid_charged_flag) {
		solenoid_charged_flag = 0;
		solenoid_eject();
		interrupt_B_enable();
	}
}