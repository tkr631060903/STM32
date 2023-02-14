
#ifndef _HOST_COMM_H_
#define _HOST_COMM_H_


float GetFloatArrayMax(float* array, int numberOfArray);
u32 GetIntArrayMax(u32* array, int numberOfArray);
void DelayUs(u16 time);
void DelayMs(u32 nMs);
void RunUserInterface();

void SystemSoftReset();


#endif