#include "includes.h"

//uint8_t testStr[] = { 0x00,0x00 }; // 测试数据



/*u8  CheckCrc(u8* dat, int len)
{
    u8 crc = 0xff, i, j;
    for (i = 0; i < len ; i++)
    {
        crc = crc ^ dat[i];
        for (j = 0; j < 8; j++)
        {
            if (crc & 0x01) crc = (crc >> 1) ^ 0x8C;
            else crc >>= 1;
        }
    }
    return crc;
} 
*/
u8  CheckCrc(u8* dat, int len)
{
    u8 crc = 0xff, i, j;
    u8 temp = 0;
    for (i = 0; i < len ; i++)
    {
        for (j = 0; j < 8; j++)
        {
            temp = ((dat[i]>>j)&0x01)^(crc>>7);
            crc <<= 1;
            if(temp) crc ^= 0x31 ;
        }
    }
    return crc;
} 


int ledDriverRegRead(u8 dev, u8 regAddr, u8 length, u8* readbackData)
{
    u8 outBuf[4] = {0x55, 0x00, 0x00, 0x00};
    u8 inBuf[UART5_BUFFER_SIZE];
    int inBufLen = 0;
    
    if(dev) dev =1;
    if(length<2) length = 1;
    else if(length<4) length = 2;
    else if(length<8) length = 4;
    else length = 8;
    
    if(length==1) outBuf[1]=0x00;
    else if(length ==2) outBuf[1]=0x10;
    else if(length ==4) outBuf[1]=0x20;
    else outBuf[1]=0x30;
    
    outBuf[2]=regAddr;
    
    outBuf[3]= CheckCrc(outBuf+1,2);
    
    if(!dev) 
    {
        ClearUart5RxBuffer();
        SendData(5,(char*)outBuf,4);
    }
    else 
    {
        ClearUart2RxBuffer();
        SendData(2,(char*)outBuf,4);
    }
    
    DelayUs(125*(length+1));//每个字节大约100us，预留25%的时间
    
    if(!dev) inBufLen=GetUart5RxStatus((char*)inBuf);
    else inBufLen=GetUart2RxStatus((char*)inBuf);
    
    if(inBufLen)
    {
        if(inBuf[inBufLen-1]==CheckCrc(inBuf,inBufLen-1))
        {
            memcpy(readbackData,inBuf,length);
            return inBufLen-1;
        }
        else//CRC 校验错误
        {
            return 0;
        }
    }
    else return 0;//请求读寄存器结果没收到回复
}


//返回值为flag0的信息，如果返回成功的flag0为0x00，则自动修改为0x80。
//返回值为0时表示发送消息失败，存在故障，此时返回值0x00不是FLGA0的真实值
u8 ledDriverRegWrite(u8 dev, u8 regAddr, u8 length, u8* writeData)
{
    u8 outBuf[12] = {0x55};
    u8 inBuf[UART5_BUFFER_SIZE];
    int inBufLen = 0;
    int i;
    
    if(dev) dev =1;
    if(length<2) length = 1;
    else if(length<4) length = 2;
    else if(length<8) length = 4;
    else length = 8;
    
    if(length==1) outBuf[1]=0x80;
    else if(length ==2) outBuf[1]=0x90;
    else if(length ==4) outBuf[1]=0xA0;
    else outBuf[1]=0xB0;
    
    outBuf[2]=regAddr;
    
    for(i=0;i<length;i++)
    {
        outBuf[3+i]= writeData[i];
    }
    
    outBuf[3+length]=CheckCrc(outBuf+1,2+length);
    
    if(!dev) 
    {
        ClearUart5RxBuffer();
        SendData(5,(char*)outBuf,4+length);
    }
    else 
    {
        ClearUart2RxBuffer();
        SendData(2,(char*)outBuf,4+length);
    }
    
    DelayUs(250);//每个字节大约100us，预留25%的时间
    
    if(!dev) inBufLen=GetUart5RxStatus((char*)inBuf);
    else inBufLen=GetUart2RxStatus((char*)inBuf);
    
    if(inBufLen)
    {
        if(inBuf[inBufLen-1]==CheckCrc(inBuf,inBufLen-1))
        {
            if(!inBuf[0]) inBuf[0]=0x80;//返回值为flag0的信息，如果返回成功的flag0为0x00，则自动修改为0x80。
            return inBuf[0];
        }
        else//CRC 校验错误
        {
            return 0;
        }
    }
    else return 0;//请求读寄存器结果没收到回复
}

static u8 DoPwmUsingStatus = 0;
void DoUnprotectedPwmModeInit()//PE9 ch0 TIM1_CH1, PE11 ch1 TIM1_CH2
{
    u16 Frequency=1000;
    u16 pwmDuty=500;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOE, ENABLE);
   
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
	
	u16 Pwmpulse,PwmPeriod;
	
	PwmPeriod = 1000000/Frequency;
	Pwmpulse = (PwmPeriod+1)*pwmDuty/1000ul;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = PwmPeriod;
	TIM_TimeBaseStructure.TIM_Prescaler = 71;	//每计一个数为1us
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	//Channel 1
	TIM_OCInitStructure.TIM_Pulse = Pwmpulse;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, DISABLE);	//预装载寄存器的内容被立即传送到影子寄存器
	TIM_Cmd(TIM1, DISABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//	高级定时器TIM1和TIM8 需要加本句话才可以输出PWM
}
//CH24（DRV-0）不支持PWM模式。
//CH25（DRV-1）的PWM的占空比只能为50%。
//DO ch25（DRV-1），一旦开启PWM模式，Beep以及Music功能随即失效。
int DoUnprotectedPwmModeWrite(float fFreq, u32 pwmDuty)//PE9 ch0 TIM1_CH1, PE11 ch1 TIM1_CH2
{
    u16 Pwmpulse,PwmPeriod;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    DoPwmUsingStatus = 1;
    BeepEnable(0);
    
    if(pwmDuty<1) pwmDuty =1 ;
    else if(pwmDuty>999) pwmDuty = 999;
    
    //pwmDuty = 1000-pwmDuty;//反向
    
    if(fFreq>200000)
    {
        PwmPeriod = (u16)(72000000/fFreq);
	    Pwmpulse = (PwmPeriod+1ul)/2ul;
        TIM_TimeBaseStructure.TIM_Prescaler = 0;	//8M时钟在跑  72/(8+1)
    }
	if(fFreq>=130)
    {
        PwmPeriod = (u16)(8000000/fFreq);
	    Pwmpulse = (PwmPeriod+1ul)/2ul;
        TIM_TimeBaseStructure.TIM_Prescaler = 8;	//8M时钟在跑  72/(8+1)
    }
    else
    {
        PwmPeriod = (u16)(20000/fFreq);
	    Pwmpulse = (PwmPeriod+1ul)/2ul;
        TIM_TimeBaseStructure.TIM_Prescaler = 3600;	//8M时钟在跑  72/(8+1)
    }
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOE, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
	
	
	TIM_TimeBaseStructure.TIM_Period = PwmPeriod;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	//Channel 1
	TIM_OCInitStructure.TIM_Pulse = Pwmpulse;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    //TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    
	
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    //TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    
	TIM_ARRPreloadConfig(TIM1, DISABLE);	//预装载寄存器的内容被立即传送到影子寄存器
	TIM_Cmd(TIM1, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//	高级定时器TIM1和TIM8 需要加本句话才可以输出PWM
 
    return 1;
}
/*
void DoUnprotectedPwmModeInit()//PE9 ch0 TIM1_CH1, PE11 ch1 TIM1_CH2
{
    u16 Frequency=1000;
    u16 pwmDuty=200;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOE, ENABLE);
    
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	u16 Pwmpulse,PwmPeriod;
	
	PwmPeriod = 8000000ul/Frequency;
	Pwmpulse = (PwmPeriod+1)*pwmDuty/1000ul;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = PwmPeriod;
	TIM_TimeBaseStructure.TIM_Prescaler = 8;	//每计一个数为1us
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = Pwmpulse;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, DISABLE);	//预装载寄存器的内容被立即传送到影子寄存器
    TIM_CtrlPWMOutputs(TIM1, ENABLE);//	高级定时器TIM1和TIM8 需要加本句话才可以输出PWM
    
	TIM_Cmd(TIM1, ENABLE);
}
*/
//fFreq 真实频率，范围0.2-1000 000， 0.2Hz-1MHz
//pwmDuty,占空比，从1-999，10表示占空比1%
/*
void DoUnprotectedPwmModeWrite(u8 ch, float fFreq, u32 pwmDuty)//PE9 ch0 TIM1_CH1, PE11 ch1 TIM1_CH2
{
    u16 Pwmpulse,PwmPeriod;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    if(!ch) DoPwmUsingStatus |= 0x01;
    else DoPwmUsingStatus |= 0x02;
    if(DoPwmUsingStatus) BeepEnable(0);
    else BeepEnable(1);
    
    if(pwmDuty<1) pwmDuty =1 ;
    else if(pwmDuty>999) pwmDuty = 999;
    
    //pwmDuty = 1000-pwmDuty;//反向
    
    
    
	if(fFreq>=200)
    {
        PwmPeriod = (u16)(8000000/fFreq);
	    Pwmpulse = (PwmPeriod+1)*pwmDuty/1000ul;
        TIM_TimeBaseStructure.TIM_Prescaler = 8;	//8M时钟在跑  72/(8+1)
    }
    else
    {
        PwmPeriod = (u16)(10000/fFreq);
	    Pwmpulse = (PwmPeriod+1)*pwmDuty/1000ul;
        TIM_TimeBaseStructure.TIM_Prescaler = 7200;	//8M时钟在跑  72/(8+1)
    }
	
	TIM_TimeBaseStructure.TIM_Period = PwmPeriod;	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = Pwmpulse;
	if(ch) TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    else TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    
    TIM_CtrlPWMOutputs(TIM1, ENABLE);//	高级定时器TIM1和TIM8 需要加本句话才可以输出PWM
	
	if(ch) TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    else TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    
	TIM_ARRPreloadConfig(TIM1, DISABLE);	//预装载寄存器的内容被立即传送到影子寄存器
	TIM_Cmd(TIM1, ENABLE);
	
    GPIO_InitTypeDef GPIO_InitStructure;
    if(ch) GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
    else GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
*/
int  DoUnprotectedIoModeWrite(u8 ch, u8 level)//PE9 ch0 TIM1_CH1, PE11 ch1 TIM1_CH2
{
    if(level) level = 0;
    else level = 1;
    
    if(ch) DoPwmUsingStatus =0;
    if(DoPwmUsingStatus) BeepEnable(0);
    else BeepEnable(1);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    if(ch) GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
    else GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
    
    if(ch) GPIO_WriteBit(GPIOE, GPIO_Pin_11, (BitAction)level);
    else GPIO_WriteBit(GPIOE, GPIO_Pin_9, (BitAction)level);
    
    return 1;
}

int DoOcpWrite(int ch, int ocp)
{
    int i;
    u8 outBuf[8];
    int retVal = 1;
    int dev = ch/12;
    
    u8 charOCP = (u8)(ocp/77.11*64-1);
    if(charOCP>0x3F) charOCP = 0x3F;
    
    //write CONF_IOUT0 to CONF_IOUT7 as 0x3F
    for(i=0;i<8;i++)
    {
        outBuf[i]=charOCP;
    }
    retVal*= ledDriverRegWrite(dev,0x00,8,outBuf);
    retVal*= ledDriverRegWrite(dev,0x08,4,outBuf);
    return retVal;
}

int DoFreqWrite(int ch, int freq)
{
    u8 outBuf[8];
    int retVal = 1;
    
    int dev = ch/12;
    ch = ch%12;
    
    //freq
    if(freq<225) outBuf[0]=0x03;
    else if(freq<275) outBuf[0]=0x13;
    else if(freq<325) outBuf[0]=0x23;
    else if(freq<375) outBuf[0]=0x33;
    else if(freq<450) outBuf[0]=0x43;
    else if(freq<550) outBuf[0]=0x53;
    else if(freq<700) outBuf[0]=0x63;
    else if(freq<900) outBuf[0]=0x73;
    else if(freq<1100) outBuf[0]=0x83;
    else if(freq<1600) outBuf[0]=0x93;
    else if(freq<3000) outBuf[0]=0xa3;
    else if(freq<5000) outBuf[0]=0xb3;
    else if(freq<7000) outBuf[0]=0xc3;
    else if(freq<9000) outBuf[0]=0xd3;
    else if(freq<15000) outBuf[0]=0xe3;
    else  outBuf[0]=0xf3;
    retVal*= ledDriverRegWrite(dev,0x57,1,outBuf);
    
    return retVal;
}

int DoPwmModeWrite(int ch, int duty)
{
    u8 outBuf[8];
    u8 inBuf[8];
    int retVal = 1;
    
    int dev = ch/12;
    ch = ch%12;
    
    /*
    //freq
    if(freq<225) outBuf[0]=0x03;
    else if(freq<275) outBuf[0]=0x13;
    else if(freq<325) outBuf[0]=0x23;
    else if(freq<375) outBuf[0]=0x33;
    else if(freq<450) outBuf[0]=0x43;
    else if(freq<550) outBuf[0]=0x53;
    else if(freq<700) outBuf[0]=0x63;
    else if(freq<900) outBuf[0]=0x73;
    else if(freq<1100) outBuf[0]=0x83;
    else if(freq<1600) outBuf[0]=0x93;
    else if(freq<3000) outBuf[0]=0xa3;
    else if(freq<5000) outBuf[0]=0xb3;
    else if(freq<7000) outBuf[0]=0xc3;
    else if(freq<9000) outBuf[0]=0xd3;
    else if(freq<15000) outBuf[0]=0xe3;
    else  outBuf[0]=0xf3;
    retVal*= ledDriverRegWrite(dev,0x57,1,outBuf);
    */
    
    //duty
    if(duty>99) duty =99;
    else if(duty<1) duty = 1;
    outBuf[0] = (u8)(duty/100.0*256);
    retVal*= ledDriverRegWrite(dev,0x20+ch,1,outBuf);
    
    //enable the channel
    int sector = ch/8;
    int chBit = ch%8;
    retVal*= ledDriverRegRead(dev,0x50+sector,1,inBuf);
    inBuf[0] |= (0x01<<chBit);
    retVal*= ledDriverRegWrite(dev,0x50+sector,1,inBuf);

    return retVal;
}

int DoIoWrite(int ch, int val)
{
    u8 inBuf[8];
    u8 outBuf[8];
    int dev = ch/12;
    ch = ch%12;
    int sector = ch/8;
    int chBit = ch%8;
    int retVal = 1;
    
    //enable the channel
    retVal*= ledDriverRegRead(dev,0x50+sector,1,inBuf);
    if(val)
    {
        outBuf[0] = 0xff;
        retVal*= ledDriverRegWrite(dev,0x20+ch,1,outBuf);
        inBuf[0] |= (0x01<<chBit);
    }
    else 
        inBuf[0] &= (~(0x01<<chBit));
    retVal*= ledDriverRegWrite(dev,0x50+sector,1,inBuf);
    
    return retVal;
}
//int ledDriverRegRead(u8 ch, u8 regAddr, u8 length, u8* readbackData);
//u8 ledDriverRegWrite(u8 ch, u8 regAddr, u8 length, u8* writeData);
int DoLedDriverInit()
{
    u8 outBuf[8];
    int retVal=0;
    int dev=0;
    int i;
    
    //write 0x00 to 0x61h, unlock device 0
    outBuf[0]=0x00;
    retVal = ledDriverRegWrite(0,0x61,1,outBuf);
    if(!retVal) return 0;
    
    retVal = ledDriverRegWrite(1,0x61,1,outBuf);
    if(!retVal) return 0;
    
    while(dev<=1)
    {
        //write 0x03 to 0x60h, Clear POR, FAULT for device 0
        //outBuf[0]= 0x17;
        outBuf[0]= 0x17;
        retVal*= ledDriverRegWrite(dev,0x60,1,outBuf);
        DelayMs(100);
        
        //unlock again
        outBuf[0]=0x00;
        retVal*= ledDriverRegWrite(dev,0x61,1,outBuf);
        
        //write 0x83 to 0x57h, set PWMFREQ=1000Hz, REFRANGE=512 for device 0
        outBuf[0]=0x83;
        retVal*= ledDriverRegWrite(dev,0x57,1,outBuf);
        
        //write CONF_IOUT0 to CONF_IOUT7 as 0x3F
        for(i=0;i<8;i++)
        {
            outBuf[i]=0x3F;
        }
        retVal*= ledDriverRegWrite(dev,0x00,8,outBuf);
        retVal*= ledDriverRegWrite(dev,0x08,4,outBuf);
        
        //write CONF_PWMLOWOUT0 to CONF_PWMLOWOUT7 as dataLow
        for(i=0;i<8;i++)
        {
            outBuf[i]=0x0F;   
        }
        retVal*= ledDriverRegWrite(dev,0x40,8,outBuf);
        retVal*= ledDriverRegWrite(dev,0x48,4,outBuf);
        for(i=0;i<8;i++)
        {
            outBuf[i]=0x00;   
        }
        retVal*= ledDriverRegWrite(dev,0x20,8,outBuf);
        retVal*= ledDriverRegWrite(dev,0x28,4,outBuf);
        
        //shutdown all the channels.
        outBuf[0]=0;
        retVal*= ledDriverRegWrite(dev,0x50,2,outBuf);
        
        //enable short detection only
        outBuf[0]=0xE8;
        retVal*= ledDriverRegWrite(dev,0x64,1,outBuf);
        
        dev++;
    }
    return retVal;
}
void DoInit()
{
    //DoUnprotectedPwmModeInit();
    DoLedDriverInit();
}

int ClearDoErrorFlag()
{
    u8 outBuf[2];
    outBuf[0]=0x03;
    ledDriverRegWrite(0,0x60,1,outBuf);
    ledDriverRegWrite(1,0x60,1,outBuf);
    return 1;
}

int ShortCircuitDetect(u32* sf)
{
    u8 inBuf[8];
    u32 scFlag = 0;
    int retVal = 1;
    if(ledDriverRegRead(0,0x00,1,inBuf))
    {
        if(inBuf[0]&0x01)
        {
            if(ledDriverRegRead(0,0x7d,2,inBuf))
            {
                scFlag = inBuf[0] + (inBuf[1]<<8);
            }
            else 
            {
                retVal = 0;
            }
        }
    }
    else 
    {
        retVal = 0;
    }
    if(ledDriverRegRead(1,0x70,1,inBuf))
    {
        if(inBuf[0]&0x01)
        {
            if(ledDriverRegRead(1,0x7d,2,inBuf))
            {
                scFlag += (inBuf[0]<<12) + (inBuf[1]<<20);
            }
            else 
            {
                retVal = 0;
            }
        }
    }
    else 
    {
        retVal = 0;
    }
    ClearDoErrorFlag();
    if(retVal) *sf = scFlag;
    return retVal;
}
/*
CH:
0x00    REF
0x01    SUPPLY
0x02    VLDO
0x03    TEMPSNS
0x04    IREF
0x05    MAXOUT
0x10    CH0
0x11    CH1
...
0x1B    CH11
*/
int GetDoADCVal(u8 dev, u8 ch,float* result)
{
    u8 outBuf[8];
    u8 inBuf[8];
    int retVal = 1;
    int errorCount = 0;
    if(dev) dev =1 ;
    if(ch>5&& ch<0x10) ch = 0x10;
    else if(ch>0x1B)ch =0x1B;
    
    outBuf[0]=ch;
    retVal *= ledDriverRegWrite(dev,0x62,1,outBuf);
    errorCount = 0;
    while(retVal *= ledDriverRegRead(dev,0x71,1,inBuf))
    {
        if(inBuf[0]&0x04) break;
        errorCount++;
        if(errorCount>100) break; 
    }
    if(errorCount>100)
    {
        return 0;
    }
    retVal *= ledDriverRegRead(dev,0x73,1,inBuf);
    switch(ch)
    {
    case 0:
        *result = 0.0101*inBuf[0]+0.007;
        break;
    case 1:
        *result = 0.0804*inBuf[0]+0.0673;
        break;
    case 2:
        *result = 0.022*inBuf[0]+0.0465;
        break;
    case 3:
        *result = 2.152*inBuf[0]-242.35;
        break;
    case 4:
        *result = 0.7461*inBuf[0]+0.7592;
        break;
    case 5:
        *result = 0.0804*inBuf[0]+0.0673;
        break;
    default:
        *result = 0.0804*inBuf[0]+0.0673;
        break;
    }
    return retVal;
}

u8 DoSelfTest()
{
    float temp;
    
    if(GetDoADCVal(0,0,&temp))
    {
        if(temp>1.235*0.9 && temp<1.235*1.1)
        {}
        else
        {
            debug("DO channel 0-11 REF voltage error.\r\n");
            return 0;
        }
    }
    else
    {
        debug("DO channel 0-11 can't be accessed.\r\n");
        return 0;
    }
    
    if(GetDoADCVal(1,0,&temp))
    {
        if(temp>1.235*0.9 && temp<1.235*1.1)
        {}
        else
        {
            debug("DO channel 12-23 REF voltage error.\r\n");
            return 0;
        }
    }
    else
    {
        debug("DO channel 12-23 can't be accessed.\r\n");
        return 0;
    }
    
    if(GetDoADCVal(0,1,&temp))
    {
        if((temp>12*0.9 && temp<12*1.1) || (temp>24*0.9 && temp<24*1.1))
        {}
        else
        {
            debug("Strange 12V/24V supply voltage:%fV.\r\n",temp);
        }
    }
    else
    {
        debug("DO channel 0-11 can't be accessed.\r\n");
        return 0;
    }
    
    if(GetDoADCVal(0,2,&temp))
    {
        if(temp>5*0.9 && temp<5*1.1)
        {}
        else
        {
            debug("DO channel 0-11 REF voltage error.\r\n");
            return 0;
        }
    }
    else
    {
        debug("DO channel 0-11 can't be accessed.\r\n");
        return 0;
    }
    
    if(GetDoADCVal(1,2,&temp))
    {
        if(temp>5*0.9 && temp<5*1.1)
        {}
        else
        {
            debug("DO channel 12-23 REF voltage error.\r\n");
            return 0;
        }
    }
    else
    {
        debug("DO channel 12-23 can't be accessed.\r\n");
        return 0;
    }
    return 1;
}