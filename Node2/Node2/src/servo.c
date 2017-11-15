/*
 * servo.c
 *
 * Created: 25.10.2017 15:55:06
 *  Author: ferdyow
 */ 


#include <stdio.h>

#include "defines.h"
#include "PWM.h"
#include "servo.h"

#include <avr/interrupt.h>

// DO NOT CHANGE THESE VALUES, as it might break the servo.
#define MAX_WIDTH 2.0
#define MIN_WIDTH 1.0

#define WIDTH_MIDPOINT (MAX_WIDTH + MIN_WIDTH) / 2
#define WIDTH_RADIUS   (MAX_WIDTH - MIN_WIDTH) / 2


void servo_init(void){
	PWM_init();
	PWM_set_width(WIDTH_MIDPOINT);
}

void servo_set(uint8_t x){
	// Calculate the width for the PWM.
	float width = MIN_WIDTH + (MAX_WIDTH - MIN_WIDTH) * ((float) x) / 255;
	
	// Make sure width is within range.
	width = SATURATE(width, MIN_WIDTH, MAX_WIDTH);
	
	PWM_set_width(width);
}