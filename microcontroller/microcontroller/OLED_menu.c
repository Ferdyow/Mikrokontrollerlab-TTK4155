/*
 * OLED_menu.c
 *
 * Created: 27.09.2017 12:10:25
 *  Author: ferdyow
 */ 
#define F_CPU 4915200  // Clock frequency in Hz
#include "OLED_driver.h"
#include "OLED_menu.h"
#include "defines.h"

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

volatile menu_node* current_node;
volatile menu_node* selected_node;


menu_node* add_node(menu_node* parent_node, char* name){
	
	// Create the new node
	menu_node* new_node = (menu_node*) malloc(sizeof(menu_node));
	new_node->name = name;
	new_node->parent_node = parent_node;
	new_node->child_node = NULL;  // A new node has no children
	new_node->next_sibling_node = NULL;  // The new child is the last child
	new_node->prev_sibling_node = NULL;
	
	// If the node has no parent, just return the node.
	if (parent_node == NULL) return new_node;
	
	// If the parent does not have any other children, point the parent directly at the child.
	if (parent_node->child_node == NULL) {
		parent_node->child_node = new_node;
	}
	
	// The parent has other children. Find the last place in the linked list.
	else {
		// The first child of the parent
		menu_node* temp_node = parent_node->child_node;
		
		// Keep going until you reach the last child (that has no younger sibling)
		while (temp_node->next_sibling_node != NULL) {
			temp_node = temp_node->next_sibling_node;
		}
		
		// Bind the new node to the current last child to become the new last child.
		temp_node->next_sibling_node = new_node;
		new_node->prev_sibling_node = temp_node;
	}
	
	// Parent/child/sibling bindings complete. Return the new node.
	return new_node;
}


void MENU_init(){
	menu_node* main_menu = add_node(NULL, "Main menu");
	
	// Main menu
	menu_node* play = add_node(main_menu, "Play game");
	menu_node* highscore = add_node(main_menu, "Highscore");
	menu_node* settings = add_node(main_menu, "Settings");
	menu_node* extras = add_node(main_menu, "Extra features");
	
	// Sub-menu settings
	menu_node* brightness = add_node(settings, "Brightness");
	menu_node* font = add_node(settings, "Set font");
	
	// Sub-menu extras
	menu_node* keyboard = add_node(extras, "Keyboard");
	menu_node* mandelbrot = add_node(extras, "Mandelbrot");
	menu_node* pong = add_node(extras, "Play pong");
	menu_node* rcube = add_node(extras, "rotating cube");
	
	current_node = main_menu;
	selected_node = play;
	MENU_draw_current_menu();
}

void MENU_select_next() {
	if (selected_node -> next_sibling_node != NULL) {
		selected_node = selected_node -> next_sibling_node;
		MENU_draw_current_menu();
	}
}

void MENU_select_prev() {
	if (selected_node -> prev_sibling_node != NULL) {
		selected_node = selected_node -> prev_sibling_node;
		MENU_draw_current_menu();
	}
}

void MENU_enter_selection() {
	if (selected_node->child_node == NULL) return;
	
	current_node = selected_node;
	selected_node = current_node->child_node;
	MENU_draw_current_menu();
}

void MENU_go_back() {
	if (current_node->parent_node == NULL) return;
	
	current_node = current_node->parent_node;
	selected_node = current_node->child_node;
	MENU_draw_current_menu();
}

void MENU_draw_current_menu() {
	OLED_reset();
	int output_row = 0;
	
	menu_node* temp_node = current_node->child_node;
	
	while( temp_node != NULL){
		OLED_pos(output_row++, 0);
		if (temp_node == selected_node) {
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
