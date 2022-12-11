/*
 * scanline_fill.h
 *
 *  Created on: 03.12.2022
 *      Author: Bojan Sofronievski
 */

#ifndef INC_SCANLINE_FILL_H_
#define INC_SCANLINE_FILL_H_

#include <stdint.h>
#include <stddef.h>
#include "color.h"
#include "point.h"

/**
 * Struct representing polygon edge info
 * needed for scanline fill algorithm
 */
typedef struct Edge
{
	int32_t xl;			// x-coordinate of lower point
	int32_t yu;			// y-coordinate of upper point
	int32_t dx;			// used as part of inverse of slope, to adjust x-coordinates
	int32_t dy;			// used as part of inverse of slope, to adjust x-coordinates
	int32_t dxCounter;  // used to adjust x-coordinates
	int32_t xInc;  		// used to adjust x-coordinates, -1 when slope < 0, +1 when slope > 0
	struct Edge *next;	// pointer to connected edge
} Edge;

/**
 * @brief Build edge table for the given polygon points
 * @param table Array of linked lists representing edge table
 * @param maxTableSize Maximum number of lists that can be put in the table
 * @param edges Array of preallocated edges
 * @param maxNumEdges Max number of edges
 * @param points Polygon points
 * @param numPoints Number of polygon points
 */
void ScanlineFill_BuildEdgeTable(Edge *table, const size_t maxTableSize, Edge *edges, const size_t maxNumEdges, const Point *points, const size_t numPoints);

/**
 * @brief Create list of active edges
 * @param table Edge table
 * @param maxTableSize Size of the edge table
 * @param active List of active edges
 * @param yScanline y-coordinate representing the current scanline
 */
void ScanlineFill_BuildActiveEdgeList(Edge *table, const size_t maxTableSize, Edge *active, int32_t yScanline);

/**
 * @brief Update list of active edges by removing completed edges
 * and resort the list
 * @param active List of active edges
 * @param yScanline y-coordinate representing the current scanline
 */
void ScanlineFill_UpdateActiveEdgeList(Edge *active, int32_t yScanline);

/**
 * @brief Fill interior points of the polygon lying on the scanline
 * @param active List of active edges
 * @param yScanline y-coordinate representing the current scanline
 * @param data 2D matrix on which the drawing is done
 * @param dataWidth Number of columns in data
 * @param dataHeight Number of rows in data
 * @param color Color to fill the scanline with
 */
void ScanlineFill_FillScanline(Edge *active, int32_t yScanline, color_t *data, const size_t dataWidth, const size_t dataHeight, const color_t color);

#endif /* INC_SCANLINE_FILL_H_ */
