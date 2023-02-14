

#include "includes.h"



/*********************************************************************************************************
功能 ：TIM4用做计数器
**********************************************************************************************************/
void Timer4Init(void)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//TIM2 clock enable

	TIM_TimeBaseStructure.TIM_Period = 10000;
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	TIM_SetCounter(TIM4,0);
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ARRPreloadConfig(TIM4, DISABLE);	//预装载寄存器的内容被立即传送到影子寄存器
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

s8 gCPULoadResult = 0;

void TIM4_IRQHandler(void)
{
    static u32 preCPULoadTimerCounter=0;
    if((TIM4->SR&TIM_IT_Update) && (TIM4->DIER&TIM_IT_Update))
    {
        TIM4->SR = 0xFFFE;	//TIM_ClearITPendingBit(TIM3, TIM_IT_Update);清除中断标志
        TIM4->CNT = 0;		//定时器CNT清零
        gCPULoadResult = 100-(gCPULoadTimerCounter-preCPULoadTimerCounter)/10;
        if(gCPULoadResult>100) gCPULoadResult =100;
        if(gCPULoadResult<0) gCPULoadResult =0;
        preCPULoadTimerCounter = gCPULoadTimerCounter;
    }
}

u8 GetCPULoadResult()
{
    return gCPULoadResult;
}