/*
 * CAN.h
 *
 * Created: 04.10.2017 16:45:19
 *  Author: ferdyow
 */ 


#ifndef CAN_H_
#define CAN_H_

struct can_message {
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
};


void CAN_init();

void CAN_message_send(&can_message msg);

void CAN_error();

void CAN_transmit_complete();

void CAN_data_receive();

// Interrupt vector
void CAN_int_vect();


#endif /* CAN_H_ */