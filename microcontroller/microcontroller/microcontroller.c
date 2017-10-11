/*
 * microcontroller.c
 *
 * Created: 06.09.2017 12:26:51
 *  Author: tobib
 */ 

#define F_CPU 4915200  // Clock frequency in Hz

#include "usart.h"
#include "adc.h"
#include "defines.h"
#include "joy.h"
#include "OLED_driver.h"
#include "MENU.h"
#include "sram.h"
#include "MCP2515.h"
#include "CAN.h"
#include "SPI.h"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


void initialize(void){
	cli();
	OLED_reset();
	usart_init(MYUBRR);
	SRAM_init();
	ADC_init();
	//JOY_init();
	OLED_init();
	MENU_init();
	CAN_init();
	sei();
}


void test(void) {
	// SRAM_test();
	// ADC_test();
	// JOY_test();
	// OLED_test();
	//MENU_test();
	//SPI_test();
	CAN_test();
	
	
}



int main(void) {
	initialize(); 	
	test();
	while(1) {
		//test();
		MENU_run();
	}
	
	return 0;
}