
/* USER CODE BEGIN 4 */

#include "fonts.h"
#include "ssd1306.h"
#include "i2c.h"
#include "display.h"
#include <stdio.h>
#include <ctype.h>

#define MAX_ROW_SIZE  50
#define LINE_SPACING  4

void displaySplash(void);

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

static uint16_t cursor_x=0, cursor_y=0;
static FontDef_t cFont;
static int chars_per_row = 10;

/** Initialize the OELD driver with params:
 *  - I2C: 1
 *  - Pos(x,y): 0,0
 *  - BG Color: Black
 *  - Font: 11x18
 *
 *https://blog.embeddedexpert.io/?p=674
 * **/
extern unsigned char logo[];
void initOLED(void){

	/* Initialize the OLED Display and
	 * link it to the I2C_1 port
	 */
	SSD1306_Init(&hi2c1);
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	SSD1306_GotoXY (0, 0);

	setFont(1);
	chars_per_row = SSD1306_WIDTH/cFont.FontWidth;

//	displaySplash();
}

void displaySplash(void){

	extern unsigned char splash[];
	extern unsigned char icon_1[];
	extern unsigned char logo[];

	int bmp_pos = SSD1306_WIDTH/2 - 69/2;

	//drawBMP(bmp_pos, 0, 69, 40, splash, 1, 0);
	drawBMP(30, 10, 15, 15, logo, 1, 0);
	SSD1306_UpdateScreen();
}

void setFont(int font_id){
	cFont = FONT_ARRAY[font_id];
}

void printAt(const char* str, int x, int y){

	cursor_x = x * (cFont.FontWidth );
	cursor_y = y * (cFont.FontHeight + LINE_SPACING );

	SSD1306_GotoXY(cursor_x, cursor_y);
	print(str);
}

void print(const char* str){

	char buff[50] = {0};

	for(int i=0; i<strlen(str); i++){
		buff[i] = toupper(str[i]);
	}

	clr_row(cursor_y, 0);
	SSD1306_GotoXY(cursor_x, cursor_y);
	SSD1306_Puts(buff, &cFont, 1);
	SSD1306_UpdateScreen();
}

void clr_row(int row, char upd){

	char row_buff[MAX_ROW_SIZE] = { 0 };
	int row_size = chars_per_row < (sizeof (row_buff)) ? chars_per_row : (sizeof row_buff);

	row*= (cFont.FontHeight + LINE_SPACING);
	memset(row_buff, ' ', row_size);
	SSD1306_GotoXY(0, row);
	SSD1306_Puts((char*)row_buff, &cFont, 1);

	if(upd){
		SSD1306_UpdateScreen();
	}
}

void clrscr(void){

	for(int i=0;i<4;i++){
		clr_row(i, 0);
	}
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

		SSD1306_Puts(buff, &cFont, 1);
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
