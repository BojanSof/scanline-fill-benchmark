/*
 * cycle_counter.h
 *
 * Interface for the cycle counter which can be used to
 * determine execution time of some code.
 *
 *  Created on: Nov 22, 2022
 *      Author: Bojan Sofronievski
 */

#ifndef INC_CYCLE_COUNTER_H_
#define INC_CYCLE_COUNTER_H_

#include <stdint.h>
#include <stm32f4xx.h>

/**
 * Initialize the cycle counter
 */
void CycleCounter_Init(void);

/**
 * @brief Get the current cycle counter value
 * @retval current cycle counter value
 */
inline uint32_t CycleCounter_GetValue(void)
{
	return DWT->CYCCNT;
}

#endif /* INC_CYCLE_COUNTER_H_ */
