/*
 * menu_simulation.c
 *
 *  Created on: Mar 31, 2026
 *      Author: bozhi
 */

#include <stdio.h>
#include <string.h>

#include "menu_config.h"
#include "menu.h"
#include "menu_simulation.h"
#include "signalGen.h"
#include "main.h"
#include "io.h"
#include "display.h"
#include "eventManager.h"
#include "signalGen.h"

static unsigned int current_frequency = 1000;

/**
* Display the simulator parameters and status:
* - Freq: 1KHz
* - Wave: Sine
* - Type: Full
* - Burst: None
**/
void simulationMenu(void){

	char str[15];

	generateSignalTable();

	//Store the last encoder readings for the previous menu
	storeEncoderLastVal();

	//Set the new encoder value as the last known frequency
	setEncoderVal(current_frequency/1000);
	enableEncoder(1);

	clrscr();
	printAt(" =MAIN-MENU=", 0, 0);
	snprintf(str, sizeof str, "Off:%dHz", current_frequency);
	printAt(str, 0, 1);

	CONFIG_STRUCT *cfg = getConfigStruct();
	snprintf(str, sizeof str, "%s:%s", getFuncName(cfg->func_type), getWaveType(cfg->wave_type));
	printAt(str, 0, 2);


	if(cfg->burst_type > 0){
		snprintf(str, sizeof str, "Burst:%d%s", cfg->burst_value, cfg->burst_type == 1 ? "(s)" : " ");
		printAt(str, 0, 3);
	}
}

/* Display the current frequency selected by the encoder.
 * This will force to display the frequency no matter its current value.
 *
 * NOTE: The frequency is updated by reading directly the encoder value.
 * */
void dispCurrentFreq(void){

	char str[15];
	snprintf(str, sizeof str, "%s:%dHz", getSimulationState() ? "ON" : "OFF", getCurrentFrequency());
	printAt(str, 0, 1);
}

/**
 * Reads the frequency and display it on the screen.
 * The value will be displayed only if the encoder is rotated.
 * */
void updateFrequency(void){

	if(current_frequency != getCurrentFrequency()){
		current_frequency = getCurrentFrequency();
		dispCurrentFreq();
	}
}

/**
 * Display the burst time left
 * */
void updBurstCounter(void){

	char str[15];
	static CONFIG_STRUCT* cfg;
	cfg = getConfigStruct();

	if(cfg->burst_type == BURST_NONE){
		return;
	}

	unsigned int time_left = getTimeLeft();

	snprintf(str, sizeof str, "Burst:%d(s)",time_left);
	printAt(str, 0, 3);
}


