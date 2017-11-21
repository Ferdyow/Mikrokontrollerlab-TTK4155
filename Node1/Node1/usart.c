/*
 * usart.c
 *
 * Created: 06.09.2017 10:34:55
 *  Author: tobib
 */ 

#include "usart.h"

#include <avr/io.h>
#include <stdio.h>


void usart_init(unsigned int ubbr) {
	// Set baud rate 
	UBRR0H = (unsigned char)(ubbr>>8);
	UBRR0L = (unsigned char)(ubbr);
	
	// Enable receiver and transmitter 
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	
	// Set frame format: 8data, 2stop bit 
	UCSR0C = (1<<URSEL0) | (1<<USBS0) | (3<<UCSZ00);
	
	
	// Link printf to usart functions, so that it sends to/receives from PC
	fdevopen(usart_transmit_to_computer, usart_receive_from_computer);
}


void usart_transmit_to_computer(unsigned int data) {
	while (!(UCSR0A & (1<<UDRE0)));	// Wait for empty transmit buffer
	UDR0 = data;					// Put data into buffer (sends the data) 
}


unsigned char usart_receive_from_computer(void) {
	// Wait for data to be received 
	while (!(UCSR0A & (1<<RXC0)));
	
	// Get and return data from buffer 
	return UDR0;
}