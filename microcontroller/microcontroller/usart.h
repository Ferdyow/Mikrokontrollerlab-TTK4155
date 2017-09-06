/*
 * usart.h
 *
 * Created: 06.09.2017 10:35:12
 *  Author: tobib
 */ 

#ifndef USART_H_
#define USART_H_

#define F_CPU 4915200  // Clock frequency in Hz
#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

/**
 * Initialize USART interface for transmit and receive.
 * @param {unsigned int) ubbr - Number calculated from baud rate/clock speed
 */
void usart_init(unsigned int ubbr);


/**
 * Transmit data from the ATMega162 controller to the connected computer
 * @param {unsigned int} data - The data to transmit to the computer
 */
void usart_transmit_to_computer(unsigned int data);


/**
 * Receive data to the ATMega162 controller from the connected computer.
 * @returns {unsigned int} - The data received from the computer.
 * (NOTE: Currently not working. Consider trying another ATMega162 controller.)
 */
unsigned char usart_receive_from_computer(void);


#endif /* USART_H_ */