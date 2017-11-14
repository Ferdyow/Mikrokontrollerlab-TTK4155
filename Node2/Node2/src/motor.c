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
#include "CAN.h"


// IMPLEMENTS OPEN LOOP CONTROL


// Ports
#define MJ1  PORTH // the port on the 2560
#define MJ2  PINK

// MJ1 Pins
#define DIR PH1 //direction
#define SEL	PH3
#define EN  PH4 //Enable
#define OE	PH5 //encoder counter output enable (active low)
#define RST	PH6 //reset (active low)

#define MOTOR_SPEED_CHANNEL 0
#define MOTOR_GAIN 0.7 * 255/100

// Make this a define? Should remain constant after having been set.
double max_speed = 0;

typedef enum {
	LEFT,
	RIGHT
} direction;


/************************************************************************/
/* HELPER FUNCTION DECLARATIONS                                         */
/************************************************************************/

/**
 * Enable servo motor, such that it can move sideways.
 */
void motor_enable(void);

/**
 * Disable servo motor, such that it cannot move sideways.
 */
void motor_disable(void);

/**
 * Set the direction of the motor (sideways)
 * @param {direction} motor_direction - The direction to set the motor to. (LEFT or RIGHT)
 */
void motor_set_direction(direction motor_direction);

/**
 * Set the sideways speed of the motor (in absolute value)
 * @param {uint8_t} motor_speed - The speed of the motor in absolute value (between 0 and 100*MOTOR_GAIN)
 */
void motor_set_speed(uint8_t motor_speed);

/**
 * Automatically find the maximum speed (in absolute value) of the motor. 
 */
void motor_find_max_speed_auto(void);

/**
 * Manually find the maximum speed of the motor.
 */
void motor_find_max_speed_manual(void);


/************************************************************************/
/* FUNCTION IMPLEMENTATIONS                                             */
/************************************************************************/

void motor_init(void){
	printf("Inside motor_init\n");
	MAX520_init(0b000); 
	
	// Other initialization below
	motor_enable();
	clear_bit(MJ1, OE);  // Enable encoder
	set_bit(MJ1, RST);
	
	motor_set_direction(LEFT);
	motor_set_speed(0);
	
	// Set PORTH as output
	DDRH = 0xFF;
	time_init();
	motor_find_max_speed_auto();
}

void motor_find_max_speed_manual(void){
	
	can_message message;
	int8_t joystick_x = 0;
	int button_is_pressed = 0;
	
	printf("---------------------------------------------------------\n");
	printf("CALIBRATE THE ENCODER BY MEASURING THE MAXIMUM VELOCITY\n");
	printf("PRESS ANY BUTTON TO CONTINUE\n");
	printf("---------------------------------------------------------\n");
	
	while(!button_is_pressed) {
		message = receive_control_inputs();
		joystick_x = message.data[0];
		motor_set_velocity(joystick_x);
		const int RL_BUTTONS = 0b011;
		button_is_pressed = message.data[2] & RL_BUTTONS;
		double speed = abs(motor_read_position_change())/(double)(time_passed());
		if (speed > max_speed) {
			max_speed = speed;
		}
	}
	
	printf("---------------------------------------------------------\n");
	printf("CALIBRATION COMPLETE\n");
	printf("---------------------------------------------------------\n");
	motor_set_velocity(0);
}

void motor_find_max_speed_auto(void){
	cli();
	const int16_t velocity[] = {50, -50, 100, -100, 150, -150};
	const uint8_t time_interval = 200; //85; // Unit unknown
	
	const int velocity_size = sizeof(velocity)/sizeof(velocity[0]); // Size of array
	for (int i = 0; i < velocity_size; i++) {
		// Used to keep track of how long the motor has been going in one direction.
		uint64_t time_sum = 0;
		printf("Set velocity to: %d\n", velocity[i]);
		
		sei(); // Interrupts need to be enabled in order to set velocity
		motor_set_velocity(velocity[i]); 
		cli();
		
		while((uint8_t)((time_sum)>>8) < time_interval) {
			double time = time_passed();
			time_sum += (uint64_t)time;
			
			double speed = abs(motor_read_position_change())/time;
			if (speed > max_speed) {
				max_speed = speed;
			}
			
			printf("speed: %d\n", (int)(speed*1000));
			_delay_ms(50);
		}
		printf("MAX SPEED: %d\n", (int)(max_speed*1000));
		
		// Wait 1 second, and reset the time_sum
		_delay_ms(1000);
	}
	
	sei(); // Interrupts need to be enabled in order to set velocity
	motor_set_velocity(0); 
	cli();
}

void motor_enable(void) {
	set_bit(MJ1, EN);
}

void motor_disable(void) {
	clear_bit(MJ1, EN);
}

void motor_set_velocity(int16_t motor_velocity) { // u
	
	// Find the motor direction. Negative velocity corresponds to left.
	direction motor_direction = (motor_velocity < 0) ? LEFT : RIGHT;
	
	// Speed is in absolute value. Make sure it is not greater than 100.
	uint8_t motor_speed = abs(motor_velocity);
	motor_speed = (motor_speed > 100) ? 100 : motor_speed;
	
	// Set direction and speed of motor.
	motor_set_direction(motor_direction);
	motor_set_speed(motor_speed * MOTOR_GAIN);
}

void motor_set_direction(direction motor_direction) {
	(motor_direction == LEFT) ? clear_bit(MJ1, DIR) : set_bit(MJ1, DIR);
}

void motor_set_speed(uint8_t motor_speed) {
	MAX520_send(MOTOR_SPEED_CHANNEL, motor_speed);
}



/*------------------------------------------*/
/*				READ FROM ENCODER			*/
/*------------------------------------------*/

int16_t motor_read_position_change(void) { 
	// Read from the encoder as described in "Motorbox User Guide" page 5
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
	
	return position;
}

int8_t motor_get_velocity(void) {
	double velocity = ((double) motor_read_position_change()) / time_passed();
	int velocity_percentage;
	velocity_percentage = velocity * 100/max_speed;
	
	if (fabs(velocity) > max_speed){
		// Calibrate velocity if max_speed is greater than previous measurements
		max_speed = velocity;
		velocity_percentage = 100 * fabs(velocity)/velocity;
	}
	else {
		velocity_percentage = velocity * 100/max_speed;
	}
	
	printf("Velocity_percentage: %d\n", velocity_percentage);
	//printf("Position: %d\n", motor_read_position());
	return velocity_percentage;
}
