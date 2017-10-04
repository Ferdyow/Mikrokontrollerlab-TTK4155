/*
 * CAN.c
 *
 * Created: 04.10.2017 16:45:07
 *  Author: ferdyow
 */ 
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "MCP2515.h"


void CAN_init() {
	
	MCP2515_init();
	//set loopback mode: 0x40
	//later use normal mode 0x00
	MCP2515_write(MCP_CANCTRL,0xF0, 0x40);
		
	
	//enable interrupt
	
	//enable rollover
	
	//ctrl0 accept std or extended ID
	
	//rcr1 ^
	
	//set filter to 0 (accept everything)
	
	//set interrupt
	
	//setup ID
	MCP2515_write(MCP_TXB0CTRL, 0x00, 0x00);
	MCP2515_write(MCP_TXB0CTRL+8,0b1101000,0x00);
	
	//setup data length
	MCP2515_write(MCP_)
}

void CAN_message_send(&can_message msg) {

	
	
	
}

void CAN_error() {
	
}

void CAN_transmit_complete() {
	
}

void CAN_data_receive() {
	
}

void CAN_int_vect() {
	
}
