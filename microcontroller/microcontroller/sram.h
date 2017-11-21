/*
 * sram.h
 *
 * Created: 06.09.2017 14:40:44
 *  Author: tobib
 */ 


#ifndef SRAM_H_
#define SRAM_H_

//

/**
 * Initialize SRAM as an external memory unit and use PC0-PC3 as external memory address pins
 */
void SRAM_init(void);

/**
 * Tests reading and writing to the SRAM
 */
void SRAM_test(void);



#endif /* SRAM_H_ */