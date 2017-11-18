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
void JOY_calibrate(int calibration_mode);
void JOY_print_calibration_status(void);

#define X_COORD 0
#define Y_COORD 1
#define LEFT_CH 2
#define RIGHT_CH 3

struct Coordinate {
	uint8_t x;
	uint8_t y;
} max, min, rad, mid;

void JOY_init(int calibraton_mode){
	//set pull-up resistors on PORTB for JOY_button
	set_bit(PORTB,PB2);
	clear_bit(DDRB,PB2);
	clear_bit(SFIOR, PUD);
	JOY_calibrate(calibraton_mode);
}

void JOY_print_calibration_status() {
	OLED_reset();
	fprintf(OLED, "%s\n", "Calibrate JOY");
	fprintf(OLED, "x_max: %6d\n", max.x);
	fprintf(OLED, "x_min: %6d\n", min.x);
	fprintf(OLED, "y_max: %6d\n", max.y);
	fprintf(OLED, "y_min: %6d\n", min.y);
	fprintf(OLED, "\n%s\n", "Click when done");
}

void JOY_calibrate(int calibraton_mode){
	max.x = JOY_DEFAULT_MAX_X;
	max.y = JOY_DEFAULT_MAX_Y;
	min.x = JOY_DEFAULT_MIN_X;
	min.y = JOY_DEFAULT_MIN_Y;
	
	if (calibraton_mode == CALIBRATION_MANUAL) {
		// Loop while joystick button is not pressed.
		while(test_bit(PINB,PINB2)) {
			// Read sensor values
			uint8_t x = ADC_read(X_COORD);
			uint8_t y = ADC_read(Y_COORD);
			
			// Find max and min values for x and y:
			max.x = MAX(x, max.x);
			max.y = MAX(y, max.y);
			min.x = MIN(x, min.x);
			min.y = MIN(y, min.y);
			
			JOY_print_calibration_status();
			_delay_ms(50);
		}
	}
	
	// Find distance from middle to edges (radius)
	rad.x = (max.x - min.x) / 2;
	rad.y = (max.y - min.y) / 2;
		
	// Find mid point using edges.
	mid.x = (max.x + min.x) / 2;
	mid.y = (max.y + min.y) / 2;
}

int JOY_button_pressed(button b) {
	switch (b) {
	case LEFT_BUTTON:
		if (test_bit(PINB, PINB0)) return 1;
		break;
	case RIGHT_BUTTON:
		if (test_bit(PINB, PINB1)) return 1;
		break;
	case JOY_BUTTON:
		if (!test_bit(PINB, PINB2)) return 1;
		break;
	}
	return 0;
}

JOY_position_t JOY_getPosition() {
	JOY_position_t JOY_position;
	JOY_position.x = (100 * (ADC_read(X_COORD) - mid.x)) / rad.x;
	JOY_position.y = (100 * (ADC_read(Y_COORD) - mid.y)) / rad.y;
	printf("x: %d, y: %d\n", JOY_position.x, JOY_position.y);
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
		printf("X-position: %4d\t\tY-position: %4d\t\t", posJ.x, posJ.y);
		printf("Left slider: %4d\t\t Right slider: %4d\t\t", posS.left, posS.right);
		printf("Left button: %4d\tRight button: %4d\tjoystick button: %4d\n", test_bit(PINB,PB0), test_bit(PINB, PB1), !test_bit(PINB, PB2));
	}
}