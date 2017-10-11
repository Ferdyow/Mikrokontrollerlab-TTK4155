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
	//Configure interrupt0 on the ATmega162 for receiving interrupts from SPI
	//see page 160 in ATmega162
	//enable int0 on the ATmega162
	set_bit(GICR, INT0);
	
	//interrupt when low
	clear_bit(MCUCR,ISC01);
	clear_bit(MCUCR, ISC00);
	

	//set mosi, sck, ss as output, others as input
	set_bit(DDRB, MOSI);
	set_bit(DDRB, SCK);
	set_bit(DDRB, SS);
	
	
	//set ATmega as master
	set_bit(SPCR, MSTR);
	
	//enable SPI
	set_bit(SPCR, SPE);
	
	//set clock to f_osc/16
	set_bit(SPCR, SPR0);
	clear_bit(SPCR,SPR1);
	clear_bit(SPCR, SPI2X);
	
	//when DORD in SPCR is 0, MSB is transmitted first
	
}


void SPI_send(uint8_t data){
	//Start transmission
	SPDR = data;		
	//wait for transmission complete SPSR: register with SPIF flag, SPIF: bit set to 1 when data is read
	loop_until_bit_is_set(SPSR, SPIF);
}


char SPI_read(){
	//must send a dummy bit to receive data
	SPI_send(0);
	
	loop_until_bit_is_set(SPSR, SPIF);
	char data = SPDR;
	return data;
	
}

void SPI_test() {
	
	SPI_send(0b10010101);
	//char spi_read_result = SPI_read();
	
	//printf("SPI_TEST: %s\n\n", spi_read_result);
	
}

void SPI_select(){
	clear_bit(PORTB, SS);
}

void SPI_deselect(){
	set_bit(PORTB, SS);
}