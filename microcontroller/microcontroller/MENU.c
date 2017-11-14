/*
 * OLED_menu.c
 *
 * Created: 27.09.2017 12:10:25
 *  Author: ferdyow
 */ 
#define F_CPU 4915200  // Clock frequency in Hz
#include "OLED_driver.h"
#include "MENU.h"
#include "defines.h"
#include "joy.h"
#include "keyboard.h"
#include "brightness.h"
#include "game.h"

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdlib.h>

volatile menu_entry* current_entry;
volatile menu_entry* selected_entry;


void test_function() {
	printf("\n\nHighscore clicked!\n\n");
}

void MENU_init() {
	menu_entry* main_menu = add_entry(NULL, "Main menu", NULL);
	
	// Main menu
	menu_entry* play = add_entry(main_menu, "Play game", NULL);
	menu_entry* highscore = add_entry(main_menu, "Highscore", test_function);
	menu_entry* settings = add_entry(main_menu, "Settings", NULL);
	menu_entry* extras = add_entry(main_menu, "Extra features", NULL);
	//menu_entry* yolo = add_entry(main_menu, "YOLO", NULL);
	
	
	// Sub-menu settings
	menu_entry* brightness = add_entry(settings, "Brightness", brightness_run);
	menu_entry* font = add_entry(settings, "Set font", NULL);
	
	// Sub-menu extras
	menu_entry* keyboard = add_entry(extras, "Keyboard", keyboard_run);
	menu_entry* mandelbrot = add_entry(extras, "Mandelbrot", NULL);
	menu_entry* pong = add_entry(extras, "Play pong", NULL);
	menu_entry* rcube = add_entry(extras, "rotating cube", NULL);
	
	current_entry = main_menu;
	selected_entry = play;
	MENU_draw_current_menu();
}


void MENU_select_next() {
	if (selected_entry -> next_sibling_node != NULL) {
		selected_entry = selected_entry -> next_sibling_node;
		MENU_draw_current_menu();
	}
}

void MENU_select_prev() {
	if (selected_entry -> prev_sibling_node != NULL) {
		selected_entry = selected_entry -> prev_sibling_node;
		MENU_draw_current_menu();
	}
}

void MENU_enter_selection() {
	if (selected_entry->function_ptr != NULL) {
		printf("HEI\n");
		selected_entry->function_ptr();
	}
	
	if (selected_entry->child_node == NULL) return;
	
	current_entry = selected_entry;
	selected_entry = current_entry->child_node;
	MENU_draw_current_menu();
}

void MENU_go_back() {
	if (current_entry->parent_node == NULL) return;
	
	current_entry = current_entry->parent_node;
	selected_entry = current_entry->child_node;
	MENU_draw_current_menu();
}

void MENU_draw_current_menu() {
	OLED_reset();
	int output_row = 0;
	
	menu_entry* temp_node = current_entry->child_node;
	
	while( temp_node != NULL){
		OLED_pos(output_row++, 0);
		if (temp_node == selected_entry) {
			OLED_print_arrow();
		}
		fprintf(OLED, " %s", temp_node->name);
		temp_node = temp_node -> next_sibling_node;
	}
}


void MENU_test(){
	int delay = 100;
	_delay_ms(delay);
	MENU_select_next();
	_delay_ms(delay);
	MENU_select_next();
	_delay_ms(delay);
	MENU_select_next();
	_delay_ms(delay);
	MENU_select_next();	
	
	_delay_ms(delay);
	MENU_select_prev();
	_delay_ms(delay);
	MENU_select_prev();
	_delay_ms(delay);
	MENU_select_prev();
	_delay_ms(delay);
	MENU_select_prev();
}


void MENU_run() {
	bool change = false;
	if (JOY_getDirection() == DOWN) {
		MENU_select_next();
		change = true;
		} else if (JOY_getDirection() == UP) {
		MENU_select_prev();
		change = true;
	}
	
	if (JOY_button_pressed(JOY_BUTTON)) {
		MENU_enter_selection();
		change = true;
	}
	
	else if (JOY_button_pressed(LEFT_BUTTON)) {
		MENU_go_back();
		change = true;
	}
	
	// Make sure the selection doesn't move multiple places at once.
	if (change){
		_delay_ms(255);
		change = false;
	}
}



menu_entry* add_entry(menu_entry* parent_node, char* name, void* function_ptr){
	
	// Create the new node
	menu_entry* new_entry = (menu_entry*) malloc(sizeof(menu_entry));
	new_entry->name = name;
	new_entry->parent_node = parent_node;
	new_entry->child_node = NULL;  // A new node has no children
	new_entry->next_sibling_node = NULL;  // The new child is the last child
	new_entry->prev_sibling_node = NULL;
	new_entry->function_ptr = function_ptr;
	
	// If the node has no parent, just return the node.
	if (parent_node == NULL) return new_entry;
	
	// If the parent does not have any other children, point the parent directly at the child.
	if (parent_node->child_node == NULL) {
		parent_node->child_node = new_entry;
	}
	
	// The parent has other children. Find the last place in the linked list.
	else {
		// The first child of the parent
		menu_entry* temp_node = parent_node->child_node;
		
		// Keep going until you reach the last child (that has no younger sibling)
		while (temp_node->next_sibling_node != NULL) {
			//printf("temp_node->next_sibling_node->name: %s\n", temp_node->next_sibling_node->name);
			temp_node = temp_node->next_sibling_node;
		}
		
		// Bind the new node to the current last child to become the new last child.
		temp_node->next_sibling_node = new_entry;
		new_entry->prev_sibling_node = temp_node;
	}
	
	// Parent/child/sibling bindings complete. Return the new node.
	return new_entry;
}