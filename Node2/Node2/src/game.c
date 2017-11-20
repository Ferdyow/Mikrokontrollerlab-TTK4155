/*
 * game.c
 *
 * Created: 19.11.2017 21:58:02
 *  Author: tobib
 */ 

#include <stdio.h>

#include "CAN.h"

/*struct {
	int8_t joystick_x;
	int8_t joystick_y;
	uint8_t slider_left;
	uint8_t slider_right;
	bool joy_button;
	bool left_button;
	bool right_button;
} controls;

void game_timer_init(void);

void game_timer_init(void) {
	
}
*/

void GAME_send_highscore(int score_ms) {
	//printf("seconds: %d.%d \n", score_ms/10, score_ms%10);
	can_message highscore;
	highscore.id = 'h';
	highscore.length = 3;
	highscore.data[0] = score_ms >> 8;
	highscore.data[1] = score_ms & 0xFF;
	CAN_message_send(&highscore);
	while(!CAN_transmit_complete(TRANSMIT_BUFFER_0));
}


void GAME_loop(void) {
	
}