/*
 * CAN.h
 *
 * Created: 04.10.2017 16:45:19
 *  Author: ferdyow
 */ 


#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>

typedef struct {
	unsigned int id;
	uint8_t length;
	int8_t data[8];
} can_message;

typedef enum { TRANSMIT_BUFFER_0, TRANSMIT_BUFFER_1, TRANSMIT_BUFFER_2 } transmit_buffer;

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
 * Prints CAN error message over UART.
 */
void CAN_error(void);

/**
 * Check if the transmit buffer is free to transmit
 * @param {transmit_buffer} tb - Transmit buffer to be checked
 * @return - 1/True when buffer free, 0/False otherwise
 */
int CAN_transmit_complete(transmit_buffer tb);

/**
 * Receives message with standard data frame from CAN 
 * @param {can_message} received_msg - received message saved to pointer value
 */
void CAN_message_receive(can_message* received_msg);

/* 
 * Sets buffer flag when buffer receives message
 */
void CAN_int_vect(void);

/*
 * Tests in loopback mode
 */
void CAN_test(void);

/**
 * Prints out a CAN message on a nice form over UART.
 * @param {can_message*} msg - A pointer to the message that will be printed.
 */
void CAN_print_message(can_message* msg);

#endif /* CAN_H_ */