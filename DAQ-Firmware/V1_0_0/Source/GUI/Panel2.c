

#include "includes.h"





int Panel2ClearAge()
{
    ///RefreshCtrl(LoadPanel(PANEL_3),PANEL_3_PICTURE_5);
    ///RefreshCtrl(LoadPanel(PANEL_3),PANEL_3_PICTURE_6);
    ///RefreshCtrl(LoadPanel(PANEL_3),PANEL_3_PICTURE_7);
    ///RefreshCtrl(LoadPanel(PANEL_3),PANEL_3_PICTURE_8);
    DispBgImage(697,211,1024,89,57);
    return 1;
}
int Panel2ShowAge(u8 age)
{
    //const int bigFigureAddr[]={1644210,1648842,1651362,1655846,1660378,1664410,1668934,1674086,1677234,1682622};
    //const int bigFigurePoint[]={1158,630,1121,1133,1008,1131,1288,787,1347,1278};
    const int smallFigureAddr[]={40000,40968,41504,42436,43368,44232,45148,46188,46828,47944};
    const int smallFigurePoint[]={242,134,233,233,216,229,260,160,279,268};
    
    age = age%100;
    
    int smallTop = 211, smallLeft = 697;
    ///GetCtrlAttribute(LoadPanel(PANEL_3),PANEL_3_PICTURE_5,ATTR_TOP,&bigTop);
    ///GetCtrlAttribute(LoadPanel(PANEL_3),PANEL_3_PICTURE_5,ATTR_LEFT,&bigLeft);
    ///GetCtrlAttribute(LoadPanel(PANEL_3),PANEL_3_PICTURE_6,ATTR_TOP,&smallTop);
    ///GetCtrlAttribute(LoadPanel(PANEL_3),PANEL_3_PICTURE_6,ATTR_LEFT,&smallLeft);
    ///DispAlphaImage(52+bigLeft,66+bigTop,bigFigureAddr[age/10%10],bigFigurePoint[age/10%10]);
    ///DispAlphaImage(52+bigLeft+46,66+bigTop,bigFigureAddr[age%10],bigFigurePoint[age%10]);
    
    int height=57;
    //GetCtrlAttribute(LoadPanel(PANEL_3),PANEL_3_PICTURE_6,ATTR_HEIGHT,&height);
    DispAlphaImage(22+smallLeft,height/2-13+smallTop,smallFigureAddr[age/10%10],smallFigurePoint[age/10%10]);
    DispAlphaImage(22+smallLeft+24,height/2-13+smallTop,smallFigureAddr[age%10],smallFigurePoint[age%10]);
    return 1;
}
int Panel2ShowSex()
{
    RefreshCtrl(LoadPanel(PANEL_3),PANEL_3_PICTURE_9);
    RefreshCtrl(LoadPanel(PANEL_3),PANEL_3_PICTURE_10);
    return 1;
}
int Panel2SetSex(UserConfigDef* pUser)
{
    if(pUser->sex)//women
    {
        SetCtrlVal(LoadPanel(PANEL_3),PANEL_3_PICTURE_9,769024+215*261*2);
        SetCtrlVal(LoadPanel(PANEL_3),PANEL_3_PICTURE_10,993484+215*261*2);
    }
    else
    {
        SetCtrlVal(LoadPanel(PANEL_3),PANEL_3_PICTURE_9,769024);
        SetCtrlVal(LoadPanel(PANEL_3),PANEL_3_PICTURE_10,993484);
    }
    
    return 1;
}
int Panel2SetAthlete(UserConfigDef* pUser)
{
    if(pUser->athlete==0)
    {
        SetCtrlVal(LoadPanel(PANEL_3),PANEL_3_PICTURE_12,1243136+59*133*2*1);
        SetCtrlVal(LoadPanel(PANEL_3),PANEL_3_PICTURE_13,1274524+59*133*2*0);
        SetCtrlVal(LoadPanel(PANEL_3),PANEL_3_PICTURE_14,1305912+59*133*2*0);
    }
    else if(pUser->athlete==1)
    {
        SetCtrlVal(LoadPanel(PANEL_3),PANEL_3_PICTURE_12,1243136+59*133*2*0);
        SetCtrlVal(LoadPanel(PANEL_3),PANEL_3_PICTURE_13,1274524+59*133*2*1);
        SetCtrlVal(LoadPanel(PANEL_3),PANEL_3_PICTURE_14,1305912+59*133*2*0);
    }
    else
    {
        SetCtrlVal(LoadPanel(PANEL_3),PANEL_3_PICTURE_12,1243136+59*133*2*0);
        SetCtrlVal(LoadPanel(PANEL_3),PANEL_3_PICTURE_13,1274524+59*133*2*0);
        SetCtrlVal(LoadPanel(PANEL_3),PANEL_3_PICTURE_14,1305912+59*133*2*1);
    }
    return 1;
}

int Panel2ShowAthlete()
{
    RefreshCtrl(LoadPanel(PANEL_3),PANEL_3_PICTURE_12);
    RefreshCtrl(LoadPanel(PANEL_3),PANEL_3_PICTURE_13);
    RefreshCtrl(LoadPanel(PANEL_3),PANEL_3_PICTURE_14);
    return 1;
}
int Panel2ShowLanguage()
{
    RefreshCtrl(LoadPanel(PANEL_3),PANEL_3_PICTURE_11);
    return 1;
}
int Panel2SetLanguage(SystemStatusDef* conf)
{
    int addr,height,width;
    GetCtrlVal(LoadPanel(PANEL_3),PANEL_3_PICTURE_11,&addr);
    GetCtrlAttribute(LoadPanel(PANEL_3),PANEL_3_PICTURE_11,ATTR_WIDTH,&width);
    GetCtrlAttribute(LoadPanel(PANEL_3),PANEL_3_PICTURE_11,ATTR_HEIGHT,&height);
    if(conf->language==1)//0中文  1 英文  
    {
        SetCtrlVal(LoadPanel(PANEL_3),PANEL_3_PICTURE_11,1217944+2*width*height*0);
    }
    else
    {
        SetCtrlVal(LoadPanel(PANEL_3),PANEL_3_PICTURE_11,1217944+2*width*height*1);
    }
    return 1;
}
int   Panel2AgeChangedCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    switch(event)
    {
    case EVENT_COMMIT:
        if(control == PANEL_3_PICTURE_8)
        {
            if(user.age<=0)
                break;
            user.age--;
            Panel2ClearAge();
            Panel2ShowAge(user.age);
        }
        else
        {
            if(user.age>=99)
                break;
            user.age++;
            Panel2ClearAge();
            Panel2ShowAge(user.age);
        }
        
        break;
    case EVENT_LONG_PRESS:
        Panel2AgeChangedCallback(panel, control, EVENT_COMMIT, callbackData, eventData1, eventData2);
        break;
    }
    return 1;
}

unsigned char bl = 0;
int   Panel2ChooseSexFemaleCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    
    switch(event)
    {
    case EVENT_COMMIT:
        user.sex=1;
        Panel2SetSex(&user);
        Panel2ShowSex();
        
        
        
        break;
    }
    return 1;
}

int   Panel2ChooseSexMaleCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    
    switch(event)
    {
    case EVENT_COMMIT:
        user.sex=0;
        Panel2SetSex(&user);
        Panel2ShowSex();
        
        
        
        break;
    }
    
    return 1;
}
int   Panel2OKCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    switch(event)
    {
    case EVENT_COMMIT:
        //添加数据保存代码
        debug("Confirm\r\n");
        DisplayPanel(LoadPanel(PANEL));
        
        break;
    }
    return 1;
}

int   Panel2LanguageSelectionCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    switch(event)
    {
    case EVENT_COMMIT: 
        if(sys.language == 1)//yingwen
            sys.language = 0;
        else
            sys.language = 1;
        Panel2SetLanguage(&sys);
        Panel2ShowLanguage();
        DisplayPanel(LoadPanel(PANEL_3));
        break;
    }
    return 1;
}
int   Panel2AthleteSelectCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    switch(event)
    {
    case EVENT_COMMIT:
        if(control == PANEL_3_PICTURE_12)
            user.athlete = 0;
        else if(control == PANEL_3_PICTURE_13)
            user.athlete = 1;
        else user.athlete = 2;
        Panel2SetAthlete(&user);
        Panel2ShowAthlete();
        break;
    }
    return 1;
}
void AudioCoreProc()
{
    KillTimer(3);
    return;
}

int   Panel2Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    switch(event)
    {
    case EVENT_IDLE:
        
        UpdateStatusPanel();
        break;
    case EVENT_BEFORE_PAINT:
        //ResetUserInformation();
        Panel2SetSex(&user);
        Panel2SetAthlete(&user);
        Panel2SetLanguage(&sys);
        gBatteryStatusChanged = 1;
        PEN_STATUS =0;
        break;
    case EVENT_AFTER_PAINT:
        Panel2ShowAge(user.age);
        UpdateStatusPanel();
        
        
        
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
