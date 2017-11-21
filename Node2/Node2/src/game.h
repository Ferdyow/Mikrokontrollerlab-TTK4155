/*
 * game.h
 *
 * Created: 19.11.2017 21:58:15
 *  Author: tobib
 */ 


#ifndef GAME_H_
#define GAME_H_
#include "CAN.h"

typedef enum {OPEN_LOOP, CLOSED_LOOP} control;

/**
 * Send game score over CAN bus to node 1.
 * @param {int} score_ms - The score to send.
 */
void GAME_send_score(int score_ms);

/**
 * Start the game loop. This is sort of the main loop for node 2.
 * Here, all the CAN messages are handled, and everything is controlled.
 */
void GAME_loop(void);

/**
 * Initialize the game timers, and choose wether you want 
 * open loop or closed loop control for the motor. 
 * The closed loop system is controlled using a PI controller.
 * @param {control} control_type - OPEN_LOOP or CLOSED_LOOP.
 */
void GAME_init(control control_type);



#endif /* GAME_H_ */