/*
 * signalGenerator.c
 *
 *  Created on: Mar 22, 2026
 *      Author: bozhi
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "eventManager.h"
#include "main.h"
#include "menu_config.h"
#include "signalGen.h"
#include "menu_simulation.h"
#include "dac.h"
#include "pwm.h"
#include "display.h"
#include "io.h"
#include "tim.h"

#define TAB_SIZE 5

static void genSineWave(char type);
static void genTriangleWave(char type);
static void genSawToothWave(char type);

uint16_t sine_table[N_SAMPLES];
static volatile unsigned char simulation_state = 0;

char *burst_type_list[] = { "None",  "Time",  "Pulse" };
char *wave_type_list[]  = { "NORMAL", "HALF", "RECT"  };

static unsigned int signal_time_start = 0;
static CONFIG_STRUCT* cfg;

/**
 * This table defines the available functions and the rectification type: (full/half/normal).
 * Not all the functions allow all the rectification types. The square wave for example,
 * does not allow any type of rectification.
 *
 * **/
SignalGenCfg signal_cfg_table[] = {

	{ FUNC_TYPE_SINE,     "SINE",   { SIGNAL_TYPE_NORMAL, SIGNAL_TYPE_FULL_RECT,  SIGNAL_TYPE_HALF_RECT }, genSineWave },
	{ FUNC_TYPE_TRIANGLE, "TRIANG", { SIGNAL_TYPE_NORMAL, SIGNAL_TYPE_HALF_RECT }, genTriangleWave  	 		       },
	{ FUNC_TYPE_SAW,      "SAW",    { SIGNAL_TYPE_NORMAL, SIGNAL_TYPE_HALF_RECT }, genSawToothWave                     },
	{ FUNC_TYPE_SQUARE,   "SQUARE", { SIGNAL_TYPE_NORMAL }, 0 },

	{0,0,{0},0}
};

void initSignalGen(void){

	cfg = getConfigStruct();
	setEncoderVal(1);
	generateSignalTable();
}

SignalGenCfg* getSignalCfgTable(void){
	return signal_cfg_table;
}

int getFuncById(FUNC_TYPE_ID id){

	int pos = 0;

	do{
		if(signal_cfg_table[pos].func_type == id){
			break;
		}
		pos++;
	}while(signal_cfg_table[pos].func_type != 0);

	return pos;
}

char* getFuncName(int func_id){

	int id = getFuncById(func_id);
	return signal_cfg_table[id].name;
}

char* getWaveType(int wave_type){
	return wave_type_list[wave_type];
}

int getWaveTabSize(void){
	return sizeof(wave_type_list) / sizeof(wave_type_list[0]);
}

char* getBurstType(int burst_type){
	return burst_type_list[burst_type];
}

int getBurstTabSize(void){
	return sizeof(burst_type_list) / sizeof(burst_type_list[0]);
}

unsigned char getSimulationState(void){
	return simulation_state;
}

unsigned char getDutyCycle(void){
	return cfg->duty_cycle;
}

/*
 * Read the encoder and return its value in Hz.
 * The default frequency value is 1000Hz
 * */
unsigned int getCurrentFrequency(void){

	unsigned int enc_data = getEncoderVal();

	if(enc_data == 0){
		enc_data++;
	}
	enc_data*=1000;

	return enc_data;
}

/**
 * Toggle the DAC channel. When the channel is ON, a sine wave signal(or another type)
 * is send to the output pin with a frequency defined by the rotary encoder.
 * The current frequency will be shown on the display.
 * */
void toggleSignalGenerator(void){

	//Start simulation
	if(simulation_state == 0){

		simulation_state  = 1;
		signal_time_start = HAL_GetTick();
		enableEncoder(0);
		dispCurrentFreq();

		//Start PWM generator
		if(cfg->func_type == FUNC_TYPE_SQUARE){
			StartPWM(getCurrentFrequency(), cfg->duty_cycle);
		}
		else{ //Start the analog signal generator: sine, triangle, etc.
			//For a full-rectified signal, the frequency must be divided by 2
			unsigned int freq = getCurrentFrequency();
			if(cfg->wave_type == SIGNAL_TYPE_FULL_RECT){
				freq/=2;
			}
			startDAC(sine_table, N_SAMPLES, freq);
		}
	}
	else{//Stop the simulation
		simulation_state = 0;

		if(cfg->func_type == FUNC_TYPE_SQUARE){
			stopPWM();
		}
		else{
			toggleDAC(0);
		}

		setState(MAIN_MENU);
		enableEncoder(1);
		dispCurrentFreq();
	}
}

/**
 * Signal generator dispatcher.
 * If an analog function type is selected(sine, triangle, sawtooth, etc),
 * a single period of the corresponding wave will be generated and stored to a buffer.
 * Later, this buffer will be used by the DAC to generate the output signal.
**/
void generateSignalTable(void){

	cfg = getConfigStruct();

	//Call the function to generate the
	if(cfg->func_type != FUNC_TYPE_SQUARE){

		int func_id = getFuncById(cfg->func_type);
		signal_cfg_table[func_id].f_ptr(cfg->wave_type);
	}
}

/******************************* The signal functions are defined below ***********************************/

static volatile unsigned int time_left = 0;
void DAC_Counter(void){

	if(simulation_state == 0){
		return;
	}

	if(cfg->burst_type == BURST_TIME){

		static unsigned int time_ellapsed = 0;
		unsigned int timeout = cfg->burst_value*1000;

		if(signal_time_start == 0){
			signal_time_start = HAL_GetTick();
		}

		time_ellapsed = HAL_GetTick() - signal_time_start;

		//The burst time is out. Stop the DAC
		if( time_ellapsed >= timeout){

			signal_time_start = HAL_GetTick();
			time_left = 0;
			updBurstCounter();
			toggleSignalGenerator();
		}
		else{
			time_left = (timeout - time_ellapsed+999)/1000;
		}
	}
	else if(cfg->burst_type == BURST_PULSES && simulation_state == 1){

		static volatile int count = 0;

		if(count >= (cfg->burst_value*2)){
			count = 0;
			toggleSignalGenerator();
			return;
		}
		count++;
	}
}

unsigned int getTimeLeft(void){
	return time_left;
}

/** Three types of wave might be generated:
 *
 *  Param:
 *  - Type: The wave rectification type.
 *
 *  - 0: No rectification
 *  - 1: Half rectified
 *  - 2: Full rectified
 * */
static void genSineWave(char type){

	memset(sine_table, 0, sizeof sine_table);

	//Pure sine wave:
	if(type == SIGNAL_TYPE_NORMAL){

		int offset   = AMPLITUDE/2;
		int temp_amp = offset;

		for (int i = 0; i < N_SAMPLES; i++){
		    sine_table[i] = (uint16_t)(offset + temp_amp * sinf(2.0f * M_PI * i / N_SAMPLES));
		}
	}
	else if(type == SIGNAL_TYPE_FULL_RECT){

	    for (int i = 0; i < N_SAMPLES; i++){
	        float s = sinf(2.0f * M_PI * i / N_SAMPLES);
	        if (s < 0.0f){
	            s = -s;   // Positive values only
	        }
	        sine_table[i] = (uint16_t)(4095.0f * s);
	    }
	}
	else if(type == SIGNAL_TYPE_HALF_RECT){

		for (int i = 0; i < N_SAMPLES; i++){
			float s = sinf(2.0f * M_PI * i / N_SAMPLES);

		    if(s < 0.0f){
		    	s = 0;   //NEGATIVE VALUES ARER SET TO 0
		    }
		    sine_table[i] = (uint16_t)(4095.0f * s);
		 }
	}
}

/** - You can generate a basic triangular wave.
 *  - A half-rectified.
 *  - Or a saw-tooth wave type.
 *
 *  @Param:
 *  - Type: The rectification type.
 *  - 0: No rectification.
 *  - 1: Half rectified.
 *  - 2: Full rectified.
 * */
static void genTriangleWave(char type){

	if(type == SIGNAL_TYPE_NORMAL){ //Normal wave. No rectification

		int pos=0;
		for(int i = 0; i < N_SAMPLES/2; i++){
			sine_table[pos++] = 2*(AMPLITUDE * i)/N_SAMPLES; //Reach max amplitude of 3.6v
		}
		for(int i = N_SAMPLES/2; i > 0; i--){
			sine_table[pos++] = 2*(AMPLITUDE * i)/N_SAMPLES;
		}
	}
	else if(type == SIGNAL_TYPE_HALF_RECT){//Half-rectified - WON'T BE USED FOR NOW

		int pos=0;
		for(int i = 0; i < N_SAMPLES; i++){
			sine_table[pos++] = (AMPLITUDE * i)/N_SAMPLES; //Reach max amplitude of 3.6v
		}
	}
	else if(type == SIGNAL_TYPE_FULL_RECT){ //Full wave rectification

	}
}

static void genSawToothWave(char type){

	int pos=0;

	for(int i = 0; i < N_SAMPLES; i++){
		sine_table[pos++] = (AMPLITUDE * i)/N_SAMPLES; //Reach max amplitude of 3.6v
	}
}


