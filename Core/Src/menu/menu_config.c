/*
 * menu_config.c
 *
 *  Created on: Mar 31, 2026
 *      Author: bozhi
 */
#include <stdio.h>
#include <string.h>

#include "menu_config.h"
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
static void show_pwm_val(void);
static void config_encoder(int param_value);

static unsigned char duty_cycle  = 50; //50% of duty cycle

static CONFIG_STRUCT cfg_struct = { 0, 0, 0, 5, 50 };

char *func_array[]  = { "Sine","Square","Triang","Saw" };
char *burst_array[] = { "None", "Time", "Pulse" 	   };
char *wave_array[]  = { "NORMAL","FULL", "HALF"        };

Menu menu_cfg_table[] = {

		{ "Func" , set_func_type , show_func_type  },
		{ "Wave" , set_wave_type , show_wave_type  },
		{ "Burst", set_burst_type, show_burst_type },
		{ "About",  about,         0		       }
};

CONFIG_STRUCT getConfigStruct(void){
	return cfg_struct;
}

static void about(void){

	printAt("(C)Bozhidar",0, 0);
	printAt("03/2026", 0, 1);
	printAt("Firm.Ver:1.0", 0, 2);
}

/** Initialize the config menu ***/
void menu_cfg(void){

	initMenu("=MENU CONFIG=", menu_cfg_table, sizeof(menu_cfg_table) / sizeof(menu_cfg_table[0]) );

	enableEncoder(0);

	//Store the last encoder readings for the previous menu
	storeEncoderLastVal();

	//Set the new encoder value as the last known frequency
	setEncoderVal(cfg_struct.burst_value);
}

char* getFuncName(int func_id){
	return func_array[func_id];
}

char* getWaveType(int wave_type){
	return wave_array[wave_type];
}

char* getBurstType(int burst_type){
	return burst_array[burst_type];
}

static void config_encoder(int param_value){

	enableEncoder(1);
	storeEncoderLastVal();
	setEncoderVal(param_value);
}

/**
 * Update burst value automatically when the encoder is rotated.
 * The current encoder value is displayed on the screen once
 * This function is called from the state machine.
 * */
void updateBurst(void){

	static unsigned int burst = 0xffff;

	if(getMenuIndex() == 2 && cfg_struct.burst_type > 0){ //burst type
		if( !isEncoderEnabled()){
			burst = 0xffff;
			config_encoder(cfg_struct.burst_value);
		}
	}
	else if(getMenuIndex() == 0 && cfg_struct.func_type == 1){//pwm
		if(!isEncoderEnabled()){
			burst = 0xffff;
			config_encoder(duty_cycle);
		}
	}

	/*burst_value = getEncoderVal();

	if(burst_value == 0){
		burst_value++;
	}

	//Enable the encoder only for the burst menu
	if(getMenuIndex() == 2 && !isEncoderEnabled()){
		//enableEncoder(1);
		config_encoder(burst_value);
	}
	else if(getMenuIndex() != 2 && isEncoderEnabled()){
		enableEncoder(0);
	}

	//If the encoder is rotated and the current option is burst type,
	//update the burst value and display it on the screen
	if(burst != burst_value && getMenuIndex() == 2){
		burst = burst_value;
		displayBurstValue(burst);
	}*/
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

    cfg_struct.burst_type++;
	if(cfg_struct.burst_type == list_size){
		cfg_struct.burst_type = 0;
	}

	show_burst_type();
}


//Show the current function type: sine, square, triangle
static void show_func_type(void){

	char str[20];

	snprintf(str,sizeof str, "%s:%s", menu_cfg_table[getMenuIndex()].op_name, func_array[cfg_struct.func_type]);
	printAt(str, 0, 1);

	if(cfg_struct.func_type == 1){
		config_encoder(duty_cycle);
		show_pwm_val();
	}
	else{
		clr_row(2, 1);
		enableEncoder(0);
	}
}

/**
 * Adjust the PWM value, range 10-100%;
 * For Square wave type only
 * **/
static void show_pwm_val(void){

	char str[10];

	int duty_cycle = 50;
	snprintf(str, sizeof str, "Duty:%d%%", duty_cycle);
	printAt(str, 0, 2);
}

/** Select the function type.
 *  So far, we have: Square wave, Sine wave, Triangle wave, Saw-tooth wave.
 *  However, we can define much more funcitons
 * */
static void set_func_type(void){

	int list_size = sizeof(func_array) / sizeof(func_array[0]);

	cfg_struct.func_type++;
	if(cfg_struct.func_type == list_size){
		cfg_struct.func_type = 0;
	}

	show_func_type();
}

//Show the current wave type
static void show_wave_type(void){

	char str[20];

	snprintf(str, sizeof str, "%s:%s",menu_cfg_table[getMenuIndex()].op_name, wave_array[cfg_struct.wave_type]);
	printAt(str, 0, 1);
}

/**
 * Select the wave type: NORMAL, HALF-RECTIFIED, FULL RECTIFIED
 * */
static void set_wave_type(void){

    int list_size = sizeof(wave_array) / sizeof(wave_array[0]);

    cfg_struct.wave_type++;
	if(cfg_struct.wave_type == list_size){
		cfg_struct.wave_type = 0;
	}
	show_wave_type();
}

//Show the current burst type
static void show_burst_type(void){

	char str[20];

	snprintf(str, sizeof str, "%s:%s", menu_cfg_table[getMenuIndex()].op_name, burst_array[cfg_struct.burst_type]);
	printAt(str, 0, 1);
	displayBurstValue(cfg_struct.burst_value);
}

/** Force update the current burst value */
static void displayBurstValue(int burst){

	char str[15] = {' '};

	if(cfg_struct.burst_type>0){
		snprintf(str, sizeof str, ">Val:%d%s", cfg_struct.burst_value, cfg_struct.burst_type==1 ? "(s)" : "(PULSES)");
	}
	printAt(str, 0, 2);
}

