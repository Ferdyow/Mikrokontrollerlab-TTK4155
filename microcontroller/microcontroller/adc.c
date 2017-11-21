/*
 * adc.c
 *
 * Created: 13.09.2017 11:58:01
 *  Author: tobib
 */ 
#include "defines.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>


void ADC_init(){
	// Set interrupt pin to input
	clear_bit(DDRE, DDE0);
}

char ADC_read(char channel){
	// Start address for the adc
	volatile char *adc = (char *) 0x1400;

	// Invalid input
	if(channel > 3) return 0;
	
	// Choose channel (1-4) - single ended
	*adc = 0x4 + channel; 
	
	// Wait until /INTR is low (triggered by ADC receiving data)
	loop_until_bit_is_clear(PINE, PINE0);
	
	// Return the read value
	return *adc; 
}