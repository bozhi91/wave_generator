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
#include "io.h"
#include "display.h"
#include "signalGen.h"

static unsigned char c_state = MAIN_MENU;

//These events will run automatically by the event manager
Event eventList[] = {
	{ ST_SIMULATION, simulation,  1000, 0 },
	{ ST_ENCODER,    readEncoder, 50,  0 },
};

//This states are set manually by the user by pressing a button or when another event is triggered
State_Manager stateList[] = {

	 //Current St |  New State    |  Trigger | New Event

	{ MAIN_MENU,    PLAY_STATE, 	isKey_1,  outputSignal }, //START SIGNAL GENERATOR
	{ PLAY_STATE,   MAIN_MENU,  	isKey_1,  outputSignal }, //STOP  SIGNAL GENERATOR

	{ MAIN_MENU,    CONFIG_MENU, 	isKey_2,  menu_cfg    }, //OPEN   CONFIG   MENU
	{ CONFIG_MENU,  CONFIG_MENU, 	isKey_2,  menuBrowser }, //BROWSE CONFIG   MENU
	{ CONFIG_MENU,  CONFIG_MENU, 	isKey_3,  menuSelect  }, //SELECT CONFIG   MENU

	{ MAIN_MENU,     SETTINGS_MENU, isKey_3,  menu_set    }, //OPEN   SETTINGS MENU
	{ SETTINGS_MENU, SETTINGS_MENU, isKey_3,  menuBrowser }, //BROWSE SETTINGS MENU
	{ SETTINGS_MENU, SETTINGS_MENU, isKey_2,  menuSelect  }, //SELECT SETTINGS MENU

	{ MAIN_MENU,     ST_ENCODER,    isKey_4,  readEncoder }, //OPEN   ENCODER  MENU

	{ ST_ENCODER,     MAIN_MENU,   	isKey_1,  mainMenu    }, //GO BACK TO MAIN MENU
	{ CONFIG_MENU,    MAIN_MENU,   	isKey_1,  mainMenu    }, //GO BACK TO MAIN MENU
	{ SETTINGS_MENU,  MAIN_MENU,   	isKey_1,  mainMenu    }, //GO BACK TO MAIN MENU
	{ ST_SIMULATION,  MAIN_MENU,   	isKey_1,  mainMenu    }, //GO BACK TO MAIN MENU

};

void menuInit(void){

	c_state = MAIN_MENU;
	resetKey();
}

void setState(DeviceStates state){
	c_state = state;
}

DeviceStates getState(void){
	return c_state;
}

void eventManager(void){

  int state_size = sizeof(stateList)/sizeof(stateList[0]);
  int event_size = sizeof(eventList)/sizeof(eventList[0]);

  //Call the events automatically after a certain timeout
  for(int i=0; i<event_size; i++){

	 //Execute the event corresponding to the current state
 	 if(eventList[i].state == c_state){
 		 //Timeout since the last event call. We call the event.
 	     if((HAL_GetTick() - eventList[i].last_call > eventList[i].timeout) || eventList[i].last_call==0 ){
 	    	eventList[i].last_call = HAL_GetTick();
 	        eventList[i].ev_ptr();
 	        //break;
 	      }
 	      else if(eventList[i].timeout == 0){ //If no timeout is defined, the function will be called without any delay
 	      	eventList[i].ev_ptr();
 	        //break;
 	      }
 	 }
 	 else{
 		eventList[i].last_call = 0; //reset the timeout for the rest of the events so we can
 	 }
  }

  //Call the functions manually when an event is triggered
  for(int i=0; i<state_size; i++){

	 //The current state is located in the state table and the trigger is activated
	 if(stateList[i].state == c_state && stateList[i].trigger() == 1 ){

		 //Set the new state and call the function
		 c_state = stateList[i].new_state;
		 if(stateList[i].ev_ptr != 0){
			 stateList[i].ev_ptr();
			 break;
		 }
	 }
  }
}
