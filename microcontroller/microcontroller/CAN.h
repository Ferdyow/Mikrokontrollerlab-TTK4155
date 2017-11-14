/*
 * CAN.h
 *
 * Created: 04.10.2017 16:45:19
 *  Author: ferdyow
 */ 


#ifndef CAN_H_
#define CAN_H_

#define BUFFER_LENGTH 16
#define TB0 0
#define TB1 1
#define TB2 2

typedef struct {
	unsigned int id;
	uint8_t length;
	int8_t data[8];
}can_message;


void CAN_init();

void CAN_message_send(can_message* msg);

void CAN_error();

bool CAN_transmit_complete(int transmit_buffer_index);

void CAN_data_receive(can_message* received_msg);

// Interrupt vector
void CAN_int_vect();

void CAN_test();

#endif /* CAN_H_ */