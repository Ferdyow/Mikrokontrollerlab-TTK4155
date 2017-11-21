/*
 * ADC.h
 *
 * Created: 01.11.2017 11:00:11
 *  Author: ferdyow
 */ 


#ifndef ADC_H_
#define ADC_H_

/**
 * Sets PINE0 to receive interrupt from ADC
 */
void ADC_init(void);

/**
 * Sets PINE0 to receive interrupt from ADC
 * @param {int} channel - Chooses which analog channel to read from
 * @returns {uint8_t} - The read value
 */
uint16_t ADC_read(int channel);


#endif /* ADC_H_ */