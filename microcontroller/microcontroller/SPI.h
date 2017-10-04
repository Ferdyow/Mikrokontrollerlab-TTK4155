/*
 * SPI.h
 *
 * Created: 04.10.2017 12:45:58
 *  Author: ferdyow
 */ 


#ifndef SPI_H_
#define SPI_H_

void SPI_init();

void SPI_send(char c);

//reads one byte from the SPI
char SPI_read();

void SPI_test();

void SPI_deselect();
void SPI_select();

#endif /* SPI_H_ */