#include "joy.h"
#include "OLED.h"
#include "defines.h"


void brightness_init(void) {
	// Initialize to max brightness
	OLED_set_brightness(255);

	OLED_reset();
	// Print instructions
	fprintf(OLED, "%s ", "l_slider:");
	OLED_goto_pos(1,0);
	fprintf(OLED, "%s ", "sets brightness");
	OLED_goto_pos(2,0);
	fprintf(OLED, "%s ", "l_button: back");
	
	// Print field showing brightness 
	for (int line = 4; line < 8; line++) {
		OLED_set_line(line);
	}
	
}

void brightness_run(void) {
	brightness_init();
	SLI_position_t slider_position;
	while (!JOY_button_pressed(LEFT_BUTTON)) {
		slider_position = SLI_getPosition();
		OLED_set_brightness(slider_position.left); 
	}
}