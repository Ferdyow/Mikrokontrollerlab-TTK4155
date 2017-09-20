/*
 * adc.c
 *
 * Created: 13.09.2017 11:58:01
 *  Author: tobib
 */ 
#include "defines.h"

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>


 //UNUSED
/*
volatile int inter;
ISR(INT2_vect){
	inter++;
	printf("hei bæsj!\n");
}*/

void ADC_init(){
	//set interrupt pin to input
	clear_bit(DDRE,DDE0);
	
}


void ADC_test(void){
	volatile char *adc = (char *) 0x1400; // Start address for the SRAM
	uint16_t adc_size = 0x400;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	uint16_t total1 = 0;
	uint16_t total2 = 0;
	printf("Starting ADC test...\n");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < adc_size; i++) {
		uint8_t some_value = rand();
		adc[i] = some_value;
		uint8_t retreived_value = adc[i];
		total1++;
		if (retreived_value != some_value) {
			//printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed); // reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < adc_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = adc[i];
		total2++;
		if (retreived_value != some_value) {
			//printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n",i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("ADC test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase \n(of %4d and %4d)\n", write_errors, retrieval_errors,total1,total2);
}

char ADC_read(char channel){
	//start adress for the adc
	volatile char *adc = (char *) 0x1400;

	//invalid input
	if(channel > 3) return 0;
	
	//choose channel (1-4) - single ended
	*adc = 0x4 + channel; 
	
	//wait 'til /INTR is low
	 while(test_bit(PINE, PINE0));
	
	 /*
	 interrupt functionality (currently not working)
	 //while(!inter)
	 
	//reset interrupt
	inter = 0;
	*/

	//return current value
	return *adc; 
}