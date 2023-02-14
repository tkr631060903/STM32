#include "includes.h"


/*
1	您好
2	您好  欢迎光临
3	欢迎再次光临，谢谢
4	欢迎使用本公司产品
5	----
6	0
7	1
8	2
9	3
10	4
11	5
12	6
13	7
14	8
15	9
16	十
17	百
18	千
19	万
20	元
21	点
22	金额
23	音乐声
24	请充值
25	电量不足，请充电
26	谢谢




控制数据	地址	中文语音内容		
0X01	1	 欢迎提示使用本产品		
0X02	2	正在测量体重，请站好		
0X03	3	你的体重是		
0X04	4	正在测量身高，请站直，		
0X05	5	你的身高是.		
0X06	6	正在测量脂肪，请等候		
0X07	7	BMI		
0X08	8	体脂肪百分之		
0X09	9	水分百分之		
0X0A	10	骨骼量		
0X0B	11	肌肉百分之		
0X0C	12	新陈代谢		
0X0D	13	偏胖		
0X0E	14	偏瘦		
0X0F	15	正常		
0X10	16	肥胖		
0X11	17	请打印凭条或扫描二维码		
0X12	18	厘米		
0X13	19	公斤		
0X14	20	0		
0X15	21	1		
0X16	22	2		
0X17	23	3		
0X18	24	4		
0X19	25	5		
0X1A	26	6		
0X1B	27	7		
0X1C	28	8		
0X1D	29	9		
0X1E	30	10		
0X1F	31	百		
0X20	32	点		
0X21	33	百分之		
0X22	34	KCAL		
0X23	35	米
0x24    36      千
0x25    37      万
0x26    38      英镑
0x27    39      英寸



*/

const int lengthofLanguageBlock = 39;

void delay_us(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=6;  //自己定义
      while(i--) ;    
   }
}
void AudioInit(u8 volume)
{
    AudioSingleVoice(0xFE);//停止播放
    if(volume)
        AudioSingleVoice(0xFC);//打开功放
    else
        AudioSingleVoice(0xFD);//打开功放
    AudioSingleVoice(0xE0+(volume-1)%4*5);//打开功放 E0~EF
}
int AudioSingleVoiceNoWait(unsigned char dat)
{
    unsigned char i;
    int count = 0;
    GPIO_InitTypeDef     GPIO_InitStructure;
    
    if(sys.audio == 0)
        return -1;
    if(PEN_STATUS)
        return 0;
    
    dat += lengthofLanguageBlock*sys.language;
    
    SetUartSwitch(FUNC_AUDIO);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    asm("CPSID  I");   //关中断
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)0);
    delay_us(3000);
    for(i=0;i < 8;i++)
    {
        GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)1);
        if(dat & 0x01)
        {
            delay_us(1200);
            GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)0);
            delay_us(400);
        }
        else
        {
            delay_us(400);
            GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)0);
            delay_us(1200);
        }
        dat >>= 1;
    }
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)1);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    if(count>800)
    {
        //语音模块不存在
        sys.audio = 0;
    }
    delay_us(350);
    asm("CPSIE  I");   //开中断
    
    return count;
}


int AudioSingleVoice(unsigned char dat)
{
    int count = 0;
    
    count  = AudioSingleVoiceNoWait(dat);
    
    while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11))
    {
        count++;
        if(PEN_STATUS)
            return 0;
        DelayMs(20);
    }
    return count;
    
}

int AudioNumericVoiceDigit(double num, int digit)
{
    //0 1....万
    const unsigned char voiceAddr[]={0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d};
    const unsigned char voice10 = 0x1e, voice100 = 0x1f, voice1000 = 0x24, voice10000 = 0x25, voiceDot = 0x20;
    const unsigned char voiceAnd = 0x60;
    const unsigned char voiceEng[] = {69, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95};
    //                                10, 11  12  13  14  15  16  17  18  19  20  30  40  50  60  70  80  90  
    if(num<0) return -1;
    long lNum = (long)num;
    
    long temp;
    int i;
    
    unsigned char voiceCore[26];
    int voicePointer = 0;
    u8 bZero = 0;
    
    if(lNum/100000000%10)
    {
        voiceCore[voicePointer++]=voiceAddr[lNum/100000000%10];
        voiceCore[voicePointer++]=voice10000;
        voiceCore[voicePointer++]=voice10000;
        bZero=0;
        lNum %= 100000000;
    }
    temp = lNum/10000%10000;
    if(temp)
    {
        if(temp/1000)//千
        {
            voiceCore[voicePointer++]=voiceAddr[temp/1000%10];
            voiceCore[voicePointer++]=voice1000;
            bZero=0;
        }
        else
        {
            if((lNum/100000000) && (temp%1000))
            {
                if(bZero==0)
                    voiceCore[voicePointer++]=voiceAddr[0];
                bZero = 1;
            }
        }
        if(temp/100%10)//百
        {
            voiceCore[voicePointer++]=voiceAddr[temp/100%10];
            voiceCore[voicePointer++]=voice100;
            bZero=0;
        }
        else
        {
            if(lNum/100000000 && temp%100)
            {
                if(bZero==0)
                    voiceCore[voicePointer++]=voiceAddr[0];
                bZero = 1;
            }
        }
        if(temp/10%10)//十
        {
            if(temp/10!=1)
                voiceCore[voicePointer++]=voiceAddr[temp/10%10];
            voiceCore[voicePointer++]=voice10;
            bZero=0;
        }
        else
        {
            if((lNum/100000000 || temp/1000 || (temp/100%10)) && temp%10)
            {
                if(bZero==0)
                    voiceCore[voicePointer++]=voiceAddr[0];
                bZero = 1;
            }
        }
        if(temp%10)//个
        {
            voiceCore[voicePointer++]=voiceAddr[temp%10];
            bZero=0;
        }
        voiceCore[voicePointer++]=voice10000;
    }
    temp = lNum%10000;
    bZero = 0;
    if(temp)
    {
        if(temp/1000)//千
        {
            voiceCore[voicePointer++]=voiceAddr[temp/1000%10];
            voiceCore[voicePointer++]=voice1000;
            bZero=0;
        }
        else
        {
            if((lNum/10000) && (temp%1000))
            {
                if(bZero==0)
                {
                    if(sys.language==0)
                        voiceCore[voicePointer++]=voiceAddr[0];
                    else
                        voiceCore[voicePointer++]= voiceAnd;
                }
                bZero = 1;
            }
        }
        if(temp/100%10)//百
        {
            voiceCore[voicePointer++]=voiceAddr[temp/100%10];
            voiceCore[voicePointer++]=voice100;
            bZero=0;
        }
        else
        {
            if(lNum/10000 && temp%100)
            {
                if(bZero==0)
                {
                    if(sys.language==0)
                        voiceCore[voicePointer++]=voiceAddr[0];
                    else
                        voiceCore[voicePointer++]=voiceAnd;
                }
                bZero = 1;
            }
        }
        if(temp/10%10)//十
        {
            if(sys.language==0)//汉语读法
            {
                if(temp/10!=1)
                    voiceCore[voicePointer++]=voiceAddr[temp/10%10];
                voiceCore[voicePointer++]=voice10;
                bZero=0;
            }
            else//英文读法
            {
                if(temp/10%10 == 1)
                {
                    voiceCore[voicePointer++]=voiceEng[temp%100-10];
                }
                else
                {
                    voiceCore[voicePointer++]=voiceEng[temp/10%10+8];
                }
                bZero = 0;
                //voiceCore[voicePointer++]=voiceAddr[temp/10%10];
            }
        }
        else
        {
            if((lNum/10000 || temp/1000 || (temp/100%10)) && temp%10)
            {
                if(bZero==0)
                {
                    if(sys.language==0)
                        voiceCore[voicePointer++]=voiceAddr[0];
                    else
                        voiceCore[voicePointer++]=voiceAnd;
                }
                bZero = 1;
            }
        }
        if(temp%10)//个
        {
            if(sys.language==1 && temp/10%10 == 1)
            {
                //do nothing.
            }
            else
            {
                voiceCore[voicePointer++]=voiceAddr[temp%10];
            }
            bZero=0;
        }
    }
    
    long dNum = (long)((num - lNum)*100000);
    int dotCount = 5;
    if(dNum%10==9)
    {
        dNum ++;
    }
    while(dNum%10 == 0 && dotCount)
    {
        dotCount --;
        dNum /= 10;
    }
    if(dotCount > digit)
    {
        dotCount = digit;
        if(dotCount >6)
            dotCount = 6;
        else if(dotCount <0)
            dotCount = 0;
    }
    
    dNum = (long)((num - lNum)*100000);
    if(dNum%10==9)
    {
        dNum ++;
    }
    if(dotCount)
    {
        if(lNum == 0)
            voiceCore[voicePointer++]=voiceAddr[0];;
        voiceCore[voicePointer++]=voiceDot;
        int t = 10000;
        for(i=0;i<dotCount;i++)
        {
            voiceCore[voicePointer++]=voiceAddr[dNum/t%10];
            dNum %= t;
            t /= 10;
        }
    }
    for(i=0;i<voicePointer;i++)
    {
        if(AudioSingleVoice(voiceCore[i])==0)
            return 0;
    }
    delay_us(300);
    
    return 1;
}


/*
int AudioNumericVoiceDigit(double num, int digit)
{
    //0 1....万
    const unsigned char voiceAddr[]={0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d};
    const unsigned char voice10 = 0x1e, voice100 = 0x1f, voice1000 = 0x24, voice10000 = 0x25, voiceDot = 0x20;
    if(num<0) return -1;
    long lNum = (long)num;
    
    long temp;
    int i;
    
    unsigned char voiceCore[26];
    int voicePointer = 0;
    u8 bZero = 0;
    
    if(lNum/100000000%10)
    {
        voiceCore[voicePointer++]=voiceAddr[lNum/100000000%10];
        voiceCore[voicePointer++]=voice10000;
        voiceCore[voicePointer++]=voice10000;
        bZero=0;
        lNum %= 100000000;
    }
    temp = lNum/10000%10000;
    if(temp)
    {
        if(temp/1000)//千
        {
            voiceCore[voicePointer++]=voiceAddr[temp/1000%10];
            voiceCore[voicePointer++]=voice1000;
            bZero=0;
        }
        else
        {
            if((lNum/100000000) && (temp%1000))
            {
                if(bZero==0)
                    voiceCore[voicePointer++]=voiceAddr[0];
                bZero = 1;
            }
        }
        if(temp/100%10)//百
        {
            voiceCore[voicePointer++]=voiceAddr[temp/100%10];
            voiceCore[voicePointer++]=voice100;
            bZero=0;
        }
        else
        {
            if(lNum/100000000 && temp%100)
            {
                if(bZero==0)
                    voiceCore[voicePointer++]=voiceAddr[0];
                bZero = 1;
            }
        }
        if(temp/10%10)//十
        {
            if(temp/10!=1)
                voiceCore[voicePointer++]=voiceAddr[temp/10%10];
            voiceCore[voicePointer++]=voice10;
            bZero=0;
        }
        else
        {
            if((lNum/100000000 || temp/1000 || (temp/100%10)) && temp%10)
            {
                if(bZero==0)
                    voiceCore[voicePointer++]=voiceAddr[0];
                bZero = 1;
            }
        }
        if(temp%10)//个
        {
            voiceCore[voicePointer++]=voiceAddr[temp%10];
            bZero=0;
        }
        voiceCore[voicePointer++]=voice10000;
    }
    temp = lNum%10000;
    bZero = 0;
    if(temp)
    {
        if(temp/1000)//千
        {
            voiceCore[voicePointer++]=voiceAddr[temp/1000%10];
            voiceCore[voicePointer++]=voice1000;
            bZero=0;
        }
        else
        {
            if((lNum/10000) && (temp%1000))
            {
                if(bZero==0)
                    voiceCore[voicePointer++]=voiceAddr[0];
                bZero = 1;
            }
        }
        if(temp/100%10)//百
        {
            voiceCore[voicePointer++]=voiceAddr[temp/100%10];
            voiceCore[voicePointer++]=voice100;
            bZero=0;
        }
        else
        {
            if(lNum/10000 && temp%100)
            {
                if(bZero==0)
                    voiceCore[voicePointer++]=voiceAddr[0];
                bZero = 1;
            }
        }
        if(temp/10%10)//十
        {
            if(temp/10!=1)
                voiceCore[voicePointer++]=voiceAddr[temp/10%10];
            voiceCore[voicePointer++]=voice10;
            bZero=0;
        }
        else
        {
            if((lNum/10000 || temp/1000 || (temp/100%10)) && temp%10)
            {
                if(bZero==0)
                    voiceCore[voicePointer++]=voiceAddr[0];
                bZero = 1;
            }
        }
        if(temp%10)//个
        {
            voiceCore[voicePointer++]=voiceAddr[temp%10];
            bZero=0;
        }
    }
    
    long dNum = (long)((num - lNum)*100000);
    int dotCount = 5;
    if(dNum%10==9)
    {
        dNum ++;
    }
    while(dNum%10 == 0 && dotCount)
    {
        dotCount --;
        dNum /= 10;
    }
    if(dotCount > digit)
    {
        dotCount = digit;
        if(dotCount >6)
            dotCount = 6;
        else if(dotCount <0)
            dotCount = 0;
    }
    
    dNum = (long)((num - lNum)*100000);
    if(dNum%10==9)
    {
        dNum ++;
    }
    if(dotCount)
    {
        if(lNum == 0)
            voiceCore[voicePointer++]=voiceAddr[0];;
        voiceCore[voicePointer++]=voiceDot;
        int t = 10000;
        for(i=0;i<dotCount;i++)
        {
            voiceCore[voicePointer++]=voiceAddr[dNum/t%10];
            dNum %= t;
            t /= 10;
        }
    }
    for(i=0;i<voicePointer;i++)
    {
        if(AudioSingleVoice(voiceCore[i])==0)
            return 0;
    }
    delay_us(300);
    
    return 1;
}
*/
int AudioNumericVoice(double num)
{
    return AudioNumericVoiceDigit(num, 6);
}