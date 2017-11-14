#include "joy.h"
#include "OLED_driver.h"
#include "defines.h"


brightness_init() {
	OLED_reset();
	fprintf(OLED, "%s ", "l_slider:");
	OLED_pos(1,0);
	fprintf(OLED, "%s ", "sets brightness");
	OLED_pos(2,0);
	fprintf(OLED, "%s ", "l_button: back");
	for (int line = 4; line < 8; line++) {
		OLED_set_line(line);
	}
	
}

brightness_run() {
	brightness_init();
	SLI_position_t slider_position;
	while (!JOY_button_pressed(LEFT_BUTTON)) {
		slider_position = SLI_getPosition();
		//printf("slider_position %d\n", slider_position.right);
		printf("slider_position (left) %d\n", slider_position.left);
		OLED_set_brightness(slider_position.left); //RIGHT funker ikke
	}
}