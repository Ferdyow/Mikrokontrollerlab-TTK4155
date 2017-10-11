/*
 * OLED_driver.c
 *
 * Created: 20.09.2017 12:46:10
 *  Author: ferdyow
 */ 
#define F_CPU 4915200  // Clock frequency in Hz
#include "OLED_driver.h"
#include "usart.h"
#include "fonts.h"
#include "defines.h"

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#define OLED_PAGE_COUNT 8
#define OLED_COLUMN_COUNT 128
volatile char* data_address = OLED_DATA_ADDRESS;
volatile char* cmd_address = OLED_CMD_ADDRESS;

uint8_t FONT_SIZE = 8;



//hjelpefunksjoner
void write_cmd(uint8_t cmd); //write_command
void write_data(uint8_t data); //write_data
void OLED_print_arrow();


void write_cmd(uint8_t cmd){
	*cmd_address = cmd;
}

void write_data(uint8_t data){
	*data_address = data;
}

void OLED_init(){
	
	
	const char addresses[] = {
		0xae, //disable display
		0xa1, //segment remap
		0xda, //common pads hardware: alternative
		0x12,
		
		0xc8, //common output scan direction:com63~com0
		0xa8, //multiplex ration mode:63
		0x3f,
		
		0xd5, //display divide ratio/osc. freq. mode
		0x80,
		
		0x81, //contrast control
		0x50,
		
		0xd9, //set pre_charged period
		0x21,
		
		//set Memory adressing mode to pages
		0x20, 
		0x02,
		
		0xdb, //VCOM deselect level Mode
		0x30,
		
		0xad, //master configuration
		0x00,
		
		0xa4, //out follows RAM content
		0xa6, //set normal display
		0xaf //display on
	};
	
	const int addresses_length = sizeof(addresses)/sizeof(addresses[0]);
	for (int i = 0; i < addresses_length; i++) {
		write_cmd(addresses[i]);
	}
	
	OLED_reset();
	
}



void OLED_reset() {
	for(int page = 0; page < OLED_PAGE_COUNT; page++) {
		OLED_clear_line(page);
	}
	OLED_home();
}




void OLED_home(){
	OLED_pos(0,0);
}

void OLED_clear_line(uint8_t line) {
	OLED_pos(line,0);
	for (int column = 0; column < OLED_COLUMN_COUNT; column++) {
		write_data(0x00);
		
	}
}

void OLED_pos(uint8_t line, uint8_t column) {
	if (line < 0 || line > 7 || column < 0 || column > 127) {
		printf("Out of range in OLED_pos()");
		return; 
	}
	
	OLED_goto_line(line);
	OLED_goto_column(column);
}

void OLED_goto_line(uint8_t line){
	if (line < 0 || line > 7){
		return;
	}
	write_cmd(0xB0+line);
}

void OLED_goto_column(uint8_t column){
	if (column < 0 || column > 127){
		return;
	}
	write_cmd(0x00+column%16); //lower column register
	write_cmd(0x10+column/16); //higher column register

}

void OLED_set_brightness(uint8_t lvl){
	//if out of bounds set to min/max
	if (lvl < 0) lvl = 0;
	else if(lvl > 255) lvl = 255;
	
	write_cmd(0x81);
	write_cmd(lvl);
}

void OLED_print_char(char c){
	if (c == '\0') return;
	uint8_t ascii_starting_point = 32;
	uint16_t letter_index = c - ascii_starting_point;
	for (int col = 0; col < FONT_SIZE; col++){
		write_data(pgm_read_byte(&font8[letter_index][col]));
	}
}


//for testing purposes
void OLED_set_screen() {
	for(int page = 0; page < OLED_PAGE_COUNT; page++) {
		OLED_set_line(page);
	}
	OLED_home();
}

void OLED_set_line(uint8_t line) {
	OLED_pos(line,0);
	for (int column = 0; column < OLED_COLUMN_COUNT; column++) {
		write_data(0xFF);
		
	}
}

void OLED_test(){
	
	//test brightness
	for(int co = 0; co < 255; co += 50){
		OLED_set_brightness(co);
		OLED_set_screen();
		_delay_ms(100);
		
	}
	
	//test moving the pointer
	//write_data(0x11);
	//
	//OLED_goto_line(5);
	//write_data(0xFF);
	//
	//OLED_pos(3,100);
	//write_data(0x55);
	
	OLED_reset();
	//for (char c = 'a'; c <= 'z'; c++){
		//OLED_print_char(c);
	//}
	OLED_pos(1,0);
	OLED_print_arrow();
	fprintf(OLED, "%s", " Extra features");
	
	
}

void OLED_print_arrow(){
	write_data(0b00011000);
	write_data(0b00011000);
	write_data(0b01111110);
	write_data(0b00111100);
	write_data(0b00011000);
}