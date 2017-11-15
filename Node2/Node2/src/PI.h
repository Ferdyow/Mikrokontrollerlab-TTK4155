/*
 * PI.h
 *
 * Created: 11.11.2017 15:34:22
 *  Author: ferdyow
 */ 


#ifndef PI_H_
#define PI_H_

typedef struct {
	double k_p;
	double k_i;
	double error_sum;
	double max_error_sum;
} PI_controller_t;


/**
 * Initialize the PI-regulator with parameters.
 */
void PI_init();

/**
 * Takes an error as input, and produces an output signal for the motor.
 * @param {double} velocity_reference - The velocity reference for the motor.
 * Side effect: Set the motor signal to the output of the regulator
 */
void PI_control(double velocity_reference);


#endif /* PI_H_ */