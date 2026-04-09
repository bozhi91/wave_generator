/*
 * menu_simulation.c
 *
 *  Created on: Mar 31, 2026
 *      Author: bozhi
 */

#include <stdio.h>
#include <string.h>

#include "menu.h"
#include "menu_simulation.h"
#include "main.h"
#include "io.h"
#include "display.h"
#include "eventManager.h"
#include "signalGen.h"

/**
* Display the simulator parameters and status:
* - Freq: 1KHz
* - Wave: Sine
* - Type: Full
* - Burst: None
**/
void mainMenu(void){

	char str[15];

	clrscr();
	printAt("=MAIN-MENU=", 0, 0);
	snprintf(str, sizeof str, "Off:%dHz", getCurrentFrequency());
	printAt(str, 0, 1);
}

void toggleSimulation(void){

}






