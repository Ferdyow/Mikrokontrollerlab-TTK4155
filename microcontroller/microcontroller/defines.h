/*
 * defines.h
 *
 * Created: 13.09.2017 12:12:04
 *  Author: tobib
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_

#include "OLED_driver.h";
#include "usart.h";

#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

// Output streams
static FILE oled_out = FDEV_SETUP_STREAM(OLED_print_char, NULL, _FDEV_SETUP_WRITE);
//static FILE pc_out = FDEV_SETUP_STREAM(usart_transmit_to_computer, NULL, _FDEV_SETUP_WRITE);
#define OLED &oled_out
//#define PC &pc_out

#define loop_until_bit_is_set( reg, bit ) while( !test_bit( reg, bit ) )
//#define loop_until_bit_is_clear( reg, bit ) while( test_bit( reg, bit ) )

#endif /* DEFINES_H_ */