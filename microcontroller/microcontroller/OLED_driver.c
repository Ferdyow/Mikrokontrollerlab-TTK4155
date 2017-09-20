/*
 * OLED_driver.c
 *
 * Created: 20.09.2017 12:46:10
 *  Author: ferdyow
 */ 

#include "OLED_driver.h"
#include "usart.h"

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#define OLED_PAGE_COUNT 8
#define OLED_COLUMN_COUNT 128
volatile char* data_address = OLED_DATA_ADDRESS;
volatile char* cmd_address = OLED_CMD_ADDRESS;

//hjelpefunksjoner
void write_cmd(uint8_t cmd); //write_command
void write_data(uint8_t data); //write_data


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
	
	for (int i = 0; i < sizeof(addresses)/sizeof(addresses[0]); i++) {
		write_cmd(addresses[i]);
	}
	
	OLED_reset();
	for (int i=0; i<127;i++){
		OLED_pos(0,i);
	}

		

	
}



void OLED_reset() {
	for(int page = 0; page < OLED_PAGE_COUNT; page++) {
		OLED_clear_line(page);
	}
}


void OLED_clear_line(int line) {
	OLED_pos(line,0);
	for (int column = 0; column < OLED_COLUMN_COUNT; column++) {
		write_data(0x00);
		
	}
}

void OLED_pos(int line, int column) {
	if (line < 0 || line > 7 || column < 0 || column > 127) {
		printf("Out of range in OLED_pos()");
		return; 
	}

	//velg column
	printf("%2x%2x\t",0x10+column/16, 0x00+column%16);
	write_cmd(0x00+column%16); //lower colum
	write_cmd(0x10+column/16); //higher column
	
	//velg line
	printf("%2x\n",0xB0+line);
	write_cmd(0xB0+line);
}

