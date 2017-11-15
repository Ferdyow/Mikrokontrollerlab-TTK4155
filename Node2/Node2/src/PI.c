/*
 * PI.c
 *
 * Created: 11.11.2017 15:34:11
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
#include "PI.h"


volatile PI_controller_t controller;


void PI_init() {
	controller.error_sum = 0;
	controller.k_p = 0.5;
	controller.k_i = 0.2;
	controller.max_error_sum = 20;
}


void PI_control(double velocity_reference) {
	double error = velocity_reference - motor_get_velocity();
	
	// Find the next error_sum.
	controller.error_sum += error;
	
	// Saturate the error sum, such that it is on the interval [-MAX_ERROR_SUM, +MAX_ERROR_SUM]
	controller.error_sum = SATURATE(controller.error_sum, -controller.max_error_sum, controller.max_error_sum);
	
	// Calculate the output signal of the PI-regulator.
	int output_signal = (controller.k_p * error) + (controller.k_i * controller.error_sum);
	
	motor_set_velocity(output_signal);
}
