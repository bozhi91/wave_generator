/*
 * signalGen.h
 *
 *  Created on: Mar 22, 2026
 *      Author: bozhi
 */

#ifndef INC_SIGNALGEN_H_
#define INC_SIGNALGEN_H_

	#define TIM6_PSC   0
	#define N_SAMPLES  100
	#define AMPLITUDE  4095
	#define V_OFFSET   0
	#define STEP       M_PI*2.0f/360.0f*3.0f

	void initSignalGen(void);
	void toggleSignalGenerator(void);
	unsigned int getCurrentFrequency(void);
	unsigned char getSimulationState(void);
	unsigned char getDutyCycle(void);

	char* getFuncName(int func_id);
	char* getWaveType(int wave_type);
	char* getBurstType(int burst_type);

	int getFuncTabSize(void);
	int getWaveTabSize(void);
	int getBurstTabSize(void);
	void generateSignalTable(void);

	typedef enum{
		FUNC_TYPE_SINE,
		FUNC_TYPE_SQUARE,
		FUNC_TYPE_TRIANGLE,
		FUNC_TYPE_SAW,
	}FUNC_TYPE_ID;

	typedef enum{
		SIGNAL_TYPE_NORMAL,
		SIGNAL_TYPE_FULL_RECT,
		SIGNAL_TYPE_HALF_RECT,
	}SIGNAL_TYPE_ID;

	typedef struct{
		FUNC_TYPE_ID func_type;
		void (*f_ptr)(char signal_type);
	}SignalGenCfg;

#endif /* INC_SIGNALGEN_H_ */
