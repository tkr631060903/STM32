

#include "includes.h"

#define ENABLE_SLIDE_SWIFT  (sys.supermode)
#define MONO_LCD    1
u16 PEN_STATUS = 0, PENX = 0, PENY =0;



//RunUserInterface运行标志
static u8 bMainUserInterfaceKeepRunning = 1;
static u16 BasePanelId = 0;
static u16 UpperPanelId = 0;
u32 SCREEN_PROTECTION_TIME = 0;

int _drawCommandButton(Gui_Control_Type_Def* control, u8 bKeyDown)
{
    LcdFillRec(control->X1,control->Y1,control->X2,control->Y2,1,control->BgColor);
#if MONO_LCD
    LcdPrintHorz(control->X1,  control->Y1,  control->X2 - control->X1+1, control->TextColor);
    LcdPrintVert(control->X1,  control->Y1,  control->Y2 - control->Y1+1, control->TextColor);
    LcdPrintHorz(control->X1,  control->Y2,  control->X2 - control->X1+1, control->TextColor);
    LcdPrintVert(control->X2,  control->Y1,  control->Y2 - control->Y1+1, control->TextColor);
#elif
    if(bKeyDown)
    {
        LcdPrintHorz(control->X1,  control->Y1,  control->X2 - control->X1+1, GRAY2);
        LcdPrintHorz(control->X1+1,control->Y1+1,control->X2 - control->X1-1, GRAY1);
        LcdPrintVert(control->X1,  control->Y1,  control->Y2 - control->Y1+1, GRAY2);
        LcdPrintVert(control->X1+1,control->Y1+1,control->Y2 - control->Y1-1, GRAY1);
        LcdPrintHorz(control->X1,  control->Y2,  control->X2 - control->X1+1, WHITE);
        LcdPrintVert(control->X2,  control->Y1,  control->Y2 - control->Y1+1, WHITE);
    }
    else
    {
        LcdPrintHorz(control->X1,  control->Y1,  control->X2 - control->X1+1, WHITE);
        LcdPrintVert(control->X1,  control->Y1,  control->Y2 - control->Y1+1, WHITE);
        LcdPrintHorz(control->X1+1,control->Y2-1,control->X2 - control->X1-1, GRAY1);
        LcdPrintHorz(control->X1,  control->Y2,  control->X2 - control->X1+1, GRAY2);
        LcdPrintVert(control->X2-1,control->Y1+1,control->Y2 - control->Y1-1, GRAY1);
        LcdPrintVert(control->X2  ,control->Y1  ,control->Y2 - control->Y1+1, GRAY2);
    }
#endif
    
    u16 left, top;
    if(control->Dimmed)
    {
        left = (control->X2+control->X1-8*strlen(control->Text))/2;
        top = (control->Y2+control->Y1-16)/2;
        LcdPrintStr(control->Text, left, top, control->TextColor,control->BgColor,0);
        LcdPrintStr(control->Text, left+1, top-1, GRAY0,control->BgColor,0);
    }
    else
    {
        left = (control->X2+control->X1-8*strlen(control->Text))/2;
        top = (control->Y2+control->Y1-16)/2;
        LcdPrintStr(control->Text, left, top, control->TextColor,control->BgColor,0);
    }
    
    return 1;
}
int _drawPictureButton(Gui_Control_Type_Def* control, u8 bKeyDown)
{
    //LcdFillRec(control->X1,control->Y1,control->X2,control->Y2,1,control->BgColor);
    /*if(bKeyDown)
    {
    LcdPrintHorz(control->X1,  control->Y1,  control->X2 - control->X1+1, GRAY2);
    LcdPrintHorz(control->X1+1,control->Y1+1,control->X2 - control->X1-1, GRAY1);
    LcdPrintVert(control->X1,  control->Y1,  control->Y2 - control->Y1+1, GRAY2);
    LcdPrintVert(control->X1+1,control->Y1+1,control->Y2 - control->Y1-1, GRAY1);
    LcdPrintHorz(control->X1,  control->Y2,  control->X2 - control->X1+1, WHITE);
    LcdPrintVert(control->X2,  control->Y1,  control->Y2 - control->Y1+1, WHITE);
}
    else
    {
    LcdPrintHorz(control->X1,  control->Y1,  control->X2 - control->X1+1, WHITE);
    LcdPrintVert(control->X1,  control->Y1,  control->Y2 - control->Y1+1, WHITE);
    LcdPrintHorz(control->X1+1,control->Y2-1,control->X2 - control->X1-1, GRAY1);
    LcdPrintHorz(control->X1,  control->Y2,  control->X2 - control->X1+1, GRAY2);
    LcdPrintVert(control->X2-1,control->Y1+1,control->Y2 - control->Y1-1, GRAY1);
    LcdPrintVert(control->X2  ,control->Y1  ,control->Y2 - control->Y1+1, GRAY2);
}*/
    long addr ;
    addr = atoi(control->Text);
    if(addr == 0)
        return 0;
    if(control->Dimmed)
    {
        DispImage(control->X1,  control->Y1, addr, control->X2-control->X1, control->Y2-control->Y1);
        //DispImage(control->X1+2,  control->Y1+2, addr, control->X2-control->X1-4, control->Y2-control->Y1-4);
    }
    else
    {
        DispImage(control->X1,  control->Y1, addr, control->X2-control->X1, control->Y2-control->Y1);
        //DispImage(control->X1+2,  control->Y1+2, addr, control->X2-control->X1-4, control->Y2-control->Y1-4);
    }
    return 1;
}
int _drawString(Gui_Control_Type_Def* control, u8 bKeyDown)
{
    u16 left, top;
    left = (control->X2+control->X1-8*strlen(control->Text))/2;
    top = (control->Y2+control->Y1-16)/2;
    
    LcdFillRec(control->X1,control->Y1,control->X2,control->Y2,1,control->BgColor);
    
#if MONO_LCD
    LcdPrintHorz(control->X1,  control->Y1,  control->X2 - control->X1+1, control->TextColor);
	LcdPrintVert(control->X1,  control->Y1,  control->Y2 - control->Y1+1, control->TextColor);
	LcdPrintHorz(control->X1,  control->Y2,  control->X2 - control->X1+1, control->TextColor);
	LcdPrintVert(control->X2,  control->Y1,  control->Y2 - control->Y1+1, control->TextColor);
    LcdPrintStr(control->Text, left, top, control->TextColor,control->BgColor,0);
    
#elif
    if(bKeyDown)
    {
        LcdPrintHorz(control->X1,  control->Y1,  control->X2 - control->X1+1, GRAY2);
        LcdPrintHorz(control->X1+1,control->Y1+1,control->X2 - control->X1-1, GRAY1);
        LcdPrintVert(control->X1,  control->Y1,  control->Y2 - control->Y1+1, GRAY2);
        LcdPrintVert(control->X1+1,control->Y1+1,control->Y2 - control->Y1-1, GRAY1);
        LcdPrintHorz(control->X1,  control->Y2,  control->X2 - control->X1+1, WHITE);
        LcdPrintVert(control->X2,  control->Y1,  control->Y2 - control->Y1+1, WHITE);
        LcdFillRec(  control->X1+2,control->Y1+2,control->X2 - 1, control->Y2-1,1,control->TextColor);
        LcdPrintStr(control->Text, left, top, control->BgColor,control->TextColor,0);
    }
    else
    {
        LcdPrintHorz(control->X1,  control->Y1,  control->X2 - control->X1+1, GRAY2);
        LcdPrintHorz(control->X1+1,control->Y1+1,control->X2 - control->X1-1, GRAY1);
        LcdPrintVert(control->X1,  control->Y1,  control->Y2 - control->Y1+1, GRAY2);
        LcdPrintVert(control->X1+1,control->Y1+1,control->Y2 - control->Y1-1, GRAY1);
        LcdPrintHorz(control->X1,  control->Y2,  control->X2 - control->X1+1, WHITE);
        LcdPrintVert(control->X2,  control->Y1,  control->Y2 - control->Y1+1, WHITE);
        LcdFillRec(  control->X1+2,control->Y1+2,control->X2 - 1, control->Y2-1,1,control->BgColor);
        LcdPrintStr(control->Text, left, top, control->TextColor,control->BgColor,0);
    }
	
#endif
    
    return 1;
}
int _drawTextMsg(Gui_Control_Type_Def* control, u8 bKeyDown)
{
    if(control->BgColor != VAL_TRANSPARENT)
        LcdFillRec(control->X1,control->Y1,control->X2,control->Y2,1,control->BgColor);
    
    u16 left, top;
    if(control->Dimmed)
    {
        left = control->X1;
        top = (control->Y2+control->Y1-16)/2;
        if(control->BgColor == VAL_TRANSPARENT)
        {
            LcdPrintStr(control->Text, left, top, control->TextColor,control->BgColor,0);
            LcdPrintStr(control->Text, left+1, top-1, GRAY0,control->BgColor,0);
        }
        else
        {
            LcdPrintStr(control->Text, left, top, control->TextColor,control->BgColor,1);
            LcdPrintStr(control->Text, left+1, top-1, GRAY0,control->BgColor,1);
        }
    }
    else
    {
        left = control->X1;
        top = (control->Y2+control->Y1-16)/2;
        if(control->BgColor == VAL_TRANSPARENT)
            LcdPrintStr(control->Text, left, top, control->TextColor,control->BgColor,0);
        else 
            LcdPrintStr(control->Text, left, top, control->TextColor,control->BgColor,1);
    }
    return 1;
}
int _drawPicture(Gui_Control_Type_Def* control, u8 bKeyDown)
{
    //LcdFillRec(control->X1,control->Y1,control->X2,control->Y2,1,control->BgColor);
    
    long addr ;
    addr = atoi(control->Text);
    if(addr == 0)
        return 0;
    //DispImage(control->X1+3,  control->Y1+3, addr, control->X2-control->X1-6, control->Y2-control->Y1-6);
    DispImage(control->X1,  control->Y1, addr, control->X2-control->X1, control->Y2-control->Y1);
    return 1;
}
int _drawDecoration(Gui_Control_Type_Def* control, u8 bKeyDown)
{
    //LcdFillRec(control->X1,control->Y1,control->X2,control->Y2,1,control->BgColor);
    //LcdFillRec(control->X1+1, control->Y1+1, control->X2  ,    control->Y2,     0, WHITE);
    //LcdFillRec(control->X1,   control->Y1,   control->X2-1,    control->Y2-1,   0, GRAY2);
    LcdFillRec(control->X1, control->Y1, control->X2-1,    control->Y2-1,   1, control->BgColor);
    return 1;
}
int _drawVSplitter(Gui_Control_Type_Def* control, u8 bKeyDown)
{
    LcdFillRec(control->X1, control->Y1, control->X2-1 ,    control->Y2,    1, control->TextColor);
    return 1;
}
int _drawHSplitter(Gui_Control_Type_Def* control, u8 bKeyDown)
{
    LcdFillRec(control->X1, control->Y1, control->X2  ,    control->Y2-1,   1, control->TextColor);
    return 1;
}

int _drawPanel(Gui_Control_Type_Def* panel)
{
    //debug("panel %d",(int)panel->BgColor);
    LcdClear(panel->BgColor);
    return 1;
}

//返回控件ID号
u16 _getActiveControl(s16 x, s16 y)
{
    u16 controlId = 0;
    u16 panelId = 0;
    s16 x1,y1,x2,y2;
    int i;
    if(UpperPanelId)
    {
        panelId = UpperPanelId;
        
        u16 width = GuiCtrl[panelId].X2 - GuiCtrl[panelId].X1;
        u16 height = GuiCtrl[panelId].Y2 - GuiCtrl[panelId].Y1;
        
        x1 = (SCREEN_X-width)/2;
        y1 = (SCREEN_Y-height)/2;
        x2 = (SCREEN_X+width)/2;
        y2 = (SCREEN_Y+height)/2;
    }
    else
    {
        panelId = BasePanelId;
        x1 = 0;
        y1 = 0;
        x2 = GuiCtrl[panelId].X2;
        y2 = GuiCtrl[panelId].Y2;
    }
    for(i = panelId+1; i<=Gui_Control_Length && GuiCtrl[i].ControlId!=NULL ;i++)
    {
        if(x>=x1+GuiCtrl[i].X1-1 && x<=x1+GuiCtrl[i].X2+1 && y>=y1+GuiCtrl[i].Y1-1 && y<=y1+GuiCtrl[i].Y2+1 && GuiCtrl[i].Dimmed ==0 && GuiCtrl[i].Visible ==1)
            controlId = i;
    }
    if(controlId ==0)
    {
        if(x>x1 && x<x2 && y>y1 && y<y2)
        {
            controlId = panelId;
        }
        else 
        {
            controlId = 0;
        }
    }
    return controlId;
}

u16 GetActivePanel()
{
    if(UpperPanelId )
        return UpperPanelId;
    else return BasePanelId;
}
u16 GetBasePanel()
{
    return BasePanelId;
}
int _drawPopupPanel(Gui_Control_Type_Def* panel)
{
    u16 width = panel->X2 - panel->X1;
    u16 height = panel->Y2 - panel->Y1;
    
    panel->X1 = (SCREEN_X-width)/2;
    panel->Y1 = (SCREEN_Y-height)/2;
    panel->X2 = (SCREEN_X+width)/2;
    panel->Y2 = (SCREEN_Y+height)/2;
    
    //    _drawPanel(panel);
    LcdFillRec(panel->X1, panel->Y1,panel->X2, panel->Y2, 1, panel->BgColor);
    
    panel->X2 = panel->X2 - panel->X1;
    panel->Y2 = panel->Y2 - panel->Y1;
    panel->X1 = 0;
    panel->Y1 = 0;
    
    return 1;
}

u16 _getControlHandle(u16 panelHandle, u16 controlId)
{
    int i;
    u16 tmpPanelHandle = 0;
    u16 controlHandle = 0;
    for (i = 1;i<=Gui_Control_Length; i++ )
    {
        if(GuiCtrl[i].ControlId==NULL)
        {
            tmpPanelHandle = i;
            continue;
        }
        if(panelHandle == tmpPanelHandle && GuiCtrl[i].ControlId == controlId)
            break;
    }
    if(i >= Gui_Control_Length+1)
        controlHandle = 0;
    else 
        controlHandle = i;
    return controlHandle;
}

int _drawControl(Gui_Control_Type_Def* control, Gui_Control_Type_Def* panelControl, u8 bKeyDown)
{
    if(control->Visible)
    {
        if(&GuiCtrl[BasePanelId] == panelControl)
            panelControl = NULL;
        if(panelControl == NULL)
        {
            // do nothing 
        }
        else
        {
            control->X1 += (SCREEN_X+panelControl->X1-panelControl->X2)/2;
            control->Y1 += (SCREEN_Y+panelControl->Y1-panelControl->Y2)/2;
            control->X2 += (SCREEN_X+panelControl->X1-panelControl->X2)/2;
            control->Y2 += (SCREEN_Y+panelControl->Y1-panelControl->Y2)/2;
        }
        switch(control->ItemType)
        {
        case TYPE_COMMANDBUTTON:
            _drawCommandButton(control,bKeyDown);
            break;
        case TYPE_PICTUREBUTTON:
            _drawPictureButton(control,bKeyDown);
            break;
        case TYPE_STRING:
            _drawString(control,bKeyDown);
            break;
        case TYPE_TEXTMSG:
            _drawTextMsg(control,bKeyDown);
            break;
        case TYPE_PICTURE:
            _drawPicture(control,bKeyDown);
            break;
        case TYPE_DECORATION:
            _drawDecoration(control,bKeyDown);
            break;
        case TYPE_VERTICAL_SPLITTER:
            _drawVSplitter(control,bKeyDown);
            break;
        case TYPE_HORIZONTAL_SPLITTER:
            _drawHSplitter(control,bKeyDown);
            break;
        default:
            break;
        }
        if(panelControl == NULL)
        {
            // do nothing 
        }
        else
        {
            control->X1 -= (SCREEN_X+panelControl->X1-panelControl->X2)/2;
            control->Y1 -= (SCREEN_Y+panelControl->Y1-panelControl->Y2)/2;
            control->X2 -= (SCREEN_X+panelControl->X1-panelControl->X2)/2;
            control->Y2 -= (SCREEN_Y+panelControl->Y1-panelControl->Y2)/2;
        }
    }
    else
    {
        //填充背景！
    }
    return 0;
}
u16 _getPrevPanelHandle(u16 panelHandle)
{
    u16 ret = panelHandle;
    int i;
    
    if(panelHandle == 1)
        return 1;
    for (i = panelHandle-1;i>=1; i-- )
    {
        if(GuiCtrl[i].ControlId==NULL)
        {
            ret = i;
            break;
        }
    }
    return ret;
}
u16 _getNextPanelHandle(u16 panelHandle)
{
    u16 ret = panelHandle;
    int i;
    if(panelHandle >= Gui_Control_Length)
        return panelHandle;
    for (i = panelHandle+1;i<=Gui_Control_Length; i++ )
    {
        if(GuiCtrl[i].ControlId==NULL)
        {
            ret = i;
            break;
        }
    }
    return ret;
}




void ForceBeep(u32 freq,float timeOfSeconds)
{
    int delayInterval =0 ;
    int delayCycle = 0;

    delayInterval = (int)(1000000*timeOfSeconds/freq/2);
    delayCycle = (int)(freq*timeOfSeconds);//响0.1秒就乘以0.1
    int i;
    for(i=0;i<delayCycle;i++)
    {
        DelayUs(delayInterval);
        GPIO_WriteBit(GPIOE, GPIO_Pin_2, (BitAction)1);
        DelayUs(delayInterval);
        GPIO_WriteBit(GPIOE, GPIO_Pin_2, (BitAction)0);
    }    
}


void QuitUserInterface()
{
    BasePanelId = 0;
    bMainUserInterfaceKeepRunning = 0;
    bMainUserInterfaceKeepRunning = bMainUserInterfaceKeepRunning;
}
//如果返回0 ，则原程序必须及时退出
int DelayWithEventProcessing (int numberOfMilliSeconds)
{
    unsigned long tmr_init, tmr_end;
    u16 tmpPanel;
    
    tmpPanel = GetActivePanel();
    
    if(numberOfMilliSeconds<10)
        numberOfMilliSeconds = 10;
    tmr_init = GetSystemTime();
    tmr_end = tmr_init + numberOfMilliSeconds/10;
    while(GetSystemTime() < tmr_end)
    {
        //ProcessSystemEvents();
    }
    
    if(tmpPanel == GetActivePanel())
        return 1;
    else return 0;
}


int DisplayPanel(u16 panelHandle)
{
    int i = 0;
    
    if(GuiCtrl[panelHandle].ControlId == NULL)
    {
        BasePanelId = panelHandle;
    }
    else 
    {
        return -1;
    }
    UpperPanelId = 0;
    if(GuiCtrl[panelHandle].Callback != NULL)
    {
        (*(controlCallbackType)GuiCtrl[panelHandle].Callback)(panelHandle, NULL, EVENT_BEFORE_PAINT,NULL,0,0);
    }
    else//没有面板回调函数
    {
    }
    _drawPanel(&GuiCtrl[panelHandle]);
    if(panelHandle+1 > Gui_Control_Length || GuiCtrl[panelHandle+1].ControlId == NULL)
    {
    }
    else
    {
        /*for(i = panelHandle+1; i<=Gui_Control_Length && GuiCtrl[i].ControlId!=NULL ;i++)
        {
        _drawControl(&GuiCtrl[i], NULL, 0);
    }
        */
        for(i = panelHandle+1; i<=Gui_Control_Length && GuiCtrl[i].ControlId!=NULL ;i++)
        {
            _drawControl(&GuiCtrl[i], NULL, 0);
        }
    }
    
    if(GuiCtrl[panelHandle].Callback != NULL)
    {
        (*(controlCallbackType)GuiCtrl[panelHandle].Callback)(panelHandle, NULL, EVENT_AFTER_PAINT,NULL,0,0);
    }
    else//没有面板回调函数
    {
    }
    return 0;
}

u16 LoadPanel(u16 panelId)
{
    int panelHandle = 0;
    int i;
    for (i = 1;i<=Gui_Control_Length; i++ )
    {
        if(GuiCtrl[i].PanelId==panelId && GuiCtrl[i].ControlId==NULL)
            break;
    }
    if(i >= Gui_Control_Length+1)
        panelHandle = 0;
    else 
        panelHandle = i;
    return panelHandle;
}
int IsHavePopup()
{
    if(UpperPanelId )
        return 1;
    else return 0;
}
int InstallPopup(u16 panelHandle)
{
    int i = 0;
    Gui_Control_Type_Def* panelControl;
    
    if(UpperPanelId)
    {
        //debug("存在冲突的popup panel\n");
        return 0;
    }
    
    if(GuiCtrl[panelHandle].ControlId == NULL)
    {
        UpperPanelId = panelHandle;
    }
    else 
    {
        return 0;
    }
    if(GuiCtrl[panelHandle].Callback != NULL)
    {
        (*(controlCallbackType)GuiCtrl[panelHandle].Callback)(panelHandle, NULL, EVENT_BEFORE_PAINT,NULL,0,0);
    }
    else//没有面板回调函数
    {
    }
    panelControl = &GuiCtrl[panelHandle];
    _drawPopupPanel(panelControl);
    if(panelHandle+1 > Gui_Control_Length || GuiCtrl[panelHandle+1].ControlId == NULL)
        return 1;
    for(i = panelHandle+1; i<=Gui_Control_Length && GuiCtrl[i].ControlId!=NULL ;i++)
    {
        _drawControl(&GuiCtrl[i], panelControl, 0);
    }
    if(GuiCtrl[panelHandle].Callback != NULL)
    {
        (*(controlCallbackType)GuiCtrl[panelHandle].Callback)(panelHandle, NULL, EVENT_AFTER_PAINT,NULL,0,0);
    }
    else//没有面板回调函数
    {
    }
    return 1;
}

void RemovePopup(u16 removePopup)
{
    removePopup = removePopup;
    UpperPanelId = 0;
    DisplayPanel(BasePanelId);
}
int RefreshPanel(u16 panel)
{
    if(panel == GetActivePanel())
    {
        if(UpperPanelId)
            InstallPopup(UpperPanelId);
        else 
            DisplayPanel(BasePanelId);
    }
    else if(panel ==BasePanelId)
    {
        DisplayPanel(BasePanelId);
        InstallPopup(UpperPanelId);
    }
    else
    {
        //do nothing
    }
    return 1;
}
int RefreshCtrl(u16 panel, u16 control)
{
    u16 controlHandle;
    controlHandle = _getControlHandle(panel, control);
    if(panel == GetActivePanel())
    {
        if(UpperPanelId)
            _drawControl(&GuiCtrl[controlHandle], &GuiCtrl[UpperPanelId] , 0);
        else 
            _drawControl(&GuiCtrl[controlHandle], NULL , 0);
    }
    if(controlHandle==0 && panel == GetActivePanel())
    {
        RefreshPanel(panel);
    }
    return 1;
}

int RefreshCtrlRect(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 panel = GetActivePanel();
    u16 controlHandle;
    controlHandle = _getActiveControl((x2+x1)/2, (y2+y1)/2);
    
    
    if(x1<GuiCtrl[panel].X1)
        x1 = GuiCtrl[panel].X1;
    if(y1<GuiCtrl[panel].Y1)
        y1 = GuiCtrl[panel].Y1;
    if(x2>GuiCtrl[panel].X2)
        x2 = GuiCtrl[panel].X2;
    if(y2>GuiCtrl[panel].Y2)
        y2 = GuiCtrl[panel].Y2;
    
    LcdFillRec(x1,y1,x2,y2,1,GuiCtrl[controlHandle].BgColor);
    return 1;
}
int SetCtrlAttribute(u16 panel, u16 control, int controlAttribute, ...)
{
    va_list ap;
    int valInt;
    char* valString =0;
    u16 controlHandle;
    va_start(ap, control);
    controlHandle = _getControlHandle(panel, control);
    if(controlHandle == 0)
        return 0;
    valString = valString;
    u16 width = 0, height =0;
    switch(controlAttribute)
    {
    case ATTR_TOP:  
        valInt = va_arg(ap, int);
        if(valInt <0 || valInt >=SCREEN_Y)
            return 0;
        height = GuiCtrl[controlHandle].Y2 - GuiCtrl[controlHandle].Y1;
        GuiCtrl[controlHandle].Y1 = valInt;
        GuiCtrl[controlHandle].Y2 = GuiCtrl[controlHandle].Y1 + height;
        break;
    case ATTR_LEFT: 
        valInt = va_arg(ap, int);
        if(valInt <0 || valInt >=SCREEN_X)
            return 0;
        width = GuiCtrl[controlHandle].X2 - GuiCtrl[controlHandle].X1;
        GuiCtrl[controlHandle].X1 = valInt;
        GuiCtrl[controlHandle].X2 = GuiCtrl[controlHandle].X1 + width;
        break;
    case ATTR_WIDTH: 
        valInt = va_arg(ap, int);
        if(valInt < 0 || GuiCtrl[controlHandle].X1 + valInt >=SCREEN_X)
            return 0;
        GuiCtrl[controlHandle].X2 = GuiCtrl[controlHandle].X1 + valInt;
        break;
    case ATTR_HEIGHT:
        valInt = va_arg(ap, int);
        if(valInt < 0 || GuiCtrl[controlHandle].Y1 + valInt >=SCREEN_Y)
            return 0;
        GuiCtrl[controlHandle].Y2 = GuiCtrl[controlHandle].Y1 + valInt;
        break;
    case ATTR_VISIBLE:
        valInt = va_arg(ap, int);
        GuiCtrl[controlHandle].Visible = (u8)valInt;
        break;
    case ATTR_DIMMED:
        valInt = va_arg(ap, int);
        GuiCtrl[controlHandle].Dimmed = (u8)valInt;
        break;
    case ATTR_TEXT_COLOR:
        valInt = va_arg(ap, int);
        GuiCtrl[controlHandle].TextColor = (u16)valInt;
        break;
    case ATTR_TEXT_BGCOLOR:
        valInt = va_arg(ap, int);
        GuiCtrl[controlHandle].BgColor = (u16)valInt;
        break;
    case ATTR_CALLBACK_DATA:
        valInt = va_arg(ap, int);
        GuiCtrl[controlHandle].Callback = (controlCallbackType)valInt;
        break;
        
    default:
        //do nothing at the moment
        break;
    }
    va_end(ap);
    switch(controlAttribute)
    {
    case ATTR_TOP:case ATTR_LEFT: case ATTR_WIDTH: case ATTR_HEIGHT:case ATTR_VISIBLE:
        //RefreshPanel(panel);
        break;
    case ATTR_DIMMED:case ATTR_TEXT_COLOR:case ATTR_TEXT_BGCOLOR:case ATTR_CALLBACK_DATA:
        //RefreshCtrl(panel, control);
        break;
        
    default:
        //do nothing at the moment
        break;
    }
    return 1;
}

int GetCtrlAttribute(u16 panel, u16 control, int controlAttribute, void* attributeValue)
{
    u16 controlHandle;
    
    controlHandle = _getControlHandle(panel, control);
    if(controlHandle == 0)
        return 0;
    switch(controlAttribute)
    {
    case ATTR_TOP:  
        *(u16 *)attributeValue = GuiCtrl[controlHandle].Y1;
        break;
    case ATTR_LEFT: 
        *(u16*)attributeValue = GuiCtrl[controlHandle].X1;
        break;
    case ATTR_WIDTH: 
        *(u16*)attributeValue = (GuiCtrl[controlHandle].X2 - GuiCtrl[controlHandle].X1);
        break;
    case ATTR_HEIGHT:
        *(u16*)attributeValue = (GuiCtrl[controlHandle].Y2 - GuiCtrl[controlHandle].Y1);
        break;
    case ATTR_VISIBLE:
        *(u8*)attributeValue = GuiCtrl[controlHandle].Visible;
        break;
    case ATTR_DIMMED:
        *(u8*)attributeValue = GuiCtrl[controlHandle].Dimmed;
        break;
    case ATTR_TEXT_COLOR:
        *(u16*)attributeValue = GuiCtrl[controlHandle].TextColor;
        break;
    case ATTR_TEXT_BGCOLOR:
        *(u16*)attributeValue = GuiCtrl[controlHandle].BgColor;
        break;
    case ATTR_CALLBACK_DATA:
        *(controlCallbackType*)attributeValue = GuiCtrl[controlHandle].Callback;
        break;
        
    default:
        //do nothing at the moment
        break;
    }
    attributeValue =attributeValue;
    return 1;
}

int SetCtrlVal (u16 panel, u16 control, ...)
{
    va_list ap;
    int valInt;
    char* valString;
    u16 controlHandle;
    va_start(ap, control);
    controlHandle = _getControlHandle(panel, control);
    if(controlHandle == 0)
        return 0;
    switch(GuiCtrl[controlHandle].ItemType)
    {
    case TYPE_COMMANDBUTTON:case TYPE_STRING:case TYPE_TEXTMSG:
        valString = va_arg(ap, char *);
        if(strlen(valString)>=sizeof(GuiCtrl[0].Text))
        {
            debug("GuiCtrl Text Overflow!\n");
            return 0;
        }
        strcpy(GuiCtrl[controlHandle].Text,valString);
        break;
    case TYPE_PICTUREBUTTON: case TYPE_PICTURE:
        valInt = va_arg(ap, int);
        char str[22];
        sprintf(str,"%d",(int)valInt);
        strcpy(GuiCtrl[controlHandle].Text,str);
        
        break;
    case TYPE_UNDEFINED: case TYPE_DECORATION:
        //do nothing
        break;
    case TYPE_PANEL: //redraw panel
        RefreshPanel(panel);
        break;
    default:
        //do nothing at the moment
        break;
    }
    va_end(ap);
    return 1;
}


int GetCtrlVal (u16 panel, u16 control, void* value)
{
    u16 controlHandle = _getControlHandle(panel, control);
    if(controlHandle == 0)
        return 0;
    switch(GuiCtrl[controlHandle].ItemType)
    {
    case TYPE_COMMANDBUTTON:case TYPE_STRING:case TYPE_TEXTMSG:
        //if(strlen(GuiCtrl[controlHandle].Text)>=sizeof((char*)value))
        //{
        //    debug("Value buf not enough!\n");
        //    return 0;
        //}
        strcpy((char*)value,GuiCtrl[controlHandle].Text);
        break;
    case TYPE_PICTUREBUTTON: case TYPE_PICTURE:
        *(int*)value = atoi(GuiCtrl[controlHandle].Text);
        break;
    case TYPE_UNDEFINED: case TYPE_DECORATION:
        value = (void*)0;
        break;
    case TYPE_PANEL: //redraw panel
        if(strlen(GuiCtrl[controlHandle].Text)>=sizeof((char*)value))
        {
            debug("Value buf not enough!\n");
            return 0;
        }
        strcpy((char*)value,GuiCtrl[controlHandle].Text);
        break;
    default:
        value = (void*)0;
        //do nothing at the moment
        break;
    }
    value = value;
    return 1;
}


int KeyPositionMapping(int event, u16 x, u16 y)
{
    PENX = x, PENY = y;
    switch(event)
    {
    case EVENT_LEFT_CLICK:
        PEN_STATUS |= 0x20;
        break;
    case EVENT_LEFT_CLICK_UP:
        PEN_STATUS |= 0x10;
        break;
    case EVENT_LONG_PRESS:
        PEN_STATUS |= 0x40;
        break;
    case EVENT_COMMIT:
        PEN_STATUS |= 0x80;
        break;
    case EVENT_ROTATE_FORWARD:
        PEN_STATUS |= 0x01;
        break;
    case EVENT_ROTATE_BACKWARD:
        PEN_STATUS &= (~0x01);
        break;
    case EVENT_ROTATE_ACCELERATE:
        PEN_STATUS |= 0x02;
        break;
    }
    return 1;
}