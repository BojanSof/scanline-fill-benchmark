/*
 * lcd_ili9488.h
 *
 * Interface for TFT-LCD based on ILI9488 controller.
 * Color format is RGB666 and the data is sent in 3 8-bit transactions:
 * RRRRRR00 GGGGGG00 BBBBBB00, which are stored in one 32-bit unsigned integer,
 * 00000000 000000RR RRRRGGGG GGBBBBBB
 *
 *  Created on: Nov 23, 2022
 *      Author: Bojan Sofronievski
 */
#ifndef INC_LCD_ILI9488_H_
#define INC_LCD_ILI9488_H_

#include <stdint.h>
#include <stddef.h>

#include "point.h"
#include "color.h"

// Constants

// display related
#define LCD_WIDTH 320
#define LCD_HEIGHT 480

/**
 * Struct holding Lcd info
 */
typedef struct {
	Point cursor;
	uint16_t width;
	uint16_t height;
} Lcd;

/**
 * Initialize the LCD controller
 * @param lcd Pointer to Lcd struct
 */
void LCD_Init(Lcd *lcd);

/**
 * @brief Set the current position for drawing on the LCD
 * @param lcd Pointer to Lcd struct
 * @param p The X and Y coordinates
 */
void LCD_SetCursor(Lcd *lcd, const Point p);

/**
 * @brief Set rectangular area of the display for drawing
 * @param lcd Pointer to Lcd struct
 * @param startPoint Upper left corner point of the area
 * @param endPoint Bottom right corner point of the area
 */
void LCD_SetArea(Lcd *lcd, const Point startPoint, const Point endPoint);

/**
 * @brief Rotate display (graphics ram -> physical pixels mapping)
 * @param lcd Pointer to Lcd struct
 * @param rotation 0 = 0 deg, 1 = 90 deg, 2 = 180 deg, 3 = 270 deg
 */
void LCD_SetOrientation(Lcd *lcd, const uint8_t rotation);

/**
 * @brief Clear the whole LCD with the given color
 * @param lcd Pointer to Lcd struct
 * @param color Color to clear the LCD with
 */
void LCD_Clear(Lcd *lcd, const color_t color);

/**
 * @brief Draw single point on the LCD
 * @param lcd Pointer to Lcd struct
 * @param p Coordinates of the point
 */
void LCD_DrawPoint(Lcd *lcd, const Point p, const color_t color);

/**
 * @brief Draw in area of the display
 * @param lcd Pointer to Lcd struct
 * @param data Array of pixel values
 * @param numRows Number of rows in the array
 * @param numCols Number of columns in the array
 */
void LCD_DrawArea(Lcd *lcd, const color_t *data, const size_t numRows, const size_t numCols);

#endif
