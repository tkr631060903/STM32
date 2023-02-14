#ifndef _USER_TASK_H_
#define _USER_TASK_H_


extern const int TFCCallbackLookupTableLen;

void SetPeriodicTask();



typedef struct
{
  u16 itemId;
  pTFCCallback tfcCallback;
  char  thumbText[18];
}TFCCallbackLookupTable_Type;

#endif