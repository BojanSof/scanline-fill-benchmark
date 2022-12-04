/*
 * cycle_counter.c
 *
 *  Created on: Nov 22, 2022
 *      Author: Bojan Sofronievski
 */

#include "cycle_counter.h"

void CycleCounter_Init(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	ITM->LAR = 0xc5acce55;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

extern uint32_t CycleCounter_GetValue(void);
