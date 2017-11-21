#include "CAN.h"
#include "joy.h"
#include "defines.h"

#include <util/delay.h>
#include <stdio.h>

volatile int score = 0;
volatile int highscore = 0;

void send_control_input(void);
void receive_score(void);

void game_play() {
	OLED_reset();
	fprintf(OLED, "time: %s", "0.0");
	fprintf(OLED, "\n%s \n", "lButton: quit");
	fprintf(OLED, "%s \n", "jButton: shoot");
	//fprintf(OLED, "%s ", "r_slider: angle\n");
	//fprintf(OLED, "%s ", "x-axis: move\n");
	_delay_ms(500);						//makes sure first press does not trigger solenoid
	
	//send start message
	can_message state;
	state.length = 0;
	state.id = 's';
	CAN_message_send(&state);
	
	while(!JOY_button_pressed(LEFT_BUTTON)){
		send_control_input();
		receive_score();
	}
	//send quit message
	state.id = 'q';
	CAN_message_send(&state);
	while(CAN_transmit_complete(0));
}

void send_control_input(void) {
	can_message control_input;
	control_input.id = 'c';
	control_input.length = 5;
	
	//printf("[joy_position]\n");
	JOY_position_t joy_position = JOY_getPosition();
	control_input.data[JOYSTICK_X] = joy_position.x;
	control_input.data[JOYSTICK_Y] = joy_position.y;
	
	//printf("[sli_position]\n");
	SLI_position_t sli_position = SLI_getPosition();
	control_input.data[SLIDER_LEFT] = sli_position.left;
	control_input.data[SLIDER_RIGHT] = sli_position.right;
	
	//printf("[BUTTONS]\n");
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
	
	CAN_message_receive(&score_message);
	//printf("score msg 0: %d \tscore msg 1: %d\n", score_message.data[0], score_message.data[1]);
	if(score_message.length){
	score = ((uint8_t)(score_message.data[0]) << 8) |(uint8_t) score_message.data[1];
	if (score > highscore){
		highscore = score;
	}
	OLED_reset_cursor();
	//printf("seconds %d.%d\n", score/10, score%10);
	fprintf(OLED, "time: %d.%d    ", score/10, score%10);
	//score_set(score_message.data[0]);
	}
	
	//int ir_disrupted = score_message.data[2];
	//if(ir_disrupted){
	//	set_highscore();
	//}
}

void print_highscore(){
	OLED_reset();
	//printf("HIGHSCORE:%d.%d seconds\n", highscore/10, highscore%10);
	fprintf(OLED, "   HIGHSCORE:\n  %d.%d seconds\n", highscore/10, highscore%10);
	fprintf(OLED, "\n\n\n\n %s", "lButton: back");
	while(!JOY_button_pressed(LEFT_BUTTON)){}
}