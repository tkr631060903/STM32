/*****************************************************************************
//WinTimer.c
ʹ�÷�������ʼ����  WinTimerInit();
ͷ�ļ���  WinTimer.h
�趨��    SetTimer(unsigned short nIDEvent,unsigned long TimeInterval,unsigned long CALLBACK);
ȡ����    KillTimer(unsigned short nIDEvent);
�¼�����OnTimer(unsigned short nIDEvent);//��Ҫ�Լ�����������������ӱ��������ݣ������������������£�



void OnTimer(unsigned short nIDEvent)
{
//  TODO: add extra Timer process here. 
//  e.g.  if(nIDEvent==1) 
//            Delay(23);
//        else if(nIDEvent==2)        
//            KillTimer(2);
//  End of TODO: add extra Timer process here. 
return;
}


##############################################################################
##############################################################################
##############################################################################
//e.g.
//�趨��������
//����1.ÿ��30*50msɨ��һ�μ���
//����2.ÿ��50*50msˢ��һ����Ļ
//����3.ÿ��3*50ms��һ�δ���������
//Ҫ�����������ڿ���1s��ʼִ��


/////////////////////////////////
//�������ݻ����ļ�main.c
/////////////////////////////////
#include "WinTimer.h"
void main()
{
SystemInit();//ϵͳ��ʼ��֮��ĸ��ִ���
WinTimerInit();
SetTimer(1,1000/50);
while(1);
}
void OnTimer(unsigned short nIDEvent)
{
if(nIDEvent==1)
{
KillTimer(1);				//ֹͣ�����ʱ��1�Ĳ���ִ��
SetTimer(2,30);			//��������2��ÿ��30*50msִ��һ��
SetTimer(3,50);			//��������3��ÿ��50*50msִ��һ��
SetTimer(4,3);			//��������4��ÿ��3*50msִ��һ��
		}
		else if(nIDEvent==2)
{
KeyScan();
		}
		else if(nIDEvent==3)
{
ScreenRefresh();
		}
		else if(nIDEvent==4)
{
DataGet();
		}
}
##############################################################################
##############################################################################
##############################################################################

*****************************************************************************/
#include "includes.h"


unsigned long tmr_index=0;

unsigned short EventQuene[MAX_EVENT_NUMBER];
unsigned short EventQueneForAccuracy[MAX_EVENT_NUMBER];
pCallback      EventQueneCallBack[MAX_EVENT_NUMBER];




/*********************************************************************************************************
���� ��Tim3��������10ms�ж�1�� 
**********************************************************************************************************/
void Timer5Init(void)
{
    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_TimeBaseStructure.TIM_Period = 10000;
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);
    
    TIM_ARRPreloadConfig(TIM5, DISABLE);
    
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
    
    int i;
    
    for(i=0;i<MAX_EVENT_NUMBER;i++)
    {
        EventQuene[i]=0;
        EventQueneForAccuracy[i]=0;
        EventQueneCallBack[i]=NULL;
    }
    
    TIM_Cmd(TIM5, ENABLE);
}
void TIM5_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        int i;
        for(i=0;i<MAX_EVENT_NUMBER;i++)
        {
            if(EventQuene[i]==0)
                continue;
            else if(tmr_index%EventQuene[i]==EventQueneForAccuracy[i])
            {
                if(EventQueneCallBack[i]!=NULL)
                {
                    (*EventQueneCallBack[i])();	//ֱ��ִ�����
                }
                else
                {
                    OnTimer(i);
                }
            }
        }
        tmr_index++;
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    }
}


void KillTimer(unsigned char nIDEvent)
{
    EventQuene[nIDEvent]=0;
    EventQueneForAccuracy[nIDEvent]=0;
    EventQueneCallBack[nIDEvent]=NULL;
}

void SetTimer(unsigned char nIDEvent,unsigned short TimeInterval, pCallback lpfnTimer)
{
    EventQuene[nIDEvent]=TimeInterval;
    EventQueneForAccuracy[nIDEvent]=(tmr_index%TimeInterval+TimeInterval-1)%TimeInterval;
    EventQueneCallBack[nIDEvent]=lpfnTimer;
}

int GetSystemTime()
{
    return tmr_index;
}