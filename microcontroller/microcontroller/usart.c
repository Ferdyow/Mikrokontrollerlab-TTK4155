/*
 * usart.c
 *
 * Created: 06.09.2017 10:34:55
 *  Author: tobib
 */ 

#include "usart.h"
#include <avr/io.h>

void usart_init(unsigned int ubbr) {
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubbr>>8);
	UBRR0L = (unsigned char)(ubbr);
	
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0) | (1<<USBS0) | (3<<UCSZ00);
	
	
	/* Enable printf-thing */
	fdevopen(usart_transmit_to_computer, usart_receive_from_computer);
}


void usart_transmit_to_computer(unsigned int data) {
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1<<UDRE0)))
	;
	
	/* Put data into buffer (sends the data) */
	UDR0 = data;
}


unsigned char usart_receive_from_computer(void) {
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)))
	;
	
	/* Get and return data from buffer */
	return UDR0;
}
