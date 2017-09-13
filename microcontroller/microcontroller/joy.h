/*
 * joystick.h
 *
 * Created: 13.09.2017 13:08:06
 *  Author: tobib
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_
#include <stdbool.h>

typedef enum{
	LEFT_BUTTON, 
	RIGHT_BUTTON,
	JOY_BUTTON
	}button;


typedef struct{
	//position (between -100 and 100 in percentage)
	int8_t x;
	int8_t y;
} joy_position_t;

typedef enum{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NEUTRAL
	}joy_direction_t;


void joy_init();
void joy_calibrate();
bool joy_button_pressed(button b);
joy_position_t joy_getPosition();
joy_direction_t joy_getDirection();



#endif /* JOYSTICK_H_ */