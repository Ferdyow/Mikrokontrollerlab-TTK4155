/*
 * CAN.c
 *
 * Created: 04.10.2017 16:45:07
 *  Author: ferdyow
 */
#define F_CPU 4915200  // Clock frequency in Hz

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "MCP2515.h"
#include "CAN.h"

#define NORMAL_MODE 0x00
#define LOOPBACK_MODE 0x40


volatile int flag_RX0 = 0;
volatile int flag_RX1 = 0;

// Interrupt service routine
ISR(INT0_vect){
	CAN_int_vect();
}

void CAN_int_vect() {
	uint8_t int_flags = MCP2515_read(MCP_CANINTF);
	if(int_flags & MCP_RX0IF){
		MCP2515_bit_modify(MCP_CANINTF, MCP_RX0IF, 0x00);
		flag_RX0 = 1;
	}
			
	if(int_flags & MCP_RX1IF){
		MCP2515_bit_modify(MCP_CANINTF, MCP_RX1IF, 0x00);
		flag_RX1 = 1;
	}
}


void CAN_init() {
	MCP2515_init();
	
	// Enable rollover: message will rollover to RX1 if RX0 is full
	// Also sets filter for RXB0 to accept all transmission
	MCP2515_bit_modify(MCP_RXB0CTRL, 0x64, 0xFF);  

	
	// Set filter for RXB1 to accept all transmission
	MCP2515_bit_modify(MCP_RXB1CTRL, 0x60, 0xFF);
	
	// Enable Received buffer register interrupts
	MCP2515_bit_modify(MCP_CANINTE,0x03, 0x03);
	
	// Set normal mode: 0x00
	MCP2515_bit_modify(MCP_CANCTRL,0xE0, NORMAL_MODE);

	
	

}


void CAN_print_message(can_message* msg) {
	if(msg->length == 0) return;
	printf("\nid: %d\tlength: %d\t DATA:\n", msg->id, msg->length);
	for(int i = 0; i < msg->length; i++) {
		printf("%x\t", msg->data[i]);
	}
	printf("\n");
}

void CAN_message_send(can_message* msg) {
	// Use only Transmit buffer register 0
	uint8_t buffer_numb = 0; 
		
	if(!CAN_transmit_complete(0)){
		return; //ERROR
	}


	// Set 11 bit ID
	uint8_t id_high = msg->id / 8;
	uint8_t id_low = msg->id % 8;
	

	MCP2515_write(MCP_TXB0SIDH + BUFFER_LENGTH * buffer_numb, id_high);
	MCP2515_write(MCP_TXB0SIDL + BUFFER_LENGTH * buffer_numb, (id_low << 5));

	// Transmit the number of data bytes
	MCP2515_write(MCP_TXB0DLC + BUFFER_LENGTH * buffer_numb, msg->length);

	// Transmit the data
	for (uint8_t byte = 0; byte < msg->length; byte++) {
		MCP2515_write((MCP_TXB0D0 + byte) + BUFFER_LENGTH * buffer_numb, msg->data[byte]);
	}
	
	uint8_t buffer_states = (uint8_t)(1 << buffer_numb);
	MCP2515_request_to_send(buffer_states);
	
	// Wait until transmission is complete
	while(!CAN_transmit_complete(0));
	
}

void CAN_error() {
	//printf("\n");
	//printf("\n| --------------------- |");
	//printf("\n| CAN_ERROR |");
	//printf("\n| --------------------- |");
	//printf("\n\n");
	//
	//if(MCP2515_read(MCP_TEC)){
		//printf("TRANSMIT ERROR");
		//
	//}
	//if(MCP2515_read(MCP_REC)){
		//printf("RECEIVED ERROR");
	//}
}


bool CAN_transmit_complete(int transmit_buffer_index) {
	const int address = MCP_TXB0CTRL + BUFFER_LENGTH * transmit_buffer_index;
	
	// Check if the TXREQ bit is clear, indication the transmit buffer is not pending transmission
	return !(MCP2515_read(address) & MCP_TXREQ);
}


void CAN_message_receive(can_message* received_msg){
	cli();
	int receive_buffer_index;
	if(flag_RX0){
		receive_buffer_index = 0;
		flag_RX0 = 0;
	}
	else if(flag_RX1){
		receive_buffer_index = 1;
		flag_RX1 = 0;
	}
	else{
		// No message was received
		received_msg->length = 0;
		sei();
		return;
	}
	
	// Set message ID
	uint8_t id_high = MCP2515_read(MCP_RXB0SIDH + BUFFER_LENGTH * receive_buffer_index);
	uint8_t id_low = MCP2515_read(MCP_RXB0SIDL + BUFFER_LENGTH * receive_buffer_index);
	
	// Only want the last 3 bits
	id_low = id_low >> 5;
	
	received_msg->id = (id_high << 3) + id_low; 
	
	// Read the data length, given by last 3 bits of the RXBnDLC register
	received_msg->length = (MCP2515_read(MCP_RXB0DLC + BUFFER_LENGTH * receive_buffer_index) % (1<<3));
	
	// Read the received data
	for (uint8_t byte = 0; byte < received_msg->length; byte++){
		int address = (MCP_RXB0D0 + byte) + BUFFER_LENGTH * receive_buffer_index;
		received_msg->data[byte] = MCP2515_read(address);
	}
	sei();
}


/* COMMENTED OUT TO SAVE MEMORY */

void CAN_test(){
	////TEST IN LOOPBACK MODE
	//MCP2515_bit_modify(MCP_CANCTRL, 0xE0, 0x40);
	////printf("\nCANSTAT before: %x\n", MCP2515_read(MCP_CANSTAT));
	//while(!CAN_transmit_complete(TB0)){}
	//can_message my_message;
	//can_message received_message;
	//my_message.id = 150;
	//my_message.length = 3;
	//my_message.data[0] = 0x00;
	//my_message.data[1] = 0xFF;
	//my_message.data[2] = 0x55;
	//CAN_message_send(&my_message);
	//printf("Before transmit complete\n");
	//int i = 0;
	//while(!CAN_transmit_complete(0))
		//;
	//printf("After transmit complete\n");
	//CAN_data_receive(&received_message);
	//printf("\n\nSENT:\nlength: %d\nid: %d\n", my_message.length, my_message.id);
	//for (uint8_t byte = 0; byte < my_message.length;byte++){
		//printf("Data nr. %d: %x\n", byte, my_message.data[byte]);
	//}
	//
	//
	//printf("\n\nRECEIVED:\n\nlength: %d\nid: %d\n", received_message.length, received_message.id);
	//for (uint8_t byte = 0; byte < received_message.length;byte++){
		//printf("Data nr. %d: %x\n", byte, received_message.data[byte]);
	//}
	//MCP2515_bit_modify(MCP_CANCTRL,0xE0, 0x00);
//
	//
	//
	//printf("ERROR FLAGS: %x\n", MCP2515_read(MCP_EFLG));
//
	//CAN_message_send(&my_message);
		//
	//printf("\nCANSTAT after: %x\n", MCP2515_read(MCP_CANSTAT));
	//printf("TXCTRL: %x\n", MCP2515_read(MCP_TXB0CTRL));
	//printf("ERROR FLAGS: %x\n", MCP2515_read(MCP_EFLG));
	//_delay_ms(500);
//
	//
	
	/*CAN_error();*/
}