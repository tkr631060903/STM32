/*****************************************************************************
//WinTimer.c
使用方法：初始化：  WinTimerInit();
头文件：  WinTimer.h
设定：    SetTimer(unsigned short nIDEvent,unsigned long TimeInterval,unsigned long CALLBACK);
取消：    KillTimer(unsigned short nIDEvent);
事件处理：OnTimer(unsigned short nIDEvent);//需要自己在主函数中自行添加本函数内容，无需声明。例程如下：



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
//设定三个任务：
//任务1.每隔30*50ms扫描一次键盘
//任务2.每隔50*50ms刷新一次屏幕
//任务3.每隔3*50ms读一次传感器数据
//要求以上任务在开机1s后开始执行


/////////////////////////////////
//以下内容基于文件main.c
/////////////////////////////////
#include "WinTimer.h"
void main()
{
SystemInit();//系统初始化之类的各种代码
WinTimerInit();
SetTimer(1,1000/50);
while(1);
}
void OnTimer(unsigned short nIDEvent)
{
if(nIDEvent==1)
{
KillTimer(1);				//停止软件定时器1的不断执行
SetTimer(2,30);			//设置任务2，每隔30*50ms执行一次
SetTimer(3,50);			//设置任务3，每隔50*50ms执行一次
SetTimer(4,3);			//设置任务4，每隔3*50ms执行一次
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
功能 ：Tim3计数器，10ms中断1次 
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
                    (*EventQueneCallBack[i])();	//直到执行完成
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