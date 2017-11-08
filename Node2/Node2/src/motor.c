/*
 * Motor.c
 *
 * Created: 08.11.2017 09:28:16
 *  Author: ferdyow
 */ 

#include "defines.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "MAX520.h"
#include "motor.h"
#include "time.h"


//IMPLEMENTS OPEN LOOP CONTROL 


// Ports
#define MJ1  PORTH // the port on the 2560
#define MJ2  PINK

// MJ1 Pins
#define DIR PH1 //direction
#define SEL	PH3
#define EN  PH4 //Enable
#define OE	PH5 //encoder counter output enable (active low)
#define RST	PH6 //reset (active low)


double max_speed = 0;

typedef enum {
	LEFT,
	RIGHT
} direction;

#define MOTOR_GAIN 0.7 * 255/100
void motor_enable(void);
void motor_disable(void);
void motor_set_direction(direction motor_direction);
void motor_set_speed(uint8_t motor_speed);
void motor_find_max_speed();



void motor_init(void){
	MAX520_init(0b000); 
	//other initialization below
	motor_enable();
	clear_bit(MJ1, OE); //encoder enabled
	set_bit(MJ1, RST);
	
	motor_set_direction(LEFT);
	motor_set_speed(0);
	
	//Set PORTH as output
	DDRH = 0xFF;
	time_init();
	motor_find_max_speed();
	
	
}


void motor_find_max_speed(){
	int direction = 1;
	for (int i = 0; i < 10; i++){
		direction *= -1;
		motor_set_velocity(100*direction);
		_delay_ms(300);
		double speed = abs(motor_read_position())/(double)(time_passed());
		if(speed > max_speed){
			max_speed = speed; 
		}
	}
	motor_set_speed(0);
	motor_set_direction(LEFT);
	//_delay_ms(10000);
}

void motor_enable(void){
	set_bit(MJ1, EN);
}

void motor_disable(void){
	clear_bit(MJ1, EN);
}


/**
 * Set the velocity of the servo motor (sideways)
 * @param {uint8_t} motor_velocity - The velocity of the motor in percentage between -100 and 100
 */
void motor_set_velocity(int16_t motor_velocity) { // u
	direction motor_direction;
	
	// Find the motor direction. Negative velocity corresponds to left.
	motor_direction = (motor_velocity < 0) ? LEFT : RIGHT;
	
	// Speed is in absolute value. Make sure it is not greater than 100.
	uint8_t motor_speed = abs(motor_velocity);
	motor_speed = (motor_speed > 100) ? 100 : motor_speed;
	
	// Set direction and speed of motor.
	motor_set_direction(motor_direction);
	motor_set_speed(motor_speed * MOTOR_GAIN);
}


void motor_set_direction(direction motor_direction){
	if (motor_direction == LEFT) {
		clear_bit(MJ1, DIR);
	}
	else {
		set_bit(MJ1, DIR);
	}
}

void motor_set_speed(uint8_t motor_speed){
	//printf("Motor speed: %3d\n", motor_speed);
	const int channel = 0;
	MAX520_send(channel, motor_speed);
}



/*------------------------------------------*/
/*				READ FROM ENCODER			*/


int16_t motor_read_position(void) { 
	//read from the encoder as described in "Motorbox User Guide" page 5
	clear_bit(MJ1, OE);
	clear_bit(MJ1, SEL);
	_delay_us(20);
	uint16_t position = (MJ2 << 8);
	
	set_bit(MJ1, SEL);
	_delay_us(20);
	position += MJ2;
	
	clear_bit(MJ1, RST);
	_delay_us(25);
	set_bit(MJ1, RST);
	set_bit(MJ1, OE);
	printf("Position change: %d\t", position);
	
	return position;
}

int8_t motor_get_velocity(void){
	double velocity = motor_read_position() / time_passed();
	int velocity_percentage = velocity * 100/max_speed ;
	printf("Velocity_percentage: %d\n", velocity_percentage);
	return velocity_percentage;
}
