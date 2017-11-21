/*
 * SPI.c
 *
 * Created: 04.10.2017 12:50:30
 *  Author: ferdyow
 */ 

#include "defines.h"

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "SPI.h"

//PINOUT for ATmega2560
#define SS PB7
#define MOSI PB2
#define MISO PB3
#define SCK PB1

void SPI_init(void){
	//set mosi, sck, ss as output, others as input
	set_bit(DDRB, MOSI);
	set_bit(DDRB, SCK);
	set_bit(DDRB, SS);
	
	//required by SPI
	set_bit(DDRB, PB0);
	
	//set ATmega as master
	set_bit(SPCR, MSTR);

	
	//set clock to f_osc/16
	set_bit(SPCR, SPR0);
		
	//enable SPI
	set_bit(SPCR, SPE);
	
	// MSB is transmitted first (default)
	
}


void SPI_send(uint8_t data){
	// Start transmission
	SPDR = data;	
	// Wait for flag to signal that transmission is complete
	loop_til_bit_is_set(SPSR, SPIF);
}


uint8_t SPI_read(void){
	// Must send a dummy bit to receive data
	SPI_send(0);
	
	// Wait for flag to signal that transmission is complete
	loop_until_bit_is_set(SPSR, SPIF);
	
	// Return received data
	return SPDR;	
}

void SPI_test(void) {
	SPI_send(0x00);
	uint8_t spi_read_result = SPI_read();
	
	printf("SPI_TEST: %x\n\n", spi_read_result);
	
}

void SPI_select(void){
	clear_bit(PORTB, SS);
}

void SPI_deselect(void){
	set_bit(PORTB, SS);
}