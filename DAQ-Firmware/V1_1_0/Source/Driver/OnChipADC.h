
#ifndef _BATTERY_H_
#define _BATTERY_H_





void AIInit(void);
u16 GetBatVol(u8 Ch);
u32 GetAIVal(u8 ch);

u8 GetRandomCharFromADC();
float GetRandomFromRange(float a, float b);

#endif