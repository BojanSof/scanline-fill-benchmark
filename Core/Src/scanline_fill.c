/*
 * scanline_fill.c
 *
 *  Created on: 03.12.2022
 *      Author: Bojan Sofronievski
 */
#include <string.h>

#include "scanline_fill.h"

/**
 * @brief Find y-coordinate of next non-horizontal edge point
 * @param points Polygon points
 * @param numPoints Number of points in the polygon
 * @param currentPoint Index of current point
 */
static int32_t ScanlineFill_yNext(const Point *points, const size_t numPoints, const size_t iCurrentPoint);

/**
 * @brief Insert edge in linked list of edges by performing insertion sort
 * based on the x-coordinate
 * @param list List of edges
 * @param edge Edge to be inserted in the list
 */
static void ScanlineFill_InsertEdge(Edge *list, Edge *edge);

/**
 * @brief Fill edge data and insert it in the edge table
 * @param table Edge table
 * @param maxTableSize Size of the edge table
 * @param edge Edge struct to fill the data in
 * @param lower Lower point of the edge (with smaller y-coordinate)
 * @param upper Upper point of the edge (with greater y-coordinate)
 * @param yComp y-coordinate of neighboring point (prev or next) to determine
 * if y-coordinates are monotonic or not
 */
static void ScanlineFill_FillEdgeData(Edge *table, const size_t maxTableSize, Edge *edge, const Point *lower, const Point *upper, int32_t yComp);

void ScanlineFill_BuildEdgeTable(Edge *table, const size_t maxTableSize, Edge *edges, const size_t maxNumEdges, const Point *points, const size_t numPoints)
{
	// Iterate through every pair of points and create edges info
	const Point *p1 = points + (numPoints - 1);  // First edge point
	const Point *p2 = NULL;  // Second point
	int32_t yPrev = points[numPoints - 2].y;
	size_t iEdge = 0;
	for(size_t iPoint = 0; iPoint < numPoints; ++iPoint)
	{
		p2 = points + iPoint;
		// Find lower and upper points of non-horizontal edge
		if(p1->y < p2->y)
		{
			// p1 is lower, p2 is upper point, edge is going upwards
			ScanlineFill_FillEdgeData(table, maxTableSize, edges + iEdge, p1, p2, ScanlineFill_yNext(points, numPoints, iPoint));
			++iEdge;
		}
		else if(p1->y > p2->y)
		{
			// p2 is lower, p1 is upper point, edge is going downwards
			ScanlineFill_FillEdgeData(table, maxTableSize, edges + iEdge, p2, p1, yPrev);
			++iEdge;
		}
		yPrev = p1->y;
		p1 = p2;
	}
}

void ScanlineFill_BuildActiveEdgeList(Edge *table, const size_t maxTableSize, Edge *active, int32_t yScanline)
{
	Edge *scanlineEdge = table[yScanline].next, *nextScanlineEdge;
	// insert edges related to current scanline in the active edges list
	while(scanlineEdge)
	{
		nextScanlineEdge = scanlineEdge->next;
		ScanlineFill_InsertEdge(active, scanlineEdge);
		scanlineEdge = nextScanlineEdge;
	}
}

void ScanlineFill_UpdateActiveEdgeList(Edge *active, int32_t yScanline)
{
	//1. Update edges x-coordinates
	Edge *currentEdge = active->next, *edgeBefore = active;
	while(currentEdge)
	{
		if(yScanline >= currentEdge->yu)
		{
			// remove completed edges
			currentEdge = currentEdge->next;
			edgeBefore->next = edgeBefore->next->next;
		}
		else
		{
			// update edges in-progress
			currentEdge->xl = currentEdge->xl + currentEdge->dx;
			edgeBefore = currentEdge;
			currentEdge = currentEdge->next;
		}
	}
	//2. Resort list of active edges by rebuilding it
	Edge *nextEdge;
	currentEdge = active->next;
	active->next = NULL;
	while(currentEdge)
	{
		nextEdge = currentEdge->next;
		ScanlineFill_InsertEdge(active, currentEdge);
		currentEdge = nextEdge;
	}
}

void ScanlineFill_FillScanline(Edge *active, int32_t yScanline, color_t *data, const size_t dataWidth, const size_t dataHeight, const color_t color)
{
	Edge *e1, *e2;
	e1 = active->next;
	while(e1)
	{
		e2 = e1->next;
		// fill pixels between the pair of edge points
		for(int32_t x = e1->xl; x < e2->xl; ++x)
		{
			data[dataWidth * yScanline + x] = color;
		}
		e1 = e2->next;
	}
}

int32_t ScanlineFill_yNext(const Point *points, const size_t numPoints, const size_t iCurrentPoint)
{
	size_t iNextPoint = (iCurrentPoint + 1) % numPoints;
	while (points[iNextPoint].y == points[iCurrentPoint].y)
	{
		iNextPoint = (iNextPoint + 1) % numPoints;
	}
	return points[iNextPoint].y;
}

void ScanlineFill_InsertEdge(Edge *list, Edge *edge)
{
	Edge *currentEdge = list->next, *edgeBefore = list;
	while(currentEdge)
	{
		if(edge->xl < currentEdge->xl)
		{
			currentEdge = NULL;
		}
		else
		{
			edgeBefore = currentEdge;
			currentEdge = currentEdge->next;
		}
	}
	// insert the new edge in the linked list
	edge->next = edgeBefore->next;
	edgeBefore->next = edge;
}

void ScanlineFill_FillEdgeData(Edge *table, const size_t maxTableSize, Edge *edge, const Point *lower, const Point *upper, int32_t yComp)
{
	// Fill edge info
	edge->xl = lower->x;
	edge->yu = (upper->y < yComp) ? upper->y - 1 : upper->y;  // adjust if edge is monotonically increasing or decreasing
	edge->dx = ((float)(upper->x - lower->x)) / (upper->y - lower->y);
	// Insert edge in edge table
	ScanlineFill_InsertEdge(table + lower->y, edge);
}
