#ifndef _LED_DRIVER_H_
#define _LED_DRIVER_H_

u8  CheckCrc(u8* dat, int len);
int ledDriverRegRead(u8 dev, u8 regAddr, u8 length, u8* readbackData);
u8 ledDriverRegWrite(u8 dev, u8 regAddr, u8 length, u8* writeData);

void DoUnprotectedPwmModeInit();//PE9 ch0 TIM1_CH1, PE11 ch1 TIM1_CH2
//void DoUnprotectedPwmModeWrite(u8 ch, float fFreq, u32 pwmDuty);//PE9 ch0 TIM1_CH1, PE11 ch1 TIM1_CH2
int DoUnprotectedPwmModeWrite(float fFreq, u32 pwmDuty);
int DoUnprotectedIoModeWrite(u8 ch, u8 level);//PE9 ch0 TIM1_CH1, PE11 ch1 TIM1_CH2

int DoOcpWrite(int ch, int ocp);
int DoFreqWrite(int ch, int freq);
int DoPwmModeWrite(int ch, int duty);
int DoIoWrite(int ch, int val);

void DoInit();

//’Ô∂œœ‡πÿ
int ShortCircuitDetect(u32* sf);
int ClearDoErrorFlag();
int GetDoADCVal(u8 dev, u8 ch,float* result);
u8 DoSelfTest();

#endif