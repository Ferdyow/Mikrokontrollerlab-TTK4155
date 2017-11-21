/*
 * timer.c
 *
 * Created: 08.11.2017 15:34:03
 *  Author: ferdyow
 */ 
#include "defines.h"

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "time.h"


void time_reset(void);

void time_init(){
	// Normal port operation, non PWM
	
	// Prescaler 256 (see table 20-9)
	set_bit(TCCR3B, CS32);
	time_reset();
	
}

void time_reset(void) {
	TCNT3 = 0;
	
	// CLear overflow flag 
	set_bit(TIFR3, TOV3);
}

uint16_t time_passed(void){
	uint16_t time_passed = TCNT3;
	time_reset();
	if (time_passed == 0){
		return 1;
	}
	return time_passed;
}
