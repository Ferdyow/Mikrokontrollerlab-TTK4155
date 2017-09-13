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

struct Coordinate {
	uint8_t x;
	uint8_t y;
} max, min;

void joy_init(){
	set_bit(PORTB,PB2);
	clear_bit(DDRB,PB2);
	clear_bit(SFIOR, PUD);
	joy_calibrate();
}
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
}

bool joy_button_pressed(button b){
	switch (b){
		case 0:
		if(test_bit(PINB,PINB0)) return true;
		case 1:
		if(test_bit(PINB,PINB1)) return true;
		case 2:
		if(!test_bit(PINB,PINB2)) return true;
	}
	return false;
}

//joy_direction_t joy_getDirection(){
	//
//}