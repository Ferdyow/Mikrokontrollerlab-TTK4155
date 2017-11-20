/*
 * microcontroller.c
 *
 * Created: 06.09.2017 12:26:51
 *  Author: tobib
 */ 

#define F_CPU 4915200  // Clock frequency in Hz

#include "defines.h"
#include "usart.h"
#include "sram.h"
#include "adc.h"
#include "joy.h"
#include "OLED_driver.h"
#include "MENU.h"
#include "MCP2515.h"
#include "CAN.h"
#include "SPI.h"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

void send_control_input(void);

void initialize(void){
	cli();
	usart_init(MYUBRR); 
	SRAM_init();
	ADC_init();
	JOY_init(CALIBRATION_DEFAULT);
	OLED_init();
	MENU_init();
	CAN_init();
	sei();
}


void test(void){
	//SRAM_test();
	//ADC_test(); //skal ikke funke
	//JOY_test();
	//OLED_test();
	//MENU_test();
	//SPI_test();
	//CAN_test(); //resets, bad-interrupt
	
	can_message my_message;
	my_message.id = 150;
	my_message.length = 3;
	my_message.data[0] = 0x00;
	my_message.data[1] = 0xFF;
	my_message.data[2] = 0x55;
	
	while(1) {
		while(!CAN_transmit_complete(TB0)){}
		CAN_message_send(&my_message);
		MENU_run();
	}
	
	
}



int main(void) {
	initialize();
	printf("Initialization of Node 1 complete!\n\n");
	while(1) {
		MENU_run();
	}

	return 0;
}