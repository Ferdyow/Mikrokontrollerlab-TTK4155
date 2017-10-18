/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
 
 #define F_CPU 4915200  // Clock frequency in Hz
 
#include <asf.h>


#include "usart.h"
#include "defines.h"
#include "CAN.h"
#include "MCP2515.h"
#include "SPI.h"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

void initialize(void){
	cli();
	usart_init(MYUBRR);
	CAN_init();
	//sei();
}

void test(void){
	can_message received_message;
	while(1){
		
		CAN_data_receive(&received_message);
		if(received_message.length){
			printf("\n\nRECEIVED:\n\nlength: %d\nid: %d\n", received_message.length, received_message.id);
			for (uint8_t byte = 0; byte < received_message.length;byte++){
				printf("Data nr. %d: %x\n", byte, received_message.data[byte]);
			}
		}
	}
}

void receive_joystick_pos(){
	can_message msg;
	msg.length = 0;
	printf("EFGL: 0x%02x\n", MCP2515_read(MCP_EFLG));
	while (!msg.length){
		CAN_data_receive(&msg);
		printf("EFGL: 0x%02x\n", MCP2515_read(MCP_EFLG));
	}
	printf("%d\n", msg.length);
	if(msg.length){
		printf("RECEIVED:\nlength: %d\nid: %d\n", msg.length, msg.id);
		printf("x: %d\ty:%d\n\n",msg.data[0],msg.data[1]);
	}
}

int main(void){
	initialize();
	printf("CANCTRL: 0x%02x\n", MCP2515_read(MCP_CANCTRL));
	printf("Usart funker.\n");
	receive_joystick_pos();
	
	//test();
	return 0;
}
