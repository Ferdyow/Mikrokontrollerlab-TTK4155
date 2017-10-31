/*
 * timer.h
 *
 * Created: 25.10.2017 14:07:59
 *  Author: ferdyow
 */ 


#ifndef TIMER_H_
#define TIMER_H_

void PWM_init();

//Makes the time module count up to a value such that it will have an ON time of the given value
void PWM_set_width(float on_time_ms);



#endif /* TIMER_H_ */