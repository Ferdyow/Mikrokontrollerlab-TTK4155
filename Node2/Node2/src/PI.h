/*
 * PI.h
 *
 * Created: 11.11.2017 15:34:22
 *  Author: ferdyow
 */ 


#ifndef PI_H_
#define PI_H_

/**
 * Initialize the PI-regulator, setting the error_sum to zero.
 */
void PI_init(void);

/**
 * Takes an error as input, and produces an output signal for the motor.
 * @param {int} error - The error in velocity for the motor.
 * Side effect: Set the motor signal to the output of the regulator
 */
void PI_regulate(int error);

#endif /* PI_H_ */