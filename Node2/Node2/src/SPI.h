/*
 * SPI.h
 *
 * Created: 04.10.2017 12:45:58
 *  Author: ferdyow
 */ 


#ifndef SPI_H_
#define SPI_H_

void SPI_init(void);

void SPI_send(uint8_t c);

//reads one byte from the SPI
uint8_t SPI_read(void);

void SPI_test(void);

void SPI_deselect(void);
void SPI_select(void);

#endif /* SPI_H_ */