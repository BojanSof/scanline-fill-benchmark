/*
 * color.h
 *
 * Common functions for storing RGB666 colors
 *
 *  Created on: 27.11.2022
 *      Author: Bojan Sofronievski
 */

#ifndef INC_COLOR_H_
#define INC_COLOR_H_

#include <stdint.h>

typedef uint32_t color_t;

// colors
#define RED 		(color_t)0x0003F000
#define GREEN		(color_t)0x00000FC0
#define BLUE		(color_t)0x0000003F
#define BLACK		(color_t)0x00000000
#define WHITE		(color_t)0x0003FFFF
#define YELLOW		(color_t)0x0003FFC0
#define MAGENTA		(color_t)0x0003F03F
#define CYAN		(color_t)0x00000FFF

/**
 * Convert RGB values to color for display
 */
static inline color_t rgbToColor(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r & 0xFC) << 10) | ((g & 0xFC) << 4) | ((b & 0xFC) >> 2);
}

#endif /* INC_COLOR_H_ */
