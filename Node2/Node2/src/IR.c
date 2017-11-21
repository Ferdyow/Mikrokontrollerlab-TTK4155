/*
 * IR.c
 *
 * Created: 01.11.2017 11:58:09
 *  Author: ferdyow
 */ 

#include <avr/io.h>
#include <stdbool.h>

#include "defines.h"
#include "ADC.h"
#include "IR.h"

#define DISRUPTION_THRESHOLD 10
#define MOVING_AVERAGE_SIZE 4

void IR_init(void) {
	ADC_init();
}


bool IR_disrupted(void) {
	uint16_t photodiode_brightness = ADC_read(0);
	
	// Shift previous measurements to make room for a new one.
	static int measurements[MOVING_AVERAGE_SIZE] = {0};
	for (int i = MOVING_AVERAGE_SIZE-1; i > 0; i--) {
		measurements[i] = measurements[i-1];
	}
	measurements[0] = photodiode_brightness;
	
	/* Use a digital filter: y_{n} = (1/k)(x_{n} + x_{n-1} + ... + x_{n-k+1})
	 * This basically calculates a moving average of the last k measurements.
	 *
	 * k: MOVING_AVERAGE_SIZE
	 */
	uint16_t brightness_sum = 0;
	for (int i = 0; i < MOVING_AVERAGE_SIZE; i++) {
		brightness_sum += measurements[i];
	}
	uint16_t filtered_brightness = brightness_sum / MOVING_AVERAGE_SIZE;
	
	// Return whether the IR signal is disrupted or not
	printf("Filtered brightness: %4d\n", filtered_brightness);
	return filtered_brightness < DISRUPTION_THRESHOLD;
}
