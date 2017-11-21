/*
 * timer.h
 *
 * Created: 25.10.2017 14:07:59
 *  Author: ferdyow
 */ 


#ifndef TIMER_H_
#define TIMER_H_


/**
 * Initialize Pulse Width Modulation (PWM). Uses TIMER1 to control width of the PWM-signal.
 */
void PWM_init(void);

//Makes the time module count up to a value such that it will have an ON time of the given value
/**
 * Set the width of the PWM pulse.
 * @param {float} on_time_ms - Width of PWM pulse in milliseconds.
 */
void PWM_set_width(float on_time_ms);


#endif /* TIMER_H_ */