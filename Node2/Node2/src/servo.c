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


#define MAX_WIDTH 2.0
#define MIN_WIDTH 1.0

#define WIDTH_MIDPOINT (MAX_WIDTH + MIN_WIDTH) / 2
#define WIDTH_RADIUS   (MAX_WIDTH - MIN_WIDTH) / 2


void servo_init(void){
	PWM_init();
	PWM_set_width(WIDTH_MIDPOINT);
}

void servo_set(int8_t x){
	// Calculate the width for the PWM.
	float width = WIDTH_MIDPOINT + x * WIDTH_RADIUS / 100.0;
	
	// Make sure width is within range.
	if      (width < MIN_WIDTH) width = MIN_WIDTH;
	else if (width > MAX_WIDTH) width = MAX_WIDTH;
	
	PWM_set_width(width);
}