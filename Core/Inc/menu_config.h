/*
 * menu_config.h
 *
 *  Created on: Apr 10, 2026
 *      Author: bozhi
 */

#ifndef INC_MENU_CONFIG_H_
#define INC_MENU_CONFIG_H_

void menu_cfg(void);
void updateBurst(void);

typedef struct{

	unsigned char func_type;
	unsigned char wave_type;
	unsigned char burst_type;
	unsigned char burst_value;
	unsigned char duty_cycle;

} CONFIG_STRUCT;

CONFIG_STRUCT getConfigStruct(void);


/**
 *
 * static unsigned char func_type   = 0;
static unsigned char wave_type   = 0;
static unsigned char burst_type  = 0;
static unsigned char burst_value = 5;  //5 seconds of burst duration
static unsigned char duty_cycle  = 50;**/

#endif /* INC_MENU_CONFIG_H_ */
