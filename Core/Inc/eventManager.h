/*
 * eventManager.h
 *
 *  Created on: Mar 9, 2026
 *      Author: bozhi
 */

#ifndef INC_EVENTMANAGER_H_
#define INC_EVENTMANAGER_H_

	typedef enum{

			PLAY_STATE,
			MAIN_MENU,
			CONFIG_MENU,
			SETTINGS_MENU,
			CONFIG_SUBMENU,
			ST_SIMULATION,
			ST_ENCODER

	}DeviceStates;

  extern void eventManager(void);

  void setState(DeviceStates state);
  DeviceStates getState(void);

  typedef struct{

	unsigned char state;		    //The current state
    void (*ev_ptr)(void);    		//Function pointer to the event
    unsigned long timeout;  		//Call the event every X milliseconds
    unsigned long last_call; 		//Elapsed time since the last call of the event
  }Event;

  typedef struct{

	unsigned char state;		    //The current state
	unsigned char new_state;		//The new state we want to jump to
	unsigned char(*trigger)(void);  //Function pointer to the trigger that will call the event
    void (*ev_ptr)(void);    		//Function pointer to the event
  }State_Manager;

#endif /* INC_EVENTMANAGER_H_ */
