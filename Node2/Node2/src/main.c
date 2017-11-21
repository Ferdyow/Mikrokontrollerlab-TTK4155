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
#include "control.h"
#include "solenoid.h"
#include "game.h"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

void initialize(void);
void test(void);
void test_servo_and_ir(void);


void initialize(void){
	cli();
	usart_init(MYUBRR);			printf("\n[NODE 2][main.c]: USART initialized.\n");
	CAN_init();					printf("[NODE 2][main.c]: CAN initialized.\n");
	servo_init();				printf("[NODE 2][main.c]: SERVO initialized.\n");
	IR_init();					printf("[NODE 2][main.c]: IR initialized.\n");
	motor_init(DEFAULT);		printf("[NODE 2][main.c]: MOTOR initialized!\n");
	PI_init();					printf("[NODE 2][main.c]: PI initialized!\n");
	GAME_init(CLOSED_LOOP);		printf("[NODE 2][main.c]: CONTROL initialized!\n");
	solenoid_init();			printf("[NODE 2][main.c]: SOLENOID initialized!\n");
								printf("\n====== INITIALIZATION COMPLETE ======\n\n");
	sei();
}


int main(void){
	initialize();
	GAME_loop();
	while(1);
	return 0;
}