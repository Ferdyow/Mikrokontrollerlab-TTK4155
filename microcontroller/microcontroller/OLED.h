/*
 * OLED.h
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


/**
 * Initialize the OLED display.
 */
void OLED_init();

/**
 * Clear the display and reset the cursor position to (0, 0).
 */
void OLED_reset();

/**
 * Reset cursor position to (0, 0).
 */
void OLED_reset_cursor();

/**
 * Switch to a different line/page on the OLED display.
 * @param {uint8_t} line - The line/page to switch to.
 */
void OLED_goto_line(uint8_t line); 

/**
 * Switch to a different column on the OLED display.
 * @param {uint8_t} column - The column to switch to.
 */
void OLED_goto_column(uint8_t column);

/**
 * Clear an entire line/page of the OLED display.
 * @param {uint8_t} line - The line/page to clear.
 */
void OLED_clear_line(uint8_t line);

/**
 * Set the cursor position to (line, column)
 * @param {uint8_t} line - The line/page to place the cursor at.
 * @param {uint8_t} column - The column to place the cursor at.
 */
void OLED_goto_pos(uint8_t line, uint8_t column);

/**
 * Print an array of characters to the OLED-display.
 * @param {char*} str - The C-string to print.
 */
void OLED_print(char* str);

/**
 * Print a single character to the OLED-display. Has support for newline/return characters.
 * @param {char} character - The character to print.
 */
void OLED_print_char(char character);

/**
 * Print the inverse of a single character to the OLED-display.
 * @param {char} character - The character to print.
 */
void OLED_print_char_inverse(char character);

/**
 * Set the brightness level of the OLED-display. (min 0, max 255)
 * @param {uint8_t} brightness_level - The amount of brightness.
 */
void OLED_set_brightness(uint8_t brightness_level);

/**
 * Turn every pixel of the OLED-display on.
 */
void OLED_set_screen();

/**
 * Turn every pixel of a single line/page on.
 * @param {uint8_t} line - The line/page that will be lit up.
 */
void OLED_set_line(uint8_t line);

/**
 * Run various tests on the OLED display, to confirm that it is working.
 */
void OLED_test();

/**
 * Print an arrow to the OLED display. Nice for displaying menu selections.
 */
void OLED_print_arrow();


#endif /* OLED_DRIVER_H_ */