/*
 * CAN.h
 *
 * Created: 04.10.2017 16:45:19
 *  Author: ferdyow
 */ 


#ifndef CAN_H_
#define CAN_H_

typedef struct {
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
}can_message;


void CAN_init();

void CAN_message_send(can_message* msg);

void CAN_error();

bool CAN_transmit_complete(int transmit_buffer_numb);

void CAN_data_receive(can_message* received_msg);

// Interrupt vector
void CAN_int_vect();


#endif /* CAN_H_ */