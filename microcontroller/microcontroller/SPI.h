/*
 * SPI.h
 *
 * Created: 04.10.2017 12:45:58
 *  Author: ferdyow
 */ 


#ifndef SPI_H_
#define SPI_H_
/*
 * Configure SPI interface on ATmega162 by setting it as master 
 */
void SPI_init(void);

/*
 * Sends one byte over SPI from ATmega162
 * @param {uint8_t} c - Byte to be sent
 */
void SPI_send(uint8_t c);


/*
 * Reads one byte received by ATmega162 from SPI
 */
char SPI_read(void);


void SPI_test(void);

/*
 * Disables transmission on SPI
 */
void SPI_deselect(void);

/*
 * Enables transmission on SPI
 */
void SPI_select(void);

#endif /* SPI_H_ */