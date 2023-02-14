

#include "includes.h"

static u8 bNeedToMeasure = 0;
static u8 bMeasuring = 0;

int Panel31ShowWeight()
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
    DispBgImage(x0,y0,1337300,123,46);
    //LcdFillRec(x0,y0,x0+123,y0+46,1,0xFFDC);
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

int Panel31ShowHeight()
{
    const int figureAddr[]={49016,50240,50876,52028,53244,54348,55548,56904,57712,59124};
    const int figurePoint[]={306,159,288,304,276,300,339,202,353,338};
    const int dotAddr = 60476, dotPoint = 16;
    const int x0 = 505, y0 = 47;
    
    float height = user.height;
    
    
    //LcdFillRec(85,110,781,135,1,0xFFDC);
    DispBgImage(85,102,1337300,781-85,135-102);
    int leftArrow = (int)(178 + (height -100)*5.18 -13);
    if(leftArrow <85) leftArrow=85;
    if(leftArrow >785) leftArrow=785;
    SetCtrlAttribute(LoadPanel(PANEL),PANEL_PICTURE_8,ATTR_LEFT,leftArrow);
    RefreshCtrl(LoadPanel(PANEL),PANEL_PICTURE_8);
    if(user.hunit == 1)
    {
        height = 0.3937008*height; 
    }
    //LcdFillRec(x0,y0,x0+123,y0+46,1,0xFFDC);
    DispBgImage(x0,y0,1337300,123,46);
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

int   Panel31ChooseHeightUnitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    int option = 0;
    switch(event)
    {
    case EVENT_COMMIT:
        GetCtrlVal(LoadPanel(PANEL),PANEL_PICTURE_5,&option);
        int height,width;
        GetCtrlAttribute(LoadPanel(PANEL),PANEL_PICTURE_5,ATTR_WIDTH,&width);
        GetCtrlAttribute(LoadPanel(PANEL),PANEL_PICTURE_5,ATTR_HEIGHT,&height);
        if(option == 2133100)
        {
            user.hunit = 1;
            SetCtrlVal(LoadPanel(PANEL),PANEL_PICTURE_5,2133100+width*height*2);
        }
        else
        {
            user.hunit = 0;
            SetCtrlVal(LoadPanel(PANEL),PANEL_PICTURE_5,2133100);
        }
        Panel31ShowHeight();
        RefreshCtrl(LoadPanel(PANEL),PANEL_PICTURE_5);
        break;
    }
    return 1;
}
int   Panel31ChooseWeightUnitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    int option = 0;
    switch(event)
    {
    case EVENT_COMMIT:
        GetCtrlVal(LoadPanel(PANEL),PANEL_PICTURE_4,&option);
        int height,width;
        GetCtrlAttribute(LoadPanel(PANEL),PANEL_PICTURE_4,ATTR_WIDTH,&width);
        GetCtrlAttribute(LoadPanel(PANEL),PANEL_PICTURE_4,ATTR_HEIGHT,&height);
        if(option == 2105300)
        {
            user.wunit = 1;
            SetCtrlVal(LoadPanel(PANEL),PANEL_PICTURE_4,2105300+width*height*2);
        }
        else
        {
            user.wunit = 0;
            SetCtrlVal(LoadPanel(PANEL),PANEL_PICTURE_4,2105300);
        }
        Panel31ShowWeight();
        RefreshCtrl(LoadPanel(PANEL),PANEL_PICTURE_4);
        break;
    }
    return 1;
}




int   Panel31HCalibCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle;
    switch(event)
    {
    case EVENT_COMMIT:
        if(SetHeightCalib())
        {
            panelHandle = LoadPanel(PANEL_8);
            SetCtrlVal(panelHandle,PANEL_8_STRING,"Calib Passed");
            InstallPopup(panelHandle);
        }
        else
        {
            panelHandle = LoadPanel(PANEL_8);
            SetCtrlVal(panelHandle,PANEL_8_STRING,"Calib Failed");
            InstallPopup(panelHandle);   
        }
        
        break;
    }
    return 1;
}
float GetArrayMax(float* array, int numberOfArray)
{
    float max=array[0];
    int i;
    if(numberOfArray<=1)
        return 0;
    for(i=1;i<numberOfArray;i++)
    {
        if(array[i]>max)
            max = array[i];
    }
    return max;
}

float GetArrayMin(float* array, int numberOfArray)
{
    float min=array[0];
    int i;
    if(numberOfArray<=1)
        return 0;
    for(i=1;i<numberOfArray;i++)
    {
        if(array[i]<min)
            min = array[i];
    }
    return min;
}

#define HISTORY_NUMBER 15
#define HEIGHT_RESOLUTION   1
int IsHeightDataStable(float* array, int numberOfArray)
{
    float max, min;
    max = GetArrayMax(array,numberOfArray);
    min = GetArrayMin(array,numberOfArray);
    if(max && min)
    {
        if(max - min< 0.4 && min > HEIGHT_MIN && max < HEIGHT_MAX)
            return 1;
        else return 0;
    }
    else return 0;
}

int GetStableNumber(float* array)
{
    int i;
    float t = array[HISTORY_NUMBER-1];
    for(i=HISTORY_NUMBER-2;i;i--)
    {
        if(t != array[i])
            break;
        t = array[i];
    }
    return HISTORY_NUMBER-2-i;
}
int GetIntMin(int a, int b)
{
    if(a>b)
        return b;
    else return a;
}
int _startSystemMeasure()
{
    int i;
    u16 panelHandle;
    static float heightHistory[2*HISTORY_NUMBER];
    static int heightHistoryIndex = 0;
    static int index = 0;
    int ret;
    u8 bHeightDataStable = 0;
    int ulHeight =0;
    bMeasuring = 1;
    
    u32 tmr_init = 0;
    
    DisplayBatteryVoltage();
    
    panelHandle = LoadPanel(PANEL);
    
    u16 pheight,pwidth;
    GetCtrlAttribute(LoadPanel(PANEL),PANEL_PICTURE_9,ATTR_WIDTH,&pwidth);
    GetCtrlAttribute(LoadPanel(PANEL),PANEL_PICTURE_9,ATTR_HEIGHT,&pheight);
    
    if(sys.supermode)
    {
        user.weight = GetRandomFromRange(45,85);
        if(AudioNumericVoiceDigit(user.weight,1)==0)
            ;//return 0;
        if(AudioSingleVoice(19)==0)//公斤
            ;//return 0;
        if(sys.audio)
            DelayMs(300);
        
        user.height = GetRandomFromRange(150,195);
        if(AudioNumericVoiceDigit(user.height,1)==0)
            ;//return 0;
        if(AudioSingleVoice(18)==0)//厘米
            ;//return 0;
        
        user.status |= 0x02;
        user.LBM = GetRandomFromRange(15,user.weight);
    }
    else
    {
        //体重
        ///SetCtrlVal(panelHandle,PANEL_PICTURE_10,994370);
        ///RefreshCtrl(panelHandle,PANEL_PICTURE_10);
        if(AudioSingleVoiceNoWait(2)==0)//正在测量体重，请站好
            ;//return 0;
        
        SetCtrlVal(panelHandle,PANEL_PICTURE_9,2162946+pwidth*pheight*2*0);
        RefreshCtrl(panelHandle,PANEL_PICTURE_9);
        
        if(GetFirstWeightData())
        {
            weightMeasureStart = 1;
        }
        else
        {
            if(GetFirstWeightData())
            {
                weightMeasureStart = 1;
            }
            else
            {
                if(GetFirstWeightData())
                {
                    weightMeasureStart = 1;
                }
                else
                {
                    
                }
            }
        }
        
        tmr_init = GetSystemTime();
        while(weightMeasureStart)
        {
            //if(weightMeasureStart)
            
            GetNormalWeightData();
            
            if(DelayWithEventProcessing(50)==0)
            {
                bMeasuring = 0;
                return 0;
            }
            if(weightMeasureStart ==0 && user.weight <1)
            {
                if(GetFirstWeightData())
                {
                    weightMeasureStart = 1;
                }
            }
            if(weightMeasureStart ==1 && user.weight <1 && (signed long)GetSystemTime()-(signed long)gWeightUpdateTime>300)
            {
                if((signed long)GetSystemTime()-(signed long)gWeightUpdateTime>600)
                    break;
                if(GetFirstWeightData())
                {
                    weightMeasureStart = 1;
                }
            }
            
            /*
            if(user.weight ==0)
            {
            if(GetFirstWeightData())
            {
            weightMeasureStart = 1;
        }
        }*/
            //DelayMs(100);
        }
        ///////////
        int height=279,width=356,x1=176,y1=194;
        
        int ct = 0;
        for(ct = 0;ct <3;ct++)
        {
            DispBgImage(x1,y1,1337300,width,height);//PICTURE_9
            DelayMs(500);
            int ph;
            ph = LoadPanel(PANEL);
            SetCtrlVal(ph,PANEL_PICTURE_9,2162946+width*height*2*0);
            RefreshCtrl(ph,PANEL_PICTURE_9);
            DelayMs(500);
        }
        ////////////
        float soundWeight = user.weight;
        if(user.wunit == 1)
        {
            soundWeight = 2.2046226*soundWeight; 
        }
        if(AudioNumericVoiceDigit(soundWeight,1)==0);
        if(user.wunit == 1)
            AudioSingleVoiceNoWait(38);//英镑
        else
            AudioSingleVoiceNoWait(19);//公斤
            //return 0;
        
        //身高
        
        if(AudioSingleVoiceNoWait(4)==0)//正在测量身高，请站直
            ;//return 0;
        ///SetCtrlVal(panelHandle,PANEL_PICTURE_10,994370+2*225*131);
        ///RefreshCtrl(panelHandle,PANEL_PICTURE_10);
        SetCtrlVal(panelHandle,PANEL_PICTURE_9,2162946+pwidth*pheight*2*1);
        RefreshCtrl(panelHandle,PANEL_PICTURE_9);
        
        for(i=0;i<2*HISTORY_NUMBER;i++)
        {
            heightHistory[i] = i+i*0.1;
        }
        tmr_init = GetSystemTime();
        heightHistoryIndex = 0;
        
        do
        { 
            if(GetFirstHeightData())
            {
                heightHistory[heightHistoryIndex] = user.height;
                heightHistory[heightHistoryIndex+HISTORY_NUMBER] = user.height;
                heightHistoryIndex++;
                if(heightHistoryIndex>=HISTORY_NUMBER)
                    heightHistoryIndex = 0;
                //IsHeightDataStable(heightHistory+heightHistoryIndex);
            }
            if(DelayWithEventProcessing(300)==0)
            {
                bMeasuring = 0;
                return 0;
            }
            if(GetSystemTime()-tmr_init>3000)
                break;
            bHeightDataStable = IsHeightDataStable(heightHistory+(heightHistoryIndex+HISTORY_NUMBER-GetIntMin(HISTORY_NUMBER,3))%HISTORY_NUMBER,GetIntMin(HISTORY_NUMBER,3));
        }while(!bHeightDataStable);
        
        if(bHeightDataStable)
        {
            ulHeight = (int)(10 * user.height);
            user.height = (float)ulHeight /10;
            ///////////
            int height=279,width=356,x1=176,y1=194;
            
            int ct = 0;
            for(ct = 0;ct <3;ct++)
            {
                DispBgImage(x1,y1,1337300,width,height);//PICTURE_9
                DelayMs(500);
                int ph;
                ph = LoadPanel(PANEL);
                SetCtrlVal(ph,PANEL_PICTURE_9,2162946+width*height*2*1);
                RefreshCtrl(ph,PANEL_PICTURE_9);
                DelayMs(500);
            }
            ////////////
            float soundHeight = user.height;
            if(user.hunit == 1)
            {
                soundHeight = 0.3937008*height; 
            }
            if(AudioNumericVoiceDigit(soundHeight,1)==0)
                ;//return 0;
            if(user.hunit == 1)
                AudioSingleVoiceNoWait(39);
            else 
                AudioSingleVoiceNoWait(18);
        }
        //体脂
        
        if(AudioSingleVoiceNoWait(6)==0)///正在测量脂肪，请等候
            ;//return 0;
        ///SetCtrlVal(panelHandle,PANEL_PICTURE_10,994370+2*225*131*2);
        ///RefreshCtrl(panelHandle,PANEL_PICTURE_10);
        SetCtrlVal(panelHandle,PANEL_PICTURE_9,2162946+pwidth*pheight*2*2);
        RefreshCtrl(panelHandle,PANEL_PICTURE_9);
        ret = 2, index = 0;
        
        tmr_init = GetSystemTime();
        while(ret ==2 || (ret==0 && index<12))
        {
            ret = GetFirstFatData();
            if(ret == 1)
            {
                ///////////
                int height=279,width=356,x1=176,y1=194;
                
                int ct = 0;
                for(ct = 0;ct <3;ct++)
                {
                    DispBgImage(x1,y1,1337300,width,height);//PICTURE_9
                    DelayMs(500);
                    int ph;
                    ph = LoadPanel(PANEL);
                    SetCtrlVal(ph,PANEL_PICTURE_9,2162946+width*height*2*2);
                    RefreshCtrl(ph,PANEL_PICTURE_9);
                    DelayMs(500);
                }
                ////////////
                break;
                //do nothing
            }
            index ++;
            if(DelayWithEventProcessing(200)==0)
            {
                bMeasuring = 0;
                return 0;
            }
            if(GetSystemTime()-tmr_init>1800)
                break;
        }
    }
    
    
    panelHandle = LoadPanel(PANEL_6);
    DisplayPanel(panelHandle);
    /*
    panelHandle = LoadPanel(PANEL_8);
    char str[22];
    sprintf(str,"%f",user.LBM);
    SetCtrlVal(panelHandle,PANEL_8_STRING,str);
    InstallPopup(panelHandle);
    */
    bMeasuring = 0;
    return 1;
}

int Panel31Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    
    switch(event)
    {
    case EVENT_IDLE:
        
        /*
        if(weightMeasureStart)
        {
            GetNormalWeightData();
        }
        */
        if(!bMeasuring)
        {
            UpdateStatusPanel();
        }
        if(bNeedToMeasure)
        {
            bNeedToMeasure = 0 ;
            _startSystemMeasure();
        }
        break;
    case EVENT_BEFORE_PAINT:
        if(user.hunit == 0)
            SetCtrlVal(LoadPanel(PANEL),PANEL_PICTURE_5,2133100);
        else 
        {
            int height,width;
            GetCtrlAttribute(LoadPanel(PANEL),PANEL_PICTURE_5,ATTR_WIDTH,&width);
            GetCtrlAttribute(LoadPanel(PANEL),PANEL_PICTURE_5,ATTR_HEIGHT,&height);
            SetCtrlVal(LoadPanel(PANEL),PANEL_PICTURE_5,2133100+width*height*2);
        }
        if(user.wunit == 0)
            SetCtrlVal(LoadPanel(PANEL),PANEL_PICTURE_4,2105300);
        else 
        {
            int height,width;
            GetCtrlAttribute(LoadPanel(PANEL),PANEL_PICTURE_4,ATTR_WIDTH,&width);
            GetCtrlAttribute(LoadPanel(PANEL),PANEL_PICTURE_4,ATTR_HEIGHT,&height);
            SetCtrlVal(LoadPanel(PANEL),PANEL_PICTURE_4,2105300+width*height*2);
        }
        gBatteryStatusChanged = 1;
        bMeasuring = 0;
        PEN_STATUS =0;
        break;
    case EVENT_AFTER_PAINT:
        //Panel31ShowWeight();
        //Panel31ShowHeight();
        if(!bMeasuring)
        {
            UpdateStatusPanel();
        }
        bNeedToMeasure = 1;
        
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
