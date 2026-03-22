
/* USER CODE BEGIN 4 */

#include "fonts.h"
#include "ssd1306.h"
#include "i2c.h"
#include "display.h"
#include <stdio.h>

/*
static void i2cscan(void){

	HAL_StatusTypeDef ret;
	unsigned char addr = 0;

	//printf("Running I2C scanner... \n");

	 for(int i=1; i<128; i++) {
	         ret = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(i<<1), 1, 20000);
		 	 ret = HAL_I2C_Master_Transmit(&hi2c1, i << 1, 0, 1, 20000);

	        if(ret == HAL_OK){
	           printf("Found I2C device: 0x%02X \n", addr);//60 ->0x3c =>
	           addr = i;
	           break;
	        }
	 }
	 printf("I2C SCAN COMPLETED... \n");
	 HAL_I2C_ModeTypeDef mode = HAL_I2C_GetMode(&hi2c1);
}*/

void initOLED(void){

	/* Initialize the OLED Display and
	 * link it to the I2C_1 port
	 */
	SSD1306_Init(&hi2c1);

	SSD1306_Fill(SSD1306_COLOR_BLACK);
	SSD1306_GotoXY (0,0);

	SSD1306_Puts ("Display RDY!", &Font_11x18, 1);
	SSD1306_UpdateScreen();
}

void display_string(const char* str){

	char buff[20];
	memset(buff, ' ', sizeof buff);
	SSD1306_GotoXY (0,0);
	SSD1306_Puts((char*)buff, &Font_11x18, 1);
	SSD1306_UpdateScreen();

	SSD1306_GotoXY (0,0);
	SSD1306_Puts((char*)str, &Font_11x18, 1);
	SSD1306_UpdateScreen();
}

void dispay_write(int c){

	static char buff[50];
	static int c_count = 0;
	static int pos_y   = 0;

	if(c_count == 0){
		memset(buff, 0, sizeof buff);
	}
	if(c_count>sizeof buff-1){
		c_count = 0;
	}
	if(c!='\n' && c!='\r'){
		buff[c_count++] = c;
	}

	//New line is found, we send the buffer to the display
	if(c=='\n' && c_count>0){

		SSD1306_Puts(buff, &Font_11x18, 1);
		SSD1306_UpdateScreen();

		c_count=0;

		//Increase the 'y' position. The next buffer will be printed on the next line
		pos_y+=15;
		if(pos_y>45){
			SSD1306_Fill(SSD1306_COLOR_BLACK);
			pos_y=0;
		}
		SSD1306_GotoXY(0, pos_y);
	}
}
