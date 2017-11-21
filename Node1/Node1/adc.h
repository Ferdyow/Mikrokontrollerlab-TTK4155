/*
 * adc.h
 *
 * Created: 13.09.2017 11:58:25
 *  Author: tobib
 */ 


#ifndef ADC_H_
#define ADC_H_

/**
 * Sets PINE0 to receive interrupt from ADC
 */
void ADC_init();

/**
 * Sets PINE0 to receive interrupt from ADC
 * @param {char} channel - Chooses which analog channel to read from
 * @returns {char} - The read value
 */
char ADC_read(char channel);


#endif /* ADC_H_ */