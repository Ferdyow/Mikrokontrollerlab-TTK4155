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
//#include <util/delay.h>
#include <avr/pgmspace.h>

#include "SPI.h"

//PINOUT for ATmega2560
#define SS PB7
#define MOSI PB2
#define MISO PB3
#define SCK PB1

void SPI_init(void){
	//set mosi, sck, ss as output, others as input
	//--MOSI - master out slave in
	//--sck - serial clock
	//--ss - slave select
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
	//default satt til 0
	//clear_bit(SPCR,SPR1);
	//clear_bit(SPCR, SPI2X);
	
	//when DORD in SPCR is 0, MSB is transmitted first
	
}


void SPI_send(uint8_t data){
	//Start transmission
	SPDR = data;	
	//wait for transmission complete SPSR: register with SPIF flag, SPIF: bit set to 1 when data is read
	loop_until_bit_is_set(SPSR, SPIF);
}


uint8_t SPI_read(void){
	//must send a dummy bit to receive data
	SPI_send(0);
	
	loop_until_bit_is_set(SPSR, SPIF);
	//printf("SPDR: %x \t", SPDR);
	uint8_t data = SPDR;
	//printf("data: %x \n", data);
	return data;
	
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