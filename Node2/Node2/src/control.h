/*
 * control.h
 *
 * Created: 15.11.2017 08:29:29
 *  Author: tobib
 */ 


#ifndef CONTROL_H_
#define CONTROL_H_


typedef enum {OPEN_LOOP, CLOSED_LOOP} control;

void CONTROL_init(void);

void CONTROL_run(control control_type);

#endif /* CONTROL_H_ */