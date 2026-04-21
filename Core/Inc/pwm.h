/*
 * pwm.h
 *
 *  Created on: Mar 22, 2026
 *      Author: bozhi
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_


	void StartPWM(int f_out, int duty_cycle);
	void stopPWM(void);
	void changePWMFreq(int f_out_hz);


#endif /* INC_PWM_H_ */
