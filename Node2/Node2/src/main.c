/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
 
 
#include <asf.h>


#include "usart.h"
#include "defines.h"
#include "CAN.h"
#include "MCP2515.h"
#include "SPI.h"
#include "PWM.h"
#include "servo.h"
#include "IR.h"
#include "motor.h"
#include "PI.h"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

void initialize(void);
can_message receive_control_inputs(void);
void test(void);
void test_servo_and_ir(void);


void initialize(void){
	cli();
	usart_init(MYUBRR);
	CAN_init();
	servo_init();
	IR_init();
	PI_init();
	motor_init();
	sei();
}

void test(void){
	can_message received_message;
	while(1){
		
		CAN_data_receive(&received_message);
		if(received_message.length){
			printf("\n\nRECEIVED:\n\nlength: %d\nid: %d\n", received_message.length, received_message.id);
			for (uint8_t byte = 0; byte < received_message.length;byte++){
				printf("Data nr. %d: %x\n", byte, received_message.data[byte]);
			}
		}
	}
}

void test_servo_and_ir(void){
	can_message message;
	int8_t joystick_x = 0;
		
	while(1){
		message = receive_control_inputs();
		joystick_x = message.data[0];
		servo_set(joystick_x);
		motor_set_velocity(joystick_x);
		motor_get_velocity();
	}
}

int main(void){
	initialize();
	//test_servo_and_ir();
	return 0;
}