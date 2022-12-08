/*
 * color.h
 *
 * Common functions for storing RGB565 colors
 *
 *  Created on: 27.11.2022
 *      Author: Bojan Sofronievski
 */

#ifndef INC_COLOR_H_
#define INC_COLOR_H_

#include <stdint.h>

typedef uint16_t color_t;

// colors
#define RED 		(color_t)0xF800
#define GREEN		(color_t)0x07E0
#define BLUE		(color_t)0x001F
#define BLACK		(color_t)0x0000
#define WHITE		(color_t)0xFFFF
#define YELLOW		(color_t)0xFFE0
#define MAGENTA		(color_t)0xF81F
#define CYAN		(color_t)0x07FF

/**
 * Convert RGB values to color for display
 */
static inline color_t rgbToColor(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

#endif /* INC_COLOR_H_ */
