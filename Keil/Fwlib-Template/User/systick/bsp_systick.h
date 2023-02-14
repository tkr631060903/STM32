#ifndef _BSP_SYSTICK_H
#define _BSP_SYSTICK_H

#include "stm32f10x.h"
#include "core_cm3.h"

void SysTick_Delay_Ms(uint32_t us);
void SysTick_Delay_Us(uint32_t ms);

#endif //_BSP_SYSTICK_H
