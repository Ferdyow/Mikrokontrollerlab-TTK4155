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

#define FONT_SIZE 8
#define OLED_PAGE_COUNT 8
#define OLED_COLUMN_COUNT 128
volatile char* data_address = OLED_DATA_ADDRESS;
volatile char* cmd_address = OLED_CMD_ADDRESS;
volatile int current_line = 0;
volatile int current_column = 0;

/************************************************************************/
/* HELPING FUNCTION DECLARATIONS                                        */
/************************************************************************/

/**
 * Write a command to the OLED controller.
 */
void write_cmd(uint8_t cmd); //write_command

/**
 * Write data to the OLED display RAM.
 */
void write_data(uint8_t data); //write_data

/**
 * Set all RAM-bits of OLED display to 0. (Making all pixels dark)
 */
void OLED_clear_screen(void);

/**
 * Draw an arrow on the OLED display.
 */
void OLED_print_arrow(void);

/************************************************************************/
/* FUNCTION IMPLEMENTATIONS                                             */
/************************************************************************/
void write_cmd(uint8_t cmd){
	*cmd_address = cmd;
}

void write_data(uint8_t data){
	*data_address = data;
}

void OLED_init(){
	const char commands[] = {
		0xAE, // Display off
		0xA1, // Segment remap
		0xDA, // Common pads hardware: alternative
		0x12,
		
		0xC8, // Common output scan direction:com63~com0
		0xA8, // Multiplex ration mode:63
		0x3F,
		
		0xD5, // Display divide ratio/osc. freq. mode
		0x80,
		
		0x81, // Contrast control
		0x50,
		
		0xD9, // Set pre_charged period
		0x21,
		
		0xA7, // Invert display
		
		0x20, // Set Memory adressing mode to pages
		0x02,
		
		0xDB, // VCOM deselect level Mode
		0x30,
		
		0xAD, // Master configuration
		0x00,
		
		0xA4, // Out follows RAM content
		0xA6, // Set normal display
		0xAF  // Display on
	};
	
	const int command_count = sizeof(commands)/sizeof(commands[0]);
	for (int i = 0; i < command_count; i++) {
		write_cmd(commands[i]);
	}
	OLED_reset();
}

void OLED_reset() {
	OLED_clear_screen();
	OLED_reset_cursor();
}

void OLED_reset_cursor(){
	OLED_pos(0,0);
}

void OLED_clear_screen() {
	for(int page = 0; page < OLED_PAGE_COUNT; page++) {
		OLED_clear_line(page);
	}
}

void OLED_clear_line(uint8_t line) {
	OLED_pos(line, 0);
	for (int column = 0; column < OLED_COLUMN_COUNT; column++) {
		write_data(0x00);
	}
}

void OLED_pos(uint8_t line, uint8_t column) {
	if (line < 0 || line > 7 || column < 0 || column > 127) {
		printf("Out of range in OLED_pos()\n");
		return; 
	}
	
	OLED_goto_line(line);
	OLED_goto_column(column);
}

void OLED_goto_line(uint8_t line){
	if (line < 0 || line > 7) 
		return;
	
	write_cmd(0xB0 + line);
	current_line = line;
}

void OLED_goto_column(uint8_t column){
	if (column < 0 || column > 127) 
		return;
	
	write_cmd(0x00 + column%16); // Lower column register
	write_cmd(0x10 + column/16); // Higher column register
	current_column = column;
}

void OLED_set_brightness(uint8_t brightness_level){
	write_cmd(0x81);
	write_cmd(brightness_level);
}

void OLED_print_char(char character){
	uint8_t ascii_starting_point = 32;
	uint16_t letter_index = character - ascii_starting_point;
	
	switch (character) {
		case '\0'  :
		break;
		
		case '\n'  :  
		// Reset cursor and move it to the next line
		OLED_pos(current_line + 1, 0);  
		break;
		
		case '\r'  :  
		// Reset cursor to start of current line
		OLED_pos(current_line, 0);  
		break;
		
		default :
		// Print character using the included fonts
		for (int col = 0; col < FONT_SIZE; col++){
			write_data(pgm_read_byte(&font8[letter_index][col]));
		}
	}
}

void OLED_print_char_inverse(char c) {
	if (c == '\0') return;
	uint8_t ascii_starting_point = 32;
	uint16_t letter_index = c - ascii_starting_point;
	for (int col = 0; col < FONT_SIZE; col++) {
		write_data(~pgm_read_byte(&font8[letter_index][col]));
	}
}



//for testing purposes
void OLED_set_screen() {
	for (int page = 0; page < OLED_PAGE_COUNT; page++) {
		OLED_set_line(page);
	}
	OLED_reset_cursor();
}

void OLED_set_line(uint8_t line) {
	OLED_pos(line,0);
	for (int column = 0; column < OLED_COLUMN_COUNT; column++) {
		write_data(0xFF);
	}
}

void OLED_test() {
	// Test brightness
	for (int co = 0; co < 255; co += 50){
		OLED_set_brightness(co);
		OLED_set_screen();
		_delay_ms(500);
		
	}
	
	OLED_reset();
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

