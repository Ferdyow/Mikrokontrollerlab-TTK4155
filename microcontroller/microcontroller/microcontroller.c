/*
 * microcontroller.c
 *
 * Created: 06.09.2017 12:26:51
 *  Author: tobib
 */ 

#include "usart.h"

#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))
#define loop_until_bit_is_set( reg, bit ) while( !test_bit( reg, bit ) )
#define loop_until_bit_is_clear( reg, bit ) while( test_bit( reg, bit ) )

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>


void initialize(void) {
	usart_init(MYUBRR);
}


int main(void) {
	initialize();
	
	while(1) {
		printf("b");
		usart_transmit_to_computer('k');
		usart_transmit_to_computer('a');
		
		char recved = usart_receive_from_computer();
		printf(recved);
	}
}