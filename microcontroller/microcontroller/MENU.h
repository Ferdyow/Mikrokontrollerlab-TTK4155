/*
 * OLED_menu.h
 *
 * Created: 27.09.2017 12:10:40
 *  Author: ferdyow
 */ 


#ifndef MENU_H_
#define MENU_H_

/************************************************************************************
*The menu system is implemented as a linked list
*Every node points to its parent, its leftmost child and its right and left sibling
*The sibling list is circular
************************************************************************************/


#include <string.h>

typedef struct menu_entry menu_entry;

struct menu_entry{
	char* name;
	menu_entry* parent_node;
	menu_entry* child_node;
	menu_entry* next_sibling_node;
	menu_entry* prev_sibling_node;
	void(*function_ptr)();
};


void MENU_init();

void MENU_test();

void MENU_select_next();

void MENU_select_prev();

void MENU_draw_current_menu();

void MENU_enter_selection();

void MENU_go_back();

void MENU_run();

menu_entry* add_entry(menu_entry* parent, char* name, void* function_ptr);

#endif /* MENU_H_ */