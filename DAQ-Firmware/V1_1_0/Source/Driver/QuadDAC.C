

#include "includes.h"



void I2C1_Init(int dataRate)//PB7 SDA, PB6 SCL
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    
    /* 使能与 I2C1 有关的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  
    
    //GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // 开漏输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    I2C_InitTypeDef  I2C_InitStructure; 
    /* I2C 配置 */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 =0x60; 
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = dataRate;
    
    /* 使能 I2C1 */
    I2C_Cmd(I2C1, ENABLE);    
    /* I2C1 初始化 */
    I2C_Init(I2C1, &I2C_InitStructure);
}


/*********************************************************************************************************
功能 ：Tim2,用作5ms中断，读取AD,XY轴
**********************************************************************************************************/
void Timer2Init(void)
{
    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_TimeBaseStructure.TIM_Period = 1000000/AO_DAC_SPS;
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    
    TIM_ARRPreloadConfig(TIM2, DISABLE);
    
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

int I2C1_ByteWriteNoAck(u8 devAddr, u8 pBuffer, u8 WriteAddr)
{
    /* Send STRAT condition */
    u32 index = 0;
    I2C_GenerateSTART(I2C1, ENABLE);
    
    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) && index <1000)
    {
        index ++;
    }
    
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Transmitter);
    
    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && index <2000)
    {
        index ++;
    }
    
    
    
    /* Send the byte to be written */
    I2C_SendData(I2C1, WriteAddr); 
    
    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) && index <3000)
    {
        index++;
    }
    
    /* Send the byte to be written */
    I2C_SendData(I2C1, pBuffer); 
    
    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)&& index <4000)
    {
        index++;
    }
    
    /* Send STOP condition */
    I2C_GenerateSTOP(I2C1, ENABLE);
    
    return 1;
}


int I2C1_ByteWrite(u8 devAddr, u8 pBuffer, u8 WriteAddr)
{
    /* Send STRAT condition */
    u32 index = 0;
    I2C_GenerateSTART(I2C1, ENABLE);
    
    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) && index <1000)
    {
        index ++;
    }
    
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Transmitter);
    
    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && index <2000)
    {
        index ++;
    }
    
    if(index >= 2000)
    {
        return 0;
    }
    
    /* Send the byte to be written */
    I2C_SendData(I2C1, WriteAddr); 
    
    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    /* Send the byte to be written */
    I2C_SendData(I2C1, pBuffer); 
    
    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    /* Send STOP condition */
    I2C_GenerateSTOP(I2C1, ENABLE);
    
    return 1;
}





int I2C1_BufferWrite(u8 devAddr, u8* buffer, u8 WriteAddr, int numToWrite)
{
    u8* pBuffer = buffer;
    u32 index = 0;
    /* Send STRAT condition */
    I2C_GenerateSTART(I2C1, ENABLE);
    
    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) && index <1000)
    {
        index ++;
    }
    
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Transmitter);
    
    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && index <2000)
    {
        index ++;
    }
    
    if(index >= 2000)
    {
        return 0;
    }
    
    /* Send the EEPROM's internal address to write to */
    I2C_SendData(I2C1, WriteAddr);
    
    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    while(numToWrite)
    {
        /* Send the byte to be written */
        I2C_SendData(I2C1, *pBuffer); 
        
        /* Test on EV8 and clear it */
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
        
        numToWrite--;
        pBuffer ++;
    }
    
    /* Send STOP condition */
    I2C_GenerateSTOP(I2C1, ENABLE);
    
    return 1;
}



int I2C1_ByteRead(u8 devAddr, u8* buffer, u8 ReadAddr)
{  
    int NumByteToRead = 1;
    u8* pBuffer = buffer;
    u32 index = 0;
    
    /* Send STRAT condition */
    I2C_GenerateSTART(I2C1, ENABLE);
    
    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) && index <1000)
    {
        index ++;
    }
    
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Transmitter);
    
    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && index <2000)
    {
        index ++;
    }
    
    if(index >= 2000)
    {
        return 0;
    }
    
    /* Send the byte to be written */
    I2C_SendData(I2C1, ReadAddr); 
    
    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    //No stop signal when in Repeat Start mode
    /* Send STOP condition */
    //I2C_GenerateSTOP(I2C1, ENABLE);
    
    
    ////////////////////////////////////////////////////////////////////
    //*((u8 *)0x4001080c) |=0x80; 
    //while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008
    
    
    /* Send STRAT condition a second time */  
    I2C_GenerateSTART(I2C1, ENABLE);
    
    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Receiver);
    
    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    /* While there is data to be read */
    while(NumByteToRead)  
    {
        if(NumByteToRead == 1)
        {
            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(I2C1, DISABLE);
            
            /* Send STOP Condition */
            I2C_GenerateSTOP(I2C1, ENABLE);
        }
        
        /* Test on EV7 and clear it */
        if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
        {      
            /* Read a byte from the EEPROM */
            *pBuffer = I2C_ReceiveData(I2C1);
            
            /* Point to the next location where the byte read will be saved */
            pBuffer++; 
            
            /* Decrement the read bytes counter */
            NumByteToRead--;        
        }   
    }
    
    
    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    
    return 1;
}

//I2C1_BufferWrite(u8 devAddr, u8* buffer, u8 WriteAddr, int numToWrite)
int  I2C1_BufferRead(u8 devAddr, u8* writeBuffer, u8 ReadAddr, int numToWrite, int NumByteToRead, u8* readBuffer)
{  
    u8* pBuffer = writeBuffer;
    u32 index = 0;
    /* Send STRAT condition */
    I2C_GenerateSTART(I2C1, ENABLE);
    
    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)&& index <1000)
    {
        index ++;
    }
    
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Transmitter);
    
    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)&& index <2000)
    {
        index ++;
    }
    
    if(index >= 2000)
    {
        return 0;
    }
    
    /* Send the EEPROM's internal address to write to */
    I2C_SendData(I2C1, ReadAddr);
    
    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    while(numToWrite)
    {
        /* Send the byte to be written */
        I2C_SendData(I2C1, *pBuffer); 
        
        /* Test on EV8 and clear it */
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
        
        numToWrite--;
        pBuffer ++;
    }
    
    
    ////////////////////////////////////////////////////////////////////
    //*((u8 *)0x4001080c) |=0x80; 
    //while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008
    
    pBuffer = readBuffer;
    /* Send STRAT condition a second time */  
    I2C_GenerateSTART(I2C1, ENABLE);
    
    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Receiver);
    
    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    /* While there is data to be read */
    while(NumByteToRead)  
    {
        if(NumByteToRead == 1)
        {
            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(I2C1, DISABLE);
            
            /* Send STOP Condition */
            I2C_GenerateSTOP(I2C1, ENABLE);
        }
        
        /* Test on EV7 and clear it */
        if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
        {      
            /* Read a byte from the EEPROM */
            *pBuffer = I2C_ReceiveData(I2C1);
            
            /* Point to the next location where the byte read will be saved */
            pBuffer++; 
            
            /* Decrement the read bytes counter */
            NumByteToRead--;        
        }   
    }
    
    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    
    return 1;
}
#if 0
void I2C1_BufferRead(u8 devAddr, u8* buffer, u8 ReadAddr, int NumByteToRead)
{  
    u8* pBuffer = buffer;
    
    /* Send STRAT condition */
    I2C_GenerateSTART(I2C1, ENABLE);
    
    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));  
    
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Transmitter);
    
    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    /* Send the byte to be written */
    I2C_SendData(I2C1, ReadAddr); 
    
    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    
    //No stop signal when in Repeat Start mode
    /* Send STOP condition */
    //I2C_GenerateSTOP(I2C1, ENABLE);
    
    
    ////////////////////////////////////////////////////////////////////
    //*((u8 *)0x4001080c) |=0x80; 
    //while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008
    
    
    /* Send STRAT condition a second time */  
    I2C_GenerateSTART(I2C1, ENABLE);
    
    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Receiver);
    
    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    /* While there is data to be read */
    while(NumByteToRead)  
    {
        if(NumByteToRead == 1)
        {
            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(I2C1, DISABLE);
            
            /* Send STOP Condition */
            I2C_GenerateSTOP(I2C1, ENABLE);
        }
        
        /* Test on EV7 and clear it */
        if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
        {      
            /* Read a byte from the EEPROM */
            *pBuffer = I2C_ReceiveData(I2C1);
            
            /* Point to the next location where the byte read will be saved */
            pBuffer++; 
            
            /* Decrement the read bytes counter */
            NumByteToRead--;        
        }   
    }
    
    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2C1, ENABLE);
}
#endif

int SetAoPowerState(u8 bOn)
{
    if(!bOn)
    {
        GPIO_WriteBit(GPIOC, GPIO_Pin_6, (BitAction)1);
    }
    else 
    {
        GPIO_WriteBit(GPIOC, GPIO_Pin_6, (BitAction)0);
    }
    return 1;
}

int SetAoChannelVol(u8 ch, u32 vol)
{
//    void I2C1_BufferWrite(u8 devAddr, u8* buffer, u16 WriteAddr, int numToWrite)
    int retVal = 0;
    
    u8 buffer[3];
    
    u16 vcode = (u16)(4010.132-0.111084*vol);
    
    buffer[0] = 0x58 | ((ch&0x03)<<1);
    buffer[1] = 0x80 | ((vcode&0x0F00)>>8);
    buffer[2] = vcode & 0x00FF;
    
    retVal = I2C1_BufferWrite(0xC0,buffer+1,buffer[0],2);

    AoDdsParam[ch&0x03].bEnable = 0;
    
    return retVal;
}

AoDDSType AoDdsParam[4];
/*
typedef struct
{
    u16 LUT[100];
    u8 lenLUT;
    float frequency;
    float step;//重要
    u8 curIndex;
    float actualIndex;
    float vpp;
    float voffset;
    u8 bEnable;
}AoDDSType;
*/

void AoInit()
{
    int i,j;
    
    Timer2Init();//AO DAC
    Timer3Init(20000);//AO Supply voltage
    for(i=0;i<4;i++)
    {
        SetAoSupplyVol(i,20000);
        if(!SetAoChannelVol(i,0))
            debug("AO I2C Comm error!\r\n");
    }
    SetAoPowerState(1);
    
    //配置参数初始化
    for(i=0;i<4;i++)
    {
        for(j=0;j<100;j++)
            AoDdsParam[i].LUT[j]=0;
        AoDdsParam[i].lenLUT =0;
        AoDdsParam[i].frequency =1 ;
        AoDdsParam[i].step = 1;
        AoDdsParam[i].curIndex = 0;
        AoDdsParam[i].actualIndex = 0;
        AoDdsParam[i].vpp = 0;
        AoDdsParam[i].voffset = 0;
        AoDdsParam[i].bEnable = 0;
    }
}



/*********************************************************************************************************
功能 ：TIM2 5ms
**********************************************************************************************************/
void TIM2_IRQHandler(void)
{
    if((TIM2->SR&TIM_IT_Update) && (TIM2->DIER&TIM_IT_Update))
    {
        //TIM2->CR1 &= 0x03FE;//关定时器
        TIM2->SR = 0xFFFE;	//TIM_ClearITPendingBit(TIM3, TIM_IT_Update);清除中断标志
        TIM2->CNT = 0;		//定时器CNT清零
        
        int i;
        int bNeedToWriteFlag, buffer_index;
        bNeedToWriteFlag = 0;
        buffer_index = 0;
        u8 buffer[13];
        for(i=0;i<4;i++)
        {
            if(AoDdsParam[i].bEnable)
            {
                if(AoDdsParam[i].lenLUT>0)
                {
                    bNeedToWriteFlag = 1;
                    
                    AoDdsParam[i].curIndex = AoDdsParam[i].curIndex%AoDdsParam[i].lenLUT;
                    u16 vcode = (u16)(4010.13184-0.000027120113*(AoDdsParam[i].LUT[AoDdsParam[i].curIndex]-2048)*(float)AoDdsParam[i].vpp-0.111083984*AoDdsParam[i].voffset);
                    if(vcode>4010) vcode = 4010;
                    if(vcode<677) vcode = 677;
                    
                    buffer[buffer_index++] = 0x40 | ((i&0x03)<<1);
                    buffer[buffer_index++] = 0x80 | ((vcode&0x0F00)>>8);
                    buffer[buffer_index++] = vcode & 0x00FF;
                    
                    
                    
                    AoDdsParam[i].actualIndex += AoDdsParam[i].step;
                    if(AoDdsParam[i].actualIndex>AoDdsParam[i].lenLUT) AoDdsParam[i].actualIndex -= AoDdsParam[i].lenLUT;
                    if(AoDdsParam[i].actualIndex<0) AoDdsParam[i].actualIndex = 0;
                    AoDdsParam[i].curIndex = (u8)AoDdsParam[i].actualIndex;
                }
            }
        }
        if(bNeedToWriteFlag)
            I2C1_BufferWrite(0xC0,buffer+1,buffer[0],buffer_index-1);
        
        //TIM2->CR1 |= 0x0001;	//使能TIM2
    }
}


u16 gTimer3Arr;

void Timer3Init(u16 workingFreq)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    gTimer3Arr=72000000/workingFreq-1;		//不分频。
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);// 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);  //使能GPIO外设时钟使能
    
    //设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ; //TIM_CH1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
    
    TIM_TimeBaseStructure.TIM_Period = gTimer3Arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值 
    TIM_TimeBaseStructure.TIM_Prescaler =0; //设置用来作为TIMx时钟频率除数的预分频值  不分频
    TIM_TimeBaseStructure.TIM_ClockDivision =0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    //TIM3通道二
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState =TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1;		
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    SetAoVoltageCtrlDuty(0);
    
    TIM_CtrlPWMOutputs(TIM3,ENABLE);        //MOE 主输出使能        
    TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器
    TIM_Cmd(TIM3, ENABLE);  //使能TIM1
}


void SetAoVoltageCtrlDuty(u16 duty)
{
    TIM3->CCR2=gTimer3Arr-(duty*gTimer3Arr/100);
}


void SetAoSupplyVol(u8 ch, u32 vol)
{
    static u32 supplyVol[4] ={0,0,0,0};
    
    if(vol<=0)  vol = 0;
    else if(vol>34412) vol = 34412;
    
    if(ch>3) ch = 3;
    supplyVol[ch] = vol;
    
    u32 maxVol = GetIntArrayMax(supplyVol,4);
    
    u16 duty = (u16)(100*maxVol/34412);
    if(duty>100) duty = 100;
    SetAoVoltageCtrlDuty(duty);
}

