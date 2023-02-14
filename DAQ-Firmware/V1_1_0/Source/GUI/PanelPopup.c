

#include "includes.h"

float gCounterFlag = 0;

void PopupAutoCloseCallback()
{
    char str[3];
    u16 popHandle;
    popHandle = LoadPanel(PANEL_2);
    if(IsHavePopup())
    {
        sprintf(str,"%3.1f",gCounterFlag);
        SetCtrlVal(popHandle, PANEL_2_COMMANDBUTTON,str);
        RefreshCtrl(popHandle, PANEL_2_COMMANDBUTTON);
        if(gCounterFlag<=0)
        {
            RemovePopup(popHandle);
        }
        LcdRefresh();
    }
    if(gCounterFlag<=0)
        KillTimer(11);
    gCounterFlag-=0.14;
}

int   PanelPopupCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    switch(event)
    {
    case EVENT_IDLE:
        break;
    case EVENT_BEFORE_PAINT:
        break;
    case EVENT_AFTER_PAINT:
        LcdRefresh();
        gCounterFlag = 0.4;
        SetTimer(11,10,PopupAutoCloseCallback);
        break;
    case EVENT_COMMIT:
        break;
    case EVENT_LEFT_CLICK:
        break;
    case EVENT_LEFT_CLICK_UP:
        break;
    case EVENT_LONG_PRESS:
        break;
    }
    return 1;
}


int   Panel61OKCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    switch(event)
    {
    case EVENT_IDLE:
        break;
    case EVENT_BEFORE_PAINT:
        
        break;
    case EVENT_AFTER_PAINT:
        
        break;
    case EVENT_COMMIT:
        break;
    }
    return 1;
}

int MessageBox(char* title, char* msg)
{
    if(!sys.lcd_exist) return 0;
    if(GetActivePanel()!=GetBasePanel())
        return 0;
    u16 popHandle = LoadPanel(PANEL_2);
    SetCtrlVal(popHandle, PANEL_2_COMMANDBUTTON,"OK");
    if(title!=NULL)
        SetCtrlVal(popHandle, PANEL_2_STRING,title);
    else
        SetCtrlVal(popHandle, PANEL_2_STRING,"Attention");
    if(msg!=NULL)
        SetCtrlVal(popHandle, PANEL_2_TEXTMSG_12,msg);
    else
        SetCtrlVal(popHandle, PANEL_2_TEXTMSG_12,"PWR Alert");

    return InstallPopup(popHandle);
    
}