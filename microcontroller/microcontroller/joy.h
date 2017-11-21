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

// Position of x- and y-axes (between -100 and 100)
typedef struct{
	int x;
	int y;
} JOY_position_t;

// Position of left and right slider (between 0 and 255)
typedef struct {
	int left;
	int right;
} SLI_position_t;

/**
 * Initializes the use of the buttons and starts calibration
 * @param {int} calibration_mode - Decides between default settings and manual calibration
 */
void JOY_init(int calibraton_mode);

/**
 * Sets PINE0 to receive interrupt from ADC
 * @param {button} b - The button to check (LEFT_BUTTON, RIGHT_BUTTON or JOY_BUTTON)
 * @returns {int} - 1 if button b is pressed, 0 otherwise
 */
int JOY_button_pressed(button b);

/**
* Calculate position (between -100 and 100) using current signal and values from calibration
* @returns {JOY_position_t} - The x and y coordinates as values between -100 and 100
*/
JOY_position_t JOY_getPosition();

/**
 * Read the direction of the joystick. The direction is considered NEUTRAL if the joystick is within {(-5,-5), (5, 5)}
 * @returns {JOY_direction_t} - The direction of the joystick. (LEFT, RIGHT, UP, DOWN or NEUTRAL)
 */
JOY_direction_t JOY_getDirection();

/**
 * Read the slider position.
 * @returns {SLI_position_t} - The slider position. Between 0 and 255.
 */
SLI_position_t SLI_getPosition();


#endif /* JOYSTICK_H_ */