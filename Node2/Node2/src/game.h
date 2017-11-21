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

void GAME_send_score(int score_ms);

void GAME_loop(void);

void GAME_init(control control_type);

void print_can_message(can_message* msg);

#endif /* GAME_H_ */