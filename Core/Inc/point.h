/*
 * point.h
 *
 *  Created on: 27.11.2022
 *      Author: Bojan Sofronievski
 */

#ifndef INC_POINT_H_
#define INC_POINT_H_

#include <stdint.h>

/**
 * Struct holding 2D Cartesian integer coordinates for a given point;
 */
typedef struct {
	int32_t x;
	int32_t y;
} Point;

/**
 * Struct holding 2D Polar coordinates for a given point
 */
typedef struct {
	uint32_t radiusSquared;
	float angle;
} PointPolar;

/**
 * @brief Convert 2D point given with Cartesian coordinates
 * in polar coordinates
 * @param point Cartesian coordinates of the point
 * @retval Point with polar coordinates
 */
PointPolar Point_CartesianToPolar(const Point *point);

/**
 * @brief Find the mass center of a given set of points
 * @param points Array of points
 * @param numPoints Number of points in the array
 * @retval Mass center of the points
 */
Point Point_FindMassCenter(const Point *points, const size_t numPoints);

/**
 * @brief Shift a set of points to a new origin
 * @param points Array of points
 * @param numPoints Number of points in the array
 * @param origin New origin for the points
 */
void Point_MoveOrigin(Point *points, const size_t numPoints, const Point *origin);

/**
 * @brief Sort set of points, anti-clockwise.
 * Sorting is done by polar coordinates (angle and radius).
 * @param points Array of points
 * @param numPoints Number of points in the array
 */
void Point_SortAntiClockwise(Point *points, const size_t numPoints);

#endif /* INC_POINT_H_ */
