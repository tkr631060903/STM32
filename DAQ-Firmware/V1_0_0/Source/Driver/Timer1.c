
#include "includes.h"

//�ѱ�DO24ռ��
void TIM1_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	//TIM1 clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period 	= 65535;		//�������Ƶ�65535���
	TIM_TimeBaseStructure.TIM_Prescaler 	= 7199;			//��һ����0.1ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;			//����Ƶ��������������趨TIM1ģ������˲����Ĺ���Ƶ��
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_ARRPreloadConfig(TIM1, DISABLE);				//Ԥװ�ؼĴ��������ݱ��������͵�Ӱ�ӼĴ���

	TIM_Cmd(TIM1, ENABLE);
}