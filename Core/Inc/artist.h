/*
 * artist.h
 *
 * Functions for drawing shapes/text on 2D matrix
 *
 *  Created on: 27.11.2022
 *      Author: Bojan Sofronievski
 */

#ifndef INC_ARTIST_H_
#define INC_ARTIST_H_

#include <stdint.h>
#include <stddef.h>

#include "font.h"
#include "point.h"
#include "color.h"

typedef struct {
	color_t *data;
	size_t numRows;
	size_t numCols;
} Canvas;

/**
 * @brief Clear the canvas with the specified collor
 * @param canvas Pointer to canvas instance
 * @param color Color to clear the canvas with
 */
void Artist_ClearCanvas(Canvas *canvas, const color_t color);

/**
 * @brief Draw the specified text in the data matrix
 * @param canvas Pointer to canvas instance
 * @param txt Null-terminated C-string
 * @param pos Upper-left corner of the text
 * @param color The color for the text
 */
void Artist_DrawText(Canvas *canvas, const char *txt, const Point pos, const color_t color);

/**
 * @brief Fill polygon with scanline fill algorithm
 * @param canvas Pointer to canvas instance
 * @param points Points of the polygon
 * @param numPoints Number of points
 * @param color Color used for filling
 */
void Artist_FillPolygon(Canvas *canvas, Point *points, const size_t numPoints, const color_t color);

#endif /* INC_ARTIST_H_ */
