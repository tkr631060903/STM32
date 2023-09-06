#include "util.h"

uint32_t time = 0;

uint32_t Time_Out(uint32_t ms)
{
    if (ms == 0) {
        time = 0;
        return time;
    }
    if(time == 0){
        time = ms;
    }
    SysTick_Delay_ms(1);
    time--;
    return time;
}
