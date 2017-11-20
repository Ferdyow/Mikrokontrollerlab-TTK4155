/*
 * IR.h
 *
 * Created: 01.11.2017 11:57:58
 *  Author: ferdyow
 */ 


#ifndef IR_H_
#define IR_H_

#include <stdbool.h>

/**
 * Initialize the IR-diode
 */
void IR_init(void);

/**
 * Read strength of light hitting the IR-diode. 
 * max = ?, min = ?
 */
bool IR_disrupted(void);

#endif /* IR_H_ */