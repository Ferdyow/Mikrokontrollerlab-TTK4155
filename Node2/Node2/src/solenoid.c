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

#include "solenoid.h"

// Clear TIMER5{A,B} interrupt flag, before enabling interupts on TIMER5{A,B} 
#define TIMER5A_START()     set_bit(TIFR5, OCF5A); set_bit(TIMSK5, OCIE5A)
#define TIMER5B_START()     set_bit(TIFR5, OCF5B); set_bit(TIMSK5, OCIE5B)

// Disable interrupts on TIMER5{A,B}
#define TIMER5A_STOP()      clear_bit(TIMSK5, OCIE5A)
#define TIMER5B_STOP()      clear_bit(TIMSK5, OCIE5B)

#define TIMER5_RESET()		TCNT5 = 0
#define SOLENOID_EJECT()    clear_bit(PORTB , PB4)
#define SOLENOID_RETRACT()  set_bit(PORTB , PB4)

volatile int solenoid_charged_flag = 0;

void solenoid_timer_init(void);


// Wait for the solenoid to charge up for ~300 ms before allowing it to fire again.
ISR(TIMER5_COMPA_vect) {
	solenoid_charged_flag++;
	TIMER5A_STOP();
}


// Retract the solenoid ~30 ms after shooting it out, and start recharging.
ISR(TIMER5_COMPB_vect) {
	SOLENOID_RETRACT();
	TIMER5B_STOP();
	TIMER5A_START();
}


void solenoid_init(){
	// Set output pin PB4
	set_bit(DDRB, DDB4);
	
	// Set high to disconnect
	set_bit(PORTB, PB4);
	
	solenoid_timer_init();
}


void solenoid_send_pulse(){
	// Only allow the solenoid to fire if it has been charged up.
	if(solenoid_charged_flag) {
		solenoid_charged_flag = 0;
		SOLENOID_EJECT();
		TIMER5B_START();
	}
}


void solenoid_timer_init(void) {
	// Timer mode 4, (CTC) [Table 17-2]
	set_bit(TCCR5B, WGM52);
	
	// Compare output mode. Clear counter on compare match [Table 17-3]
	set_bit(TCCR5A, COM5A1);
	
	// Use a prescaler of 1024 [Table 17-6]
	set_bit(TCCR5B, CS50);
	set_bit(TCCR5B, CS52);
	
	// Set interrupt counter compare match values
	OCR5A = (int) (F_CPU * TIMER_5A_DELAY / TIMER5_PRESCALER);
	OCR5B = (int) (F_CPU * TIMER_5B_DELAY / TIMER5_PRESCALER);
	
	// The solenoid starts in a charged up state.
	solenoid_charged_flag = 1;
}