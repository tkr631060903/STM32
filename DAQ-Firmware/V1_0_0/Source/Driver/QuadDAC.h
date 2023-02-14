#ifndef __TOUCHCS_H__
#define __TOUCHCS_H__

#include "includes.h"

//DAC每秒采样点数
#define AO_DAC_SPS 1000

typedef struct
{
    s16 LUT[100];
    u8 lenLUT;
    float frequency;
    float step;//重要
    u8 curIndex;
    float actualIndex;
    s32 vpp;
    s32 voffset;
    u8 bEnable;
}AoDDSType;

void Timer2Init(void);


void I2C1_Init(int dataRate);
int I2C1_ByteWrite(u8 devAddr, u8 pBuffer, u8 WriteAddr);
void I2C1_ByteRead(u8 devAddr, u8* buffer, u8 ReadAddr);
void I2C1_BufferRead(u8 devAddr, u8* buffer, u8 ReadAddr, int NumByteToRead);
void I2C1_BufferWrite(u8 devAddr, u8* pBuffer, u8 WriteAddr, int numToWrite);

int SetAoChannelVol(u8 ch, u32 vol);
int SetAoPowerState(u8 bOn);
void AoInit();

extern AoDDSType AoDdsParam[4];

void Timer3Init(u16 workingFreq);
void SetAoVoltageCtrlDuty(u16 duty);
void SetAoSupplyVol(u8 ch, u32 vol);

#endif







