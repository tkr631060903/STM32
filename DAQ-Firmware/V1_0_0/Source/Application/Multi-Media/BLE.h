#ifndef _BLE_H_
#define _BLE_H_

int CheckBLEMsg();
int CheckReceivedBLEMsg(char* buf);
int SendBLEMsg(char* buf);

int SendTestResultToBLE(u8);

#endif