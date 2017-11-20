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

void CAN_init(void);

void CAN_message_send(can_message* msg);

void CAN_error(void);

int CAN_transmit_complete(transmit_buffer tb);

void CAN_message_receive(can_message* received_msg);

// Interrupt vector
void CAN_int_vect(void);

void CAN_test(void);

can_message receive_control_inputs(void);


#endif /* CAN_H_ */