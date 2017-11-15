ifndef PRINT_H_
#define PRINT_H_

//Print number amount of blank/filled columns at the current position
void print_blank(int number);
void print_fill(int number);


//If inverse is 1 it prints the inverse
void print_shift(int inverse);
void print_left_arrow(int inverse);
void print_right_arrow(int inverse);
void print_spacebar(int inverse);
void print_options_bar(int shift_inverse, int space_inverse, int left_inverse, int right_inverse);
void print_char(char c, int inverse);



#endif /* PRINT_H_ */