/*
 * motor.h
 *
 * Created: 08.11.2017 09:28:31
 *  Author: ferdyow
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

void motor_init(void);

/**
 * Set the velocity of the servo motor (sideways)
 * @param {int16_t} motor_velocity - The velocity of the motor in percentage between -100 and 100
 */
void motor_set_velocity(int16_t motor_speed);
int16_t motor_read_position_change(void);
int8_t motor_get_velocity(void);


#endif /* MOTOR_H_ */