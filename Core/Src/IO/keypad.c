/*
 * keypad.c
 *
 *  Created on: Mar 4, 2026
 *      Author: bozhi
 */

#include "io.h"
#include "main.h"
#include "gpio.h"
#include "dac.h"
#include "display.h"
#include "tim.h"

#define GPIO_A GPIOA->IDR
#define GPIO_B GPIOB->IDR

#define KEY_1   (GPIO_A & GPIO_PIN_11)
#define KEY_2   (GPIO_B & GPIO_PIN_5)
#define KEY_3   (GPIO_B & GPIO_PIN_4)
#define ENC_SW  (GPIO_B & GPIO_PIN_1)	//Encoder - switch key

#define KEY_UP_DELAY 	100
#define KEY_DOWN_DELAY  200

static unsigned char last_key = 0;
static volatile unsigned char enc_l = 0;
static volatile unsigned char enc_r = 0;

uint8_t read_buttons(void);

static short encode_count       = 0;
static unsigned char encode_dir = 0;
static short encoder_last_val   = 0;
static char encoder_state 		= 0;

/* Toggle the encoder.
 * If enabled, the encoder values are updated while rotating.
 * Otherwise, the values won't be updated.
 *
 * The encoder state is controlled from the Timer register(TIM2)
 * */
void enableEncoder(char status){

	encoder_state = status;
	if(status == 1){
		HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	}
	else{
		HAL_TIM_Encoder_Stop(&htim2, TIM_CHANNEL_ALL);
	}
}

/* Returns the current encoder state
 *
 * 1: Enabled
 * 0: Disabled
 * */
char isEncoderEnabled(void){
	return encoder_state;
}

/* Get the encoder rotating direction.
 *
 * @Returns:
 * 	 --> 'r' for right
 * 	 --> 'l' for left
 * */
unsigned char getEncoerDir(void){
	return encode_dir;
}

/* Set the RAW encoder value. */
void setEncoderVal(short value){
	encode_count = value;
	__HAL_TIM_SET_COUNTER(getTimInst(2), -4*value);
}

/** Get the RAW encoder value **/
short getEncoderVal(void){
	return encode_count;
}

short getEncoderLastVal(void){
	return encoder_last_val;
}

//Store the current encoder value and reset the encoder
void storeEncoderLastVal(void){
	encoder_last_val = encode_count;
}

/* Read the encoder data.
 * This will update the current encoder values such as: encoder_counter and direction.
 * For now only a positive values are allowed.
 * */
void readEncoderData(void){

	//Get the encoder direction - left or right
	encode_dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(getTimInst(2)) ? 'r' : 'l';

	//Get the encoder step counter. Right: increased. Left: decreased.
	int16_t time_count = __HAL_TIM_GET_COUNTER(&htim2);
	encode_count = time_count/4;
	encode_count*=-1;

	//Allow positive values only!
	if(encode_count < 0){
		encode_count = 0;
		__HAL_TIM_SET_COUNTER(getTimInst(2), 0);
	}
}

void kbdDriver(void){

	static unsigned char pressed = 0;
	static unsigned char key_id  = 0;
	static uint32_t t_press      = 0;
	static uint32_t t_release    = 0;

	//Detect which key is pressed
	if (KEY_1)      key_id  = 1;
	else if (KEY_2) key_id  = 2;
	else if (KEY_3) key_id  = 3;
	else if (ENC_SW) key_id = 4;

	else if(pressed){//ALL keys are released - previous state was 'pressed'

		t_release = HAL_GetTick();
		last_key  = ((t_release - t_press) > KEY_UP_DELAY) ? key_id : 0;
		pressed   = 0;
		key_id    = 0;
	}
	else{
		last_key = 0;
		key_id   = 0;
	}

	//Key is pressed for the first time
	if(key_id != 0 && !pressed){

		t_press = HAL_GetTick();
		if((t_press - t_release) > KEY_DOWN_DELAY ){
	        pressed = 1;
	    }
	    else{
	    	pressed  = 0;
	    	last_key = 0;
	    }
	}

	readEncoderData();
}

unsigned char getLastKey(void){
	return last_key;
}

void resetKey(void){
	last_key = 0;
}

unsigned char isKey_1(void){
	return isKeyPressed(1);
}

unsigned char isKey_2(void){
	return isKeyPressed(2);
}

unsigned char isKey_3(void){
	return isKeyPressed(3);
}

unsigned char isKey_4(void){
	return isKeyPressed(4);
}

//Returns 1 if the requested key is pressed.
//Returns 0 otherwise.
unsigned char isKeyPressed(int id){
	return  getLastKey() == id;
}

void toggleGpio(int state){
	GPIOB->BSRR = state == 1 ? GPIO_BSRR_BS_3 : GPIO_BSRR_BR_3;
}

