/*
 * servo.h
 *
 * Created: 25.10.2017 15:55:19
 *  Author: ferdyow
 */ 


#ifndef SERVO_H_
#define SERVO_H_

void servo_init(void);

//Takes a percentage between -100 and 100 and converts it to PWM output between 0.9 and 2.1 ms
void servo_set(int8_t);



#endif /* SERVO_H_ */