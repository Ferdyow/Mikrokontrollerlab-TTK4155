/*
 * OLED_driver.h
 *
 * Created: 20.09.2017 12:46:22
 *  Author: ferdyow
 */ 


#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_


#define OLED_DATA_ADDRESS 0x1200
#define OLED_CMD_ADDRESS 0x1000


void OLED_init();
void OLED_reset();
void OLED_home();
void OLED_goto_line(line);
void OLED_clear_line(line);
void OLED_pos(row, column);
void OLED_print(char* str);
void OLED_set_brightness(int lvl);



#endif /* OLED_DRIVER_H_ */