#ifndef _GPIO__G_DEF_H_
#define _GPIO__G_DEF_H_


void GPIOInit(void);

void SetLedStatus(u8 red, u8 blue);
int GetLedStatus(u8* red, u8* blue);
u8 GetDeviceHardwareId(char* str);


#endif