/*
 * OLED_menu.h
 *
 * Created: 27.09.2017 12:10:40
 *  Author: ferdyow
 */ 


#ifndef OLED_MENU_H_
#define OLED_MENU_H_

/************************************************************************************
*The menu system is implemented as a linked list
*Every node points to its parent, its leftmost child and its right and left sibling
*The sibling list is circular
************************************************************************************/


#include <string.h>

typedef struct menu_node menu_node;

struct menu_node{
	char* name;
	menu_node* parent_node;
	menu_node* child_node;
	menu_node* next_sibling_node;
	menu_node* prev_sibling_node;
};

menu_node* add_node(menu_node* parent, char* name);


void MENU_init();


void MENU_test();

void MENU_select_next();

void MENU_select_prev();

void MENU_draw_current_menu();

void MENU_enter_selection();

void MENU_go_back();

#endif /* OLED_MENU_H_ */