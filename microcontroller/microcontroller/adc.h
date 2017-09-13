/*
 * adc.h
 *
 * Created: 13.09.2017 11:58:25
 *  Author: tobib
 */ 


#ifndef ADC_H_
#define ADC_H_

void adc_init();
char adc_read(char channel);
void adc_test(void);


#endif /* ADC_H_ */