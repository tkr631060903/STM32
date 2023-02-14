

#include "includes.h"



/*
        int i,j;
        for(j=0;j<50;j++)
        {
            for(i=0;i<15;i++)
            {
                GetADCVol(i);
            }
            for(i=0;i<8;i++)
            {
                GetADCVol(15);
            }
        }
        
        debug("50 cycle ADC Time: %d\r\n",GetSystemTime());*/

void AIInit(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;
    ADC_InitTypeDef     ADC_InitStructure;
    
    //����ʱ��
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);//ADCLKʱ�ӳ���14M��ʹAD���������½�
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA  |RCC_APB2Periph_GPIOB  |RCC_APB2Periph_GPIOC , ENABLE);
    
    //IO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //����ADC1����ģʽ
    ADC_DeInit(ADC1); 
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;			//�����ڵ���ģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;				//��ͨ��
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;			//����
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//���ⲿ����
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;			//�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1;					//��ͨ��ת����ͨ����
    ADC_Init(ADC1, &ADC_InitStructure);
    
    ADC_Cmd(ADC1, ENABLE);
    
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    
    //ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    //ADC1->SMPR1 = 0x00000000;		//����ͨ��Ĭ��ת�����ڶ�Ϊ1.5������
    //ADC1->SMPR2 = 0x00000000;
    
    //ADC1->CR2 |= 0x01;			//����AD
}
u16 _readSamDots(u8 Ch)
{
    ADC_RegularChannelConfig(ADC1, Ch, 1, ADC_SampleTime_239Cycles5 );   //ADC1,ADCͨ��,����ʱ��Ϊ239.5���ڡ�Rin max = 350kR.
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);     //ʹ��ָ����ADC1�����ת����������   
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
    return ADC_GetConversionValue(ADC1);    //�������һ��ADC1�������ת�����
}

u8 GetRandomCharFromADC()
{
    static int index = 0;
    u16 temp;
    u8 adc1, adc2,adc;
    ADC_RegularChannelConfig(ADC1, 1, 1, ADC_SampleTime_1Cycles5 );   //ADC1,ADCͨ��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);     //ʹ��ָ����ADC1�����ת����������   
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
    temp = ADC_GetConversionValue(ADC1);    //�������һ��ADC1�������ת�����
    temp >>= 5;
    adc1 = temp & 0xff;
    
    ADC_RegularChannelConfig(ADC1, 1, 1, ADC_SampleTime_1Cycles5 );   //ADC1,ADCͨ��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);     //ʹ��ָ����ADC1�����ת����������   
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
    temp = ADC_GetConversionValue(ADC1);    //�������һ��ADC1�������ת�����
    temp >>= 5;
    adc2 = temp & 0xff;
    
    index += adc1*adc2;
    index += (adc1&0x0f + (adc2<<4));
    
    adc = index & 0xFF;
    
    return adc;
}

u32 _getMCUADCVol(u8 Ch)
{
    const u16 ADC_AVE = 8;
    u32 val;
    u16  j=0;
    
    val = 0;
    j= ADC_AVE;
    while(j--)
        val += _readSamDots(Ch);
    val = val/ADC_AVE;
    
    u8 hardwareId = GetDeviceHardwareId(NULL);
    if(hardwareId == 0x0F)//DAQ-V1_0, MCU��Vref�����ⲿ3.3V���磬ʵ�ʵ�ѹ3.44V
    {
        if(Ch!=15)
            val = val*344/195;//MV,2.1*3440/4095
        else
            val = val*7912/585;//MV,HV channel ��ѹ������һ��,16.1*3440/4095
    }
    else//DAQ-V1_0, MCU��Vref�����ⲿ3.3V��׼Դ���磬ʵ�ʵ�ѹ3.3V
    {
        if(Ch!=15)
            val = val*22/13;//MV,2.1*3300/4095
        else
            val = val*506/39;//MV,HV channel ��ѹ������һ��,16.1*3300/4095
    }
    
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);  
    return val;
}

u32 GetAIVal(u8 ch)
{
    u32 counter = 0;
    if(ch>=22) ch = 22;
    if(ch<=7)
    {
        GPIO_WriteBit(GPIOE, GPIO_Pin_4, (BitAction)((ch&0x04)>>2));
        GPIO_WriteBit(GPIOE, GPIO_Pin_3, (BitAction)((ch&0x02)>>1));
        GPIO_WriteBit(GPIOE, GPIO_Pin_2, (BitAction)(ch&0x01));
        counter = 100;
        while(counter--);
        return _getMCUADCVol(15);
    }
    else
    {
        return _getMCUADCVol(ch-8);
    }
}

u16 GetBatVol(u8 Ch)
{
    u32 val;
    u16  j=32;
    val = 0;
    
    while(j--)
        val += _readSamDots(Ch);
    val >>= 5;
    
    val = val*3295/4095;//MV
    
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);  
    return val;
}
//0  BAT����  1 ���ڳ��  2 ������   3 AC�����޵��
int _calculateAcDc()
{
    u16 chrg = 0, stb = 0;
    u8 flag = 0;
    
    chrg = (u16)(5.65*GetBatVol(11));
    stb = (u16)(5.65*GetBatVol(12));

    
    //ADC123_11     PC1     CHRG    0           0           1           1
    //ADC123_12     PC2     STB     0           1           0           1
    //                              BAT only    Chrging     Full        AC only
    if(chrg>5000)
        flag += 2;
    if(stb>5000)
        flag += 1;
    /*debug("chrg:%d, stb:%d\r\n",(int)chrg,(int)stb);
    switch(flag)
    {
    case 0:
        debug("BAT only\r\n");
        break;
    case 1:
        debug("Charging\r\n");
        break;
    case 2:
        debug("Full\r\n");
        break;
    case 3:
        debug("AC only\r\n");
        break;
    }*/
//0  BAT����  1 ���ڳ��  2 ������   3 AC�����޵��
    return flag;
}
/*
//AC ���� 1  ��ط���0
int _calculateAcDc(long* array)
{
    int i;
    int tIndex,gIndex;
    int nMax, nMin;
    for(i=0,tIndex=0,gIndex=0,nMax=0,nMin=65536;i<RECENT_VOLTAGE_SIZE-1;i++)
    {
        if(abs(array[i]-array[i+1])>gIndex)
        {
            gIndex = abs(array[i]-array[i+1]);
            tIndex = i;
        }
        if(nMax<array[i])
            nMax = array[i];
        if(nMin>array[i])
            nMin = array[i];
    }
    if(nMin>8350)
        return 1;
    if(nMax-nMin<30)
        return 0;
    if(nMax-nMin>50)
        return 1;
    
    if(tIndex <= 0 || tIndex >= RECENT_VOLTAGE_SIZE-2)
        return 1;
    long t1=0,t2=0;
    for(i=0;i<=tIndex;i++)
    {
        t1+=array[i];
    }
    t1 /= (tIndex+1);
    for(i=tIndex+1;i<RECENT_VOLTAGE_SIZE;i++)
    {
        t2+=array[i];
    }
    t2 /= (RECENT_VOLTAGE_SIZE-tIndex-1);
    if(abs(t2-t1)>15)
        return 0;
    else return 1;
}*/




float GetRandomFromRange(float a, float b)
{
    float result=0;
    u8 t[4];
    u32 i;
    u32 lRnd;
    
    for(i=0;i<4;i++)
        t[i] = GetRandomCharFromADC();
    lRnd = t[0] + ((u32)t[1]<<8) + ((u32)t[2]<<16) + ((u32)t[2]<<24);
    lRnd %= 1000001;
    float percent = ((float)lRnd)/(1000001);
    result = a + (b-a)*percent;
    
    float min,max ;
    if(a>b)
    {
        min = b;
        max = a;
    }
    else
    {
        min = a;
        max = b;
    }
    if(result >max) result = max;
    else if(result < min) result = min;
    return result;
}