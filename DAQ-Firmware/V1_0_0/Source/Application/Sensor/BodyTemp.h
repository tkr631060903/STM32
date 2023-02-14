#ifndef _BODY_TEMP_H_
#define _BODY_TEMP_H_


extern u8 heightMeasureStart;
extern u32 gHeightUpdateTime;
extern u8 HeightSensorPID;

int EventBodyTempDataReceived(char* str);
int GetFirstBodyTempData();


#endif