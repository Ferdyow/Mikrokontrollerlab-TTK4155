#include "CAN.h"
#include "joy.h"
#include "defines.h"


void game_play() {
	can_message msg;
	msg.id = 0;
	msg.length = 3;
	JOY_position_t pos;

	pos = JOY_getPosition();
	msg.data[0] = pos.x;
	msg.data[1] = pos.y;

	//contains 000 0 JOYSTICK RIGHT LEFT button
	msg.data[2] = JOY_button_pressed(JOY_BUTTON) << JOY_BUTTON | JOY_button_pressed(RIGHT_BUTTON) << RIGHT_BUTTON | JOY_button_pressed(LEFT_BUTTON) << LEFT_BUTTON;
	printf("SENDING:\nx: %d\ty:%d \tbuttons: %d	\n\n", pos.x, pos.y, msg.data[2]);
	//printf("BUTTONS: \t%2d\t\t%2d\t\t%2d\n", test_bit(PINB, PINB0), test_bit(PINB, PINB1), !test_bit(PINB, PINB2));
	CAN_message_send(&msg);
	while (!CAN_transmit_complete(TB0))
		;
	//printf("1. EFGL: 0x%02x\n", MCP2515_read(MCP_EFLG));
}