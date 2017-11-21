/*
 * game.h
 *
 * Created: 14.11.2017 17:12:15
 *  Author: ferdyow
 */ 


#ifndef GAME_H_
#define GAME_H_

/**
 * Start the game. This mostly consists of sending control inputs and receiving score.
 */
void GAME_play(void);

/**
 * Print the high score on the OLED screen.
 */
void GAME_print_highscore(void);


#endif /* GAME_H_ */