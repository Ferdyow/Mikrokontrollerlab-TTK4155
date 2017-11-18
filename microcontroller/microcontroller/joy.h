/*
 * joy.h
 *
 * Created: 13.09.2017 13:08:06
 *  Author: tobib
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_
#include <stdbool.h>

#define JOY_DEFAULT_MAX_X 255
#define JOY_DEFAULT_MAX_Y 255
#define JOY_DEFAULT_MIN_X 0
#define JOY_DEFAULT_MIN_Y 0

typedef enum { LEFT_BUTTON, RIGHT_BUTTON, JOY_BUTTON } button;
typedef enum { LEFT, RIGHT, UP, DOWN, NEUTRAL } JOY_direction_t;

typedef struct{
	//position (between -100 and 100 in percentage)
	int x;
	int y;
} JOY_position_t;

typedef struct {
	int left;
	int right;
} SLI_position_t;

/*Initializes the use of the buttons and starts calibration*/
void JOY_init(int calibraton_mode);

/*returns true/1 if the button is pressed, false/0 otherwise*/
int JOY_button_pressed(button b);

/**
* Calculate position (between -100 and 100) using current signal and values from calibration
* @returns {JOY_position_t} - The x and y coordinates as values between -100 and 100
*/
JOY_position_t JOY_getPosition();

/*returns the most extreme direction of the joystick if it is moved beyond a threshold of 5% from neutral*/
JOY_direction_t JOY_getDirection();

/*returns the slider position as a value between 0 and 255*/
SLI_position_t SLI_getPosition();

#endif /* JOYSTICK_H_ */