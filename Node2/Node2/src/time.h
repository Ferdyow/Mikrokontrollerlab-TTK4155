/*
 * timer.h
 *
 * Created: 08.11.2017 15:34:14
 *  Author: ferdyow
 */ 

/**
 * Initialize the timer that is used for velocity measurements.
 */
void time_init(void);

/**
 * Reset the timer counter. Also clears the timer interrupt.
 */
void time_reset(void);

/**
 * Get the time since last time this function was called. Also resets timer.
 * @returns {uint16_t} - The time that has passed.
 */
uint16_t time_passed(void);