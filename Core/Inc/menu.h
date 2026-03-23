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
		void (*f_ptr)(void);
		void (*f_hover)(int param);
		int param;
	}Menu;

	void menuBrowser(void);
	void initMenu(Menu* menu_ptr, int menu_size);
	void menu_cfg(void);
	void menu_set(void);
	void mainMenu(void);
	void menuSelect(void);

	void simulation(void);

#endif
