/*
 * MAX520.h
 *
 * Created: 08.11.2017 08:57:28
 *  Author: ferdyow
 */ 


#ifndef MAX520_H_
#define MAX520_H_
#include <stdio.h>

/**
 * Initialize motor box, and TWI_Master for transferring data.
 * @param {uint8_t} three_bit_address - Used to calculate max520_address.
 */
void MAX520_init(uint8_t three_bit_address);

/**
 * Send data to the motor box.
 * @param {uint8_t} channel - The channel to send the data through
 * @param {uint8_t} data - The data to send
 */
void MAX520_send(uint8_t channel, uint8_t data);

#endif /* MAX520_H_ */