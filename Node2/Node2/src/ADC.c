/*
 * ADC.c
 *
 * Created: 01.11.2017 10:59:57
 *  Author: ferdyow
 */ 


#include <avr/io.h>

#include "defines.h"
#include "ADC.h"


void ADC_init(void){
	// Enable ADC
	set_bit(ADCSRA, ADEN); 
	
	// Using a prescaler of 128. (To determine frequency of successive approximations. ADPS bit in ADCSRA)
	set_bit(ADCSRA, ADPS2);
	set_bit(ADCSRA, ADPS1);
	set_bit(ADCSRA, ADPS0);
	
	// Voltage reference: AVCC with external capacitor at AREF pin [Table 26-3]
	set_bit(ADMUX, REFS0); 
	
}

uint16_t ADC_read(int channel){
	// Set bit 7 to 0, bit 6 to 1, bit 5 to 0, and let the rest be decided by the channel.
	ADMUX = 0x40 + channel;

	set_bit(ADCSRA, ADSC); // Start a conversion

	// Wait until conversion is complete
	while(test_bit(ADCSRA, ADSC));

	uint16_t adc_result = ADC;
	return adc_result;
}