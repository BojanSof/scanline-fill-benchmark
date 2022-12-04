/*
 * point.c
 *
 *  Created on: 29.11.2022
 *      Author: Bojan Sofronievski
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "point.h"
#include "constants.h"

typedef struct
{
	PointPolar point;
	size_t index;
} PolarPointWithIndex;

// compare polar points based on angle and radius
static int PolarPointWithIndex_compare(const void *a, const void *b)
{
	static const float eps = 1e-5f;
	const float angleDiff = ((PolarPointWithIndex*)a)->point.angle - ((PolarPointWithIndex*)b)->point.angle;
	if (fabsf(angleDiff) < eps)
	{
		const uint32_t radiusDiff = ((PolarPointWithIndex*)a)->point.radiusSquared - ((PolarPointWithIndex*)b)->point.radiusSquared;
		if (radiusDiff < 0)
		{
			return 1;
		}
		else if (radiusDiff > 0)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}
	else if (angleDiff > 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

PointPolar Point_CartesianToPolar(const Point *point)
{
	PointPolar polar = {};
	polar.radiusSquared = point->x * point->x + point->y * point->y;
	polar.angle = atan2(point->y, point->x);
	return polar;
}

Point Point_FindMassCenter(const Point *points, const size_t numPoints)
{
	int32_t xSum = 0, ySum = 0;
	for(const Point *p = points; p != (points + numPoints); ++p)
	{
		xSum += p->x;
		ySum += p->y;
	}

	Point massCenter = {
			.x = xSum / numPoints,
			.y = ySum / numPoints
	};

	return massCenter;
}

void Point_MoveOrigin(Point *points, const size_t numPoints, const Point *origin)
{
	for(Point *p = points; p != (points + numPoints); ++p)
	{
		p->x -= origin->x;
		p->y -= origin->y;
	}
}

void Point_SortAntiClockwise(Point *points, const size_t numPoints)
{
	// Make a copy of the points
	Point pointsOriginal[POINT_MAX_NUM] = {};
	memcpy(pointsOriginal, points, numPoints*sizeof(points[0]));
	// 1. Find mass center
	Point massCenter = Point_FindMassCenter(points, numPoints);
	// 2. Shift points origin to mass center
	Point_MoveOrigin(points, numPoints, &massCenter);
	// 3. Convert to polar coordinates
	PolarPointWithIndex polarPointsWithIndex[POINT_MAX_NUM] = {};
	for (size_t iPoint = 0; iPoint < numPoints; ++iPoint)
	{
		polarPointsWithIndex[iPoint].point = Point_CartesianToPolar(points + iPoint);
		polarPointsWithIndex[iPoint].index = iPoint;
	}
	// 4. Sort points based on polar coordinates
	qsort(polarPointsWithIndex, numPoints, sizeof(PolarPointWithIndex), PolarPointWithIndex_compare);
	// 5. Reorder original points based on the sorted order of polar points
	for(size_t iPoint = 0; iPoint < numPoints; ++iPoint)
	{
		points[iPoint] = pointsOriginal[polarPointsWithIndex[iPoint].index];
	}
}
