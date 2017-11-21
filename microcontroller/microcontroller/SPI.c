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
#include <util/delay.h>
#include <avr/pgmspace.h>

#define SS PB4
#define MOSI PB5
#define MISO PB6
#define SCK PB7

void SPI_init(){
	// Configure interrupt0 when low on the ATmega162 for receiving interrupts from SPI (P. 160 ATmega162)
	set_bit(GICR, INT0);
	
	// Set mosi, sck, ss as output, others as input
	set_bit(DDRB, MOSI);
	set_bit(DDRB, SCK);
	set_bit(DDRB, SS);
	
	
	// Set ATmega162 as master
	set_bit(SPCR, MSTR);
	
	// Enable SPI
	set_bit(SPCR, SPE);
	
	// Set clock to f_osc/16
	set_bit(SPCR, SPR0);
	
	// MSB is transmitted first (default)
	
}


void SPI_send(uint8_t data){
	// Start transmission
	SPDR = data;
			
	// Wait for flag to signal that transmission is complete
	loop_until_bit_is_set(SPSR, SPIF);
}


char SPI_read(){
	// Must send a dummy bit to receive data
	SPI_send(0);
	
	// Wait for flag to signal that transmission is complete
	loop_until_bit_is_set(SPSR, SPIF);
	
	// Return received data
	return SPDR;
	
}

void SPI_test() {
	// Send value to read with oscilloscope
	SPI_send(0b10010101); 
}

void SPI_select(){
	clear_bit(PORTB, SS);
}

void SPI_deselect(){
	set_bit(PORTB, SS);
}