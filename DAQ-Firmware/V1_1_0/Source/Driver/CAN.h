#ifndef __CAN___H_
#define __CAN___H_



//波特率参数
#define		CAN_SJW_10K	CAN_SJW_1tq
#define		CAN_SJW_20K	CAN_SJW_1tq
#define		CAN_SJW_33K	CAN_SJW_1tq//33k
#define		CAN_SJW_50K	CAN_SJW_1tq
#define		CAN_SJW_62K	CAN_SJW_1tq//62.5
#define		CAN_SJW_83K	CAN_SJW_1tq//83
#define		CAN_SJW_100K	CAN_SJW_1tq
#define		CAN_SJW_125K	CAN_SJW_1tq
#define		CAN_SJW_250K	CAN_SJW_1tq
#define		CAN_SJW_500K	CAN_SJW_1tq
#define		CAN_SJW_750K	CAN_SJW_1tq
#define		CAN_SJW_800K	CAN_SJW_1tq//800
#define		CAN_SJW_1000K	CAN_SJW_1tq

#define		CAN_BS1_10K	CAN_BS1_15tq
#define		CAN_BS1_20K	CAN_BS1_10tq
#define		CAN_BS1_33K	CAN_BS1_7tq//33
#define		CAN_BS1_50K	CAN_BS1_7tq
#define		CAN_BS1_62K	CAN_BS1_7tq//62
#define		CAN_BS1_83K	CAN_BS1_8tq//83
#define		CAN_BS1_100K	CAN_BS1_7tq
#define		CAN_BS1_125K	CAN_BS1_9tq
#define		CAN_BS1_250K	CAN_BS1_9tq
#define		CAN_BS1_500K	CAN_BS1_9tq
#define		CAN_BS1_750K	CAN_BS1_6tq
#define		CAN_BS1_800K	CAN_BS1_7tq//
#define		CAN_BS1_1000K	CAN_BS1_6tq

#define		CAN_BS2_10K	CAN_BS2_2tq
#define		CAN_BS2_20K	CAN_BS2_7tq
#define		CAN_BS2_33K	CAN_BS2_3tq//
#define		CAN_BS2_50K	CAN_BS2_4tq
#define		CAN_BS2_62K	CAN_BS2_4tq//
#define		CAN_BS2_83K	CAN_BS2_3tq//
#define		CAN_BS2_100K	CAN_BS2_2tq
#define		CAN_BS2_125K	CAN_BS2_8tq
#define		CAN_BS2_250K	CAN_BS2_8tq
#define		CAN_BS2_500K	CAN_BS2_8tq
#define		CAN_BS2_750K	CAN_BS2_1tq
#define		CAN_BS2_800K	CAN_BS2_1tq//
#define		CAN_BS2_1000K	CAN_BS2_2tq

#define		CAN_Prescaler_10K	200
#define		CAN_Prescaler_20K	100
#define		CAN_Prescaler_33K	100//
#define		CAN_Prescaler_50K	60
#define		CAN_Prescaler_62K	48//
#define		CAN_Prescaler_83K	36//
#define		CAN_Prescaler_100K	36
#define		CAN_Prescaler_125K	16
#define		CAN_Prescaler_250K	8
#define		CAN_Prescaler_500K	4
#define		CAN_Prescaler_750K	6
#define		CAN_Prescaler_800K	5//
#define		CAN_Prescaler_1000K	4


int CAN_Baud_Config(u8 channel, u16 baud);

void CAN1_Init(u16 Baud);
void CAN1_TxMessage(u8 bExtAddress, u32 can_id, int dataLen, u8 *txBuf);
void CAN1_Config(u16 Baud);
int GetCan1RxStatus(CanRxMsg* rxMsg);

void CAN2_Init(u16 Baud);
void CAN2_TxMessage(u8 bExtAddress, u32 can_id, int dataLen, u8 *txBuf);
void CAN2_Config(u16 Baud);
int GetCan2RxStatus(CanRxMsg* rxMsg);

int CAN_SendData(u8 channel, CanTxMsg* txMsg);

extern CanTxMsg    CAN1_Tx_Buffer[CAN_BUFFER_SIZE];
extern int         CAN1_Tx_Index;
extern int         CAN1_TX_Repeat_No;
extern CanTxMsg    CAN2_Tx_Buffer[CAN_BUFFER_SIZE];
extern int         CAN2_Tx_Index;
extern int         CAN2_TX_Repeat_No;

void Can1RepeatSendingCallback();
void Can2RepeatSendingCallback();

void CANFilterConfig(u8 ch, u8 bEnable,u32 bListMode, u32 canId, u8 bExtId, u8 bRemoteFrame, u32 canIdMask, u8 bMaskExtId, u8 bMaskRemoteFrame);
void ClearCanRxBuffer(u8 channel);

#endif