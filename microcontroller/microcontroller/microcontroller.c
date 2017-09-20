/*
 * microcontroller.c
 *
 * Created: 06.09.2017 12:26:51
 *  Author: tobib
 */ 

#include "usart.h"
#include "adc.h"
#include "defines.h"
#include "joy.h"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

void initialize(void){
	usart_init(MYUBRR);
	SRAM_init();
	ADC_init();
	//JOY_init();
	OLED_init();
}


int main(void) {
	initialize(); 	
	//SRAM_test();
	//ADC_test();
	//JOY_test();

	while(1) {
		 
	} return 0;
}