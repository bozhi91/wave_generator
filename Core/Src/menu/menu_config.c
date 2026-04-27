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
static void display_burst(void);
static void displayBurstValue(short burst);
static void display_pwm_val(void);

static void config_encoder(unsigned short *param_value);

static CONFIG_STRUCT cfg_struct = {
		FUNC_TYPE_SINE,
		SIGNAL_TYPE_NORMAL,
		BURST_NONE, 5, 50 };

/** Define the options for the configuration menu **/
Menu menu_cfg_table[] = {
		  //tile   //confirm        //view function
		{ "Func",  set_func_type , display_func_type  },
		{ "Wave" , set_wave_type , display_wave_type  },
		{ "Burst", set_burst_type, display_burst	  },
		{ "About", about,          0		          }
};

CONFIG_STRUCT* getConfigStruct(void){
	return &cfg_struct;
}

static void about(void){

	printAt( "(C)Bozhidar",  0, 0 );
	printAt( "03/2026",	     0, 1 );
	printAt( "Firm.Ver:1.0", 0, 2 );
	printAt( "Hw.Ver:1.0",   0, 3 );
}

/** Initialize the config menu ***/
void menu_cfg(void){

	initMenu("=MENU CONFIG=", menu_cfg_table, sizeof(menu_cfg_table) / sizeof(menu_cfg_table[0]) );

	enableEncoder(0);

	//Store the last encoder readings for the previous menu
	storeEncoderLastVal();

	//Set the new encoder value as the last known burst type
	setEncoderVal(cfg_struct.burst_value);
}

/** Configure the rotary encoder. We can reuse it for a different purposes in the configuration menu.
 *  We can use the encoder to adjust values such as: pwm, burst value(pulses, time, etc).
 *	We pass as a parameter the new encoder's value we want to adjust.
 *	For example, we switch to burst(type: time). So we want to adjust the time(in seconds).
 *
 *  The function will store the current encoder value to a temporary variable.
 *  Then will set the current encoder value using the variable we've passed as a parameter
 *  and then enable the encoder.
 *
 *  As we rotate the encoder, its initial value will be the value we've passed as a parameter.
 *
 *	@Params:
 *	- param_value: The new encoder value we want to set.
 * */
static unsigned short* encoder_ptr = 0;
static void config_encoder(unsigned short *param_value){

	encoder_ptr = param_value;
	enableEncoder(1);
	//storeEncoderLastVal();
	//setEncoderVal(param_value);
}

/**
 * Update burst value automatically when the encoder is rotated.
 * The current encoder value is displayed on the screen once
 * This function is called from the state machine.
 * */
void updateBurst(void){

	static unsigned char cfg_encoder = 1;

	//Read current encoder value and allow only positive values
	short tmp_val = getEncoderVal();

	if(tmp_val == 0){
		tmp_val++;
	}

	//Current menu option is: BURST TYPE and the Burst type != NONE. We adjust the burst value
	if(getMenuIndex() == 2){

		//Configure the encoder to adjust the burst values: time or pulses
		if(cfg_struct.burst_type == 1 && cfg_encoder == 1){ //burst type -> time
			cfg_encoder = 0;
			config_encoder(&cfg_struct.burst_value);
		}
		else if(cfg_struct.burst_type == 2 && cfg_encoder == 1){ //burst type -> pulses

			//TODO: replace the burst_value with pulse_burst

			cfg_encoder = 0;
			config_encoder(&cfg_struct.burst_value);
		}
		else if(cfg_struct.burst_type == 0 && isEncoderEnabled()){//BURST TYPE: NONE -> DISABLE THE ENCODER
			cfg_encoder = 1;
			enableEncoder(0);
		}

		//If the encoder value has changed, we update the config structure and display the burst values on screen
		if(*encoder_ptr != tmp_val && isEncoderEnabled()){
			*encoder_ptr = tmp_val;
			displayBurstValue(*encoder_ptr);
		}
	}
	else{//Disable encoder otherwise
		cfg_encoder = 1;
		enableEncoder(0);
	}
}

/** Display the current PWM value **/
static void display_pwm_val(void){

	char str[10];

	int duty_cycle = 50;
	snprintf(str, sizeof str, "Duty:%d%%", duty_cycle);
	printAt(str, 0, 2);
}

/** Select the function type and store it to the configuration structure.
 *  So far, we have: Square wave, Sine wave, Triangle wave, Saw-tooth wave.
 *  For more information, check the file: signalGenerator.c
 * */
static void set_func_type(void){

	SignalGenCfg* signal_cfg   = getSignalCfgTable();
	static unsigned char index = 0;

	cfg_struct.wave_type = SIGNAL_TYPE_NORMAL;

	//The end of the function table is reached. Restart the index
	if(signal_cfg[index].func_type == 0){
		index = 0;
	}

	//Assign the current function type to the config struct
	cfg_struct.func_type = signal_cfg[index].func_type;
	display_func_type();
	index++;
}

//Display the current function type: SINE, SQUARE, TRIANGLE, ETC...
static void display_func_type(void){

	char str[20];

	snprintf(str,sizeof str, "%s:%s",  menu_cfg_table[getMenuIndex()].op_name , getFuncName(cfg_struct.func_type));
	printAt(str, 0, 1);
}

/**
 * Select the wave type: NORMAL, HALF-RECTIFIED, FULL RECTIFIED
 * */
static void set_wave_type(void){

	//Wave rectification is allowed ONLY for the analog wave types: SINE, TRIANGULAR WAVE, SAW-TOOTH, etc.
	//Square waves generated by the PWM channel doesn't have rectification.

	SignalGenCfg* signal_cfg   = getSignalCfgTable();
	unsigned char* wave_types  = signal_cfg[getFuncById(cfg_struct.func_type)].wave_type;
	static unsigned char index = 0;
	char wave_types_size 	   = sizeof(signal_cfg[getFuncById(cfg_struct.func_type)].wave_type);

	//End of the type list is reached. Restart the index
	if(index >= wave_types_size){
		index = 0;
	}

	cfg_struct.wave_type = wave_types[index++];
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

	display_burst();
}

//Show the current burst type
static void display_burst(void){

	char str[20];

	snprintf(str, sizeof str, "%s:%s", menu_cfg_table[getMenuIndex()].op_name, getBurstType(cfg_struct.burst_type) );
	printAt(str, 0, 1);
	displayBurstValue(cfg_struct.burst_value);
}

/** Display the current burst value: time, pulses, etc. */
static void displayBurstValue(short burst){

	char str[15] = {' '};

	if(cfg_struct.burst_type>0){
		snprintf(str, sizeof str, ">Val:%d%s", burst, cfg_struct.burst_type==1 ? "(s)" : "(PULSES)");
	}
	printAt(str, 0, 2);
}

