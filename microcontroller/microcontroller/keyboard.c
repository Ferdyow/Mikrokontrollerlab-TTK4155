#include "joy.h"
#include "OLED_driver.h"
#include "defines.h"

#include <string.h>

#define NUMB_LETTERS 48
#define LINE_LENGTH  12


/* redefinition of characters to print æ, ø, å, Æ, Ø, Å
* æ = '$'
* ø = '%'
* å = '&'
* Æ = '''
* Ø = '('
* Å = ')'
*/

const char LETTERS_SMALL[NUMB_LETTERS] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '!', '?', 
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '&', '+', 
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '%', '$', '-',
	'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', ';', ':', '#'
};

const char LETTERS_BIG[NUMB_LETTERS] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '!', '?',
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', ')', '+',
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '(', ''', '-',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', ';', ':', '#'
};

char* letters = LETTERS_SMALL;
char*  written_string = "";

typedef struct {
	int x, y
}position;

position pos = {0,0};

void print_blank(int number) {
	for (int i = 0; i < number; i++) {
		write_data(0x00);
	}
}

void print_fill(int number) {
	for (int i = 0; i < number; i++) {
		write_data(0xFF);
	}
}

void print_shift() {
	print_blank(4);
	//up-arrow
	write_data(0b00010000);
	write_data(0b00011000);
	write_data(0b00111100);
	write_data(0b00111110);
	write_data(0b00111100);
	write_data(0b00011000);
	write_data(0b00010000);

	print_blank(4);
}

void print_shift_inverse() {
	print_fill(4);

	//up-arrow
	write_data(~0b00001000);
	write_data(~0b00011000);
	write_data(~0b00111100);
	write_data(~0b01111100);
	write_data(~0b00111100);
	write_data(~0b00011000);
	write_data(~0b00001000);

	print_fill(4);
}

void print_left_arrow() {
	left_arrow = { 0b00010000,  }

	print_blank(6);
	//left-arrow
	
	write_data(0b00010000);
	write_data(0b00111000);
	write_data(0b01111100);
	
	print_blank(6);
}

void print_left_arrow_inverse() {
	print_fill(6);
	//left-arrow
	write_data(~0b00010000);
	write_data(~0b00111000);
	write_data(~0b01111100);

	print_fill(6)
}

void print_right_arrow() {
	print_blank(6);

	//left-arrow
	write_data(0b01111100);
	write_data(0b00111000);
	write_data(0b00010000);



	

	print_blank(6);
}

void print_right_arrow_inverse() {
	print_fill(6);

	//left-arrow
	write_data(~0b01111100);
	write_data(~0b00111000);
	write_data(~0b00010000);

	print_fill(6);
}


void print_spacebar() {
	print_blank(4);

	//spacebar
	write_data(0b00111000);
	for (int i = 0; i < 39;i++) write_data(0b00100000);
	write_data(0b00111000);

	print_blank(4);
}

void print_spacebar_inverse() {
	print_blank(4);

	//spacebar
	write_data(~0b00011100);
	for (int i = 0; i < 39;i++) write_data(~0b00000100);
	write_data(~0b00011100);

	print_blank(4);
}


void print_options_bar() {
	print_shift();
	print_spacebar();
	print_left_arrow();
	print_right_arrow();
}

// 5 lines x 16 columns
void keyboard_goto(int line, int column) {
	OLED_pos(line + 2, (column+2) * 8);
}

void keyboard_goto_line(int line) {
		keyboard_goto(line, 0);
	}


void keyboard_init() {
	OLED_reset();
	letters = LETTERS_SMALL;
	keyboard_goto_line(0);
	pos.x = 0;
	pos.y = 0;

	//The first letter is selected by default
	OLED_print_char_inverse(letters[0]);
	//OLED_print_char(letters[0]);

	//print the rest of the letters
	for (int i = 1; i < NUMB_LETTERS; i++) {
		if (i % LINE_LENGTH == 0) {
			keyboard_goto_line(++pos.y);
			printf("\n");
		}
		OLED_print_char(letters[i]);
		printf("%c",letters[i]);
	}
	//print the tool buttons
	keyboard_goto_line(5);
	print_options_bar();

	//reset y-position
	pos.y = 0;

}

void keyboard_print() {
	keyboard_goto_line(0);
	//print the letters
	for (int i = 0; i < NUMB_LETTERS; i++) {
		if (i % LINE_LENGTH == 0) {
			keyboard_goto_line(++pos.y)
		}
		OLED_print_char(letters[i]);
	}
	//print the tool buttons
	keyboard_goto_line(5);
	print_options_bar();

}

void toggle_shift() {
	if (letters == LETTERS_SMALL) {
		letters = LETTERS_BIG;
	}
	else {
		letters = LETTERS_SMALL;
	}

	keyboard_print();
}


void keyboard_run() {
	keyboard_init();
	int old_y;
	int old_x;
	JOY_direction_t direction;

	//Probably need a delay so we only move one at the time

	while (!JOY_button_pressed(RIGHT_BUTTON)) {

		//Check if we switch letter
		direction = JOY_getDirection();
		if (direction == NEUTRAL);
		else if (direction == DOWN && pos.y < 5) {
			old_y = pos.y++;
		}
		else if (direction == UP && pos.y > 0) {
			old_y = pos.y--;
		}
		else if (direction == LEFT && pos.x > 0) {
			old_x = pos.x--;
		}
		else if (direction == RIGHT && pos.x < LINE_LENGTH) {
			old_x = pos.x++;
		}

		//Add logic to handle the bottom taskbar
		if (old_y != pos.y || old_x != pos.x) {
			//unselect the old letter and select the new one

			//old item is a letter
			if (old.y < 5) {
				OLED_print_char(letters[old_y * LINE_LENGTH + old_x]);
			}

			//old item is in toolbar
			else {
				if (pos.x < 2) {
					pos.x = 0;
					keyboard_goto(pos.y, pos.x);
					print_shift();
				}
				else if (pos.x < 8) {
					pos.x = 2; //start of spacebar
					keyboard_goto(pos.y, pos.x);
					print_spacebar();
				}
				else if (pos.x < 10) {
					pos.x = 9; //start of left arrow
					keyboard_goto(pos.y, pos.x);
					print_left_arrow();
				}
				else {
					pos.x = 11; //start of right arrow
					keyboard_goto(pos.y, pos.x);
					print_right_arrow();
				}
			}
			//selected item is a letter
			if (pos.y < 5) {
				OLED_print_char_inverse(letters[pos.y * LINE_LENGTH + pos.y]);
			}

			//selected menu item is in toolbar
			else {
				if (pos.x < 2) {
					pos.x = 0;
					keyboard_goto(pos.y, pos.x);
					print_shift_inverse();
				}
				else if (pos.x < 8) {
					pos.x = 2; //start of spacebar
					keyboard_goto(pos.y, pos.x);
					print_spacebar_inverse();
				}
				else if (pos.x < 10) {
					pos.x = 9; //start of left arrow
					keyboard_goto(pos.y, pos.x);
					print_left_arrow_inverse();
				}
				else {
					pos.x = 11; //start of right arrow
					keyboard_goto(pos.y, pos.x);
					print_right_arrow_inverse();
				}
			}
		}


		//check if we wish to add the current letter to our string or activate an option
		if (JOY_button_pressed(JOY_BUTTON)) {
			if (pos.y < 5) {
				written_string = strcat(written_string, letters[pos.y * LINE_LENGTH + pos.x]);
				OLED_home();
				//replace with a print function that remembers which part is printed, used with < >
				fprintf(OLED, "%s", written_string);
			}
			else {

			}
		}

		//check if we wish to remove a letter from our string
		else if (JOY_button_pressed(LEFT_BUTTON)) {
			written_string[strlen(written_string) - 1] = 0;
			OLED_home();
			fprintf(OLED, "%s", written_string);
		}
	}
}