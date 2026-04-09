/*
 * eventManaget.c
 *
 *  Created on: Mar 9, 2026
 *      Author: bozhi
 */

#include <stdio.h>
#include "eventManager.h"
#include "main.h"
#include "menu.h"
#include "menu_simulation.h"
#include "io.h"
#include "display.h"
#include "signalGen.h"

static unsigned char c_state = MAIN_MENU;

//These events will run automatically by the event manager
Event eventList[] = {
	{ MAIN_MENU,	updateFrequency, 100,  0 },
};

//This states are set manually by the user by pressing a button or when another event is triggered
State_Manager STATE_LIST_TABLE[] = {

	//Current St |  New State    |  Trigger | New Event
	{ MAIN_MENU,    PLAY_STATE, 	isKey_1,  outputSignal }, //START SIGNAL GENERATOR
	{ PLAY_STATE,   MAIN_MENU,  	isKey_1,  outputSignal }, //STOP  SIGNAL GENERATOR

	{ MAIN_MENU,    CONFIG_MENU, 	isKey_2,  menu_cfg    }, //OPEN   CONFIG   MENU
	{ CONFIG_MENU,  CONFIG_MENU, 	isKey_2,  menuBrowser }, //BROWSE CONFIG   MENU
	{ CONFIG_MENU,  CONFIG_MENU, 	isKey_3,  menuSelect  }, //SELECT CONFIG   MENU

	{ MAIN_MENU,     SETTINGS_MENU, isKey_3,  menu_set    }, //OPEN   SETTINGS MENU
	{ SETTINGS_MENU, SETTINGS_MENU, isKey_3,  menuBrowser }, //BROWSE SETTINGS MENU
	{ SETTINGS_MENU, SETTINGS_MENU, isKey_2,  menuSelect  }, //SELECT SETTINGS MENU

	{ MAIN_MENU,     ST_ENCODER,    isKey_4,  updateFrequency }, //OPEN   ENCODER  MENU

	{ ST_ENCODER,     MAIN_MENU,   	isKey_1,  mainMenu    }, //GO BACK TO MAIN MENU
	{ CONFIG_MENU,    MAIN_MENU,   	isKey_1,  mainMenu    }, //GO BACK TO MAIN MENU
	{ SETTINGS_MENU,  MAIN_MENU,   	isKey_1,  mainMenu    }, //GO BACK TO MAIN MENU
	{ ST_SIMULATION,  MAIN_MENU,   	isKey_1,  mainMenu    }, //GO BACK TO MAIN MENU

};

void menuInit(void){

	c_state = MAIN_MENU;
	setEncoderVal(1);
	resetKey();
	mainMenu();
}

//Set a new state for the state machine
void setState(DeviceStates state){
	c_state = state;
}

//Get the current state of the state machine
DeviceStates getState(void){
	return c_state;
}

/** Go through the states table and execute the function from the current state.
 *  Also, we check
 * */
void eventManager(void){

  int state_size = sizeof(STATE_LIST_TABLE)/sizeof(STATE_LIST_TABLE[0]);
  int event_size = sizeof(eventList)/sizeof(eventList[0]);

  //Call the events automatically after a certain timeout
  for(int i=0; i<event_size; i++){

	 //Execute the event corresponding to the current state
 	 if(eventList[i].state == c_state){
 		 //Timeout since the last event call. We call the event.
 	     if((HAL_GetTick() - eventList[i].last_call > eventList[i].timeout) || eventList[i].last_call==0 ){
 	    	eventList[i].last_call = HAL_GetTick();
 	        eventList[i].ev_ptr();
 	      }
 	      else if(eventList[i].timeout == 0){ //If no timeout is defined, the function will be called without any delay
 	      	eventList[i].ev_ptr();
 	      }
 	 }
 	 else{
 		eventList[i].last_call = 0; //reset the timeout for the rest of the events so we can
 	 }
  }

  //Call the functions manually when an event is triggered
  for(int i=0; i<state_size; i++){

	 //The current state is located in the state table and the trigger is activated
	 if(STATE_LIST_TABLE[i].state == c_state && STATE_LIST_TABLE[i].trigger() == 1 ){

		 //Set the new state and call the function
		 c_state = STATE_LIST_TABLE[i].new_state;
		 if(STATE_LIST_TABLE[i].ev_ptr != 0){
			 STATE_LIST_TABLE[i].ev_ptr();
			 break;
		 }
	 }
  }
}
