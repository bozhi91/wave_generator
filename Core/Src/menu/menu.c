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

#define MAX_OPTIONS 10

static int menu_index = 0;
static int menu_size  = 0;

void op1(void);
void op2(void);
void op3(void);

static int amin_id = 0;

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
	display_string("option 3");
}

/**
 * Initialize the menu by a given menu table
 * and display the first menu option.
**/
void initMenu(Menu *menu_ptr, int size){

	menu_index = 0;
	menu_size  = size;

	memset(menu_table, 0,  sizeof(menu_table));
	memcpy(menu_table, menu_ptr, sizeof(Menu)*size);
	menuBrowser();
}

void menuBrowser(void){

	if(menu_index == menu_size){
		menu_index = 0;
	}

	//Display the menu option label
	display_string(menu_table[menu_index].op_name);

	//It the hover function is set, call it.
	if(menu_table[menu_index].f_hover != 0){
		menu_table[menu_index].f_hover(menu_table[menu_index].param);
	}
	menu_index++;
}

void menuSelect(void){

	int tmp_index = (menu_index == 0) ? (menu_size-1) : menu_index-1;
	menu_table[tmp_index].f_ptr();
	//set_state(MAIN_MENU)
}

static unsigned char func_type   = 0;
static unsigned char wave_type   = 0;
static unsigned char burst_type  = 0;
static unsigned char burst_value = 0;

void set_func(void){

	static int index   = 1;
	char *func_array[] = {
			"Sine",
			"Square",
			"Triang.",
			"Saw"
	};
	char str[15];
	int menu_size = sizeof(func_array) / sizeof(func_array[0]);

	index++;
	if(index == menu_size){
		index = 0;
	}
	func_type = index;

	snprintf(str, sizeof str, "Func:%s", func_array[func_type]);
	display_string(str);
}

void set_wave(void){

	static int index   = 1;
	char *wave_array[] = {
			"Full",
			"Half",
			"Rect",
	};
	char str[15];
    int menu_size = sizeof(wave_array) / sizeof(wave_array[0]);

	snprintf(str, sizeof str, "Wave:%s", wave_array[index]);
	display_string(str);

	wave_type = index;
	index++;
	if(index == menu_size){
		index = 0;
	}
}

void set_burst(void){

	static int index    = 1;
	char *burst_array[] = {
			"None",
			"Time",
			"Pulse",
	};
	char str[15];
    int menu_size = sizeof(burst_array) / sizeof(burst_array[0]);

	snprintf(str, sizeof str, "Burst:%s", burst_array[index]);
	display_string(str);

	burst_type = index;
	index++;
	if(index == menu_size){
		index = 0;
	}
}


void hover_func(int param){

	char str[15];
	char *func_array[] = {
			"Sine",
			"Square",
			"Triang.",
			"Saw"
	};

	if(param == 1){
		//todo: print the submenu text on a new line
		//Let the menu label remain visible
		snprintf(str, sizeof str, "Func:%s", func_array[func_type]);
		display_string(str);
	}
}

void about(void){
	display_string("(C)Bozhidar");
}

void menu_cfg(void){

	Menu menu_cfg[] = {
			{ "Func" , set_func , hover_func, 1 }, //todo: when an menu option is displayed, open the submenu and display the selected parameter
			{ "Wave" , set_wave , hover_func, 2 },
			{ "Burst", set_burst, hover_func, 3 },
			{ "About", about,     0}
	};

	initMenu(menu_cfg, sizeof(menu_cfg) / sizeof(menu_cfg[0]) );
}

void menu_set(void){

	Menu menu_set[] = {
			{ "ANIMATION 1", op1, 0 },
			{ "ANIMATION 2", op2, 0 },
			{ "SET 3",       op3, 0 }
	};

	initMenu(menu_set, sizeof(menu_set) / sizeof(menu_set[0]) );
}


void anim_1(void){

	char* text[] = {"Another", "Test","Running"};
	static int st = 0;

	display_string(text[st++]);
	if(st>2){
		st=0;
	}
}

void anim_2(void){

	char* text[] = {"Running", "Simulation","Program"};
	static int st = 0;

	display_string(text[st++]);
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
	display_string("STOPED");
}

void mainMenu(void){
	display_string("=MAIN-MENU=");
}

void readEncoder(void){

	char str[10];
	static short enc_val = 0;

	if(enc_val !=  getEncoderVal()){
		enc_val = getEncoderVal();
		snprintf(str, sizeof str, "Enc:%d", enc_val);
		display_string(str);
	}
}

