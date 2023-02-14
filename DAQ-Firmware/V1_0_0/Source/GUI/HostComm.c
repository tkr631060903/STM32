
#include "includes.h"



u32 gCPULoadTimerCounter = 0;

u32 GetIntArrayMax(u32* array, int numberOfArray)
{
    u32 max=array[0];
    int i;
    if(numberOfArray<=1)
        return 0;
    for(i=1;i<numberOfArray;i++)
    {
        if(array[i]>max)
            max = array[i];
    }
    return max;
}

float GetFloatArrayMax(float* array, int numberOfArray)
{
    float max=array[0];
    int i;
    if(numberOfArray<=1)
        return 0;
    for(i=1;i<numberOfArray;i++)
    {
        if(array[i]>max)
            max = array[i];
    }
    return max;
}

float GetArrayMin(float* array, int numberOfArray)
{
    float min=array[0];
    int i;
    if(numberOfArray<=1)
        return 0;
    for(i=1;i<numberOfArray;i++)
    {
        if(array[i]<min)
            min = array[i];
    }
    return min;
}

int GetIntMin(int a, int b)
{
    if(a>b)
        return b;
    else return a;
}

void DelayUs(u16 time)
{
    u16 i = 0;
    while(time--)
    {
        i = 10;
        while(i--);
    }
}
void DelayMs(u32 nMs)
{
    u32 i;
    for(; nMs !=0; nMs--)
    {
        i = 10301;
        while(i--);
    }
}

void SystemSoftReset()
{
    NVIC_SystemReset();
    while(1);
}



void RunUserInterface()
{
    u32 i;
    while(1)    //CPU空闲时，1ms执行一次循环
    {
        int a = !GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13);
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)a);
        i = 10261;
        while(i--);
        gCPULoadTimerCounter ++;
    }
}