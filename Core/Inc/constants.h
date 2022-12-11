/*
 * constants.h
 *
 *  Created on: 04.12.2022
 *      Author: Bojan Sofronievski
 */

#ifndef INC_CONSTANTS_H_
#define INC_CONSTANTS_H_

/***** Point related *****/
#define POINT_MAX_NUM 20

/***** Polygon buffer related *****/
#define POLYGON_BUF_WIDTH	200
#define POLYGON_BUF_HEIGHT	200

/***** Scanline fill related *****/
#define SCANLINE_FILL_TABLE_SIZE 		POLYGON_BUF_HEIGHT
#define SCANLINE_FILL_EDGES_MAX_NUM		20

/***** Benchmark related *****/
#define BENCHMARK_SORT_POINTS			0
#define BENCHMARK_BUILD_TABLE			1
#define BENCHMARK_PROCESS_SCANLINES		2
#define BENCHMARK_TOTAL_TIME			3
#define BENCHMARK_NUM_TIMES				4

#endif /* INC_CONSTANTS_H_ */
