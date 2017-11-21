/*
 * solenoid.h
 *
 * Created: 15.11.2017 14:56:50
 *  Author: tobib
 */ 


#ifndef SOLENOID_H_
#define SOLENOID_H_

/************************************************************************/
/* TIMER 5A: Wait for the solenoid to charge after firing.              */
/* TIMER 5B: Wait before retracting solenoid after ejecting.            */
/************************************************************************/

#define TIMER_5A_DELAY ((double) 300)/1000  // s
#define TIMER_5B_DELAY ((double) 30)/1000   // s
#define TIMER5_PRESCALER 1024

/**
 * Initialize solenoid. Also set up timer interrupts on TIMER5{A,B}.
 * The solenoid starts up in a "charged" state
 */
void solenoid_init(void);

/**
 * Send a pulse if the solenoid has been charged up. Otherwise do nothing.
 */
void solenoid_send_pulse(void);


#endif /* SOLENOID_H_ */