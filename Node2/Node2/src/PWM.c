/*
 * timer.c
 *
 * Created: 25.10.2017 14:07:47
 *  Author: ferdyow
 */ 


#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#include <avr/interrupt.h>

#include "defines.h"
#include "PWM.h"


#define PERIOD_MS 20
#define PRESCALER 8


void PWM_init(void){
	//if clk_io = 1 / F_CPU: prescalar = 8, TOP = 39 999

	//select source of clock signal and set prescaler = 8
	clear_bit(TCCR1B, CS12);
	set_bit(TCCR1B, CS11);
	clear_bit(TCCR1B, CS10);
	
	
	//fast PWM (16 bit, TOP in ICR1) page 145 (mode 14 table 17-2)
	set_bit(TCCR1B, WGM13);
	set_bit(TCCR1B, WGM12);
	set_bit(TCCR1A, WGM11);
	clear_bit(TCCR1A, WGM10);
	
	//set TOP so that we have a period of 20 ms
	ICR1 = F_CPU/PRESCALER*PERIOD_MS/1000 - 1; 
	
	//compare output mode (Normal mode)
	set_bit(TCCR1A, COM1A1);
	
	//set output pin PB5(OC1A: output clock 1 A)
	set_bit(DDRB, DDB5);

	//compares TCTn and OCRnx - cleared when match -> TCTn = BOTTOM
}

void PWM_set_width(float on_time_ms){
	OCR1A = F_CPU/PRESCALER*on_time_ms/1000-1; 
}

//"Accessing 16-bit Registers” on page 135
//interrupt requests: TIFRn
//“Output Compare Units” on page 141.
//50 hz signal 
//16-bit registers using two read/write read/write - high byte first -  NOT OCRnA/B/C
//CLOCK: prescaling page 164