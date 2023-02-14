

#include "includes.h"


int   Panel7CloseCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle;
    switch(event)
    {
    case EVENT_COMMIT:
        panelHandle = LoadPanel(PANEL_8);
        RemovePopup(panelHandle);
        
        if(GetActivePanel() == LoadPanel(PANEL_4))
        {
            panelHandle = LoadPanel(PANEL_3);
            DisplayPanel(panelHandle);
        }
        
        break;
    }
    return 1;
}

int   Panel7DefaultAgeCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle;
    char str[20];
    u8 age;
    switch(event)
    {
    case EVENT_COMMIT:
        panelHandle = LoadPanel(PANEL_8);
        
        SetCtrlVal(panelHandle,PANEL_8_STRING,"waiting");
        RefreshCtrl(panelHandle,PANEL_8_STRING);
        
        GetCtrlVal(panelHandle,PANEL_8_STRING_3,str);
        
        age = atoi(str);
        if(control == PANEL_8_COMMANDBUTTON_8)
            age --;
        else
            age ++;
        
        sprintf(str,"%d",age);
        SetCtrlVal(panelHandle,PANEL_8_STRING_3, str);
        RefreshCtrl(panelHandle,PANEL_8_STRING_3);

        sys.defaultAge = age;
        if(StoreSystemParam())
        {
            SetCtrlVal(panelHandle,PANEL_8_STRING,"OK!");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
        }
        else
        {
            SetCtrlVal(panelHandle,PANEL_8_STRING,"Error");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
        }
        break;
    }
    return 1;
}




int   Panel7DefaultSexCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle;
    switch(event)
    {
    case EVENT_COMMIT:
        
        panelHandle = LoadPanel(PANEL_8);
        SetCtrlVal(panelHandle,PANEL_8_STRING,"waiting");
        RefreshCtrl(panelHandle,PANEL_8_STRING);
        
        if(control == PANEL_8_COMMANDBUTTON_10)//men
        {
            sys.defaultSex = 0;
        }
        else 
        {
            sys.defaultSex = 1;
        }
        if(StoreSystemParam())
        {   
            SetCtrlVal(panelHandle,PANEL_8_STRING,"OK!");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
        }
        else
        {
            SetCtrlVal(panelHandle,PANEL_8_STRING,"Error");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
        }
        break;
    }
    return 1;
}

int   Panel7EnterWeightCalibCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle;
    switch(event)
    {
    case EVENT_COMMIT:
        panelHandle = LoadPanel(PANEL_7);
        DisplayPanel(panelHandle);
        break;
    }
    return 1;
}
int   Panel7HeightCalibOKCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle;
    switch(event)
    {
    case EVENT_COMMIT:
        panelHandle = LoadPanel(PANEL_8);
        SetCtrlVal(panelHandle,PANEL_8_STRING,"waiting");
        RefreshCtrl(panelHandle,PANEL_8_STRING);
        
        if(SetHeightCalib())
        {
            SetCtrlVal(panelHandle,PANEL_8_STRING,"Calib Passed");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
            
        }
        else
        {
            SetCtrlVal(panelHandle,PANEL_8_STRING,"Calib failed");
            RefreshCtrl(panelHandle,PANEL_8_STRING);   
        }
        StoreSystemParam();
        
        break;
    }
    return 1;
}
int   Panel7HeightCalibCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle;
    char str[20];
    int lHeight;
    switch(event)
    {
    case EVENT_COMMIT:
        panelHandle = LoadPanel(PANEL_8);
        GetCtrlVal(panelHandle,PANEL_8_STRING_2,str);
        if(str[3]=='.')
        {
            lHeight = 1000*(str[0]-'0')+100*(str[1]-'0')+10*(str[2]-'0')+(str[4]-'0');
            if(control == PANEL_8_COMMANDBUTTON_2)
                lHeight --;
            else if(control == PANEL_8_COMMANDBUTTON_3)
                lHeight ++;
            else if(control == PANEL_8_COMMANDBUTTON_13)
                lHeight +=20;
            else if(control == PANEL_8_COMMANDBUTTON_12)
                lHeight -=20;
            if(lHeight <1200)
                lHeight = 1200;
            else if(lHeight >3000)
                lHeight = 3000;
            sprintf(str,"%c%c%c.%c",lHeight/1000%10+'0',lHeight/100%10+'0',lHeight/10%10+'0',lHeight%10+'0');
            SetCtrlVal(panelHandle,PANEL_8_STRING_2, str);
            RefreshCtrl(panelHandle,PANEL_8_STRING_2);
            sys.heightOffset = lHeight;
        }
        break;
    }
    return 1;
}

int   Panel7SetLanguageCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle;
    switch(event)
    {
    case EVENT_COMMIT:
        
        panelHandle = LoadPanel(PANEL_8);
        SetCtrlVal(panelHandle,PANEL_8_STRING,"waiting");
        RefreshCtrl(panelHandle,PANEL_8_STRING);
        
        if(control == PANEL_8_COMMANDBUTTON_5)//chinese
        {
            sys.language = 0;
            
        }
        else 
        {
            sys.language = 1;
            
        }
        if(StoreSystemParam())
        {
            u16 panelHandle;
            panelHandle = LoadPanel(PANEL_8);
            SetCtrlVal(panelHandle,PANEL_8_STRING,"OK!");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
        }
        else
        {
            u16 panelHandle;
            panelHandle = LoadPanel(PANEL_8);
            SetCtrlVal(panelHandle,PANEL_8_STRING,"Error");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
        }
        break;
    }
    return 1;
}

int EnterMaintainMode()
{
    sFLASH_LowLevel_DeInit();
    Sleep_Mode();
    return 1;
}

int   Panel7UserModeCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle;
    switch(event)
    {
    case EVENT_COMMIT:
        
        panelHandle = LoadPanel(PANEL_8);
        SetCtrlVal(panelHandle,PANEL_8_STRING,"waiting");
        RefreshCtrl(panelHandle,PANEL_8_STRING);
        
        if(control == PANEL_8_COMMANDBUTTON_14)//Admin
        {
            sys.supermode = 1;
            
        }
        else if(control == PANEL_8_COMMANDBUTTON_15)//user mode
        {
            sys.supermode = 0;
            
        }
        else 
        {
            EnterMaintainMode();
            return 0;
        }
        
        if(StoreSystemParam())
        {
            u16 panelHandle;
            panelHandle = LoadPanel(PANEL_8);
            SetCtrlVal(panelHandle,PANEL_8_STRING,"OK!");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
        }
        else
        {
            u16 panelHandle;
            panelHandle = LoadPanel(PANEL_8);
            SetCtrlVal(panelHandle,PANEL_8_STRING,"Error");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
        }
        break;
    }
    return 1;
}
//是否开启BT
int   Panel7SetBTModeCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle;
    switch(event)
    {
    case EVENT_COMMIT:
        panelHandle = LoadPanel(PANEL_8);
        SetCtrlVal(panelHandle,PANEL_8_STRING,"waiting");
        RefreshCtrl(panelHandle,PANEL_8_STRING);
        
        
        if(control == PANEL_8_COMMANDBUTTON_16)
            sys.bt = 1;
        else sys.bt = 0;
        if(StoreSystemParam())
        {
            SetCtrlVal(panelHandle,PANEL_8_STRING,"OK!");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
        }
        else
        {
            SetCtrlVal(panelHandle,PANEL_8_STRING,"Error");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
        }
        break;
    }
    return 1;
}

int   Panel7SetAudioModeCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle;
    switch(event)
    {
    case EVENT_COMMIT:
        panelHandle = LoadPanel(PANEL_8);
        SetCtrlVal(panelHandle,PANEL_8_STRING,"waiting");
        RefreshCtrl(panelHandle,PANEL_8_STRING);
        
        
        if(control == PANEL_8_COMMANDBUTTON_19)
            sys.audio = 1;
        else sys.audio = 0;
        if(StoreSystemParam())
        {
            SetCtrlVal(panelHandle,PANEL_8_STRING,"OK!");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
        }
        else
        {
            SetCtrlVal(panelHandle,PANEL_8_STRING,"Error");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
        }
        break;
    }
    return 1;
}


int   Panel7VolumeSetCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle;
    switch(event)
    {
    case EVENT_COMMIT:
        panelHandle = LoadPanel(PANEL_8);
        SetCtrlVal(panelHandle,PANEL_8_STRING,"waiting");
        RefreshCtrl(panelHandle,PANEL_8_STRING);
        
        if(control == PANEL_8_COMMANDBUTTON_21)
            sys.volume = 15;
        else sys.volume = 1;
        if(StoreSystemParam())
        {
            SetCtrlVal(panelHandle,PANEL_8_STRING,"OK!");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
        }
        else
        {
            SetCtrlVal(panelHandle,PANEL_8_STRING,"Error");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
        }
        
        break;
    }
    return 1;
}


//调节体重的大小，显示值为实际值加上这个偏移值
int   Panel7WeightOffsetAdjustCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle;
    long woffset =0;
    char str[22];
    switch(event)
    {
    case EVENT_COMMIT:
        panelHandle = LoadPanel(PANEL_8);
        GetCtrlVal(panelHandle,PANEL_8_STRING_4,str);
        if(str[3]=='.')
        {
            woffset = 100*(str[1]-'0')+10*(str[2]-'0')+(str[4]-'0');
            if(str[0]=='-')
                woffset = - woffset;
            if(control == PANEL_8_COMMANDBUTTON_27)
                woffset --;
            else 
                woffset ++;
            
            if(woffset <-999)
                woffset = -999;
            else if(woffset >999)
                woffset = 999;
            
            
            if(woffset>=0)
            {
                strcpy(str," ");
            }
            else
            {
                strcpy(str,"-");
                woffset = - woffset;
            }
            sprintf(str,"%s%c%c.%c",str,woffset/100%10+'0',woffset/10%10+'0',woffset%10+'0');
            SetCtrlVal(panelHandle,PANEL_8_STRING_4, str);
            RefreshCtrl(panelHandle,PANEL_8_STRING_4);
        }
        else
        {
            SetCtrlVal(panelHandle,PANEL_8_STRING,"error 51589");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
        }
        
        break;
    }
    return 1;
}

//保存调节结果
int   Panel7WeightOffsetSaveCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle;
    char str[22];
    long woffset = 0;
    switch(event)
    {
    case EVENT_COMMIT:
        panelHandle = LoadPanel(PANEL_8);
        
        SetCtrlVal(panelHandle,PANEL_8_STRING,"waiting");
        RefreshCtrl(panelHandle,PANEL_8_STRING);
        
        panelHandle = LoadPanel(PANEL_8);
        GetCtrlVal(panelHandle,PANEL_8_STRING_4,str);
        if(str[3]=='.')
        {
            woffset = 100*(str[1]-'0')+10*(str[2]-'0')+(str[4]-'0');
            if(str[0]=='-')
                woffset = - woffset;
            if(woffset <-999)
                woffset = -999;
            else if(woffset >999)
                woffset = 999;
            
            sys.weightOffset = woffset;
            
            if(StoreSystemParam())
            {
                SetCtrlVal(panelHandle,PANEL_8_STRING,"OK!");
                RefreshCtrl(panelHandle,PANEL_8_STRING);
            }
            else
            {
                SetCtrlVal(panelHandle,PANEL_8_STRING,"Error");
                RefreshCtrl(panelHandle,PANEL_8_STRING);
            }   
        }
        else
        {
            SetCtrlVal(panelHandle,PANEL_8_STRING,"error 51589");
            RefreshCtrl(panelHandle,PANEL_8_STRING);
        }
        break;
    }
    return 1;
}



int   Panel7Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle;
    char str[20];
    long woffset = 0;
    const  char SoftwareVersionInfo[]= {'v',BUILD_YEAR_CH2, BUILD_YEAR_CH3, BUILD_MONTH_CH0, BUILD_MONTH_CH1, BUILD_DAY_CH0, BUILD_DAY_CH1,'-', BUILD_HOUR_CH0, BUILD_HOUR_CH1, BUILD_MIN_CH0, BUILD_MIN_CH1, '\0'};
    
    switch(event)
    {
    case EVENT_IDLE:
        
        break;
    case EVENT_BEFORE_PAINT:
        panelHandle = LoadPanel(PANEL_8);
        sprintf(str,"%c%c%c.%c",sys.heightOffset/1000%10+'0',sys.heightOffset/100%10+'0',sys.heightOffset/10%10+'0',sys.heightOffset%10+'0');
        SetCtrlVal(panelHandle,PANEL_8_STRING_2, str);
        sprintf(str,"%d",sys.defaultAge);
        SetCtrlVal(panelHandle,PANEL_8_STRING_3, str);
        
        SetCtrlVal(panelHandle,PANEL_8_STRING, SoftwareVersionInfo);
        
        woffset = sys.weightOffset;
        if(woffset>=0)
        {
            strcpy(str," ");
        }
        else
        {
            strcpy(str,"-");
            woffset = - woffset;
        }
        sprintf(str,"%s%c%c.%c",str,woffset/100%10+'0',woffset/10%10+'0',woffset%10+'0');
        SetCtrlVal(panelHandle,PANEL_8_STRING_4, str);
        
        break;
    case EVENT_AFTER_PAINT:
        
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
