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
void outputSignal(void);
void dispCurrentFreq(void);

#endif /* INC_SIGNALGEN_H_ */
