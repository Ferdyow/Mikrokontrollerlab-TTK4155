/*
 * MAX520.h
 *
 * Created: 08.11.2017 08:57:28
 *  Author: ferdyow
 */ 


#ifndef MAX520_H_
#define MAX520_H_
#include <stdio.h>

void MAX520_init(uint8_t three_bit_adress);

void MAX520_send(uint8_t channel, uint8_t data);

#endif /* MAX520_H_ */