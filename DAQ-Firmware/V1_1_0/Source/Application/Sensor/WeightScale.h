#ifndef _WEIGHT_SCALE_H_
#define _WEIGHT_SCALE_H_


extern u8 weightMeasureStart;
extern u32 gWeightUpdateTime;

int GetFirstWeightData();
int GetNormalWeightData();
int GetFirstFatData();
int CalculateLBMArray(long* param, float* bmi);
int GetBMIIndex(float bmi);
int WeightSensorDetect();
int EnterCalibFunction();

#endif