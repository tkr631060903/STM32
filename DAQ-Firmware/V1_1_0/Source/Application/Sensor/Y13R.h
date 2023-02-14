
#ifndef _Y13R_H_
#define _Y13R_H_


extern u32 gHeightUpdateTime;

int GetFirstRFIDData();

int LoadRFIDData32(char* str, u8 blockNum);
int StoreRFIDData32(char* str, u8 blockNum);

#endif