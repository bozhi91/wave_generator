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
static void set_func_type(void);
static void set_wave_type(void);
static void set_burst_type(void);

static void show_func_type(void);
static void show_wave_type(void);
static void show_burst_type(void);
static void displayBurstValue(int burst);

static unsigned char func_type   = 0;
static unsigned char wave_type   = 0;
static unsigned char burst_type  = 0;
static unsigned char burst_value = 5; //5 seconds of burst duration

char *func_array[]  = { "Sine","Square","Triangle","SawTooth" };
char *burst_array[] = { "None", "Time", "Pulse" 			  };
char *wave_array[]  = { "NORMAL","FULL-Rect", "HALF-RECT"     };

/** Initialize the config menu
 *
 * */
void menu_cfg(void){

	Menu menu_cfg[] = {

			{ "Func" , set_func_type , show_func_type  },
			{ "Wave" , set_wave_type , show_wave_type  },
			{ "Burst", set_burst_type, show_burst_type },
			{ "About", about,          0			   }
	};

	initMenu("=MENU CONFIG=", menu_cfg, sizeof(menu_cfg) / sizeof(menu_cfg[0]) );

	enableEncoder(0);

	//Store the last encoder readings for the previous menu
	storeEncoderLastVal();

	//Set the new encoder value as the last known frequency
	setEncoderVal(burst_value);
}


/**
 * Update burst value automatically when the encoder is rotated.
 * The current encoder value is displayed on the screen once
 * This function is called from the state machine.
 * */
void updateBurst(void){

	static unsigned int burst = 0xffff;
	burst_value = getEncoderVal();

	if(burst_value == 0){
		burst_value++;
	}

	//Enable the encoder only for the burst menu
	if(getMenuIntex() == 3 && !isEncoderEnabled()){
		enableEncoder(1);
	}
	else if(getMenuIntex() != 3 && isEncoderEnabled()){
		enableEncoder(0);
	}

	//If the encoder is rotated and the current option is burst type,
	//update the burst value and display it on the screen
	if(burst != burst_value && getMenuIntex() == 3){
		burst = burst_value;
		displayBurstValue(burst);
	}
}

/**
 * Force update the current burst value
 * */
static void displayBurstValue(int burst){

	char str[15];

	snprintf(str, sizeof str, "Val:%d%s",burst_value, burst_type==1 ? "(s)" : "(PULSES)");
	printAt(str, 0, 2);
}

static void about(void){

	printAt("(C)Bozhidar",0, 0);
	printAt("03/2026", 0, 1);
	printAt("Ver:1.0", 0, 2);
}

/** Select the function type.
 *  So far, we have: Square wave, Sine wave, Triangle wave, Saw-tooth wave.
 *  However, we can define much more funcitons
 * */
static void set_func_type(void){

	int list_size = sizeof(func_array) / sizeof(func_array[0]);

	func_type++;
	if(func_type == list_size){
		func_type = 0;
	}

	show_func_type();
}

/**
 * Select the wave type: NORMAL, HALF-RECTIFIED, FULL RECTIFIED
 * */
static void set_wave_type(void){

    int list_size = sizeof(wave_array) / sizeof(wave_array[0]);

    wave_type++;
	if(wave_type == list_size){
		wave_type = 0;
	}

	show_wave_type();
}

/**
 * Set the duration/number of the pulses.
 * - For example, a 10s burst will generate pulses of a certain frequency during 10 seconds.
 *   After that, the generator will stop automatically.
 *
 *  - A burst of 10 pulses, will generate 10 pulses at the given frequency.
 *  - If no burst mode is selected, then a continuous signal will be generated until its interrupted by the user.
 * */
static void set_burst_type(void){

    int list_size = sizeof(burst_array) / sizeof(burst_array[0]);

    burst_type++;
	if(burst_type == list_size){
		burst_type = 0;
	}

	show_burst_type();
}

//Show the current wave type
static void show_wave_type(void){

	char str[20];

	snprintf(str, sizeof str, "WAVE:%s", wave_array[wave_type]);
	printAt(str,0,1);
}

//Show the current burst type
static void show_burst_type(void){

	char str[20];

	snprintf(str, sizeof str, "BURST:%s", burst_array[burst_type]);
	printAt(str, 0, 1);
	displayBurstValue(burst_value);
}

//Show the current function type: sine, square, triangle
static void show_func_type(void){

	char str[20];

	snprintf(str, sizeof str, "FUNC:%s", func_array[func_type]);
	printAt(str, 0, 1);
}

