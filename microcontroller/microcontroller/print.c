#include "OLED.h"

#include <stdio.h>
#include <string.h>

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

void print_shift(int inverse) {
	if (!inverse) {
		print_blank(4);
		//up-arrow
		write_data(0b00010000);
		write_data(0b00011000);
		write_data(0b00111100);
		write_data(0b00111110);
		write_data(0b00111100);
		write_data(0b00011000);
		write_data(0b00010000);

		print_blank(5);
	}
	else {
		print_fill(4);
		//up-arrow
		write_data(~0b00010000);
		write_data(~0b00011000);
		write_data(~0b00111100);
		write_data(~0b00111110);
		write_data(~0b00111100);
		write_data(~0b00011000);
		write_data(~0b00010000);
		print_fill(5);
	}
}

void print_left_arrow(int inverse) {
	if (!inverse) {
		print_blank(6);
		//left-arrow

		write_data(0b00010000);
		write_data(0b00111000);
		write_data(0b01111100);

		print_blank(7);
	}
	else {
		print_fill(6);
		//left-arrow
		write_data(~0b00010000);
		write_data(~0b00111000);
		write_data(~0b01111100);

		print_fill(7);
	}
}


void print_right_arrow(int inverse) {
	if (!inverse) {
		print_blank(6);

		//right-arrow
		write_data(0b01111100);
		write_data(0b00111000);
		write_data(0b00010000);

		print_blank(7);
	}
	else {
		print_fill(6);

		//right-arrow
		write_data(~0b01111100);
		write_data(~0b00111000);
		write_data(~0b00010000);

		print_fill(7);
	}
}


void print_spacebar(int inverse) {
	if (!inverse) {
		print_blank(4);

		//spacebar
		write_data(0b00111000);
		for (int i = 0; i < 39;i++) write_data(0b00100000);
		write_data(0b00111000);

		print_blank(5);
	}
	else {
		print_blank(4);

		//spacebar
		write_data(~0b00111000);
		for (int i = 0; i < 39;i++) write_data(~0b00100000);
		write_data(~0b00111000);

		print_blank(5);
	}
}


void print_options_bar(int shift_inverse, int space_inverse, int left_inverse,  int right_inverse) {
	print_shift(shift_inverse);
	print_spacebar(space_inverse);
	print_left_arrow(left_inverse);
	print_right_arrow(right_inverse);
}

void print_char(char c, int inverse) {
	if (!inverse) {
		OLED_print_char(c);
	}
	else {
		OLED_print_char_inverse(c);
	}
}
