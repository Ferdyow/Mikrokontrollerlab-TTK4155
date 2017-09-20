/*
 * adc.h
 *
 * Created: 13.09.2017 11:58:25
 *  Author: tobib
 */ 


#ifndef ADC_H_
#define ADC_H_


//sets our "interrupt" pin on the ATmega162 to input
void ADC_init();


//Reads the value from the chosen channel and returns it
char ADC_read(char channel);

//tests the memory locations of the ADC
void ADC_test(void);


#endif /* ADC_H_ */