

#include "includes.h"



int   PanelDashboardCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    switch(event)
    {
    case EVENT_IDLE:
        break;
    case EVENT_BEFORE_PAINT:
        
        break;
    case EVENT_AFTER_PAINT:
        LcdRefresh();
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
/*
typedef struct
{
    u8 ch1_state;
    float ch1_vset;
    float ch1_iset;
    float ch1_vrt;
    float ch1_irt;
    float ch2_vset;
    float ch2_iset;
    float ch2_vrt;
    float ch2_irt;
    u32 updateCheck;
}PowerStatusDef;*/

int UpdatePowerDashboard(PowerStatusDef* ps)
{
    char strbuf[40];
    u16 panelHandle = GetBasePanel();
    if(panelHandle != LoadPanel(PANEL))
        return 0;
    if(IsHavePopup())
        return 0;
    if(ps->ch_state[0])
    {
        SetCtrlVal(panelHandle,PANEL_TEXTMSG,"On");
        if(ps->ch_vrt[0]<-999) strcpy(strbuf,"---");
        else sprintf(strbuf,"%4.1f V",ps->ch_vrt[0]);
        SetCtrlVal(panelHandle,PANEL_STRING_4,strbuf);
        if(ps->ch_irt[0]<-999) strcpy(strbuf,"---");
        else sprintf(strbuf,"%4.2f A",ps->ch_irt[0]);
        SetCtrlVal(panelHandle,PANEL_STRING_3,strbuf);
    }
    else
    {
        SetCtrlVal(panelHandle,PANEL_TEXTMSG,"Off");
        if(ps->ch_vset[0]<-999) strcpy(strbuf,"---");
        else sprintf(strbuf,"%4.1f V",ps->ch_vset[0]);
        SetCtrlVal(panelHandle,PANEL_STRING_4,strbuf);
        if(ps->ch_iset[0]<-999) strcpy(strbuf,"---");
        else sprintf(strbuf,"%4.2f A",ps->ch_iset[0]);
        SetCtrlVal(panelHandle,PANEL_STRING_3,strbuf);
    }
    
    if(ps->ch_state[1])
    {
        SetCtrlVal(panelHandle,PANEL_TEXTMSG_2,"On");
        if(ps->ch_vrt[1]<-999) strcpy(strbuf,"---");
        else sprintf(strbuf,"%4.1f V",ps->ch_vrt[1]);
        SetCtrlVal(panelHandle,PANEL_STRING,strbuf);
        if(ps->ch_irt[1]<-999) strcpy(strbuf,"---");
        else sprintf(strbuf,"%4.2f A",ps->ch_irt[1]);
        SetCtrlVal(panelHandle,PANEL_STRING_2,strbuf);
    }
    else
    {
        SetCtrlVal(panelHandle,PANEL_TEXTMSG_2,"Off");
        if(ps->ch_vset[1]<-999) strcpy(strbuf,"---");
        else sprintf(strbuf,"%4.1f V",ps->ch_vset[1]);
        SetCtrlVal(panelHandle,PANEL_STRING,strbuf);
        if(ps->ch_iset[1]<-999) strcpy(strbuf,"---");
        else sprintf(strbuf,"%4.2f A",ps->ch_iset[1]);
        SetCtrlVal(panelHandle,PANEL_STRING_2,strbuf);
    }
    RefreshPanel(panelHandle);
    
    return LcdRefresh();
}