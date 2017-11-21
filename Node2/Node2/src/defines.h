/*
 * defines.h
 *
 * Created: 13.09.2017 12:12:04
 *  Author: tobib
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_

#include "usart.h"

// CPU clock speed 16 MHz
#define F_CPU 16000000

// Useful function macros
#define set_bit( reg, bit )						(reg |= (1 << bit))
#define clear_bit( reg, bit )					(reg &= ~(1 << bit))
#define test_bit( reg, bit )					(reg & (1 << bit))
#define loop_til_bit_is_set( reg, bit )			while( !test_bit( reg, bit ) )
#define loop_until_bit_is_clear( reg, bit )		while( test_bit( reg, bit ) )
#define MAX(a,b)								((a < b) ?  (b) : (a))
#define MIN(a,b)								((a > b) ?  (b) : (a))
#define SATURATE(x,low,high)					( MIN( MAX((x), (low)), (high) ) )

// Defines control input mapping over CAN bus
enum {JOYSTICK_X, JOYSTICK_Y, SLIDER_LEFT, SLIDER_RIGHT, BUTTONS};
typedef enum { LEFT_BUTTON, RIGHT_BUTTON, JOY_BUTTON } button;

#endif /* DEFINES_H_ */
