/*
 * brightness.h
 *
 * Created: 14.11.2017 17:09:41
 *  Author: ferdyow
 */ 


#ifndef BRIGHTNESS_H_
#define BRIGHTNESS_H_

/*
 * Initializes to max brightness and prints screen 
 */
void brightness_init(void);

/*
 * Takes input from left slider to change brightness, exit with left button
 */
void brightness_run(void);


#endif /* BRIGHTNESS_H_ */