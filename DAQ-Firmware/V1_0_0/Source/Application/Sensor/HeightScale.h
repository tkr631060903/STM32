#ifndef _HEIGHT_SCALE_H_
#define _HEIGHT_SCALE_H_


extern u8 heightMeasureStart;
extern u32 gHeightUpdateTime;
extern u8 HeightSensorPID;

int EventHeightDataReceived(char* str);
int GetFirstHeightData();
int SetHeightTest();
int SetHeightCalib();
int DetectHeightSensor();

#endif