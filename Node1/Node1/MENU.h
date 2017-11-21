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

/**
 * Initialized main menu and submenu, set current menu to main menu
 */
void MENU_init();

/**
 * select next MENU item if not the last one
 */
void MENU_select_next();

/**
 * select previous MENU item if not the first one
 */
void MENU_select_prev();

/**
 * draw the selected menu on the OLED screen
 */
void MENU_draw_current_menu();

/**
 * Enters function if bottom item, enters submenu otherwise, does nothing if it has neither
 */
void MENU_enter_selection();

/**
 * Go to parent menu
 */
void MENU_go_back();

/**
 * Navigates the menu based on input from joystick
 */
void MENU_run();

/**
 * Creates a menu-entry
 * @param {menu_entry} parent - pointer to parent menu
 * @param {char} name - name of the menu-entry
 * @param {void} function_ptr - pointer to function to be called when selected
 * @return - the created menu-entry
 */
menu_entry* add_entry(menu_entry* parent, char* name, void* function_ptr);

#endif /* MENU_H_ */