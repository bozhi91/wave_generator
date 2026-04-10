/*
 * io.h
 *
 *  Created on: Mar 4, 2026
 *      Author: bozhi
 */

#ifndef INC_IO_H_
#define INC_IO_H_

	char getBtnState(void);
	void toggleGpio(int state);
	char waitBtn(char btn_id);
	unsigned char getLastKey(void);
	void resetKey(void);
	void kbdDriver(void);

	unsigned char isKey_1(void);
	unsigned char isKey_2(void);
	unsigned char isKey_3(void);
	unsigned char isKey_4(void);

	unsigned char isKeyPressed(int id);
	short getEncoderVal(void);
	void setEncoderVal(short value);

	char isEncoderEnabled(void);
	void enableEncoder(char status);

	short getEncoderLastVal(void);
	void storeEncoderLastVal(void);


#endif /* INC_IO_H_ */
