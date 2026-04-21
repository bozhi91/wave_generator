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

static void display_func_type(void);
static void display_wave_type(void);
static void display_burst_type(void);
static void displayBurstValue(int burst);
static void display_pwm_val(void);

static void config_encoder(int param_value);

static CONFIG_STRUCT cfg_struct = { 0, 0, 0, 5, 50 };

Menu menu_cfg_table[] = {

		{ "Func" , set_func_type , display_func_type  },
		{ "Wave" , set_wave_type , display_wave_type  },
		{ "Burst", set_burst_type, display_burst_type },
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

/** Configure the rotary encoder. We can reuse it for a different purposes in the configuration menu.
 *  We can use the encoder to adjust values such as: pwm, burst value(pulses, time, etc).
 *
 *  The function will store the current encoder value to a temporary variable, set the variable we've passed as a parameter
 *  and then enable the encoder.
 *
 *	@Params:
 *	- param_value: The new encoder value we want to set.
 * */
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

	static unsigned int burst = 0;
	static char updEncoder   = 1;

	//Current menu option is: BURST TYPE and the Burst type != NONE. We adjust the burst value
	if(getMenuIndex() == 2){
		if(cfg_struct.burst_type > 0 && updEncoder == 1){
			burst = 0;
			updEncoder = 0;
			config_encoder(cfg_struct.burst_value);
		}
	}
	else if(getMenuIndex() != 2){//Disable encoder otherwise
		burst = 0;
		updEncoder = 1;
		enableEncoder(0);
	}

	/*else if(getMenuIndex() == 0 && cfg_struct.func_type == 1){//ADJUST THE PWM VALUE
		if(!isEncoderEnabled()){
			burst = 0xffff;
			config_encoder(getDutyCycle());
		}
	}*/

	//Read current encoder value and allow only positive values
	unsigned int burst_value = getEncoderVal();
	if(burst_value == 0){
		burst_value++;
	}

	if(burst != burst_value && getMenuIndex() == 2){
		burst = burst_value;
		displayBurstValue(burst);
	}

	/*
	burst_value = getEncoderVal();

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

/** Select the function type.
 *  So far, we have: Square wave, Sine wave, Triangle wave, Saw-tooth wave.
 *  However, we can define much more funcitons
 * */
static void set_func_type(void){

	cfg_struct.func_type++;

	if(cfg_struct.func_type == getFuncTabSize()){
		cfg_struct.func_type = 0;
	}

	display_func_type();
}

//Display the current function type: SINE, SQUARE, TRIANGLE, ETC...
static void display_func_type(void){

	char str[20];

	snprintf(str,sizeof str, "%s:%s", menu_cfg_table[getMenuIndex()].op_name, getFuncName(cfg_struct.func_type) );
	printAt(str, 0, 1);

	//Adjust the duty cycle
	/*if(cfg_struct.func_type == 1){
		config_encoder(getDutyCycle());
		display_pwm_val();
	}
	else{
		clr_row(2, 1);
		enableEncoder(0);
	}*/
}

/** Display the current PWM value **/
static void display_pwm_val(void){

	char str[10];

	int duty_cycle = 50;
	snprintf(str, sizeof str, "Duty:%d%%", duty_cycle);
	printAt(str, 0, 2);
}

/**
 * Select the wave type: NORMAL, HALF-RECTIFIED, FULL RECTIFIED
 * */
static void set_wave_type(void){

    cfg_struct.wave_type++;

	if(cfg_struct.wave_type == getWaveTabSize()){
		cfg_struct.wave_type = 0;
	}
	display_wave_type();
}

//Show the current wave type
static void display_wave_type(void){

	char str[20];

	snprintf(str, sizeof str, "%s:%s", menu_cfg_table[getMenuIndex()].op_name, getWaveType(cfg_struct.wave_type));
	printAt(str, 0, 1);
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

    cfg_struct.burst_type++;

	if(cfg_struct.burst_type == getBurstTabSize()){
		cfg_struct.burst_type = 0;
	}

	display_burst_type();
}

//Show the current burst type
static void display_burst_type(void){

	char str[20];

	snprintf(str, sizeof str, "%s:%s", menu_cfg_table[getMenuIndex()].op_name, getBurstType(cfg_struct.burst_type) );
	printAt(str, 0, 1);
	displayBurstValue(cfg_struct.burst_value);
}

/** Display the current burst value: time, pulses, etc. */
static void displayBurstValue(int burst){

	char str[15] = {' '};

	cfg_struct.burst_value = burst;

	if(cfg_struct.burst_type>0){
		snprintf(str, sizeof str, ">Val:%d%s", cfg_struct.burst_value, cfg_struct.burst_type==1 ? "(s)" : "(PULSES)");
	}
	printAt(str, 0, 2);
}

