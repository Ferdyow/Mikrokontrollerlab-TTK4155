#include "joy.h"
#include "OLED_driver.h"
#include "print.h"
#include "defines.h"

#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#define NUMB_LETTERS 48
#define LINE_LENGTH  12


/* redefinition of characters to print �, �, �, �, �, �
* � = '$'
* � = '%'
* � = '&'
* � = '''
* � = '('
* � = ')'
*/

//Inverse is defined as 1
enum{ NORMAL, INVERSE};

const char LETTERS_SMALL[NUMB_LETTERS] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '!', '?', 
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '&', '+', 
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '%', '$', '-',
	'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', ';', ':', '#'
};

const char LETTERS_BIG[NUMB_LETTERS] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '!', '?',
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', ')', '+',
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '(', '\'', '-',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', ';', ':', '#'
};

/*	VARIABLES						*/
/************************************/
char* letters = LETTERS_SMALL;
char  written_string[40];
int string_position=0; 

typedef struct {
	int x, y
}position;

position pos = {0,0};


/* HELPER FUNCTION		*/
/************************************/

void append_char(char c) {
	int len = strlen(written_string);
	written_string[len] = c;
	written_string[len + 1] = '\0';
	string_position++;
}

void print_string() {
	OLED_home();
	char print_string[16];
	mempcy(print_string, written_string[strlen(written_string)-string_position, 16)
	fprintf(OLED, "%s", print_string);
	keyboard_goto(pos.y, pos.x);
}

void remove_last_char() {
	written_string[strlen(written_string) - 1] = '\0';
}

/* FUNCTION IMPLEMENTATIONS			*/
/************************************/

void keyboard_init() {
	OLED_reset();
	letters = LETTERS_SMALL;
	keyboard_goto_line(0);
	pos.x = 0;
	pos.y = 0;
	strcpy(written_string, "\0");

	//The first letter is selected by default
	print_char(letters[0], INVERSE);

	//print the rest of the letters
	for (int i = 1; i < NUMB_LETTERS; i++) {
		if (i % LINE_LENGTH == 0) {
			keyboard_goto_line(++pos.y);
		}
		print_char(letters[i], NORMAL);
	}
	//print the tool buttons
	keyboard_goto_line(4);
	print_options_bar(NORMAL, NORMAL, NORMAL, NORMAL);

	//reset y-position
	pos.y = 0;

}


void keyboard_print() {
	keyboard_goto_line(0);
	pos.y = 0;
	//print the letters
	for (int i = 0; i < NUMB_LETTERS; i++) {
		if (i % LINE_LENGTH == 0 && i != 0) {
			keyboard_goto_line(++pos.y);
		}
		print_char(letters[i], NORMAL);
	}
	//print the tool buttons
	keyboard_goto_line(4);
	print_options_bar(NORMAL, NORMAL, NORMAL, NORMAL);


}

// 5 lines x 16 columns
void keyboard_goto(int line, int column) {
	OLED_pos(line + 3, (column+2) * 8);
}

void keyboard_goto_line(int line) {
		keyboard_goto(line, 0);
	}

void toggle_shift() {
	if (letters == LETTERS_SMALL) {
		letters = LETTERS_BIG;
	}
	else {
		letters = LETTERS_SMALL;
	}

	keyboard_print();
	keyboard_goto_line(4);
	print_shift(INVERSE);
		
	pos.y = 4;
	pos.x = 0;
}


void keyboard_run() {
	keyboard_init();
	_delay_ms(500);
	int old_y=0;
	int old_x=0;
	JOY_direction_t direction;
	int position_moved = 0;
	int button_pressed = 0;

	while (!JOY_button_pressed(LEFT_BUTTON)) {
		//Check if we switch letter
		direction = JOY_getDirection();
		if (direction == NEUTRAL);
		else if (direction == DOWN && pos.y < 4) {
			old_y = pos.y++;
		}
		else if (direction == UP && pos.y > 0) {
			old_y = pos.y--;
		}
		else if (direction == LEFT && pos.x > 0) {
			old_x = pos.x--;
		}
		else if (direction == RIGHT && pos.x < LINE_LENGTH-1) {
			
			if(pos.y == 4){
				if (pos.x < 2){
					old_x = 0;
					pos.x = 2;
				}
				 else if (pos.x < 8){
					old_x = 2;
					pos.x = 8;
				}
				else if(pos.x < 10){
					old_x = 8;
					pos.x = 10;	
				}
			}
			else{
				old_x = pos.x++;
			}
		}

		//Add logic to handle the bottom taskbar
		if (old_y != pos.y || old_x != pos.x) {
			printf("old_y: %d, y: %d, old_x: %d, old_y %d\n", old_y, pos.y, old_x, pos.x);
			position_moved = 1;
			//unselect the old letter and select the new one

			//old item is a letter
			if (old_y < 4) {
				keyboard_goto(old_y, old_x);
				print_char(letters[old_y * LINE_LENGTH + old_x], NORMAL);
			}

			//old item is in toolbar
			else {
				if (old_x < 2) {
					old_x = 0;
					keyboard_goto(old_y, old_x);
					print_shift(NORMAL);
				}
				else if (old_x < 8) {
					old_x = 2; //start of spacebar
					keyboard_goto(old_y, old_x);
					print_spacebar(NORMAL);
				}
				else if (old_x < 10) {
					old_x = 8; //start of left arrow
					keyboard_goto(old_y, old_x);
					print_left_arrow(NORMAL);
				}
				else {
					old_x = 10; //start of right arrow
					keyboard_goto(old_y, old_x);
					print_right_arrow(NORMAL);
				}
			}
			//selected item is a letter
			if (pos.y < 4) {
				keyboard_goto(pos.y,pos.x);
				print_char(letters[pos.y * LINE_LENGTH + pos.x], INVERSE);
			}

			//selected menu item is in toolbar
			else {
				if (pos.x < 2) {
					pos.x = 0;
					keyboard_goto(pos.y, pos.x);
					print_shift(INVERSE);
				}
				else if (pos.x < 8) {
					pos.x = 2; //start of spacebar
					keyboard_goto(pos.y, pos.x);
					print_spacebar(INVERSE);
				}
				else if (pos.x < 10) {
					pos.x = 8; //start of left arrow
					keyboard_goto(pos.y, pos.x);
					print_left_arrow(INVERSE);
				}
				else {
					pos.x = 10; //start of right arrow
					keyboard_goto(pos.y, pos.x);
					print_right_arrow(INVERSE);
				}
			}
			old_x = pos.x;
			old_y = pos.y;
		}


		//check if we wish to add the current letter to our string or activate an option
		if (JOY_button_pressed(JOY_BUTTON)) {
			button_pressed = 1;
			if (pos.y < 4) {
				append_char(letters[pos.y * LINE_LENGTH + pos.x]);
				//printf("APPEND written string: %s", written_string);
				//replace with a print function that remembers which part is printed, used with < >
				print_string();
			}
			else {
				if(pos.x < 2){
					toggle_shift();
				}
				else if(pos.x < 8){
					append_char(32);
					print_string();
				}
				ëlse if (pos.x < 10 && string_position > 0) {
					string_position--;
				}
				else if (pos.x < 12 && string_position < strlen(written_string)) {
					string_position++;
				}
			}
		}

		//check if we wish to remove a letter from our string
		else if (JOY_button_pressed(RIGHT_BUTTON)) {
			button_pressed = 1;
			remove_last_char();
			//printf("REMOVE written string: %s", written_string);
			print_string();
		}
		
		if(position_moved){
			position_moved = 0;
			_delay_ms(250);
		}
		if(button_pressed){
			button_pressed = 0;
			_delay_ms(500);
		}
	}
}