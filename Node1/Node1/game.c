#include "CAN.h"
#include "joy.h"
#include "defines.h"

#include <util/delay.h>
#include <stdio.h>

volatile int score = 0;
volatile int highscore = 0;
volatile int last_score = 0;

void send_control_input(void);
void receive_score(void);

void GAME_play() {
	OLED_reset();
	fprintf(OLED, "score: %s", "0.0");
	fprintf(OLED, "\n%s \n", "lButton: quit");
	fprintf(OLED, "%s \n", "jButton: shoot");
	
	// Make sure first press does not trigger solenoid
	_delay_ms(500);						
	
	//send start message
	can_message state;
	state.length = 1;
	state.id = 's';
	state.data[0] = 0xFF;
	CAN_message_send(&state);
	
	while(!JOY_button_pressed(LEFT_BUTTON)){
		send_control_input();
		receive_score();
	}
	//send quit message

	state.id = 'q';
	CAN_message_send(&state);
}

void send_control_input(void) {
	can_message control_input;
	control_input.id = 'c';
	control_input.length = 5;
	
	JOY_position_t joy_position = JOY_getPosition();
	control_input.data[JOYSTICK_X] = joy_position.x;
	control_input.data[JOYSTICK_Y] = joy_position.y;
	
	SLI_position_t sli_position = SLI_getPosition();
	control_input.data[SLIDER_LEFT] = sli_position.left;
	control_input.data[SLIDER_RIGHT] = sli_position.right;
	
	int joy_button = JOY_button_pressed(JOY_BUTTON) << JOY_BUTTON;
	int left_button = JOY_button_pressed(LEFT_BUTTON) << LEFT_BUTTON;
	int right_button = JOY_button_pressed(RIGHT_BUTTON) << RIGHT_BUTTON;
	control_input.data[BUTTONS] = joy_button | left_button | right_button;
	
	CAN_message_send(&control_input);
	//while(!CAN_transmit_complete(TB0));
}

void receive_score(void){
	can_message score_message;
	score_message.length = 0;
	int new_score = 0;
	
	CAN_message_receive(&score_message);
	CAN_print_message(&score_message);
	if(score_message.length && score_message.id == 'h'){
		
		new_score = ((uint8_t)(score_message.data[0]) << 8) |(uint8_t) score_message.data[1];
		if (new_score <= score+10 && new_score >= score){
			score = new_score;

		}
		else if(new_score == 0 && score != 0){
			last_score = score;
			score = new_score;
			OLED_goto_pos(7,0);
			fprintf(OLED, "last score:%d.%d", last_score/10, last_score%10);
		}
		if (score > highscore){
			highscore = score;
		}
		OLED_reset_cursor();
		fprintf(OLED, "score: %d.%d    ", score/10, score%10);
	}
	
}

void GAME_print_highscore(){
	OLED_reset();
	fprintf(OLED, "   HIGHSCORE:\n  %d.%d seconds\n", highscore/10, highscore%10);
	fprintf(OLED, "\n\n\n\n %s", "lButton: back");
	while(!JOY_button_pressed(LEFT_BUTTON)){}
}