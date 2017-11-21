#ifndef PRINT_H_
#define PRINT_H_

/*
 * Print blank columns of page to OLED screen at current position
 * @param {int} number - amount of blank columns
 */
void print_blank(int number);

/*
 * Print filled columns of page to OLED screen at current position
 * @param {int} number - amount of filled columns
 */
void print_fill(int number);

/*
 * Prints shift button on OLED (used for drawing keyboard)
 * @param {int} inverse - if 1/True prints the inverse character, if 0/False print normally
 */
void print_shift(int inverse);

/**
 * Print an arrow pointing to the left on OLED (used for drawing keyboard)
 * @param {int} inverse - if 1/True prints the inverse character, if 0/False print normally
 */
void print_left_arrow(int inverse);

/**
 * Print an arrow pointing to the right on OLED (used for drawing keyboard)
 * @param {int} inverse - if 1/True prints the inverse character, if 0/False print normally
 */
void print_right_arrow(int inverse);

/**
 * Print a spacebar on OLED (used for drawing keyboard)
 * @param {int} inverse - if 1/True prints the inverse character, if 0/False print normally
 */
void print_spacebar(int inverse);

/**
 * Print a character on OLED (used for drawing keyboard)
 * @param {int} inverse - if 1/True prints the inverse character, if 0/False print normally
 */
void print_char(char c, int inverse);

/**
 * Print the options bar on OLED (used for drawing keyboard)
 * @param {int} inverse - if 1/True prints the inverse character, if 0/False print normally
 */
void print_options_bar(int shift_inverse, int space_inverse, int left_inverse, int right_inverse);




#endif /* PRINT_H_ */