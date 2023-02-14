#include "includes.h"

u8 bBeepEnabled = 1;

void BeepEnable(u8 enabled)
{
    if(enabled) 
    {
        bBeepEnabled =1;
    }
    else 
    {
        bBeepEnabled = 0;
    }
}

u8 IsBeepEnabled()
{
    return bBeepEnabled;
}

void BeepInit()//PE13  u16 Frequency,u16 pwmDuty
{
    u16 Frequency=1000;
    u16 pwmDuty=500;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOE, ENABLE);
   
  	
  	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
    
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
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, DISABLE);	//预装载寄存器的内容被立即传送到影子寄存器
	TIM_Cmd(TIM1, DISABLE);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//	高级定时器TIM1和TIM8 需要加本句话才可以输出PWM
    BeepEnable(1);
}
void StopNormalBeep()
{
    KillTimer(2);
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
    
//    TIM_Cmd(TIM1, DISABLE);
}
void ModelBeep(u32 freq, u32 ms) //Beep and delay, xxx ms. 
{
    if(!IsBeepEnabled()) return;
    
    u16 Pwmpulse,PwmPeriod;
	
	PwmPeriod = 1000000/freq;
	Pwmpulse = (PwmPeriod+1)*500/1000ul;
    
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
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
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, DISABLE);	//预装载寄存器的内容被立即传送到影子寄存器
	TIM_Cmd(TIM1, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//	高级定时器TIM1和TIM8 需要加本句话才可以输出PWM
    
    DelayMs(ms);
    
    //TIM_Cmd(TIM1, DISABLE);
    StopNormalBeep();
}

void Beep(u32 freq, u32 ms)      //Beep without blocking.
{
    if(!IsBeepEnabled()) return;
    u16 Pwmpulse,PwmPeriod;
	
	PwmPeriod = 1000000/freq;
	Pwmpulse = (PwmPeriod+1)*500/1000ul;
    
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
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
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, DISABLE);	//预装载寄存器的内容被立即传送到影子寄存器
	TIM_Cmd(TIM1, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//	高级定时器TIM1和TIM8 需要加本句话才可以输出PWM
    
    
    
    SetTimer(2,ms/10+1,StopNormalBeep);
    
}
const u32 chord_freq[30]={0,262,294,330,349,392,440,494,0,0,0,523,578,659,698,784,880,988,0,0,0,1046,1175,1318,1397,1568,1760,1976};
const s16 xuemaojiao[]={21,22,23,15,21,23,23,0,22,21,22,25,25,25,25,0,21,17,21,21,21,21,21,0,17,21,17,21,17,16,15,0,
                        15,15,16,16,16,16,16,0,15,13,15,13,15,22,-21,0,15,23,23,23,24,25,21,21,122,123,22,0,
                        32767};
const s16 songbie[]={-15,13,15,-21,0,-16,21,16,-15,0,-15,11,12,-13,12,11,-12,0,0,0,-15,13,15,-21,17,-16,-21,-15,0,-15,12,13,-14,7,-11,0,0,0,
                        32767};
const s16 liangzhilaohu[]={-11,-12,-13,-11, -11,-12,-13,-11, -13,-14,-115,0, -13,-14,-115, 0,15,16,15,14,-13,-11,15,16,15,14,-13,-11,-12,-5,-111,0,-12,-5,-111,
                        32767};
const s16 fsChord[]={21,22,23,15,32767};
static int gMusicLen = 0;
static int gMusicIndex = 0;
static int gMusicDuration = 200;
const s16* gMusicPointer;
void PlayStartMusic(s16* music,int duration)
{
    int i;
    int musicLen;
    if(duration<50) duration = 50;
    if(duration>500)duration = 500;
    for(i=0;i<1000;i++)
    {
        if(music[i]>=32767)
            break;
    }
    musicLen = i;
    for(i=0;i<musicLen;i++)
    {
        if(music[i])
        {
            if(music[i]>0)
                ModelBeep(chord_freq[music[i]%100],duration/(music[i]/100+1));
            else
                ModelBeep(chord_freq[(0-music[i])%100],duration*((0-music[i])/100+1));
        }
        else DelayMs(duration);
    }
}
void PlayMusicMultiThreadInit(int music_id,u32 duration)
{
    int i;
    const s16* music_index[]={xuemaojiao,songbie,liangzhilaohu,fsChord};
    if(music_id<1) music_id = 1;
    if(music_id>4) music_id = 4;
    gMusicPointer = music_index[music_id-1];
    /*switch(music_id)
    {
    case 0:
        gMusicPointer = xuemaojiao;
    case 1:
        gMusicPointer = songbie;
    }*/
    for(i=0;i<1000;i++)
    {
        if(gMusicPointer[i]>=32767)
            break;
    }
    gMusicLen = i;
    gMusicIndex = 0;
    gMusicDuration = duration;
}

void PlayMusicMultiThread()
{
    if(gMusicPointer[gMusicIndex])
    {
        if(gMusicPointer[gMusicIndex]>0)
            Beep(chord_freq[gMusicPointer[gMusicIndex]%100],gMusicDuration/(gMusicPointer[gMusicIndex]/100+1));
        else
            Beep(chord_freq[(0-gMusicPointer[gMusicIndex])%100],gMusicDuration*((0-gMusicPointer[gMusicIndex])/100+1));
    }
    
    gMusicIndex ++;
    if(gMusicIndex>=gMusicLen)
    {
        KillTimer(5);
        gMusicIndex = 0;
    }
}