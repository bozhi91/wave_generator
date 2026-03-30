/*
 * menu_config.c
 *
 *  Created on: Mar 31, 2026
 *      Author: bozhi
 */
#include <stdio.h>
#include <string.h>

#include "menu.h"
#include "menu_simulation.h"
#include "main.h"
#include "io.h"
#include "display.h"
#include "eventManager.h"
#include "signalGen.h"

static void about(void);
static void set_func(void);
static void set_wave(void);
static void hover_func(int param);
static void set_burst(void);

static unsigned char func_type   = 0;
static unsigned char wave_type   = 0;
static unsigned char burst_type  = 0;
static unsigned char burst_value = 0;

static char *func_array[] = {
		"Sine",
		"Square",
		"Triangle",
		"SawTooth"
};

void menu_cfg(void){

	Menu menu_cfg[] = {

			{ "Func" , set_func , hover_func, 1 },
			{ "Wave" , set_wave , hover_func, 2 },
			{ "Burst", set_burst, hover_func, 3 },
			{ "About", about,     0}
	};

	initMenu("=MENU CONF=", menu_cfg, sizeof(menu_cfg) / sizeof(menu_cfg[0]) );
}

static void about(void){

	printAt("(C)Bozhidar",0,0);
	printAt("03/2026",0,1);
}

static void set_func(void){

	static int index = 1;
	char str[15];
	int menu_size = sizeof(func_array) / sizeof(func_array[0]);

	index++;
	if(index == menu_size){
		index = 0;
	}
	func_type = index;

	snprintf(str, sizeof str, "Func:%s", func_array[func_type]);
	print(str);
}

static void set_wave(void){

	static int index   = 1;
	char *wave_array[] = {
			"FULL-Rect",
			"HALF-RECT",
			"NORMAL",
	};
	char str[15];
    int menu_size = sizeof(wave_array) / sizeof(wave_array[0]);

	snprintf(str, sizeof str, "Wave:%s", wave_array[index]);
	print(str);

	wave_type = index;
	index++;
	if(index == menu_size){
		index = 0;
	}
}

static void set_burst(void){

	static int index    = 1;
	char *burst_array[] = {
			"None",
			"Time",
			"Pulse",
	};
	char str[15];
    int menu_size = sizeof(burst_array) / sizeof(burst_array[0]);

	snprintf(str, sizeof str, "Burst:%s", burst_array[index]);
	print(str);

	burst_type = index;
	index++;
	if(index == menu_size){
		index = 0;
	}
}

static void hover_func(int param){

	char str[15];

	if(param == 1){
		//todo: print the submenu text on a new line
		//Let the menu label remain visible
		snprintf(str, sizeof str, "Func:%s", func_array[func_type]);
		print(str);
	}
}
