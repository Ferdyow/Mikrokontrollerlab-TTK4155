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


void solenoid_init(){
	//set output pin PB4
	set_bit(DDRB, DDB4);
	
	//set high to disconnect
	set_bit(PORTB, PB4);
}


void solenoid_send_pulse(){
	clear_bit(PORTB, PB4);
	_delay_ms(30);
	set_bit(PORTB, PB4);
}