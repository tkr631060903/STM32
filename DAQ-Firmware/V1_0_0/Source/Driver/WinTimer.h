#ifndef _WIN_TIMER_H_
#define _WIN_TIMER_H_



/*********************************************************************************************************
����WinTimer,�밴��ע���е�˵����WinTimer��������
*********************************************************************************************************/
//  ��������������ʱ���������趨��Χ1-255����ֵԽСԽ��ʡCPU��Դ��
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