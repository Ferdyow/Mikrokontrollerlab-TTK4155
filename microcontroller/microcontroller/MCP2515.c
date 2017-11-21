/*
 * MCP2515.c
 *
 * Created: 04.10.2017 15:16:37
 *  Author: ferdyow
 */ 

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "MCP2515.h"
#include "SPI.h"

#define RESET 0xC0
#define READ 0x03
#define WRITE 0x02
#define READ_STATUS 0xA0
#define RX_STATUS 0xB0
#define BIT_MODIFY 0x05
#define RTS_BASE 0x80


uint8_t MCP2515_init() {
	SPI_init();
	MCP2515_reset();
	// Check bit 7-5 of MCP_CANSTAT which signify the operation mode
	const uint8_t device_mode = MCP2515_read(MCP_CANSTAT) & MODE_MASK;
	if (device_mode != MODE_CONFIG) {
		printf("ERROR: MCP2515 is NOT in configuration mode after reset!\n");
		return 1;
	}	
	return 0;
}

char MCP2515_read(char address) {
	SPI_select();
	SPI_send(READ);
	SPI_send(address);
	char data = SPI_read();
	SPI_deselect();
	return data;
}

void MCP2515_write(char address, char data) {
	SPI_select();
	SPI_send(WRITE);
	SPI_send(address);
	SPI_send(data);
	SPI_deselect();
}


void MCP2515_request_to_send(uint8_t buffer_states) {
	SPI_select();
	SPI_send(RTS_BASE+buffer_states);
	SPI_deselect();
}


void MCP2515_bit_modify(char reg_address, char masked_bits, char new_data) {
	SPI_select();
	SPI_send(BIT_MODIFY);
	SPI_send(reg_address);
	SPI_send(masked_bits);
	SPI_send(new_data);
	SPI_deselect();
}

void MCP2515_reset() {
	SPI_select();
	SPI_send(RESET);
	SPI_deselect();
}

char MCP2525_read_status() {
	SPI_select();
	SPI_send(RX_STATUS);
	char status = SPI_read();
	SPI_deselect();
	return status;
}