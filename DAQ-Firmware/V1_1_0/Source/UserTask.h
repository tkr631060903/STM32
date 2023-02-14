#ifndef _USER_TASK_H_
#define _USER_TASK_H_


extern const int TFCCallbackLookupTableLen;

void SetPeriodicTask();



typedef struct
{
  u16 itemId;
  u8 hwVersionMask;//00 全版本适用
  u8 hwVersionRange;//实际版本&Mask必须等于该值
  pTFCCallback tfcCallback;
  char  thumbText[18];
}TFCCallbackLookupTable_Type;

#endif