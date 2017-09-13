/*
 * microcontroller.c
 *
 * Created: 06.09.2017 12:26:51
 *  Author: tobib
 */ 

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "usart.h"
#include "adc.h"
#include "defines.h"
#include "joy.h"

void initialize(void){
	usart_init(MYUBRR);
	SRAM_init();
	adc_init();
	joy_calibrate();
}


int main(void) {
	initialize(); 	
	//SRAM_test();
	//adc_test();
	
	int count=0;
	while(1) {
		 //printf("X-axis: %4d\t\tY-axis: %4d\n",adc_read(0),adc_read(1));
		 //printf("left slider: %4d\t\tright slider: %4d\n",adc_read(2),adc_read(3));
		 if(!test_bit(PINB,PINB2)){
			 printf("knapp er trykt inn\n");
		 }
	} return 1;
}