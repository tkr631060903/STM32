#ifndef _USER_TASK_H_
#define _USER_TASK_H_


extern const int TFCCallbackLookupTableLen;

void SetPeriodicTask();



typedef struct
{
  u16 itemId;
  u8 hwVersionMask;//00 ȫ�汾����
  u8 hwVersionRange;//ʵ�ʰ汾&Mask������ڸ�ֵ
  pTFCCallback tfcCallback;
  char  thumbText[18];
}TFCCallbackLookupTable_Type;

#endif