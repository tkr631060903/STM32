#include "includes.h"



const u8 CanTiming[13][4] = 
{
	{CAN_SJW_10K,  CAN_BS1_10K,  CAN_BS2_10K,  CAN_Prescaler_10K},
	{CAN_SJW_20K,  CAN_BS1_20K,  CAN_BS2_20K,  CAN_Prescaler_20K},
	{CAN_SJW_33K,  CAN_BS1_33K,  CAN_BS2_33K,  CAN_Prescaler_33K},
	{CAN_SJW_50K,  CAN_BS1_50K,  CAN_BS2_50K,  CAN_Prescaler_50K},
	{CAN_SJW_62K,  CAN_BS1_62K,  CAN_BS2_62K,  CAN_Prescaler_62K},
	{CAN_SJW_83K,  CAN_BS1_83K,  CAN_BS2_83K,  CAN_Prescaler_83K},
	{CAN_SJW_100K, CAN_BS1_100K, CAN_BS2_100K, CAN_Prescaler_100K},
	{CAN_SJW_125K, CAN_BS1_125K, CAN_BS2_125K, CAN_Prescaler_125K},
	{CAN_SJW_250K, CAN_BS1_250K, CAN_BS2_250K, CAN_Prescaler_250K},
	{CAN_SJW_500K, CAN_BS1_500K, CAN_BS2_500K, CAN_Prescaler_500K},
	{CAN_SJW_750K, CAN_BS1_750K, CAN_BS2_750K, CAN_Prescaler_750K},
	{CAN_SJW_800K, CAN_BS1_800K, CAN_BS2_800K, CAN_Prescaler_800K},
	{CAN_SJW_1000K,CAN_BS1_1000K,CAN_BS2_1000K,CAN_Prescaler_1000K}
};


int CAN_Baud_Config(u8 channel, u16 baud)
{
    u16 i = 8;
    int retVal  = 1;
	
	CAN_InitTypeDef CAN_InitStructure;				//结构体实例
	CAN_StructInit(&CAN_InitStructure);				//写入默认值
    
    if(channel==1)
        retVal *= CAN_OperatingModeRequest(CAN1,CAN_OperatingMode_Initialization);
    else
        retVal *= CAN_OperatingModeRequest(CAN2,CAN_OperatingMode_Initialization);
	
	//CAN1 参数初始化
	CAN_InitStructure.CAN_TTCM = DISABLE;	//禁止时间触发通信方式
	CAN_InitStructure.CAN_ABOM = ENABLE;	//禁止总线自动关闭
	CAN_InitStructure.CAN_AWUM = DISABLE;	//禁止自动唤醒
	CAN_InitStructure.CAN_NART = DISABLE;	//禁止自动重发(设置成使能 发不出数)
	CAN_InitStructure.CAN_RFLM = DISABLE;	//禁止接收FIFO锁定
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  	  
	switch(baud)
	{
		case   10: i = 0; break;	//10K
		case   20: i = 1; break;	//20k
		case   33: i = 2; break;	//33k
		case   50: i = 3; break;	//50k
		case   62: i = 4; break;	//62k
		case   83: i = 5; break;	//83k
		case  100: i = 6; break;	//100k
		case  125: i = 7; break;	//125k
		case  250: i = 8; break;	//250k
		case  500: i = 9; break;	//500k
		case  750: i = 10; break;	//750k
		case  800: i = 11; break;	//800k
		case 1000: i = 12; break;	//1M
		default  : i = 9; break;	//默认为500Kbps
	}
	CAN_InitStructure.CAN_SJW  	= CanTiming[i][0];
	CAN_InitStructure.CAN_BS1  	= CanTiming[i][1];
	CAN_InitStructure.CAN_BS2 	= CanTiming[i][2];
	CAN_InitStructure.CAN_Prescaler = CanTiming[i][3];
    
    if(channel==1)
    	CAN_Init(CAN1, &CAN_InitStructure);
    else 
        CAN_Init(CAN2, &CAN_InitStructure);
    
    if(channel==1)
        retVal *= CAN_OperatingModeRequest(CAN1,CAN_OperatingMode_Normal);
    else
        retVal *= CAN_OperatingModeRequest(CAN2,CAN_OperatingMode_Normal);
    
    if(retVal) retVal = 1;
    return retVal;
}
/************************************************************************************************************
功能 : 61(PB8),62(PB9)为CAN总线
************************************************************************************************************/
void CAN1_Init(u16 Baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;		//端口结构体实例
	NVIC_InitTypeDef NVIC_InitStructure;		//中断结构体实例
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//配置GPIO功能
	//IO端口重新映射到PB8 PB9(如果不重新映射，该句话屏蔽，再改引脚号即可)
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1 , ENABLE);
	
	//开启CAN1时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	//配置NVIC和向量表基地址
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	CAN1_Config(Baud);
}


void CAN1_Config(u16 Baud)
{
	u16 i = 8;
	
	CAN_InitTypeDef CAN_InitStructure;				//结构体实例
	CAN_FilterInitTypeDef CAN_FilterInitStructure;	//结构体实例
	CAN_DeInit(CAN1);		   						//复位CAN寄存器
	CAN_StructInit(&CAN_InitStructure);				//写入默认值
	
	//CAN1 参数初始化
	CAN_InitStructure.CAN_TTCM = DISABLE;	//禁止时间触发通信方式
	CAN_InitStructure.CAN_ABOM = ENABLE;	//禁止总线自动关闭
	CAN_InitStructure.CAN_AWUM = DISABLE;	//禁止自动唤醒
	CAN_InitStructure.CAN_NART = DISABLE;	//禁止自动重发(设置成使能 发不出数)
	CAN_InitStructure.CAN_RFLM = DISABLE;	//禁止接收FIFO锁定
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  	  
	switch(Baud)
	{
		case   10: i = 0; break;	//10K
		case   20: i = 1; break;	//20k
		case   33: i = 2; break;	//33k
		case   50: i = 3; break;	//50k
		case   62: i = 4; break;	//62k
		case   83: i = 5; break;	//83k
		case  100: i = 6; break;	//100k
		case  125: i = 7; break;	//125k
		case  250: i = 8; break;	//250k
		case  500: i = 9; break;	//500k
		case  750: i = 10; break;	//750k
		case  800: i = 11; break;	//800k
		case 1000: i = 12; break;	//1M
		default  : i = 9; break;	//默认为500Kbps
	}
	CAN_InitStructure.CAN_SJW  	= CanTiming[i][0];
	CAN_InitStructure.CAN_BS1  	= CanTiming[i][1];
	CAN_InitStructure.CAN_BS2 	= CanTiming[i][2];
	CAN_InitStructure.CAN_Prescaler = CanTiming[i][3];
	CAN_Init(CAN1, &CAN_InitStructure);
  	//过滤第一组ID
	CAN_FilterInitStructure.CAN_FilterNumber 	 = 0;
	CAN_FilterInitStructure.CAN_FilterMode   	 = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale  	 = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh 	 = 0;//(((u32)CPU_CAN_0<<21)&0xFFFF0000)>>16;
	CAN_FilterInitStructure.CAN_FilterIdLow 	 = 0;//(((u32)CPU_CAN_0<<21)|CAN_ID_STD|CAN_RTR_DATA)&0xFFFF;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh 	 = 0;//0xFFFF;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow  	 = 0;//0xFFFF;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation 	 = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	//过滤第二组ID
	//CAN_FilterInitStructure.CAN_FilterNumber 	 = 1;CAN_RTR_REMOTE
	//CAN_FilterInitStructure.CAN_FilterIdHigh 	 = (((u32)CPU_CAN_1<<21)&0xFFFF0000)>>16;
	//CAN_FilterInitStructure.CAN_FilterIdLow 	 = (((u32)CPU_CAN_1<<21)|CAN_ID_STD|CAN_RTR_DATA)&0xFFFF;
	//CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}
/************************************************************************************************************
功能 : CAN1接收中断
u8 Uart2_Rx_Buffer[UART2_BUFFER_SIZE*2]={0};
int Uart2_Rx_Index = 0;
************************************************************************************************************/

CanRxMsg    CAN1_Rx_Buffer[CAN_BUFFER_SIZE];
int         CAN1_Rx_Start_Index = 0;
int         CAN1_Rx_End_Index = 0;

void CAN1_RX0_IRQHandler(void)
{
    //int i=0;
    CAN_Receive(CAN1, 0, &(CAN1_Rx_Buffer[CAN1_Rx_End_Index]));
    /*debug("received %d\r\n",CAN1_Rx_Buffer[CAN1_Rx_End_Index].DLC);
    for(i=0;i<CAN1_Rx_Buffer[CAN1_Rx_End_Index].DLC;i++)
        debug("0x%x,",i,CAN1_Rx_Buffer[CAN1_Rx_End_Index].Data[i]);
    debug("\r\n");
    */
    CAN1_Rx_End_Index ++;
    if(CAN1_Rx_End_Index>=CAN_BUFFER_SIZE)
        CAN1_Rx_End_Index = 0;
    if(CAN1_Rx_Start_Index ==CAN1_Rx_End_Index)
    {
        CAN1_Rx_Start_Index++;
        if(CAN1_Rx_Start_Index>=CAN_BUFFER_SIZE)
            CAN1_Rx_Start_Index=0;
    }
    SetTFCEvent(EVENT_CAN1);
}


//返回值为   本报文+缓冲区的报文的总报文数
int GetCan1RxStatus(CanRxMsg* rxMsg)
{
    if(CAN1_Rx_Start_Index ==CAN1_Rx_End_Index)
        return 0;
    memcpy(rxMsg,&(CAN1_Rx_Buffer[CAN1_Rx_Start_Index]),sizeof(CanRxMsg));
    CAN1_Rx_Start_Index ++;
    if(CAN1_Rx_Start_Index>=CAN_BUFFER_SIZE)
        CAN1_Rx_Start_Index=0;
    return (CAN1_Rx_End_Index+CAN_BUFFER_SIZE-CAN1_Rx_Start_Index)%CAN_BUFFER_SIZE+1;
}
void CAN1_TxMessage(u8 bExtAddress, u32 can_id, int dataLen, u8 *txBuf)
{
    CanTxMsg CAN1_TxMsg;		//发送帧结构体实例
	u8 TransmitMailbox = 0;
    
    if(dataLen<1) return;
    else if(dataLen>8) dataLen = 8;
	
    if(bExtAddress)
    {
        CAN1_TxMsg.IDE = CAN_ID_EXT;
        CAN1_TxMsg.ExtId = can_id;
    }
    else
    {
        CAN1_TxMsg.IDE = CAN_ID_STD;
    	CAN1_TxMsg.StdId = can_id;
    }
    
	CAN1_TxMsg.RTR = CAN_RTR_DATA;
	CAN1_TxMsg.DLC = dataLen;
	memcpy(CAN1_TxMsg.Data, txBuf, dataLen);
	
	TransmitMailbox = CAN_Transmit(CAN1, &CAN1_TxMsg);
	while(CAN_TransmitStatus(CAN1, TransmitMailbox) != CANTXOK);
}







/************************************************************************************************************
功能 : 61(PB8),62(PB9)为CAN总线
************************************************************************************************************/
void CAN2_Init(u16 Baud)//PB12 RX   PB13 TX
{
	GPIO_InitTypeDef GPIO_InitStructure;		//端口结构体实例
	NVIC_InitTypeDef NVIC_InitStructure;		//中断结构体实例
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//开启CAN2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

	//配置NVIC和向量表基地址
	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	CAN2_Config(Baud);
}
void CAN2_Config(u16 Baud)
{
	u16 i = 8;
	
	CAN_InitTypeDef CAN_InitStructure;				//结构体实例
	CAN_FilterInitTypeDef CAN_FilterInitStructure;	//结构体实例
	CAN_DeInit(CAN2);		   						//复位CAN寄存器
	CAN_StructInit(&CAN_InitStructure);				//写入默认值
	
	//CAN2 参数初始化
	CAN_InitStructure.CAN_TTCM = DISABLE;	//禁止时间触发通信方式
	CAN_InitStructure.CAN_ABOM = ENABLE;	//禁止总线自动关闭
	CAN_InitStructure.CAN_AWUM = DISABLE;	//禁止自动唤醒
	CAN_InitStructure.CAN_NART = DISABLE;	//禁止自动重发(设置成使能 发不出数)
	CAN_InitStructure.CAN_RFLM = DISABLE;	//禁止接收FIFO锁定
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  	  
	switch(Baud)
	{
		case   10: i = 0; break;	//10K
		case   20: i = 1; break;	//20k
		case   33: i = 2; break;	//33k
		case   50: i = 3; break;	//50k
		case   62: i = 4; break;	//62k
		case   83: i = 5; break;	//83k
		case  100: i = 6; break;	//100k
		case  125: i = 7; break;	//125k
		case  250: i = 8; break;	//250k
		case  500: i = 9; break;	//500k
		case  750: i = 10; break;	//750k
		case  800: i = 11; break;	//800k
		case 1000: i = 12; break;	//1M
		default  : i = 9; break;	//默认为500Kbps
	}
	CAN_InitStructure.CAN_SJW  	= CanTiming[i][0];
	CAN_InitStructure.CAN_BS1  	= CanTiming[i][1];
	CAN_InitStructure.CAN_BS2 	= CanTiming[i][2];
	CAN_InitStructure.CAN_Prescaler = CanTiming[i][3];
	CAN_Init(CAN2, &CAN_InitStructure);
  	//过滤第一组ID
	CAN_FilterInitStructure.CAN_FilterNumber 	 = 14;
	CAN_FilterInitStructure.CAN_FilterMode   	 = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale  	 = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh 	 = 0;
	CAN_FilterInitStructure.CAN_FilterIdLow 	 = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh 	 = 0;//0xFFFF;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow  	 = 0;//0xFFFF;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation 	 = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	//过滤第二组ID
	//CAN_FilterInitStructure.CAN_FilterNumber 	 = 1;
	//CAN_FilterInitStructure.CAN_FilterIdHigh 	 = (((u32)CPU_CAN_1<<21)&0xFFFF0000)>>16;
	//CAN_FilterInitStructure.CAN_FilterIdLow 	 = (((u32)CPU_CAN_1<<21)|CAN_ID_STD|CAN_RTR_DATA)&0xFFFF;
	//CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}
/************************************************************************************************************
功能 : CAN2接收中断
u8 Uart2_Rx_Buffer[UART2_BUFFER_SIZE*2]={0};
int Uart2_Rx_Index = 0;
************************************************************************************************************/
CanRxMsg    CAN2_Rx_Buffer[CAN_BUFFER_SIZE];
int         CAN2_Rx_Start_Index = 0;
int         CAN2_Rx_End_Index = 0;

void CAN2_RX0_IRQHandler(void)
{
    //int i=0;
    CAN_Receive(CAN2, 0, &(CAN2_Rx_Buffer[CAN2_Rx_End_Index]));
    /*debug("received %d\r\n",CAN2_Rx_Buffer[CAN2_Rx_End_Index].DLC);
    for(i=0;i<CAN2_Rx_Buffer[CAN2_Rx_End_Index].DLC;i++)
        debug("0x%x,",i,CAN2_Rx_Buffer[CAN2_Rx_End_Index].Data[i]);
    debug("\r\n");
    */
    CAN2_Rx_End_Index ++;
    if(CAN2_Rx_End_Index>=CAN_BUFFER_SIZE)
        CAN2_Rx_End_Index = 0;
    if(CAN2_Rx_Start_Index ==CAN2_Rx_End_Index)
    {
        CAN2_Rx_Start_Index++;
        if(CAN2_Rx_Start_Index>=CAN_BUFFER_SIZE)
            CAN2_Rx_Start_Index=0;
    }
    
    SetTFCEvent(EVENT_CAN2);
}
//返回值为   本报文+缓冲区的报文的总报文数
int GetCan2RxStatus(CanRxMsg* rxMsg)
{
    if(CAN2_Rx_Start_Index ==CAN2_Rx_End_Index)
        return 0;
    memcpy(rxMsg,&(CAN2_Rx_Buffer[CAN2_Rx_Start_Index]),sizeof(CanRxMsg));
    CAN2_Rx_Start_Index ++;
    if(CAN2_Rx_Start_Index>=CAN_BUFFER_SIZE)
        CAN2_Rx_Start_Index=0;
    return (CAN2_Rx_End_Index+CAN_BUFFER_SIZE-CAN2_Rx_Start_Index)%CAN_BUFFER_SIZE+1;
}
void CAN2_TxMessage(u8 bExtAddress, u32 can_id, int dataLen, u8 *txBuf)
{
    CanTxMsg CAN2_TxMsg;		//发送帧结构体实例
	u8 TransmitMailbox = 0;
    
    if(dataLen<1) return;
    else if(dataLen>8) dataLen = 8;
	
    if(bExtAddress)
    {
        CAN2_TxMsg.IDE = CAN_ID_EXT;
        CAN2_TxMsg.ExtId = can_id;
    }
    else
    {
        CAN2_TxMsg.IDE = CAN_ID_STD;
    	CAN2_TxMsg.StdId = can_id;
    }
    
	CAN2_TxMsg.RTR = CAN_RTR_DATA;
	CAN2_TxMsg.DLC = dataLen;
	memcpy(CAN2_TxMsg.Data, txBuf, dataLen);
	
	TransmitMailbox = CAN_Transmit(CAN2, &CAN2_TxMsg);
	while(CAN_TransmitStatus(CAN2, TransmitMailbox) != CANTXOK);
}


int CAN_SendData(u8 channel, CanTxMsg* txMsg)
{
    u8 TransmitMailbox = 0;
    u32 errorCounter = 0;
    
    
    if(txMsg->DLC>8) txMsg->DLC = 8;
    
    if(channel ==2)
    {
        TransmitMailbox = CAN_Transmit(CAN2, txMsg);
        errorCounter = 0 ;
    	while(CAN_TransmitStatus(CAN2, TransmitMailbox) != CANTXOK && errorCounter<10000)
            errorCounter ++;
        if(errorCounter>=10000) return 0;
        //CAN2_TxMessage(txMsg->IDE,txMsg->IDE?txMsg->ExtId:txMsg->StdId,txMsg->DLC,txMsg->Data);
    }
    else
    {
        TransmitMailbox = CAN_Transmit(CAN1, txMsg);
        errorCounter = 0 ;
    	while(CAN_TransmitStatus(CAN1, TransmitMailbox) != CANTXOK && errorCounter<10000)
            errorCounter ++;
        if(errorCounter>=10000) return 0;
        //CAN2_TxMessage(txMsg->IDE,txMsg->IDE?txMsg->ExtId:txMsg->StdId,txMsg->DLC,txMsg->Data);
    }
    return 1;
}



CanTxMsg    CAN1_Tx_Buffer[CAN_BUFFER_SIZE];
int         CAN1_Tx_Index = 0;
int         CAN1_TX_Repeat_No = 1;
CanTxMsg    CAN2_Tx_Buffer[CAN_BUFFER_SIZE];
int         CAN2_Tx_Index = 0;
int         CAN2_TX_Repeat_No = 1;

//CAN定时发送的回调函数
void Can1RepeatSendingCallback()
{
    int i;
    int startIndex, curIndex;
    startIndex = (CAN1_Tx_Index+CAN_BUFFER_SIZE+1-CAN1_TX_Repeat_No)%CAN_BUFFER_SIZE;
    for(i=0;i<CAN1_TX_Repeat_No;i++)
    {
        curIndex = (startIndex +i)%CAN_BUFFER_SIZE;
        CAN_SendData(1,&(CAN1_Tx_Buffer[curIndex]));
    }
}

//CAN定时发送的回调函数
void Can2RepeatSendingCallback()
{
    int i;
    int startIndex, curIndex;
    startIndex = (CAN2_Tx_Index+CAN_BUFFER_SIZE+1-CAN2_TX_Repeat_No)%CAN_BUFFER_SIZE;
    for(i=0;i<CAN2_TX_Repeat_No;i++)
    {
        curIndex = (startIndex +i)%CAN_BUFFER_SIZE;
        CAN_SendData(2,&(CAN2_Tx_Buffer[curIndex]));
    }
}



void CANFilterConfig(u8 ch, u8 bEnable,u32 bListMode, u32 canId, u8 bExtId, u8 bRemoteFrame, u32 canIdMask, u8 bMaskExtId, u8 bMaskRemoteFrame)
{
    CAN_FilterInitTypeDef CAN_FilterInitStructure;	//结构体实例
    u32 tmp = 0;
    //过滤第一组ID
    if(ch==1)
    	CAN_FilterInitStructure.CAN_FilterNumber 	 = 0;
    else 
        CAN_FilterInitStructure.CAN_FilterNumber 	 = 14;
    
    if(bEnable)
    {
        if(bListMode)
            CAN_FilterInitStructure.CAN_FilterMode   	 = CAN_FilterMode_IdList;
        else 
            CAN_FilterInitStructure.CAN_FilterMode   	 = CAN_FilterMode_IdMask;
        tmp = 0;
        if(bExtId) tmp |= CAN_ID_EXT;
        else tmp|= CAN_ID_STD;
        if(bRemoteFrame) tmp |= CAN_RTR_REMOTE;
        else tmp|= CAN_RTR_DATA;
        if(bExtId)
        {
            CAN_FilterInitStructure.CAN_FilterIdHigh 	 = ((canId<<3)&0xFFFF0000)>>16;
            CAN_FilterInitStructure.CAN_FilterIdLow 	 = ((canId<<3)|tmp)&0x0000FFFF;//(((u32)CPU_CAN_0<<21)|CAN_ID_STD|CAN_RTR_DATA)&0xFFFF;
        }
        else
        {
            CAN_FilterInitStructure.CAN_FilterIdHigh 	 = ((canId<<21)&0xFFFF0000)>>16;
            CAN_FilterInitStructure.CAN_FilterIdLow 	 = ((canId<<21)|tmp)&0x0000FFFF;//(((u32)CPU_CAN_0<<21)|CAN_ID_STD|CAN_RTR_DATA)&0xFFFF;
        }
        
        tmp = 0;
        if(bMaskExtId) tmp |= CAN_ID_EXT;
        else tmp|= CAN_ID_STD;
        if(bMaskRemoteFrame) tmp |= CAN_RTR_REMOTE;
        else tmp|= CAN_RTR_DATA;
        if(bMaskExtId)
        {
            CAN_FilterInitStructure.CAN_FilterMaskIdHigh 	 = ((canIdMask<<3)&0xFFFF0000)>>16;
            CAN_FilterInitStructure.CAN_FilterMaskIdLow 	 = ((canIdMask<<3)|tmp)&0x0000FFFF;//(((u32)CPU_CAN_0<<21)|CAN_ID_STD|CAN_RTR_DATA)&0xFFFF;
        }
        else
        {
            CAN_FilterInitStructure.CAN_FilterMaskIdHigh 	 = ((canIdMask<<21)&0xFFFF0000)>>16;
            CAN_FilterInitStructure.CAN_FilterMaskIdLow 	 = ((canIdMask<<21)|tmp)&0x0000FFFF;//(((u32)CPU_CAN_0<<21)|CAN_ID_STD|CAN_RTR_DATA)&0xFFFF;
        }
    }
    else
    {
        CAN_FilterInitStructure.CAN_FilterMode   	 = CAN_FilterMode_IdMask;
        CAN_FilterInitStructure.CAN_FilterIdHigh 	 = 0;//(((u32)CPU_CAN_0<<21)&0xFFFF0000)>>16;
        CAN_FilterInitStructure.CAN_FilterIdLow 	 = 0;//(((u32)CPU_CAN_0<<21)|CAN_ID_STD|CAN_RTR_DATA)&0xFFFF;
        CAN_FilterInitStructure.CAN_FilterMaskIdHigh 	 = 0;//0xFFFF;
        CAN_FilterInitStructure.CAN_FilterMaskIdLow  	 = 0;//0xFFFF;
    }
	CAN_FilterInitStructure.CAN_FilterScale  	 = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation 	 = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
}

void ClearCanRxBuffer(u8 channel)
{
    if(channel == 1)
    {
        CAN1_Rx_Start_Index=CAN1_Rx_End_Index;
        ClearTFCEvent(EVENT_CAN1);
    }
    else 
    {
        CAN2_Rx_Start_Index=CAN2_Rx_End_Index;
        ClearTFCEvent(EVENT_CAN2);
    }
    
}