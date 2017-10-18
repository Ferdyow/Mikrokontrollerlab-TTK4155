/*
 * joy.c
 *
 * Created: 13.09.2017 15:10:13
 *  Author: tobib
 */ 

#define F_CPU 4915200  // Clock frequency in Hz
#include "JOY.h"
#include "adc.h"
#include "defines.h"

#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

 /*"on the go"-calibration is done manually (not needed in the .h file)*/
void JOY_calibrate();

#define X_COORD 0
#define Y_COORD 1
#define LEFT_CH 2
#define RIGHT_CH 3

struct Coordinate {
	uint8_t x;
	uint8_t y;
} max, min, rad, mid;

void JOY_init(){
	//set pull-up resistors on PORTB for JOY_button
	set_bit(PORTB,PB2);
	clear_bit(DDRB,PB2);
	clear_bit(SFIOR, PUD);
	JOY_calibrate();
}

void JOY_calibrate(){
	max.x = 0;
	max.y = 0;
	min.x = 255;
	min.y = 255;
	uint8_t x;
	uint8_t y;
	OLED_reset();
	printf("\nMove the joystick to all extreme points.\n");
	printf("Press the joystick when you're done.\n");
	//check channels 
	while(test_bit(PINB,PINB2)){
		// Read sensor values
		x = ADC_read(0);
		y = ADC_read(1);
		
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

bool JOY_button_pressed(button b) {
	switch (b) {
	case 0:
		if (test_bit(PINB, PINB0)) return true;
		break;
	case 1:
		if (test_bit(PINB, PINB1)) return true;
		break;
	case 2:
		if (!test_bit(PINB, PINB2)) return true;
		break;
	}
	return false;
}

JOY_position_t JOY_getPosition() {
	JOY_position_t JOY_position;
	JOY_position.x = (100 * (ADC_read(X_COORD) - mid.x)) / rad.x;
	JOY_position.y = (100 * (ADC_read(Y_COORD) - mid.y)) / rad.y;
	return JOY_position;
}

JOY_direction_t JOY_getDirection() {
	JOY_position_t position = JOY_getPosition();
	
	// The JOYstick is within {(-5, -5), (5, 5)}, and the position is considered neutral.
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

SLI_position_t SLI_getPosition() {
	SLI_position_t pos;
	pos.left = ADC_read(LEFT_CH);
	pos.right = ADC_read(RIGHT_CH);
	return pos;
}

void JOY_test() {
	while (1) {
		JOY_position_t posJ = JOY_getPosition();
		SLI_position_t posS = SLI_getPosition();
		printf("X-position: %4d\t\tY-position: %4d\t\tLeft slider: %4d\t\t Right slider: %4d\t\tLeft button: %4d\tRight button: %4d\tjoystick button: %4d\n",posJ.x, posJ.y, posS.left, posS.right, test_bit(PINB,PB0), test_bit(PINB, PB1), !test_bit(PINB, PB2));
	}
}