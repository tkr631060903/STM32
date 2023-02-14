

#include "includes.h"


void _dispQRResult()
{
    char x,y;
    const char sex[]={'m','w'};
    int leftCoor = 256, topCoor = 113;
    LcdFillRec(leftCoor,topCoor,leftCoor+16+8*29+16,topCoor+16+8*29+16,1,VAL_WHITE);
    sprintf((char*)strinbuf,"[kl]a:%d;h:%d;w:%d;l:%d;s:%c;p:%d",(int)user.age,(int)(10*user.height),(int)(10*user.weight),(int)(1000*user.LBM),sex[user.sex],(int)user.athlete);
    qrencode();
    for (x = 0; x < WD; x+=1)
    {
        for (y = 0; y < WD; y++)
        {
            if ( QRBIT(x,y) )
                //CanvasDrawRect (panelHandle,PANEL_CANVAS,MakeRect(10*(x),10*y,10,10), VAL_DRAW_FRAME_AND_INTERIOR);   
                LcdFillRec(leftCoor+16+8*y,topCoor+16+8*x,leftCoor+16+8*y+7,topCoor+16+8*x+7,1,VAL_BLACK);
            else
                LcdFillRec(leftCoor+16+8*y,topCoor+16+8*x,leftCoor+16+8*y+7,topCoor+16+8*x+7,1,VAL_WHITE);
        }
    }
    return;
}




int   Panel4RefreshQRCodeCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    switch(event)
    {
    case EVENT_COMMIT:
        _dispQRResult();
        break;
    }
    return 1;
}


int   Panel4HomepageCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
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

int   Panel4CloseCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    u16 panelHandle = 0;
    switch(event)
    {
    case EVENT_COMMIT:
        panelHandle = LoadPanel(PANEL_6);
        DisplayPanel(panelHandle);
        break;
    }
    return 1;
}

int   Panel4Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    switch(event)
    {
    case EVENT_IDLE:
        
        //UpdateStatusPanel();
        break;
    case EVENT_BEFORE_PAINT:
        gBatteryStatusChanged = 1;
        break;
    case EVENT_AFTER_PAINT:
        UpdateStatusPanel();
        _dispQRResult();
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
