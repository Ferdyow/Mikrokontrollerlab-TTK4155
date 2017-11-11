/*
 * microcontroller.c
 *
 * Created: 06.09.2017 12:26:51
 *  Author: tobib
 */ 

#define F_CPU 4915200  // Clock frequency in Hz

#include "defines.h"
#include "usart.h"
#include "sram.h"
#include "adc.h"
#include "joy.h"
#include "OLED_driver.h"
#include "MENU.h"
#include "MCP2515.h"
#include "CAN.h"
#include "SPI.h"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


void initialize(void){
	cli();
	OLED_reset();
	usart_init(MYUBRR);
	SRAM_init();
	ADC_init();
	JOY_init();
	OLED_init();
	//MENU_init();
	CAN_init();
	sei();
	
	
}


void test(void){
	//SRAM_test();
	//ADC_test(); //skal ikke funke
	//JOY_test();
	//OLED_test();
	//MENU_test();
	//SPI_test();
	//CAN_test(); //resets, bad-interrupt
	
	can_message my_message;
	my_message.id = 150;
	my_message.length = 3;
	my_message.data[0] = 0x00;
	my_message.data[1] = 0xFF;
	my_message.data[2] = 0x55;
	
	while(1) {
		while(!CAN_transmit_complete(TB0)){}
		CAN_message_send(&my_message);
		MENU_run();
	}
	
	
}

void send_joystick_pos(){
	can_message msg;
	msg.id  = 0;
	msg.length = 3;
	JOY_position_t pos;
	
	pos = JOY_getPosition();
	msg.data[0] = pos.x;
	msg.data[1] = pos.y;
	
	//contains 000 0 JOYSTICK RIGHT LEFT button
	msg.data[2] = JOY_button_pressed(JOY_BUTTON) << JOY_BUTTON | JOY_button_pressed(RIGHT_BUTTON) << RIGHT_BUTTON  | JOY_button_pressed(LEFT_BUTTON) << LEFT_BUTTON;
	printf("SENDING:\nx: %d\ty:%d \tbuttons: %d	\n\n", pos.x, pos.y, msg.data[2]);
	//printf("BUTTONS: \t%2d\t\t%2d\t\t%2d\n", test_bit(PINB, PINB0), test_bit(PINB, PINB1), !test_bit(PINB, PINB2));
	CAN_message_send(&msg);
	while(!CAN_transmit_complete(TB0))
		;
	//printf("1. EFGL: 0x%02x\n", MCP2515_read(MCP_EFLG));
	
}

int main(void) {
	initialize();
	while(1){
		//MENU_run();
		send_joystick_pos();
	}

	//test();
	//CAN_test();
	
	return 0;
}