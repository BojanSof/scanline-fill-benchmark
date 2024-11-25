/*
 * artist.c
 *
 *  Created on: 27.11.2022
 *      Author: Bojan Sofronievski
 */

#include "artist.h"
#include "font.h"
#include "scanline_fill.h"
#include "constants.h"

#include "cycle_counter.h"

void Artist_ClearCanvas(Canvas *canvas, const color_t color)
{
	for(size_t iRow = 0; iRow < canvas->numRows; ++iRow)
	{
		for(size_t iCol = 0; iCol < canvas->numCols; ++iCol)
		{
			canvas->data[iRow * canvas->numCols + iCol] = color;
		}
	}
}

void Artist_DrawText(Canvas *canvas, const char *txt, const Point pos, const color_t color)
{
	Point currentPos = pos;
	for (const char *c = txt; *c != '\0'; ++c)
	{
		// get character bytes from font array and write in data array
		for(uint8_t x = 0; x < FONT_WIDTH; ++x)
		{
			currentPos.y = pos.y;
			uint8_t fontCol = font6x8[(*c - 32)*FONT_WIDTH + x];
			for(uint8_t y = 0; y < FONT_HEIGHT; ++y)
			{
				if(fontCol & (1 << y))
				{
					canvas->data[currentPos.x + currentPos.y * canvas->numCols] = color;
				}
				currentPos.y += 1;
			}
			currentPos.x += 1;
		}
	}
}

void Artist_FillPolygon(Canvas *canvas, Point *points, const size_t numPoints, const color_t color, uint32_t *benchmarkTimes)
{
	//1. Sort points anti-clockwise
	uint32_t start_time = CycleCounter_GetValue();

	Point_SortAntiClockwise(points, numPoints);

	uint32_t end_time = CycleCounter_GetValue();
	benchmarkTimes[BENCHMARK_SORT_POINTS] = end_time - start_time;

	//2. Find ymin and ymax for the polygon
	int32_t ymin = points[0].y, ymax = points[1].y;
	for(const Point *p = points + 1; p != (points + numPoints); ++p)
	{
		if(p->y < ymin) ymin = p->y;
		if(p->y > ymax) ymax = p->y;
	}
	// Create required lists
	Edge table[SCANLINE_FILL_TABLE_SIZE] = {};
	Edge edges[SCANLINE_FILL_EDGES_MAX_NUM] = {};
	Edge active = {}; // head of list of active edges
	//3. Build edge table
	start_time = CycleCounter_GetValue();

	ScanlineFill_BuildEdgeTable(table, SCANLINE_FILL_TABLE_SIZE, edges, SCANLINE_FILL_EDGES_MAX_NUM, points, numPoints);

	end_time = CycleCounter_GetValue();
	benchmarkTimes[BENCHMARK_BUILD_TABLE] = end_time - start_time;

	//4. Iterate through each scanline, applying scanline fill
	start_time = CycleCounter_GetValue();

	for(int32_t yScanline = ymin; yScanline <= ymax; ++yScanline)
	{
		//4.1. Build list of active edges
		ScanlineFill_BuildActiveEdgeList(table, SCANLINE_FILL_TABLE_SIZE, &active, yScanline);
		if(active.next)
		{
			//4.2. Fill the pixels inside the polygon on the given scanline
			ScanlineFill_FillScanline(&active, yScanline, canvas->data, canvas->numCols, canvas->numRows, color);
			//4.3. Update active edges list
			ScanlineFill_UpdateActiveEdgeList(&active, yScanline);
		}
	}

	end_time = CycleCounter_GetValue();
	benchmarkTimes[BENCHMARK_PROCESS_SCANLINES] = end_time - start_time;

	//5. Color polygon points
	for(const Point *p = points; p != (points + numPoints); ++p)
	{
		canvas->data[p->y * canvas->numCols + p->x] = YELLOW;
	}
}
