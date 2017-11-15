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

//dont change ADMUX until one ADC clock cycle after ADSC is written (should be met by using a wait loop in this function)
uint16_t ADC_read(int channel){
	// Set bit 7 to 0, bit 6 to 1, bit 5 to 0, and let the rest be decided by the channel.
	// 010X XXXX
	
	ADMUX = 0x40 + channel;

	set_bit(ADCSRA, ADSC); //Start a conversion

	//wait til conversion is complete
	while(test_bit(ADCSRA, ADSC))
		;

	//uint16_t adc_result = (ADCH << 8) + ADCL; //ADC could possibly be accessed directly
	uint16_t adc_result = ADC;
	//check <avr/io.h> for details
	return adc_result;
}






/* TODO:
 *
 * 10-bit resolution
 *
 * 8 available channels on Port F.
 *
 * Manual configuration
 * - Use AVCC as reference
 * - Use single conversion
 * - The ADC's operation is well described in the datasheet.
 *
 * Use digital filering: y_{n} = (1/4)(x_{n} + x_{n-1} + x_{n-2} + x_{n-3})
 *
 */


// Threshold - at what point do we consider the ball to be missing?
// Filtration amount - how large is the moving average?