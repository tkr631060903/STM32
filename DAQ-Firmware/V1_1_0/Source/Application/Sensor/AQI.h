#ifndef _AQI_H_
#define _AQI_H_


int GetAQIData(u8 type);
int SendAQIRequest(u8 type);
int CheckAQIData();
int DispPreviousAQIData();

#endif