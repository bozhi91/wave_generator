/*
 * menu.h
 *
 *  Created on: Mar 8, 2026
 *      Author: bozhi
 */

#ifndef __MENU__
#define __MENU__

	typedef struct{
		char op_name[15];
		void (*f_select)(void);	//Function to be called when the select/confirm button is pressed
		void (*f_view)(void);   //Function to be called while browsing the menu. This will display the option name and some extra parameters.
	}Menu;

	void menuBrowser(void);
	void initMenu(char* title, Menu* menu_ptr, int menu_size);
	void menu_set(void);
	void menuSelect(void);
	int getMenuIndex(void);


	void simulation(void);

#endif
