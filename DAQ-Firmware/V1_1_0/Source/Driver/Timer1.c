
#include "includes.h"

//已被DO24占用
void TIM1_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	//TIM1 clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period 	= 65535;		//计数器计到65535溢出
	TIM_TimeBaseStructure.TIM_Prescaler 	= 7199;			//计一个数0.1ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;			//不分频，这个设置用于设定TIM1模块里的滤波器的工作频率
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_ARRPreloadConfig(TIM1, DISABLE);				//预装载寄存器的内容被立即传送到影子寄存器

	TIM_Cmd(TIM1, ENABLE);
}