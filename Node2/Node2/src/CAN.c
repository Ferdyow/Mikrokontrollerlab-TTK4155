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
#include <avr/interrupt.h>

#include "defines.h"
#include "MCP2515.h"
#include "CAN.h"

#include <util/delay.h>


#define BUFFER_LENGTH 16

volatile int flag_RX0 = 0;
volatile int flag_RX1 = 0;


//test functions
void test_usart_communication(void);



//interrupt service routine when message is received
ISR(INT5_vect){
	//interrupt when a message is received
	CAN_int_vect();
}


void CAN_int_vect(void) {
	cli();
	uint8_t int_flags = MCP2515_read(MCP_CANINTF);
	//printf("INTERRUPT VECTOR CALLED\Nflags: %x\n",int_flags);
	if(int_flags & MCP_RX0IF){
		MCP2515_bit_modify(MCP_CANINTF, MCP_RX0IF, 0x00);
		flag_RX0 = 1;
	}		
	if(int_flags & MCP_RX1IF){
		MCP2515_bit_modify(MCP_CANINTF, MCP_RX1IF, 0x00);
		flag_RX1 = 1;
	}
	sei();
}

//hex to binary is left as an exercise to the reader :)
void CAN_init(void) {
	//global interrupt enable
	clear_bit(EIMSK, INT5);
	
	//interrupt when low (done by default
	clear_bit(EICRB, ISC51);
	clear_bit(EICRB, ISC50);
	
	//enable interrupt on ATmega2560
	set_bit(EIMSK, INT5);
	
	
	//interrupt when low (egentlig default satt)
	//clear_bit(MCUCR,ISC01);
	//clear_bit(MCUCR, ISC00);
	
	
	MCP2515_init();
	
	//printf("CANSTAT in init: 0x%02x\n", MCP2515_read(MCP_CANSTAT));
	//enable rollover: message will rollover to RX1 if RX0 is full
	//also sets filter for RXB0 to accept all transmission
	MCP2515_bit_modify(MCP_RXB0CTRL, 0x64, 0xFF);  //0b 0010 0100

	
	//set filter for RXB1 to accept all transmission
	MCP2515_bit_modify(MCP_RXB1CTRL, 0x60, 0xFF);
	

	//CANINTE contains the interrupt enable bits for each individual interrupt
	//CANINTF cointains the interrupt flags for each interrupt source. this should be cleared by a bit_modify
	
	//MCP2515_bit_modify(MCP_CANINTE,0x03, 0x03);
	MCP2515_write(MCP_CANINTE, 0x03);
	//printf("CANINTE: 0x%02x\n", MCP2515_read(MCP_CANINTE));
	//interrupts for RX1, RX0 enabled
	
	//set loopback mode: 0x40
	//later use normal mode 0x00
	MCP2515_bit_modify(MCP_CANCTRL,0xE0, 0x00);

	//printf("CANSTAT: 0x%02x\n", MCP2515_read(MCP_CANSTAT));
}

void CAN_message_send(can_message* msg) {
	
	//transmit is done using the TX registers, have to check which transmit_buffer_register we are writing from 
	uint8_t buffer_numb = 0; //Not sure how this logic is done yet
	//if (!CAN_transmit_complete(buffer_numb)){
		//printf("[NODE2][CAN_message_send] Noe har gått galt");
		//return;
	//}
	if(!CAN_transmit_complete(0)){
		return; //ERROR
	}

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
	MCP2515_request_to_send(buffer_states);
	
}

void CAN_error(void) {
	printf("\n");
	printf("\n| --------------------- |");
	printf("\n| CAN_ERROR |");
	printf("\n| --------------------- |");
	printf("\n\n");
	
	if(MCP2515_read(MCP_TEC)){
		printf("TRANSMIT ERROR");
		
	}
	if(MCP2515_read(MCP_REC)){
		printf("RECEIVE ERROR");
	}
	
	
}


int CAN_transmit_complete(transmit_buffer tb) {
	//printf("CAN_TRANSMIT_COMPLETE\n");
	const int address = MCP_TXB0CTRL + BUFFER_LENGTH * tb;
	return !(MCP2515_read(address) & MCP_TXREQ);
}


void CAN_message_receive(can_message* received_msg){
	cli();
	int receive_buffer_index = 0;
	if(flag_RX0){
		receive_buffer_index = 0;
		flag_RX0 = 0;
	}
	else if(flag_RX1){
		receive_buffer_index = 1;
		flag_RX1 = 0;
	}
	else{
		//printf("ELSE: received_msg->id:  %d\n", received_msg->id);
		received_msg->length = 0;
		received_msg->id = 0;
		sei();
		return;
	}
	//prevents crashing
	//_delay_ms(10);

	uint8_t id_high = MCP2515_read(MCP_RXB0SIDH + BUFFER_LENGTH * receive_buffer_index);
	uint8_t id_low = MCP2515_read(MCP_RXB0SIDL + BUFFER_LENGTH * receive_buffer_index);
	
	//only want the last 3 bits
	id_low = id_low >> 5;
	
	/*
		id_low:	    X XXXX LLL
		id_high: HHHH HHHH
		id:		 HHHH HHHH LLL
	*/
	received_msg->id = (id_high << 3) + id_low; 
	
	//read the data length contained in the last 3 bits of the RXBnDLC register
	received_msg->length = (MCP2515_read(MCP_RXB0DLC + BUFFER_LENGTH * receive_buffer_index) % (1<<3));
	
	//read the data
	for (uint8_t byte = 0; byte < received_msg->length; byte++){
		int address = (MCP_RXB0D0 + byte) + BUFFER_LENGTH * receive_buffer_index;
		received_msg->data[byte] = MCP2515_read(address);
	}
	sei();
}


void test_usart_communication(void){
	int received_data;
	for (int data = 0x0; data <= 0x10; data++) {
		MCP2515_write(MCP_CANCTRL, data);
		received_data = MCP2515_read(MCP_CANCTRL);
		if (received_data == data){
			printf("great success!\n");
		}
		else{
			printf("for fucks sake\n");
		}
		printf("data sent: 0x%02x\ndata received: 0x%02x\n\n", data, received_data);
		
	}
}

void CAN_test(void){
	//test_usart_communication();
	
	//TEST IN LOOPBACK MODE
	MCP2515_bit_modify(MCP_CANCTRL, 0xE0, 0x40);
	printf("\nCANSTAT before: %x\n", MCP2515_read(MCP_CANSTAT));
	while(!CAN_transmit_complete(TRANSMIT_BUFFER_0)){}
	can_message my_message;
	can_message received_message;
	received_message.length = 0;
	my_message.id = 150;
	my_message.length = 3;
	my_message.data[0] = 0x00;
	my_message.data[1] = 0xFF;
	my_message.data[2] = 0x55;
	CAN_message_send(&my_message);
	printf("Before transmit complete\n");
	while(!CAN_transmit_complete(0)) {
	} 
	printf("After transmit complete\n");
	CAN_message_receive(&received_message);
	
	
	printf("\n\nSENT:\nlength: %d\nid: %d\n", my_message.length, my_message.id);
	for (uint8_t byte = 0; byte < my_message.length;byte++){
		printf("Data nr. %d: %x\n", byte, my_message.data[byte]);
	}
	printf("\n\nRECEIVED:\n\nlength: %d\nid: %d\n", received_message.length, received_message.id);
	for (uint8_t byte = 0; byte < received_message.length;byte++){
		printf("Data nr. %d: %x\n", byte, received_message.data[byte]);
	}
	MCP2515_bit_modify(MCP_CANCTRL,0xE0, 0x00);
	
	printf("ERROR FLAGS: %x\n", MCP2515_read(MCP_EFLG));
	
}
