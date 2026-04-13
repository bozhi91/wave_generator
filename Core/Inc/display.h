/*
 * display.h
 *
 *  Created on: Sep 24, 2025
 *      Author: bozhi
 */

#ifndef INC_PERIPHERALS_DISPLAY_H_
#define INC_PERIPHERALS_DISPLAY_H_

	void dispay_write(int c);
	void print(const char* str);
	void printAt(const char* str, int x, int y);

	void initOLED(void);
	void clr_row(int row, char upd);
	void clrscr(void);
	void setFont(int font_id);

#endif /* INC_PERIPHERALS_DISPLAY_H_ */
