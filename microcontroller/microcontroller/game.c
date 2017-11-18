#include "CAN.h"
#include "joy.h"
#include "defines.h"

#include <util/delay.h>

void game_play() {
	OLED_reset();
	fprintf(OLED, "%s \n", "lButton: quit");
	fprintf(OLED, "%s \n", "jButton: shoot");
	//fprintf(OLED, "%s ", "r_slider: angle\n");
	//fprintf(OLED, "%s ", "x-axis: move\n");
	_delay_ms(1000);
	while(!JOY_button_pressed(LEFT_BUTTON)){
		send_control_input();
	}
}

void send_control_input(void) {
	can_message control_input;
	control_input.id = 0;
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
	while(!CAN_transmit_complete(TB0))
	;
}