/*
 * CAN.h
 *
 * Created: 04.10.2017 16:45:19
 *  Author: ferdyow
 */ 


#ifndef CAN_H_
#define CAN_H_


#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER_LENGTH 16
#define TB0 0
#define TB1 1
#define TB2 2

typedef struct {
	unsigned int id;
	uint8_t length;
	int8_t data[8];
}can_message;

/**
 * Sets Normal mode, enables rollover in receive buffer registers
 */
void CAN_init(void);

/**
 * Prints all elements of CAN message
 * @param {can_message} msg - Prints the msg that is pointed at
 */
void CAN_print_message(can_message* msg);

/**
 * Transmits message over CAN using a standard data frame
 * @param {can_message} msg - Message to be sent
 */
void CAN_message_send(can_message* msg);


/*
 * DOES NOTHING!
 */
void CAN_error(void);


/**
 * Check if the transmit buffer is free to transmit
 * @param {int} transmit_buffer_numb - Transmit buffer to be checked
 * @return - 1/True when buffer free, 0/False otherwise
 */
bool CAN_transmit_complete(int transmit_buffer_numb);

/**
 * Receives message with standard data frame from CAN 
 * @param {can_message} received_msg - received message saved to pointer value
 */
void CAN_message_receive(can_message* received_msg);

/* 
 * Sets buffer flag when buffer receives message
 */
void CAN_int_vect();

/*
 * Tests in loopback mode
 */
void CAN_test();

#endif /* CAN_H_ */