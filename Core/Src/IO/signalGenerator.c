/*
 * signalGenerator.c
 *
 *  Created on: Mar 22, 2026
 *      Author: bozhi
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "signalGen.h"
#include "dac.h"
#include "pwm.h"
#include "display.h"
#include "io.h"

static void GenerateSineTable(void);
static void genTriangleWave(void);

uint16_t sine_table[N_SAMPLES];
//uint16_t triangle_table[N_SAMPLES];

/*  unsigned long sys_clock = HAL_RCC_GetSysClockFreq();
  unsigned int tim1 = HAL_RCC_GetPCLK2Freq();

  printf("SYSCLK: %lu\n", HAL_RCC_GetSysClockFreq());
  printf("HCLK  : %lu\n", HAL_RCC_GetHCLKFreq());
  printf("PCLK1 : %lu\n", HAL_RCC_GetPCLK1Freq());*/

//DAC_Write(2047); // Send a constant impulse of 2048(out of 4095) to the DAC chanel

void initSignalGen(void){

	//StartPWM(500, 50);
	//genTrianbleWave();
	//DAC_Write(2048);//12bits DAC - 0:4095

	GenerateSineTable();
	startDAC(sine_table, N_SAMPLES, 2000);
}

static unsigned char signal_state = 0;
void outputSignal(void){

	char str[20];

	if(signal_state==0){

		signal_state = 1;

		unsigned int enc_data = getEncoderVal();

		if(enc_data<0){
			enc_data*=-1;
		}
		else if(enc_data == 0){
			enc_data++;
		}
		enc_data*=1000;

		snprintf(str,sizeof str, "ON:%d Hz",enc_data);
		display_string(str);

		Set_DAC_Frequency(enc_data);
		toggleDAC(1);
	}
	else{
		signal_state=0;
		display_string("DAC: OFF");
		toggleDAC(0);
	}
}

static void GenerateSineTable(void){

	memset(sine_table, 0, sizeof sine_table);

	//Pure sine wave:
	/*int offset   = AMPLITUDE/2;
	int temp_amp = offset;
	for (int i = 0; i < N_SAMPLES; i++){
	    sine_table[i] = (uint16_t)(offset + temp_amp * sinf(2.0f * M_PI * i / N_SAMPLES));
	}*/

    //Full-wave rectified
    for (int i = 0; i < N_SAMPLES; i++){

        float s = sinf(2.0f * M_PI * i / N_SAMPLES);
        if (s < 0.0f){
            s = -s;   // valor absoluto
        }
        sine_table[i] = (uint16_t)(4095.0f * s);
    }
}

static void genTriangleWave(void){

	for(int i = 0; i < N_SAMPLES; i++){

		//TRIANGLE WAVE
		uint32_t value = (2UL * AMPLITUDE * i) / N_SAMPLES;
		if(value > AMPLITUDE){
			value = 2 * AMPLITUDE - value;
		}
		sine_table[i] = (uint16_t)value;

		/*
		 //SAWTOOTH WAVE
		 uint32_t value = (2UL * AMPLITUDE * i) / N_SAMPLES-value;

		if(value > AMPLITUDE){
			value =0; //2 * AMPLITUDE - value;
		}
		triangle_table[i] = (uint16_t)value;*/

		/*
	    if (i < N_SAMPLES/2){
	    	triangle_table[i] = (2.0f * i / N_SAMPLES) * AMPLITUDE;
	    }
	    else{
	    	triangle_table[i] = (2.0f - (2.0f * i / N_SAMPLES)) * AMPLITUDE;
	    }*/
	}
}

