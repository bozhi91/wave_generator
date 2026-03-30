/**
 * original author:  Tilen Majerle<tilen@majerle.eu>
 * modification for STM32f10x: Alexander Lutsai<s.lyra@ya.ru>
   ----------------------------------------------------------------------
   	Copyright (C) Alexander Lutsai, 2016
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
 */
#ifndef FONTS_H
#define FONTS_H 120

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 *
 * Default fonts library. It is used in all LCD based libraries.
 *
 * \par Supported fonts
 * 
 * Currently, these fonts are supported:
 *  - 7 x 10 pixels
 *  - 11 x 18 pixels
 *  - 16 x 26 pixels
 */
#include "stm32f3xx_hal.h"
#include "string.h"

/**
 * @brief  Font structure used on my LCD libraries
 */
typedef struct {
	uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const unsigned char *data; /*!< Pointer to data font data array */
	size_t size_data;
} FontDef_t;

/**
 * @brief  String length and height
 */
typedef struct {
	uint16_t Length;      /*!< String length in units of pixels */
	uint16_t Height;      /*!< String height in units of pixels */
} FONTS_SIZE_t;

extern FontDef_t FONT_ARRAY[];

//15x12
static unsigned char antena[] = {

  0x7F, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00,
  0x0C, 0x08, 0x00, 0x0C, 0x0C, 0x0E, 0xFC, 0x0F,
  0x0C, 0x0E, 0x00, 0x0C, 0x0C, 0x08, 0x00, 0x00,
  0x1E, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00
};


/**
 * @brief  Calculates string length and height in units of pixels depending on string and font used
 * @param  *str: String to be checked for length and height
 * @param  *SizeStruct: Pointer to empty @ref FONTS_SIZE_t structure where informations will be saved
 * @param  *Font: Pointer to @ref FontDef_t font used for calculations
 * @retval Pointer to string used for length and height
 */
char* FONTS_GetStringSize(char* str, FONTS_SIZE_t* SizeStruct, FontDef_t* Font);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
