/*
 * CAN.c
 *
 * Created: 04.10.2017 16:45:07
 *  Author: ferdyow
 */ 
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdbool.h>

#include "MCP2515.h"
#include "CAN.h"

#define BUFFER_LENGTH 16
#define TB0 0
#define TB1 1
#define TB2 2

void CAN_init() {
	
	MCP2515_init();
	//set loopback mode: 0x40
	//later use normal mode 0x00
	MCP2515_write(MCP_CANCTRL, 0x40);
		
	
	
	//enable rollover: message will rollover to RX1 if RX0 is full
	//also sets filter for RXB0 to only accept standard transmission
	MCP2515_write(MCP_RXB0CTRL, 0x24);  //0b 0010 0100

	
	//set filter for RXB1 to accept standard transmission
	MCP2515_write(MCP_RXB1CTRL, 0x20);
	

	//CANINTE contains the interrupt enable bits for each individual interrupt
	//CANINTF cointains the interrupt flags for each interrupt source. this should be cleared by a bit_modify
	
	MCP2515_write(MCP_CANINTE, 0x1F);
	//interrupts for  TX2, TX1, TX0, RX1, RX0 enabled
	

	
	

}

void CAN_message_send(can_message* msg) {
	//transmit is done using the TX registers, have to check which transmit_buffer_register we are writing from 
	uint8_t buffer_numb = 0; //Not sure how this logic is done yet


	//transmit the correct ID
	uint8_t id_high = msg->id / 8;
	uint8_t id_low = msg->id % 8;

	MCP2515_write(MCP_TXB0SIDH + BUFFER_LENGTH * buffer_numb, id_high);
	MCP2515_write(MCP_TXB0SIDL + BUFFER_LENGTH * buffer_numb, (id_low << 5));
	//resten av TXB0SIDL er 0 siden vi har standard identifier

	//transmit the data length
	MCP2515_write(MCP_TXB0DLC + BUFFER_LENGTH * buffer_numb, msg->length);

	//transmit the data
	for (uint8_t byte = 0; byte < msg->length; byte++) {
		MCP2515_write((MCP_TXB0D0 + byte) + BUFFER_LENGTH * buffer_numb, msg->data[byte]);
	}
	
	uint8_t buffer_states = (uint8_t)(1 << buffer_numb);
	printf("buffer_states: %d\n", buffer_states);
	MCP2515_request_to_send(buffer_states);
	
	
}

void CAN_error() {
	
}

// Interrupt når buffer blir fylt => buffer_available[i] = 0
// Interrupt når buffer blir sendt/tømt. => buffer_available[i] = 1
// Finn index første element i buffer_available som er 1 (ledig)


#define NUMBER_OF_BUFFERS 3
int buffer_available[NUMBER_OF_BUFFERS] = {1, 1, 1};
	
int first_free_transmit_buffer(void) {
	for (int i = 0; i < NUMBER_OF_BUFFERS; i++) {
		if (buffer_available[i]) {
			return i;
		}
	}
	return -1;
}

bool CAN_transmit_complete(int transmit_buffer_numb) {
	//if transmission is complete TXREQ is cleared
	//transmit buffer can be TX2, TX1, TX0
	uint8_t interrupt_flags = MCP2515_read(MCP_CANINTF); //0bxxx 1/0 xxx

	//identify the bits that are supposed to be zero
	uint8_t filter = 0x00 + (1 << (transmit_buffer_numb + 2)); //0b 0000 1 000

	//check if they are 1 or not
	if ((interrupt_flags | filter) != 0) {
		return true;
	}
	
	return false; 



}


void CAN_data_receive(int receive_buffer_numb, can_message* received_msg){
	uint8_t id_high = MCP2515_read(MCP_RXB0SIDH + BUFFER_LENGTH * receive_buffer_numb);
	uint8_t id_low = MCP2515_read(MCP_RXB0SIDL + BUFFER_LENGTH * receive_buffer_numb);
	
	//only want the last 3 bits
	id_low = id_low >> 5;
	
	/*
		id_low:	    X XXXX LLL
		id_high: HHHH HHHH
		id:		 HHHH HHHH LLL
	*/
	received_msg->id = (id_high << 3) + id_low; 
	
	//read the data length contained in the last 3 bits of the RXBnDLC register
	received_msg->length = (MCP2515_read(MCP_RXB0DLC + BUFFER_LENGTH * receive_buffer_numb) % (1<<3));
	
	//read the data
	for (uint8_t byte = 0; byte < received_msg->length; byte++){
		int address = (MCP_RXB0D0 + byte) + BUFFER_LENGTH * receive_buffer_numb;
		received_msg->data[byte] = MCP2515_read(address);
	}
	
	
	
	
}

void CAN_int_vect() {
	
}

void CAN_test(){
	while(!CAN_transmit_complete(TB0)){}
	
	can_message my_message;
	can_message received_message;
	my_message.id = 3;
	my_message.length = 3;
	my_message.data[0] = 0x00;
	my_message.data[1] = 0x00;
	my_message.data[2] = 0x00;
	
	CAN_message_send(&my_message);
	CAN_data_receive(0, &received_message);
	//printf("length of received message: %s\n",received_message.length);
	for (uint8_t byte = 0; byte < received_message.length;byte++){
		printf("%d", received_message.data[byte]);
	}
}