/*
 * servo.h
 *
 * Created: 25.10.2017 15:55:19
 *  Author: ferdyow
 */ 


#ifndef SERVO_H_
#define SERVO_H_

/**
 * Initialize the servo by setting up PWM (pulse width modulation).
 */
void servo_init(void);


//Takes a percentage between -100 and 100 and converts it to PWM output between 0.9 and 2.1 ms

/**
 * Sets the servo position. Accepts numbers between -100 and 100 (numbers outside the range are saturated),
 * and converts it to PWM output between 1.0 and 2.0.
 * @param {uint8_t} x - Position to set the servo to.
 */
void servo_set(uint8_t x);


#endif /* SERVO_H_ */