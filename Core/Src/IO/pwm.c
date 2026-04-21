/*
 * pwm.c
 *
 *  Created on: Mar 22, 2026
 *      Author: bozhi
 */

#include "pwm.h"
#include "tim.h"
#include <math.h>
#include <stdio.h>
#include <string.h>


/* Start the PWM signal generator at pin D9(internal: PA8 )
 * Timer_1, CH_1
 * */
void StartPWM(int f_out, int duty_cycle){

	/*
	 example: F_pwm = 8mhz/((7+1)(0+1)) = 1mhz
	 pwm F = clock_f_tim1/((ARR+1)(PSC+1)) = 8mhz/(71+1) = 111khz
	 ARR   = (f_tim1/f_out)-1
	 PSC   = 0; --> no division. We're using the system clock frequency

	 NOTE: Max ARR value(16bits - 0:65535)

	 PWM duty cycle has 8 levels:
	 	0/7 = 0%
		4/7 ≈ 50%
		7/7 ≈ ~100%
	 */

	//NOTE: THIS CONFIGURATION WORKS ONLY FOR FREQUENCIES >=1KHz.
	//For lower frequencies, use a different prescaler.

	//unsigned int in_freq = getTim1Freq();

	unsigned short psc = ceil(getTim1Freq() /( f_out * 65536.0f))-1;
	unsigned short arr = (getTim1Freq()/( f_out * (psc+1)))-1;

	//Default PWM parameters:
    TIM1->PSC  = psc; //ceil(getTim1Freq()/(freq*65536))-1; // Divides the clock of TIM1. If presacaler is 0, the TIM1 runs at the system clock frequency(8mhz).
    												 // Check HAL_RCC_GetSysClockFreq() to get the system clock frequency.

    TIM1->ARR  = arr;     		  // F = 1MHz -> TIM1_freq/(out_freq)-1
    TIM1->CCR1 = arr/(100/duty_cycle);  // Defines the duty cycle 50% duty. This value is a division in '%' of the ARR register.
    							  // An ARR value of 7(or 0 to 7 range are 8 combinations), the 50% duty cycle is So CCR1 is 4.
      	  	  	  	      	  	  // An ARR value of 9(10 possible divisions), the 50% of duty is 5. So CCR1 is 5.

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

void stopPWM(void){

	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

void changePWMFreq(int f_out_hz){

	//int tim1 = getTim1Freq();
	int arr = (getTim1Freq()/f_out_hz)-1;

	 HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);

	__HAL_TIM_SET_AUTORELOAD(&htim1, arr);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, arr/2); //50% Duty cycle

	 HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	//float f = clk / ((psc + 1.0f) * (arr + 1.0f));
}
