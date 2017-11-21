#include "joy.h"
#include "OLED.h"
#include "print.h"
#include "defines.h"

#include <util/delay.h>
#include <stdio.h>
#include <string.h>


#include <avr/pgmspace.h>

#define NUMB_LETTERS 48
#define LINE_LENGTH  12
#define MAX_STRING_SIZE 40

/* redefinition of characters to print �, �, �, �, �, �
* � = '$'
* � = '%'
* � = '&'
* � = '''
* � = '('
* � = ')'
*/

// INVERSE is defined as 1, NORMAL as 0
enum{ NORMAL, INVERSE};

// Contains keyboard layout
const char PROGMEM LETTERS_SMALL[NUMB_LETTERS] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '!', '?', 
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '&', '+', 
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '%', '$', '-',
	'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', ';', ':', '#'
};

const char PROGMEM LETTERS_BIG[NUMB_LETTERS] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '!', '?',
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', ')', '+',
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '(', '\'', '-',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', ';', ':', '#'
};

/*	VARIABLES						*/
/************************************/
// Allows changing between normal and caps-lock
char* letters = (char*) LETTERS_SMALL;

char  written_string[MAX_STRING_SIZE];

typedef struct {
	int x, y
} position;

position pos = {0,0};
position prev_pos = { 0,0 };


/* HELPER FUNCTION DECLARATIONS*/
/************************************/
void keyboard_print();

// 5 lines x 12 columns
void keyboard_goto(int line, int column);
void keyboard_goto_line(int line);
void toggle_shift();
void keyboard_register_position_change(JOY_direction_t direction);
void keyboard_unselect_prev();
void keyboard_select_curr();
void keyboard_item_pressed();


/* PRINT STRING FUNCTION   */

void append_char(char c) {
	int len = strlen(written_string);
	if (len < MAX_STRING_SIZE){
		written_string[len] = c;
		written_string[len + 1] = '\0';
	}

}

void print_string() {
	OLED_reset_cursor();
	fprintf(OLED, "%s", written_string);

	// Make sure to clear rest of screen
	for (int i = strlen(written_string); i < 16; i++){
		fprintf(OLED, " ");
	}
	
	// Set block at current pointer position
	int pointer = MIN(strlen(written_string)%16,15)*8;
	OLED_goto_pos(0, pointer);
	print_fill(8);
	
	// Return pointer to keyboard position
	keyboard_goto(pos.y, pos.x);
}

void remove_last_char() {
	if (strlen(written_string) > 0){
		written_string[strlen(written_string) - 1] = '\0';
	}
	
}

/* FUNCTION IMPLEMENTATIONS			*/
/************************************/

void keyboard_init() {
	OLED_reset();
	written_string[0]='\0';
	print_string();
	letters = (char*)LETTERS_SMALL;
	keyboard_goto_line(0);
	pos.x = 0;
	pos.y = 0;
	strcpy(written_string, "\0");

	// The first letter is selected by default
	print_char(pgm_read_byte(&letters[0]), INVERSE);

	// Print the rest of the letters
	for (int i = 1; i < NUMB_LETTERS; i++) {
		if (i % LINE_LENGTH == 0) {
			keyboard_goto_line(++pos.y);
		}
		print_char(pgm_read_byte(&letters[i]), NORMAL);
	}
	// Print the tool buttons
	keyboard_goto_line(4);
	print_options_bar(NORMAL, NORMAL, NORMAL, NORMAL);

	// Reset y-position
	pos.y = 0;

	// Ignore button pressed right away
	_delay_ms(500);

}


void keyboard_print() {
	keyboard_goto_line(0);
	pos.y = 0;
	// Print the letters
	for (int i = 0; i < NUMB_LETTERS; i++) {
		if (i % LINE_LENGTH == 0 && i != 0) {
			keyboard_goto_line(++pos.y);
		}
		print_char(pgm_read_byte(&letters[i]), NORMAL);
	}
	// Print the tool buttons
	keyboard_goto_line(4);
	print_options_bar(NORMAL, NORMAL, NORMAL, NORMAL);


}


void keyboard_goto(int line, int column) {
	OLED_goto_pos(line + 3, (column+2) * 8);
}

void keyboard_goto_line(int line) {
		keyboard_goto(line, 0);
	}

void toggle_shift() {
	if (letters == LETTERS_SMALL) {
		letters = (char*)LETTERS_BIG;
	}
	else {
		letters = (char*)LETTERS_SMALL;
	}

	keyboard_print();
	
	// Shift is still selected
	keyboard_goto_line(4);
	print_shift(INVERSE);
	
	pos.y = 4;
	pos.x = 0;
}

void keyboard_register_position_change(JOY_direction_t direction) {
	if (direction == NEUTRAL);
	else if (direction == DOWN && pos.y < 4) {
		prev_pos.y = pos.y++;
	}
	else if (direction == UP && pos.y > 0) {
		prev_pos.y = pos.y--;
	}
	else if (direction == LEFT && pos.x > 0) {
		prev_pos.x = pos.x--;
	}
	else if (direction == RIGHT && pos.x < LINE_LENGTH - 1) {
		// Move extra spaces on big toolbar items
		if (pos.y == 4) {
			if (pos.x < 2) {
				prev_pos.x = 0;
				pos.x = 2;
			}
			else if (pos.x < 8) {
				prev_pos.x = 2;
				pos.x = 8;
			}
			else if (pos.x < 10) {
				prev_pos.x = 8;
				pos.x = 10;
			}
		}
		else {
			prev_pos.x = pos.x++;
		}
	}
}


void keyboard_unselect_prev() {
	// If previous item is a letter
	if (prev_pos.y < 4) {
		keyboard_goto(prev_pos.y, prev_pos.x);
		print_char(pgm_read_byte(&letters[prev_pos.y * LINE_LENGTH + prev_pos.x]), NORMAL);
	}

	// If previous item is in toolbar
	else {
		if (prev_pos.x < 2) {
			prev_pos.x = 0;
			keyboard_goto(prev_pos.y, prev_pos.x);
			print_shift(NORMAL);
		}
		else if (prev_pos.x < 8) {
			prev_pos.x = 2; // Start of spacebar
			keyboard_goto(prev_pos.y, prev_pos.x);
			print_spacebar(NORMAL);
		}
		else if (prev_pos.x < 10) {
			prev_pos.x = 8; // Start of left arrow
			keyboard_goto(prev_pos.y, prev_pos.x);
			print_left_arrow(NORMAL);
		}
		else {
			prev_pos.x = 10; // Start of right arrow
			keyboard_goto(prev_pos.y, prev_pos.x);
			print_right_arrow(NORMAL);
		}
	}
}

void keyboard_select_curr() {
	// Selected item is a letter
	if (pos.y < 4) {
		keyboard_goto(pos.y, pos.x);
		print_char(pgm_read_byte(&letters[pos.y * LINE_LENGTH + pos.x]), INVERSE);
	}

	// Selected item is in toolbar
	else {
		if (pos.x < 2) {
			pos.x = 0;
			keyboard_goto(pos.y, pos.x);
			print_shift(INVERSE);
		}
		else if (pos.x < 8) {
			pos.x = 2; // Start of spacebar
			keyboard_goto(pos.y, pos.x);
			print_spacebar(INVERSE);
		}
		else if (pos.x < 10) {
			pos.x = 8; // Start of left arrow
			keyboard_goto(pos.y, pos.x);
			print_left_arrow(INVERSE);
		}
		else {
			pos.x = 10; // Start of right arrow
			keyboard_goto(pos.y, pos.x);
			print_right_arrow(INVERSE);
		}
	}
}


void keyboard_item_pressed() {
	if (pos.y < 4) {
		append_char(pgm_read_byte(&letters[pos.y * LINE_LENGTH + pos.x]));
		print_string();
	}
	else {
		if (pos.x < 2) {
			toggle_shift();
		}
		else if (pos.x < 8) {
			append_char(32);
			print_string();
		}
		
		//Last two buttons not implemented
		else{
			return;
		}
	}
}



void keyboard_run() {
	keyboard_init();
	JOY_direction_t direction;

	// Detect changes
	int position_moved = 0;
	int button_pressed = 0;

	while (!JOY_button_pressed(LEFT_BUTTON)) {
		// Check if we switch letter
		direction = JOY_getDirection();
		keyboard_register_position_change(direction);
		
		if (prev_pos.y != pos.y || prev_pos.x != pos.x) {
			position_moved = 1; // True
			// Unselect the old letter and select the new one
			keyboard_unselect_prev();
			keyboard_select_curr();

			// Reset position
			prev_pos.x = pos.x;
			prev_pos.y = pos.y;
		}


		// Check if we wish to add the current letter to our string or activate an option
		if (JOY_button_pressed(JOY_BUTTON)) {
			button_pressed = 1;
			keyboard_item_pressed();
			
		}
		// Check if we wish to remove a letter from our string
		else if (JOY_button_pressed(RIGHT_BUTTON)) {
			button_pressed = 1;
			remove_last_char();
			print_string();
		}
		
		// Delay so it doesn't move several times when using Joystick
		if(position_moved){
			position_moved = 0;
			_delay_ms(250);
		}
		if(button_pressed){
			button_pressed = 0;
			_delay_ms(350);
		}
	}
}