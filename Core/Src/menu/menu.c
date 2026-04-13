/*
 * menu.c
 *
 *  Created on: Mar 8, 2026
 *      Author: bozhi
 */

#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "main.h"
#include "io.h"
#include "display.h"
#include "eventManager.h"
#include "signalGen.h"

#define MAX_OPTIONS 10

static int menu_index = 0;
static int menu_size  = 0;

void op1(void);
void op2(void);
void op3(void);

static int amin_id = 0;
static char menuTitle[20];

Menu menu_table[MAX_OPTIONS];

void op1(void){
	amin_id = 1;
	setState(ST_SIMULATION);
}
void op2(void){
	amin_id = 2;
	setState(ST_SIMULATION);
}
void op3(void){
	print("option 3");
}


/**
 * Initialize any menu by a given menu table
 * and display the first menu option.
 *
 * KEYS:
 *
 * 1) Play/Stop the signal generator. If the config menu is opened,
 *    you'll go back to the main menu
 * 2) Open and browse through the menu.
 * 3) Select a menu option
**/
void initMenu(char* title, Menu *menu_ptr, int size){

	menu_index = -1;
	menu_size  = size;

	storeEncoderLastVal();

	strcpy(menuTitle, title);
	memset(menu_table, 0,  sizeof(menu_table));
	memcpy(menu_table, menu_ptr, sizeof(Menu)*size);
	menuBrowser();
}

/** Browse the menu options.
 *  Each time the KEY 2 is pressed, the next menu option will appear on the screen.
 *  The menu will be browsed in a loop until the key 1 is pressed.
 *  This will only display the menu title and the currently selected submenu option.
 *
 *  - To enter/browse the menu, press key 2.
 * */
void menuBrowser(void){

	menu_index++;
	//The end of the menu is reached. Restart the menu index and show the first option
	if(menu_index == menu_size){
		menu_index = 0;
	}

	//Before switching to the next menu option and displaying its parameters,
	clrscr();
	printAt(menuTitle, 0, 0);

	//Display the sub menu or the parameters of the current menu option
	if(menu_table[menu_index].f_view != 0){
		menu_table[menu_index].f_view();
	}
	else{
		printAt(menu_table[menu_index].op_name, 0, 1);
	}
}

/** Select the current menu option displayed on the screen.
 *  - By pressing key 3, you open the sub-menu which can either allows you to see more options
 *    or change the parameters of the current option.
 *
 * */
void menuSelect(void){
	menu_table[menu_index].f_select();
}

int getMenuIndex(void){
	return menu_index;
}

//EXAMPLE FUNCTIONS. YOU CAN DELETE THEM
/////////////////////////////////////////////////////////////////////////////
void menu_set(void){

	Menu menu_set[] = {

			{ "ANIMATION 1", op1, 0 },
			{ "ANIMATION 2", op2, 0 },
			{ "SET 3",       op3, 0 }
	};

	initMenu("=SETTINGS=",menu_set, sizeof(menu_set) / sizeof(menu_set[0]) );
}

void anim_1(void){

	char* text[] = {"Another", "Test","Running"};
	static int st = 0;

	print(text[st++]);
	if(st>2){
		st=0;
	}
}

void anim_2(void){

	char* text[] = {"Running", "Simulation","Program"};
	static int st = 0;

	print(text[st++]);
	if(st>2){
		st=0;
	}
}

void simulation(void){

	if(amin_id==1){
		anim_1();
	}
	else if(amin_id==2){
		anim_2();
	}
}

void stop(void){
	print("STOPED");
}

