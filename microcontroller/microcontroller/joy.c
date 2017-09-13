/*
 * joystick.c
 *
 * Created: 13.09.2017 15:10:13
 *  Author: tobib
 */ 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "joy.h"
#include "defines.h"

#define X_COORD 0
#define Y_COORD 1

struct Coordinate {
	uint8_t x;
	uint8_t y;
} max, min, rad, mid;


void joy_calibrate(){
	max.x = 0;
	max.y = 0;
	min.x = 255;
	min.y = 255;
	uint8_t x;
	uint8_t y;
	printf("Move the joystick to all extreme points\n");
	//check channels
	while(!test_bit(PINB,PINB0)){
		// Read sensor values
		x = adc_read(0);
		y = adc_read(1);
		
		// Calibrate x
		if (min.x > x) min.x = x; 
		else if (max.x < x) max.x = x;
		
		// Calibrate y
		if (min.y > y) min.y = y;
		else if (max.y < y) max.y = y;		
	}

	// Find distance from middle to edges (radius)
	rad.x = (max.x - min.x) / 2;
	rad.y = (max.y - min.y) / 2;
	
	// Find mid point using edges.
	mid.x = (max.x + min.x) / 2;
	mid.y = (max.y + min.y) / 2;
}

/**
 * Calculate position (between -100 and 100) using current signal and values from calibration
 * @returns {joy_position_t} - The x and y coordinates as values between -100 and 100
 */
joy_position_t joy_getPosition() {
	joy_position_t joy_position;
	joy_position.x = (100 * (adc_read(X_COORD) - mid.x)) / rad.x;
	joy_position.y = (100 * (adc_read(Y_COORD) - mid.y)) / rad.y;
	return joy_position;
}

joy_direction_t joy_getDirection() {
	joy_position_t position = joy_getPosition();
	
	// The joystick is within {(-5, -5), (5, 5)}, and the position is considered neutral.
	if (abs(position.x) < 5 && abs(position.y) < 5) return NEUTRAL;

	// The x-coordinate is dominating
	else if (abs(position.x) > abs(position.y)) {
		// Positive x => RIGHT
		if (position.x > 0) return RIGHT;
		// Negative x => LEFT
		else return LEFT;

	// The y-coordinate is dominating
	} else {
		// Positive y => UP
		if (position.y > 0) return UP;
		// Negative y => DOWN
		else return DOWN;
	}
}

bool joy_button_pressed(){
	
}