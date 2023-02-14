#ifndef _WIN_TIMER_H_
#define _WIN_TIMER_H_



/*********************************************************************************************************
定义WinTimer,请按照注释中的说明对WinTimer进行配置
*********************************************************************************************************/
//  定义允许定义的最大定时器数量，设定范围1-255。数值越小越节省CPU资源。
//  e.g.   #define MAX_EVENT_NUMBER            10
#define MAX_EVENT_NUMBER           15
/*********************************************************************************************************
End of WinTimer Configuration
*********************************************************************************************************/



extern unsigned long tmr_index;


void OnTimer(unsigned short nIDEvent);
void SetTimer(unsigned char nIDEvent,unsigned short TimeInterval, pCallback lpfnTimer);
void KillTimer(unsigned char nIDEvent);
void Timer5Init();
int GetSystemTime();


#endif