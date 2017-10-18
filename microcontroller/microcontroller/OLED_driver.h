/*
 * OLED_driver.h
 *
 * Created: 20.09.2017 12:46:22
 *  Author: ferdyow
 */ 


#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_

#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>

#define OLED_DATA_ADDRESS 0x1200
#define OLED_CMD_ADDRESS 0x1000



void OLED_init();

//turns off screen, resets writing to (0,0)
void OLED_reset();

//resets writing to (0,0)
void OLED_home();

//go to page taken as input
void OLED_goto_line(uint8_t line); 

//go to column taken as input
void OLED_goto_column(uint8_t column);


void OLED_clear_line(uint8_t line);

//set screen pointer to given position
void OLED_pos(uint8_t line, uint8_t column);


void OLED_print(char* str);

//prints a character c to the current pointer pos
void OLED_print_char(char c);

//sets the brightness between 0 and 255
void OLED_set_brightness(uint8_t lvl);

//for testing purposes
void OLED_set_screen();
void OLED_set_line(uint8_t line);
void OLED_test();

void OLED_print_arrow();


#endif /* OLED_DRIVER_H_ */