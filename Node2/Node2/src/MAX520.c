/*
 * MAX520.c
 *
 * Created: 08.11.2017 08:57:12
 *  Author: ferdyow
 */ 


#include "TWI_Master.h"
#include "MAX520.h"
#include "defines.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

// bit 4-7 are factory programmed
// bit 1-3 are connected to VDD or DGND (chosen)
// bit 0 is low since we want to send to the MAX520
#define MAX520_ADDRESS_BASE 0b01010000
#define MAX520_MESSAGE_SIZE 3


uint8_t max520_address = 0x00;


void MAX520_init(uint8_t three_bit_address){
	TWI_Master_Initialise(); //Initialize TWI for transferring
	sei();
	max520_address = MAX520_ADDRESS_BASE + (three_bit_address << 1); //Set the correct address for the TWI bus
}


// Channel must be between 0 and 3 (DAC0 - DAC3)
void MAX520_send(uint8_t channel, uint8_t data){
	//Message has the following format {address, command, output}
	uint8_t message[MAX520_MESSAGE_SIZE] = {
		max520_address, 
		0x00 + (channel << 1), 
		data
	};
	
	//Send the data over TWI
	TWI_Start_Transceiver_With_Data(message, MAX520_MESSAGE_SIZE);	
}