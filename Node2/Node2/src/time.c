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
	//Normal port operation, non PWM
	
	//set IOCLK, no prescaler
	set_bit(TCCR3B, CS30);
	time_reset();
	
}

//setter time started til current time
void time_reset(void) {
	TCNT3 = 0;
}

uint16_t time_passed(void){
	uint16_t time_passed = TCNT3;
	time_reset();
	return time_passed;
	
}

/*
double time_passed(void){
	uint16_t time_passed = TCNT3;
	double seconds = time_passed/(double)F_CPU;
	time_reset();
	return time_passed;
	
}*/