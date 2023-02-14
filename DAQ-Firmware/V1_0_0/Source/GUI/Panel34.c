#include "includes.h"




int Panel34ShowWeight()
{
    const int figureAddr[]={49016,50240,50876,52028,53244,54348,55548,56904,57712,59124};
    const int figurePoint[]={306,159,288,304,276,300,339,202,353,338};
    const int dotAddr = 60476, dotPoint = 16;
    const int x0 = 115, y0 = 47;
    
    float weight = user.weight;
    
    if(user.wunit == 1)
    {
        weight = 2.2046226*weight; 
    }
    //LcdFillRec(x0,y0,x0+123,y0+46,1,0xFFDC);
    DispBgImage(x0,y0,2761122,123,46);
    long lWeight = (long)(10*weight);
    if(lWeight/1000)
        DispAlphaImage(x0,y0,figureAddr[lWeight/1000%10],figurePoint[lWeight/1000%10]);
    if(lWeight/100%10 || lWeight/1000)
        DispAlphaImage(x0+26,y0,figureAddr[lWeight/100%10],figurePoint[lWeight/100%10]);
    DispAlphaImage(x0+26*2,y0,figureAddr[lWeight/10%10],figurePoint[lWeight/10%10]);
    DispAlphaImage(x0+26*3-5,y0,dotAddr,dotPoint);
    DispAlphaImage(x0+26*4-10,y0,figureAddr[lWeight%10],figurePoint[lWeight%10]);
    return 1;
}

int Panel34ShowHeight()
{
    const int figureAddr[]={49016,50240,50876,52028,53244,54348,55548,56904,57712,59124};
    const int figurePoint[]={306,159,288,304,276,300,339,202,353,338};
    const int dotAddr = 60476, dotPoint = 16;
    const int x0 = 505, y0 = 47;
    
    float height = user.height;
    
    //LcdFillRec(85,110,781,135,1,0xFFDC);
    DispBgImage(85,102,2761122,781-85,135-102);
    int leftArrow = (int)(178 + (height -100)*5.18 -13);
    if(leftArrow <85) leftArrow=85;
    if(leftArrow >785) leftArrow=785;
    SetCtrlAttribute(LoadPanel(PANEL_6),PANEL_6_PICTURE_8,ATTR_LEFT,leftArrow);
    RefreshCtrl(LoadPanel(PANEL_6),PANEL_6_PICTURE_8);
    if(user.hunit == 1)
    {
        height = 0.3937008*height; 
    }
    //LcdFillRec(x0,y0,x0+123,y0+46,1,0xFFDC);
    DispBgImage(x0,y0,2761122,123,46);
    long lHeight = (long)(10*height);
    if(lHeight/1000)
        DispAlphaImage(x0,y0,figureAddr[lHeight/1000%10],figurePoint[lHeight/1000%10]);
    if(lHeight/100%10 || lHeight/1000)
        DispAlphaImage(x0+26,y0,figureAddr[lHeight/100%10],figurePoint[lHeight/100%10]);
    DispAlphaImage(x0+26*2,y0,figureAddr[lHeight/10%10],figurePoint[lHeight/10%10]);
    DispAlphaImage(x0+26*3-5,y0,dotAddr,dotPoint);
    DispAlphaImage(x0+26*4-10,y0,figureAddr[lHeight%10],figurePoint[lHeight%10]);
    return 1;
}



int Panel34ShowLBM()
{
    const int smallFigureAddr[]={60540,60940,61152,61528,61932,62284,62664,63092,63384,63864};
    const int smallFigurePoint[]={100,53,94,101,88,95,107,73,120,103};
    const int smallDotAddr = 64276, smallDotPoint = 4;
    const int x0_bias = -16, y0_bias = 0;
    const int x0[]={110+x0_bias,268+x0_bias,412+x0_bias,110+x0_bias,264+x0_bias,481+x0_bias};
    const int y0[]={237+y0_bias,237+y0_bias,237+y0_bias,340+y0_bias,340+y0_bias,312+y0_bias};
    
    float bmi;
    long param[6]={170,110,220,96,270,50};
    if(CalculateLBMArray(param, &bmi)==0)
        return 0;
    
    
    //显示前五个数字
    int i;
    for(i=0;i<4;i++)
    {
        if(i==1)
            continue;
        //LcdFillRec(x0[i],y0[i],x0[i]+14*3,y0[i]+21,1,0xFFDC);
        DispBgImage(x0[i],y0[i],2761122,14*3,21);
        if(param[i]/100)
            DispAlphaImage(x0[i]-30,y0[i],smallFigureAddr[param[i]/100%10],smallFigurePoint[param[i]/100%10]);
        if(param[i]/10%10 || param[i]/100)
                DispAlphaImage(x0[i]+16,y0[i],smallFigureAddr[param[i]/10%10],smallFigurePoint[param[i]/10%10]);
        DispAlphaImage(x0[i]+16*2,y0[i],smallFigureAddr[param[i]%10],smallFigurePoint[param[i]%10]);
    }
    //Bone
    i=1;
    //LcdFillRec(x0[i],y0[i],x0[i]+14*3,y0[i]+21,1,0xFFDC);
    DispBgImage(x0[i],y0[i],2761122,14*3,21);
    DispAlphaImage(x0[i],y0[i],smallFigureAddr[param[i]/10%10],smallFigurePoint[param[i]/10%10]);
    DispAlphaImage(x0[i]+16-5,y0[i],smallDotAddr,smallDotPoint);
    DispAlphaImage(x0[i]+16*2-10,y0[i],smallFigureAddr[param[i]%10],smallFigurePoint[param[i]%10]);
    //xianshi Kcal
    i=4;
    //LcdFillRec(x0[i],y0[i],x0[i]+14*4,y0[i]+21,1,0xFFDC);
    DispBgImage(x0[i],y0[i],2761122,14*4,21);
    if(param[i]/1000)
        DispAlphaImage(x0[i],y0[i],smallFigureAddr[param[i]/1000%10],smallFigurePoint[param[i]/1000%10]);
    if(param[i]/100)
        DispAlphaImage(x0[i]+16,y0[i],smallFigureAddr[param[i]%1000/100%10],smallFigurePoint[param[i]%1000/100%10]);
    if(param[i]/10)
        DispAlphaImage(x0[i]+16*2,y0[i],smallFigureAddr[param[i]/10%10],smallFigurePoint[param[i]/10%10]);
    DispAlphaImage(x0[i]+16*3,y0[i],smallFigureAddr[param[i]%10],smallFigurePoint[param[i]%10]);
    
    //显示BMI
    //LcdFillRec(x0[5],y0[5],x0[5]+14*4,y0[5]+21,1,0xFFDC);
    DispBgImage(x0[5],y0[5],2761122,14*4,21);
    int lBmi = (int)(10*bmi);
    if(lBmi/100)
        DispAlphaImage(x0[5],y0[5],smallFigureAddr[lBmi/100%10],smallFigurePoint[lBmi/100%10]);
    DispAlphaImage(x0[5]+16,y0[5],smallFigureAddr[lBmi/10%10],smallFigurePoint[lBmi/10%10]);
    DispAlphaImage(x0[5]+16*2-5,y0[5],smallDotAddr,smallDotPoint);
    DispAlphaImage(x0[5]+16*3-10,y0[5],smallFigureAddr[lBmi%10],smallFigurePoint[lBmi%10]);
    //更新BMI图片
    int bmiIndex = 0;
    bmiIndex = GetBMIIndex(bmi)%4;
    int height,width;
    GetCtrlAttribute(LoadPanel(PANEL_6),PANEL_6_PICTURE_10,ATTR_WIDTH,&width);
    GetCtrlAttribute(LoadPanel(PANEL_6),PANEL_6_PICTURE_10,ATTR_HEIGHT,&height);
    SetCtrlVal(LoadPanel(PANEL_6),PANEL_6_PICTURE_10,3529122+bmiIndex*102*103*2);
    
    GetCtrlAttribute(LoadPanel(PANEL_6),PANEL_6_PICTURE_11,ATTR_WIDTH,&width);
    GetCtrlAttribute(LoadPanel(PANEL_6),PANEL_6_PICTURE_11,ATTR_HEIGHT,&height);
    SetCtrlVal(LoadPanel(PANEL_6),PANEL_6_PICTURE_11,3613170+bmiIndex*19*247*2);
    
    RefreshCtrl(LoadPanel(PANEL_6),PANEL_6_PICTURE_10);
    RefreshCtrl(LoadPanel(PANEL_6),PANEL_6_PICTURE_11);
    
    //更新建议
    GetCtrlAttribute(LoadPanel(PANEL_6),PANEL_6_PICTURE,ATTR_WIDTH,&width);
    GetCtrlAttribute(LoadPanel(PANEL_6),PANEL_6_PICTURE,ATTR_HEIGHT,&height);
    SetCtrlVal(LoadPanel(PANEL_6),PANEL_6_PICTURE,3650714+bmiIndex*143*168*2);
    RefreshCtrl(LoadPanel(PANEL_6),PANEL_6_PICTURE);
    return 1;
}


int   Panel34HomepageCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    int panelHandle = 0;
    switch(event)
    {
    case EVENT_COMMIT:
        ResetUserInformation();
        gWeightUpdateTime = 0x7fffffff;
        panelHandle = LoadPanel(PANEL_3);
        DisplayPanel(panelHandle);
        
        break;
    }
    return 1;
}
void _printQRResult()
{
    char x,y;
    const char sex[]={'m','w'};
    char prtStr[60]="";
 
    sprintf((char*)strinbuf,"[kl]a:%d;h:%d;w:%d;l:%d;s:%c;p:%d",(int)user.age,(int)(10*user.height),(int)(10*user.weight),(int)(1000*user.LBM),sex[user.sex],(int)user.athlete);
    
    qrencode();
    for (x = 0; x+1 < WD; x+=2)
    {
        for (y = 0; y < WD; y++)
        {
            if ( QRBIT(x,y)==1 && QRBIT(x+1,y)==1 )
            {
                prtStr[y] = 219;
                prtStr[y+1]=0;
            }
            else if ( QRBIT(x,y)==1 && QRBIT(x+1,y)==0 )
            {
                prtStr[y] = 223;
                prtStr[y+1]=0;
            }
            else if ( QRBIT(x,y)==0 && QRBIT(x+1,y)==1 )
            {
                prtStr[y] = 220;
                prtStr[y+1]=0;
            }
            else 
            {
                prtStr[y] = ' ';
                prtStr[y+1]=0;
            }    
        }
        PrintQRLine(prtStr);
    }
    if(x== WD-1)
    {
        for (y = 0; y < WD; y++)
        {
            if ( QRBIT(x,y)==1  )
            {
                prtStr[y] = 223;
                prtStr[y+1]=0;
            }
            else 
            {
                prtStr[y] = ' ';
                prtStr[y+1]=0;
            }    
        }
        PrintQRLine(prtStr);
    }
    return;
}
    
    
    
int   Panel34PrintCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    //0  汉语  1 英语
    char language = sys.language;
    const char statusTextLanguageOffset = 4;
    const char *statusText[]={
	"偏瘦",
	"正常",
	"偏胖",
        "肥胖",
        "Thin",
	"Standard",
	"Overweight",
        "Obesity",
    };
    const char *tipText[]={
	"多吃营养食物，多运动促进新陈代谢。",
	"请保持现状，坚持多运动。",
	"平衡营养饮食，少吃高脂肪，高热量的食物，坚持多运动。",
        "平衡营养饮食，少吃高脂肪，高热量的食物，坚持多运动。",
        " 1.Watches your diet and eat health.\r\n 2.Eat more nutritional foods, eat less greasy food.\r\n 3.Get more exercise.",
        " Pls keep your current situation and get more exercise.",
        " 1.Please balance your diet.\r\n 2.Eat less greasy food and high-calorie food, and get more vegetable.\r\n 3.Get more exercise.",
        " 1.Please balance your diet.\r\n 2.Eat less greasy food and high-calorie food, and get more vegetable.\r\n 3.Get more exercise. \r\n 4.Make a healthy weight loss program with your doctor or fitness instructor.",
    };
    const char itemTextOffset = 14;
    const char *itemText[]={
	"健康与你常在",
        "体重：",
        "身高：",
        "人体成分",
        "体水分：",
        "肌肉量：",
        "骨  量：",
        "内脏脂肪量：",
        "脂肪率：",
        "BMI:",
        "健康提示：",
        "健康建议：",
        "**（以上数据，仅供参考，健康提示以BMI为标准，感谢您使用本产品。）",
        "新陈代谢：",
        "May health be always with you",
        "Weight:",
        "Height:",
        "Body composition",
        "Water:",
        "Muscle:",
        "Bone:",
        "Visceral Fat:",
        "Fat:",
        "BMI:",
        "Health Tips:",
        "Advice on Health:",
        "**(Above data are for reference only. Health tips are based on BMI. Thank you for using this product.)",
        "Metabolism:",
    };
    const char warningTextOffset = 1;
    const char *warningText[]={
	"健康参数测量不完整，请完整测量之后再打印",
        "Haven't finished health test, please print after you finished every test.",
    };
        
    long param[6]={170,110,220,96,270,50};
    long weight, height;
    float bmi;
    
    switch(event)
    {
    case EVENT_COMMIT:
        
        InitPrinter();
        SetPrintStyle(BOLD);
        PrintLine("%s",itemText[0+itemTextOffset*language]);
        PrintLine("");
        SetPrintStyle(0);
        
        
        
        if(CalculateLBMArray(param, &bmi)==0)
        {
            PrintLine("%s",warningText[0+warningTextOffset*language]);
            PrintLine("");
            PrintLine("");
            return 0;
        }
        //分别为 water, bone, fat, muscle, kcal, vf;
        //PrintLine("%s%f %s",itemText[1+itemTextOffset*language],((user.wunit)?(user.weight*2.2046226):(user.weight)),((user.wunit)?("lbs"):("kg")));
        char str[50];
        int len;
        sprintf(str,"%s",itemText[1+itemTextOffset*language]);
        weight = (u32)((user.wunit)?(user.weight*22.046226):(user.weight*10));
        if(weight/1000)
        {
            len = strlen(str);
            str[len]='0'+weight/1000;
            str[len+1]=0;
        }
        if(weight/100)
        {
            len = strlen(str);
            str[len]='0'+weight/100%10;
            str[len+1]=0;
        }
        
        len = strlen(str);
        str[len]='0'+weight/10%10;
        str[len+1]=0;
        
        strcat(str,".");
        len = strlen(str);
        str[len]='0'+weight%10;
        str[len+1]=0;
        strcat(str,((user.wunit)?("lbs"):("kg")));
        PrintLine(str);
        
        //PrintLine("%s%f %s",itemText[2+itemTextOffset*language],((user.hunit)?(user.height*0.3937008):(user.height)),((user.hunit)?("inch"):("cm")));
        sprintf(str,"%s",itemText[2+itemTextOffset*language]);
        height = (u32)((user.hunit)?(user.height*3.937008):(10*user.height));
        if(height/1000)
        {
            len = strlen(str);
            str[len]='0'+height/1000;
            str[len+1]=0;
        }
        if(height/100)
        {
            len = strlen(str);
            str[len]='0'+height/100%10;
            str[len+1]=0;
        }
        len = strlen(str);
        str[len]='0'+height/10%10;
        str[len+1]=0;
        strcat(str,".");
        len = strlen(str);
        str[len]='0'+height%10;
        str[len+1]=0;
        strcat(str,((user.hunit)?("inch"):("cm")));
        PrintLine(str);
        
        PrintLine("");
        PrintLine("%s",itemText[3+itemTextOffset*language]);
        PrintLine("  %s%d%%",itemText[4+itemTextOffset*language],param[0]);
        PrintLine("  %s%d%%",itemText[5+itemTextOffset*language],param[3]);
        PrintLine("  %s%d.%dkg",itemText[6+itemTextOffset*language],param[1]/10,param[1]%10);
        PrintLine("  %s%d%%",itemText[7+itemTextOffset*language],param[5]);
        PrintLine("  %s%d%%",itemText[8+itemTextOffset*language],param[2]);
        PrintLine("  %s%dkcal",itemText[13+itemTextOffset*language],param[4]);
        
        
        //PrintLine("  %s%f\r\n",itemText[9+itemTextOffset*language], bmi);
        sprintf(str,"  %s",itemText[9+itemTextOffset*language]);
        if(bmi/10)
        {
            len = strlen(str);
            str[len]='0'+((int)bmi)/10;
            str[len+1]=0;
        }
        len = strlen(str);
        str[len]='0'+((int)bmi)%10;
        str[len+1]=0;
        strcat(str,".");
        len = strlen(str);
        str[len]='0'+((int)(10*bmi))%10;
        str[len+1]=0;
        PrintLine(str);
        
        PrintLine("%s%s",itemText[10+itemTextOffset*language],statusText[GetBMIIndex(bmi)%4+statusTextLanguageOffset*language]);
        PrintLine("%s\r\n%s\r\n",itemText[11+itemTextOffset*language],tipText[GetBMIIndex(bmi)%4+statusTextLanguageOffset*language]);
        PrintLine("%s",itemText[12+itemTextOffset*language]);
        PrintLine("");
        
        _printQRResult();
        
        PrintLine("");
        PrintLine("");
        PrintLine("");
        PrintLine("");
        
        break;
    }
    return 1;
}
int   Panel34ScanCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    int panelHandle = 0;
    switch(event)
    {
    case EVENT_COMMIT:
        panelHandle = LoadPanel(PANEL_2);
        DisplayPanel(panelHandle);
        break;
    }
    return 1;
}

int   Panel34ChooseHeightUnitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    int option = 0;
    switch(event)
    {
    case EVENT_COMMIT:
        GetCtrlVal(LoadPanel(PANEL_6),PANEL_6_PICTURE_5,&option);
        int height,width;
        GetCtrlAttribute(LoadPanel(PANEL_6),PANEL_6_PICTURE_5,ATTR_WIDTH,&width);
        GetCtrlAttribute(LoadPanel(PANEL_6),PANEL_6_PICTURE_5,ATTR_HEIGHT,&height);
        if(option == 2133100)
        {
            user.hunit = 1;
            SetCtrlVal(LoadPanel(PANEL_6),PANEL_6_PICTURE_5,2133100+width*height*2);
        }
        else 
        {
            user.hunit = 0;
            SetCtrlVal(LoadPanel(PANEL_6),PANEL_6_PICTURE_5,2133100);
        }
        Panel34ShowHeight();
        RefreshCtrl(LoadPanel(PANEL_6),PANEL_6_PICTURE_5);
        break;
    }
    return 1;
}


int   Panel34ChooseWeightUnitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    int option = 0;
    switch(event)
    {
    case EVENT_COMMIT:
        
        GetCtrlVal(LoadPanel(PANEL_6),PANEL_6_PICTURE_4,&option);
        int height,width;
        GetCtrlAttribute(LoadPanel(PANEL_6),PANEL_6_PICTURE_4,ATTR_WIDTH,&width);
        GetCtrlAttribute(LoadPanel(PANEL_6),PANEL_6_PICTURE_4,ATTR_HEIGHT,&height);
        if(option == 2105300)
        {
            user.wunit = 1;
            SetCtrlVal(LoadPanel(PANEL_6),PANEL_6_PICTURE_4,2105300+width*height*2);
        }
        else
        {
            user.wunit = 0;
            SetCtrlVal(LoadPanel(PANEL_6),PANEL_6_PICTURE_4,2105300);
        }
        Panel34ShowWeight();
        RefreshCtrl(LoadPanel(PANEL_6),PANEL_6_PICTURE_4);
        break;
    }
    return 1;
}

void TestResultAudioReport()
{
    long param[6]={170,110,220,96,270,50};
    float bmi;
    const char voiceBMI[4]={14,15,13,16};
    int res = 1;
    
    if(sys.audio==0)
        return ;
    
    if(CalculateLBMArray(param, &bmi)==0)
    {
        return ;
    }
    if(user.weight && user.height)
    {
        res *= AudioSingleVoice(3);//你的体重是
        //
        float soundWeight = user.weight;
        if(user.wunit == 1)
        {
            soundWeight = 2.2046226*soundWeight; 
        }
        res *= AudioNumericVoiceDigit(soundWeight,1);
        if(user.wunit == 1)
            res *= AudioSingleVoice(38);//英镑
        else
            res *= AudioSingleVoice(19);//公斤
        //
        if(res ==0) return;
        DelayMs(300);
        
        
        
        res *= AudioSingleVoice(5);//你的身高是
        //
        float soundHeight = user.height;
        if(user.hunit == 1)
        {
            soundHeight = 0.3937008*soundHeight; 
        }
        res *= AudioNumericVoiceDigit(soundHeight,1);
        if(user.hunit == 1)
            res *= AudioSingleVoice(39);
        else 
            res *= AudioSingleVoice(18);
        //
        if(res ==0) return;
        DelayMs(300);
    }
    else return;
    
    //分别为 water, bone, fat, muscle, kcal, vf;
    res *= AudioSingleVoice(9);//水分
    res *= AudioNumericVoiceDigit(param[0],1);
    if(res ==0) return;
    DelayMs(300);
    
    
    res *= AudioSingleVoice(10);//骨骼量
    res *= AudioNumericVoiceDigit((float)param[1]/10.0,1);
    res *= AudioSingleVoice(19);//公斤
    if(res ==0) return;
    DelayMs(300);
    
    res *= AudioSingleVoice(8);//脂肪
    res *= AudioNumericVoiceDigit(param[2],1);
    if(res ==0) return;
    DelayMs(300);
    
    res *= AudioSingleVoice(11);//肌肉
    res *= AudioNumericVoiceDigit(param[3],1);
    if(res ==0) return;
    DelayMs(300);
    
    res *= AudioSingleVoice(12);//新陈代谢
    res *= AudioNumericVoiceDigit(param[4],1);
    res *= AudioSingleVoice(34);//kcal
    if(res ==0) return;
    DelayMs(300);
    
    res *= AudioSingleVoice(7);//BMI
    res *= AudioNumericVoiceDigit(bmi,1);
    if(res ==0) return;
    DelayMs(300);
    
    res *= AudioSingleVoice(voiceBMI[GetBMIIndex(bmi)%4]);//BMI结果
    if(res ==0) return;
    DelayMs(300);
    
    res *= AudioSingleVoice(17);//请打印凭条或者取二维码
    if(res ==0) return;
    DelayMs(300);
}

int   Panel34Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    int height=50, width=139;
    static u8 bBLEMsgSent = 0;
    switch(event)
    {
    case EVENT_IDLE:
        UpdateStatusPanel();
        if(sys.bt)
        {
            if(bBLEMsgSent ==0)
            {
                if(GetSystemTime() % 300==150)
                {
                    SendTestResultToBLE(0);
                }
                if(CheckBLEMsg()>=0)
                {
                    bBLEMsgSent =1;
                }
            }
            else if(bBLEMsgSent ==1)
            {
                if(GetSystemTime() % 300==152)
                {
                    SendTestResultToBLE(1);
                }
                if(CheckBLEMsg()>=0)
                {
                    bBLEMsgSent =2;
                }
            }
        }
        
        break;
    case EVENT_BEFORE_PAINT:
        if(user.hunit == 0)
            SetCtrlVal(LoadPanel(PANEL_6),PANEL_6_PICTURE_5,2133100);
        else 
        {
            SetCtrlVal(LoadPanel(PANEL_6),PANEL_6_PICTURE_5,2133100+width*height*2);
        }
        if(user.wunit == 0)
            SetCtrlVal(LoadPanel(PANEL_6),PANEL_6_PICTURE_4,2105300);
        else 
        {
            SetCtrlVal(LoadPanel(PANEL_6),PANEL_6_PICTURE_4,2105300+width*height*2);
        }
        gBatteryStatusChanged = 1;
        PEN_STATUS =0;
        break;
    case EVENT_AFTER_PAINT:
        UpdateStatusPanel();
        
        Panel34ShowWeight();
        Panel34ShowHeight();
        Panel34ShowLBM();
        
        if(user.status & 0x10)
        {
        }
        else
        {
            user.status |= 0x10;
            TestResultAudioReport();
            bBLEMsgSent = 0;
            SendTestResultToBLE(0);
        }
        
        break;
    
    case EVENT_COMMIT:
        break;
    case EVENT_LEFT_CLICK:
        break;
    case EVENT_LEFT_CLICK_UP:
        break;
    }
    return 1;
}
