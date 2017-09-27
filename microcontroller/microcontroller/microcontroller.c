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
#include "OLED_menu.h"
#include "sram.h"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>


void initialize(void){
	usart_init(MYUBRR);
	SRAM_init();
	ADC_init();
	JOY_init();
	OLED_init();
	MENU_init();
}


void test(void) {
	// SRAM_test();
	// ADC_test();
	// JOY_test();
	// OLED_test();
	//MENU_test();
	
}

void main_menu() {
	if (JOY_getDirection() == DOWN) {
		MENU_select_next();
	} else if (JOY_getDirection() == UP) { 
		MENU_select_prev();
	} 
	
	if (JOY_button_pressed(JOY_BUTTON)) {
		MENU_enter_selection();
	}
	
	/* FIXME: LEFT_BUTTON always pressed? */
	else if (JOY_button_pressed(LEFT_BUTTON)) {
		MENU_go_back();
	}
	// Make sure the selection doesn't move multiple places at once.
	_delay_ms(300);
}

int main(void) {
	initialize(); 	
	
	test();
	
	while(1) {
		main_menu();
	}
	
	return 0;
}