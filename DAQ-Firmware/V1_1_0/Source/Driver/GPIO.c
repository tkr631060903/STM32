#include "includes.h"


/**************************************************************************************************************
函数描述 : GPIO初始化
***************************************************************************************************************/
void GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//开启全部IO时钟信号
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | 
                           RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF 
                               | RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, 
                               ENABLE );
	
	//初始值为上拉
	GPIOA->ODR = 0xFFFF;
	GPIOB->ODR = 0xFFFF;
	GPIOC->ODR = 0xFFFF;
	GPIOD->ODR = 0xFFFF;
	GPIOE->ODR = 0xFFFF;
	GPIOF->ODR = 0xFFFF;
	GPIOG->ODR = 0xFFFF;
	
	//设置所有IO为输入上拉
	GPIOA->CRL  = 0x88888888;
	GPIOA->CRH  = 0x88888888;
	GPIOB->CRL  = 0x88888888;
	GPIOB->CRH  = 0x88888888;
	GPIOC->CRL  = 0x88888888;
	GPIOC->CRH  = 0x88888888;
	GPIOD->CRL  = 0x88888888;
	GPIOD->CRH  = 0x88888888;
	GPIOE->CRL  = 0x88888888;
	GPIOE->CRH  = 0x88888888;
	GPIOF->CRL  = 0x88888888;
	GPIOF->CRH  = 0x88888888;
	GPIOG->CRL  = 0x88888888;
	GPIOG->CRH  = 0x88888888;
	
	//PTE14,15 LED
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
    GPIO_WriteBit(GPIOE, GPIO_Pin_14, (BitAction)1);
    GPIO_WriteBit(GPIOE, GPIO_Pin_15, (BitAction)0);
    
    //PD12，13，14，15  Reserved Pins as float input.
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    //PTE7,8,10,12 as D3,D2,D1,D0 of the hardware ID. 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_7| GPIO_Pin_10| GPIO_Pin_12 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
    
    //PTC13, watchdog and cpu usage indicator
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)0);
    
    //PD8,9,10,11, Key 0-3
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_11; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    //将GPIO口与中断线映射起来
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource8);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource9);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource10);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource11);
    //中断线上进行中断初始化
    EXTI_InitTypeDef EXTI_InitStructure;//定义初始化结构体
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式，可选值为中断 EXTI_Mode_Interrupt 和事件 EXTI_Mode_Event。
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//触发方式，可以是下降沿触发 EXTI_Trigger_Falling，上升沿触发 EXTI_Trigger_Rising，或者任意电平（上升沿和下降沿）触发EXTI_Trigger_Rising_Falling
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Line=EXTI_Line8 ; //中断线的标号 取值范围为EXTI_Line0~EXTI_Line15
    EXTI_Init(&EXTI_InitStructure);//根据结构体信息进行初始化
     EXTI_InitStructure.EXTI_Line=EXTI_Line9 ; //中断线的标号 取值范围为EXTI_Line0~EXTI_Line15
    EXTI_Init(&EXTI_InitStructure);//根据结构体信息进行初始化
     EXTI_InitStructure.EXTI_Line=EXTI_Line10 ; //中断线的标号 取值范围为EXTI_Line0~EXTI_Line15
    EXTI_Init(&EXTI_InitStructure);//根据结构体信息进行初始化
     EXTI_InitStructure.EXTI_Line=EXTI_Line11 ; //中断线的标号 取值范围为EXTI_Line0~EXTI_Line15
    EXTI_Init(&EXTI_InitStructure);//根据结构体信息进行初始化
    //中断优先级配置
    NVIC_InitTypeDef NVIC_InitStructure;//定义结构体    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; //使能外部中断所在的通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级 2， 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5; //子优先级 2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道 
    NVIC_Init(&NVIC_InitStructure); //根据结构体信息进行优先级初始化
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //使能外部中断所在的通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级 2， 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5; //子优先级 2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道 
    NVIC_Init(&NVIC_InitStructure); //根据结构体信息进行优先级初始化
    
    
    if(GetDeviceHardwareId(NULL)&0x08)//DAQ板
    {
        //PTE1,2,3,4. PTE1 AO LDAC, PTE2,3,4: ADC Select
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOE,&GPIO_InitStructure);
        GPIO_WriteBit(GPIOE, GPIO_Pin_1, (BitAction)0);
        GPIO_WriteBit(GPIOE, GPIO_Pin_2, (BitAction)0);
        GPIO_WriteBit(GPIOE, GPIO_Pin_3, (BitAction)0);
        GPIO_WriteBit(GPIOE, GPIO_Pin_4, (BitAction)0);
        
        //PTE9,11  HIGH Current OUT
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOE,&GPIO_InitStructure);
        GPIO_WriteBit(GPIOE, GPIO_Pin_9, (BitAction)1);
        GPIO_WriteBit(GPIOE, GPIO_Pin_11, (BitAction)1);
        
        //PTC6,7  AO Power control.
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6  ;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC,&GPIO_InitStructure);
        GPIO_WriteBit(GPIOC, GPIO_Pin_6, (BitAction)1);
        /*GPIO_WriteBit(GPIOC, GPIO_Pin_7, (BitAction)0);
        GPIO_WriteBit(GPIOC, GPIO_Pin_6, (BitAction)1);*/
        
        //PTD4，PTD1, DO Fail-safe mode selection
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 ;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOD,&GPIO_InitStructure);
        GPIO_WriteBit(GPIOD, GPIO_Pin_1, (BitAction)0);
        GPIO_WriteBit(GPIOD, GPIO_Pin_4, (BitAction)0);
        
        //PTD3,PTD0,DO Error Input 
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 ;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOD,&GPIO_InitStructure);
        
        //将GPIO口与中断线映射起来
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource0);
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource3);
        //中断线上进行中断初始化
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式，可选值为中断 EXTI_Mode_Interrupt 和事件 EXTI_Mode_Event。
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//触发方式，可以是下降沿触发 EXTI_Trigger_Falling，上升沿触发 EXTI_Trigger_Rising，或者任意电平（上升沿和下降沿）触发EXTI_Trigger_Rising_Falling
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_InitStructure.EXTI_Line=EXTI_Line0 ; //中断线的标号 取值范围为EXTI_Line0~EXTI_Line15
        EXTI_Init(&EXTI_InitStructure);//根据结构体信息进行初始化
        EXTI_InitStructure.EXTI_Line=EXTI_Line3 ; //中断线的标号 取值范围为EXTI_Line0~EXTI_Line15
        EXTI_Init(&EXTI_InitStructure);//根据结构体信息进行初始化
        //中断优先级配置
        NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //使能外部中断所在的通道
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级 2， 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5; //子优先级 2
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道 
        NVIC_Init(&NVIC_InitStructure); //根据结构体信息进行优先级初始化
        
        NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; //使能外部中断所在的通道
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级 2， 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5; //子优先级 2
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道 
        NVIC_Init(&NVIC_InitStructure); //根据结构体信息进行优先级初始化
    }
    else//PWR板
    {        
        //PTD4，PTD1, RUN0,RUN1 selection.
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 ;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOD,&GPIO_InitStructure);
        GPIO_WriteBit(GPIOD, GPIO_Pin_1, (BitAction)0);
        GPIO_WriteBit(GPIOD, GPIO_Pin_4, (BitAction)0);
        
        //PTE0, LTC Alert input
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  ;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOE,&GPIO_InitStructure);
        
        //PTE1, LTC WP output
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOE,&GPIO_InitStructure);
        GPIO_WriteBit(GPIOE, GPIO_Pin_1, (BitAction)0);
        
        //PTA8, PTC9, LTC PG input
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  ;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA,&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9  ;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC,&GPIO_InitStructure);
        
        //PTD3,PTD0,LTC FLT input
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 ;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOD,&GPIO_InitStructure);
        
        //将GPIO口与中断线映射起来
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource0);
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource3);
        //中断线上进行中断初始化
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式，可选值为中断 EXTI_Mode_Interrupt 和事件 EXTI_Mode_Event。
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//触发方式，可以是下降沿触发 EXTI_Trigger_Falling，上升沿触发 EXTI_Trigger_Rising，或者任意电平（上升沿和下降沿）触发EXTI_Trigger_Rising_Falling
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_InitStructure.EXTI_Line=EXTI_Line0 ; //中断线的标号 取值范围为EXTI_Line0~EXTI_Line15
        EXTI_Init(&EXTI_InitStructure);//根据结构体信息进行初始化
        EXTI_InitStructure.EXTI_Line=EXTI_Line3 ; //中断线的标号 取值范围为EXTI_Line0~EXTI_Line15
        EXTI_Init(&EXTI_InitStructure);//根据结构体信息进行初始化
        //中断优先级配置
        NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //使能外部中断所在的通道
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级 2， 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5; //子优先级 2
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道 
        NVIC_Init(&NVIC_InitStructure); //根据结构体信息进行优先级初始化
        
        NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; //使能外部中断所在的通道
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级 2， 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5; //子优先级 2
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道 
        NVIC_Init(&NVIC_InitStructure); //根据结构体信息进行优先级初始化
    }
}

void SetLedStatus(u8 red, u8 blue)
{
    GPIO_WriteBit(GPIOE, GPIO_Pin_14, (BitAction)(!red));
    GPIO_WriteBit(GPIOE, GPIO_Pin_15, (BitAction)(!blue));
}

int GetLedStatus(u8* red, u8* blue)
{
    int a = GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_14);
    int b = GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_15);
    *red = !a;
    *blue =!b;
    return 1;
}
const char  HardwareVersionInfo[16][18]={
    "Unknown",//0000
    "Unknown",//0001
    "Unknown",//0010
    "Unknown",//0011
    "Unknown",//0100
    "Unknown",//0101
    "Unknown",//0110
    "TFC-PWR-V1_0",//0111
    "Unknown",//1000
    "Unknown",//1001
    "Unknown",//1010
    "Unknown",//1011
    "Unknown",//1100
    "Unknown",//1101
    "TFC-DAQ-V1_1",//1110
    "TFC-DAQ-V1_0"//1111
};

u8 GetDeviceHardwareId(char* str)
{
    u8 hwid = 0;
    hwid = (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12)<<0)+(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10)<<1)+(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8)<<2)+(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)<<3);
    if(hwid>15) hwid = 15;
    if(str!=NULL)
        strcpy(str,HardwareVersionInfo[hwid]);
    return hwid;
}