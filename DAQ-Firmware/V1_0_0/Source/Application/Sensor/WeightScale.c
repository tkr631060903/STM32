

#include "includes.h"


u8 weightMeasureStart = 0;
u32 gWeightUpdateTime = 0x7fffffff;

int EventWeightDataReceived(char* str)
{
    int ulWeight ;
    switch(str[0])
    {
    case '6'://量測體重狀態
        if(str[1] == 'T')//“T” 臨時不穩定值 臨時不穩定值 臨
        {
            SendData(PORT_SENSOR,"5--------35\n",12);
            SendData(PORT_SENSOR,"5--------35\n",12);
            user.weight = 100*(str[3]-'0')+10*(str[4]-'0')+1*(str[5]-'0')+0.1*(str[7]-'0');
           
            user.weightStatus = 'T';
            //user.status |= 0x02;
        }
        else if(str[1]=='R')//“R” 量鎖定值
        {
            SendData(PORT_SENSOR,"5--------35\n",12);
            SendData(PORT_SENSOR,"5--------35\n",12);
            user.weight = 100*(str[3]-'0')+10*(str[4]-'0')+1*(str[5]-'0')+0.1*(str[7]-'0');
            user.status |= 0x02;
            weightMeasureStart = 0;
            
            ulWeight = (int)(user.weight *10);
            ulWeight += sys.weightOffset;
            user.weight = (float)ulWeight/10; 
            
            if(user.weight <0) user.weight = 0;
            user.weightStatus = 'R';
            
           
            
        }
        gWeightUpdateTime = GetSystemTime();
        
        break;
    case '7'://體脂量測結果
        //do nothing 
        user.LBM = 100*str[1]+10*str[2]+1*str[3]+0.1*str[5]+0.01*str[6]+0.001*str[7];
        break;
    case '8'://標定狀態
        break;
    case '9'://接收應答反饋
        //do nothing at the moment
        break;
    default:
        break;
    }
    return 1;
}
//经测试不能用2016-03-15
int WeightSensorDetect()
{
    int i;

    u8 age = user.age;
    float height = user.height;
    float weight = user.weight;
    
    user.age = 20;
    user.height = 180;
    user.weight = 70;
    for(i=0;i<5;i++)
    {
        if(GetFirstFatData())
        {
            user.age = age;
            user.height = height;
            user.weight = weight;
            return 1;
        }
    }
    user.age = age;
    user.height = height;
    user.weight = weight;
    return 0;
}


int GetFirstWeightData()
{
    char buf[2*BUFFER_SIZE];
    static int errorCounter  = 0;
    
    
    
    SetBusy(1);
    SetUartSwitch(FUNC_WEIGHT);
    SendData(PORT_SENSOR,"1K-------57\n",12);
    SetBusy(0);
    
    int counter = 40;//在20ms内完成接收
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        if(CheckReceivedString(buf))
        {
            EventWeightDataReceived(buf);
            return 1;
        }
        else
        {
            errorCounter++;
            //校验错误
        }
        
    }
    else
    {
        //do nothing
    }
    
    return 0;
}

int GetNormalWeightData()
{
    static int errorCounter = 0;
    char buf[2*BUFFER_SIZE];
    
    
    SetUartSwitch(FUNC_WEIGHT);
    
    if(GetUart1RxStatus(buf))
    {
        if(CheckReceivedString(buf))
        {
            EventWeightDataReceived(buf);
            return 1;
        }
        else
        {
            errorCounter++;
            //校验错误
        }
    }
    
    return 0;
}


////
//return 0 失败  1 成功  2 接触不良
int GetFirstFatData()
{
    const char table[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    
    char buf[2*BUFFER_SIZE];
    static int errorCounter  = 0;
   
    char ins[]="2-M02517053\n";
    ins[4]='0'+user.age%100/10;
    ins[5]='0'+user.age%10;
    ins[6]='0'+(int)user.height/100;
    ins[7]='0'+(int)user.height/10%10;
    ins[8]='0'+(int)user.height%10;
    int i;
    u8 ret;
    for(i=1,ret=ins[0];i<9;i++)
    {
        ret ^= ins[i];
    }
    ins[9] = table[ret>>4];
    ins[10] = table[ret&0x0F];
    
    SetBusy(1);
    SetUartSwitch(FUNC_WEIGHT);
    SendData(PORT_SENSOR,ins,12);
    SetBusy(0);
    
    int counter = 40;//在20ms内完成接收
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        if(CheckReceivedString(buf))
        {
            SendData(PORT_SENSOR,"5--------35\n",12);
            SendData(PORT_SENSOR,"5--------35\n",12);
            if(buf[0]=='9')
            {
                //go on in next step
            }
            //继续等待4秒直到收到结果
            else if(buf[0]=='7')
            {
                if(buf[1]=='F')//没有触摸好
                {
                    return 2;
                }
                else
                {
                    //go on in next step
                    //user.status |= 0x08;
                    user.LBM = 100*(buf[1]-'0')+10*(buf[2]-'0')+1*(buf[3]-'0')+0.1*(buf[5]-'0')+0.01*(buf[6]-'0')+0.001*(buf[7]-'0');
                    return 1;
                }
                
            }
        }
        else
        {
            errorCounter++;
            //校验错误
            return 0;
        }
    }
    else
    {
        return 0;
    }
    //继续等待4秒直到收到结果
    counter = 4000;
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        SendData(PORT_SENSOR,"5--------35\n",12);
        SendData(PORT_SENSOR,"5--------35\n",12);
        if(CheckReceivedString(buf))
        {
            //继续等待4秒直到收到结果
            if(buf[0]=='7')
            {
                if(buf[1]=='F')//没有触摸好
                {
                    return 2;
                }
                else
                {
                    //go on in next step
                    user.status |= 0x08;
                    user.LBM = 100*(buf[1]-'0')+10*(buf[2]-'0')+1*(buf[3]-'0')+0.1*(buf[5]-'0')+0.01*(buf[6]-'0')+0.001*(buf[7]-'0');
                }
                
            }
            else
            {
                return 0;
            }
        }
        else
        {
            errorCounter++;
            //校验错误
            return 0;
        }
    }
    else
    {
        return 0;
    }
    return 1;
}

//int param[6]分别为 water, bone, fat, muscle, kcal, vf;
//user.height 身高，单位cm  user.weight 体重，单位kg
//骨量为实际值的10倍
int CalculateLBMArray(long* param, float* bmi)
{
    float water=0, bone=0, fat=0, muscle=0, kcal=0, vf = 0;
	float tmpLBM =0;
    
    if(user.height <HEIGHT_MIN || user.weight < WEIGHT_MIN)
        return 0;
    if(user.height>HEIGHT_MAX || user.weight >WEIGHT_MAX)
        return 0;
    
    //运动员的LBM需要加权
    if(user.athlete == 0)
        tmpLBM = user.LBM;
    else if(user.athlete == 1)
        tmpLBM = 1.0427*user.LBM;
    else
        tmpLBM = 1.0958*user.LBM;
    //计算浮点值
    fat = ((1-tmpLBM/user.weight)*100);
    water = ((1-((float)fat)/100)*73);
    muscle = (54.8*tmpLBM/user.weight);
    bone = (0.05158*tmpLBM);
    *bmi = 10000*user.weight/(user.height*user.height);
    if(user.sex == 0)//men
    {
        kcal = 665+1.38*user.weight+5*user.height-6.8*user.age;
    }
    else
    {
        kcal = 655+9.6*user.weight+1.9*user.height-4.7*user.age;
    }
    if(user.athlete == 0)
        kcal = 1.32*kcal;
    else if(user.athlete ==1)
        kcal = 1.54*kcal;
    else kcal = 1.98*kcal;
    
    if(user.sex==0)//men
    {
        if(user.height<1.6*user.weight+63)
        {
            vf = 380*user.weight/(0.0826*user.height*user.height-0.4*user.height+48)-5+0.15*user.age;
        }
        else
        {
            vf = (0.765 -0.2/100*user.height)*user.weight-0.143*user.height+0.15*user.age-2.9;
        }
    }
    else
    {
        if(user.weight>0.5*user.height-13)
        {
            vf = 500*user.weight/(0.1158*user.height*user.height+1.45*user.height-120)-6+0.07*user.age;
        }
        else
        {
            vf = (0.691-0.24/100*user.height)*user.weight-2.7/100*user.height+0.07*user.age-10.5;
        }
    }
    if(vf<=10)
    {
        if(user.athlete == 0)
            vf = vf;
        else if(user.athlete == 1)
            vf = vf -2;
        else vf = vf-4;
    }
    else if(vf<21)
    {
        if(user.athlete ==0)
            vf = vf;
        else if(user.athlete ==1)
            vf = vf*0.8;
    }
    else
    {
        if(user.athlete == 0)
            vf = vf;
        else vf = vf*0.85;
    }
    
    //转换为整数
    param[0] = (int)water;
    param[1] = (int)(10*bone);
    param[2] = (int)fat;
    param[3] = (int)muscle;
    param[4] = (int)kcal;
    param[5] = (int)vf;
    
    //临界范围检查
    if(param[0]<35)
        param[0]=35;
    if(param[0]>75)
        param[0] =75;
    if(param[1]<5)
        param[1]=5;
    if(param[1]>80)
        param[1]=80;
    if(param[2]<5)
        param[2]=5;
    if(param[2]>60)
        param[2]=60;
    if(param[3]<5)
        param[3]=5;
    if(param[3]>90)
        param[3]=90;
    if(param[4]<500)
        param[4]=500;
    if(param[4]>9999)
        param[4]=9999;
    if(param[5]<1)
        param[5]=1;
    if(param[5]>50)
        param[5]=50;
    return 1;
}


int GetBMIIndex(float bmi)
{
    int bmiIndex = 0;
    if(bmi<18.5) bmiIndex=0;
    else if(bmi<24) bmiIndex=1;
    else if(bmi<28)  bmiIndex=2;
    else bmiIndex =3;
    return bmiIndex;
}




int EnterCalibFunction()
{
    char buf[25];
    static int errorCounter  = 0;
    int index =0;
    index = index;
    char statusReport[25];
    
    SetBusy(1);
    SetUartSwitch(FUNC_WEIGHT);
    SendData(PORT_SENSOR,"3--------33\n",12);
    SetBusy(0);
    
    int counter = 200;//在20ms内完成接收
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        if(CheckReceivedString(buf))
        {
            if(buf[0]=='9')
            {
                //go on in next step
                SendData(PORT_SENSOR,"5--------35\n",12);
                SendData(PORT_SENSOR,"5--------35\n",12);
            }
            else
            {
                return 0;
            }
        }
        else
        {
            errorCounter++;
            //校验错误
            return 0;
        }
    }
    else
    {
        //do nothing
        return 0;
    }
    strcpy(statusReport,"RBWC0----HL");
    statusReport[4] = '0'+0;
    statusReport[11]= 0x0a;
    AddCheckString(statusReport);
    UART2_PrintArray(statusReport,12);
    
    //如果收到等待50kg的命令说明0kg校正结束
    //继续等待30秒直到收到结果    
        
    counter = 30000;
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        if(CheckReceivedString(buf))
        {
            //继续等待4秒直到收到结果
            if(buf[0]=='8' && buf[1]=='0' && buf[2]=='5' && buf[3]=='0')
            {
                SendData(PORT_SENSOR,"5--------35\n",12);
                SendData(PORT_SENSOR,"5--------35\n",12);
                //Show PANEL_7_PICTURE_8
                
                //go on in next step
            }
            else
            {
                return 0;
            }
        }
        else
        {
            errorCounter++;
            //校验错误
            return 0;
        }
    }
    else
    {
        return 0;
    }
    strcpy(statusReport,"RBWC0----HL");
    statusReport[4] = '0'+1;
    statusReport[11]= 0x0a;
    AddCheckString(statusReport);
    UART2_PrintArray(statusReport,12);
    
    //如果收到等待100kg的命令说明50kg校正结束
    //继续等待30秒直到收到结果
    
    counter = 30000;
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        if(CheckReceivedString(buf))
        {
            //继续等待4秒直到收到结果
            if(buf[0]=='8' && buf[1]=='1' && buf[2]=='0' && buf[3]=='0')
            {
                SendData(PORT_SENSOR,"5--------35\n",12);
                SendData(PORT_SENSOR,"5--------35\n",12);
                //Show PANEL_7_PICTURE_8
                //go on in next step
            }
            else
            {
                return 0;
            }
        }
        else
        {
            errorCounter++;
            //校验错误
            return 0;
        }
    }
    else
    {
        return 0;
    }
    strcpy(statusReport,"RBWC0----HL");
    statusReport[4] = '0'+2;
    statusReport[11]= 0x0a;
    AddCheckString(statusReport);
    UART2_PrintArray(statusReport,12);
    
    //如果收到等待150kg的命令说明100kg校正结束
    //继续等待30秒直到收到结果
    
    
    counter = 30000;
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        if(CheckReceivedString(buf))
        {
            //继续等待4秒直到收到结果
            if(buf[0]=='8' && buf[1]=='1' && buf[2]=='5' && buf[3]=='0')
            {
                SendData(PORT_SENSOR,"5--------35\n",12);
                SendData(PORT_SENSOR,"5--------35\n",12);
                //Show PANEL_7_PICTURE_8
               
                //go on in next step
            }
            else
            {
                return 0;
            }
        }
        else
        {
            errorCounter++;
            //校验错误
            return 0;
        }
    }
    else
    {
        return 0;
    }
    strcpy(statusReport,"RBWC0----HL");
    statusReport[4] = '0'+3;
    statusReport[11]= 0x0a;
    AddCheckString(statusReport);
    UART2_PrintArray(statusReport,12);
    
    //如果收到等待200kg的命令说明150kg校正结束
    //继续等待30秒直到收到结果
    
    counter = 30000;
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        if(CheckReceivedString(buf))
        {
            //继续等待4秒直到收到结果
            if(buf[0]=='8' && buf[1]=='2' && buf[2]=='0' && buf[3]=='0')
            {
                SendData(PORT_SENSOR,"5--------35\n",12);
                SendData(PORT_SENSOR,"5--------35\n",12);
                //Show PANEL_7_PICTURE_8
                //go on in next step
            }
            else
            {
                return 0;
            }
        }
        else
        {
            errorCounter++;
            //校验错误
            return 0;
        }
    }
    else
    {
        return 0;
    }
    strcpy(statusReport,"RBWC0----HL");
    statusReport[4] = '0'+4;
    statusReport[11]= 0x0a;
    AddCheckString(statusReport);
    UART2_PrintArray(statusReport,12);
    
    
    //如果收到ok的命令说明200kg校正结束
    //继续等待30秒直到收到结果
    
    counter = 30000;
    while(GetUart1RxStatus(buf)==0 && counter)
    {
        counter--;
        DelayMs(1);
    }
    if(counter)
    {
        if(CheckReceivedString(buf))
        {
            //继续等待4秒直到收到结果
            if(buf[0]=='8' && buf[1]=='o' && buf[2]=='k' && buf[3]=='!')
            {
                SendData(PORT_SENSOR,"5--------35\n",12);
                SendData(PORT_SENSOR,"5--------35\n",12);
                //Show PANEL_7_PICTURE_8
                //SetCtrlVal(LoadPanel(PANEL_7),PANEL_7_PICTURE_3,0);
                
                //go on in next step
            }
            else
            {
                SendData(PORT_SENSOR,"5--------35\n",12);
                SendData(PORT_SENSOR,"5--------35\n",12);
                
                
                return 0;
            }
        }
        else
        {
            errorCounter++;
            //校验错误
            return 0;
        }
    }
    else
    {
        return 0;
    }
    strcpy(statusReport,"RBWC0----HL");
    statusReport[4] = '0'+5;
    statusReport[11]= 0x0a;
    AddCheckString(statusReport);
    UART2_PrintArray(statusReport,12);
    
    
    return 1;
}