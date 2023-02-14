

#include "includes.h"

#define ENABLE_SLIDE_SWIFT  (sys.supermode)


//RunUserInterface���б�־
static u8 bMainUserInterfaceKeepRunning = 1;
static u16 BasePanelId = 0;
static u16 UpperPanelId = 0;
static u8 bSleepMode = 0;

int _drawCommandButton(Gui_Control_Type_Def* control, u8 bKeyDown)
{
    LcdFillRec(control->X1,control->Y1,control->X2,control->Y2,1,control->BgColor);
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
    //LcdPrintStr(control->Text, left+1, top-1, GRAY0,control->BgColor,0);
    
    return 1;
}
int _drawTextMsg(Gui_Control_Type_Def* control, u8 bKeyDown)
{
    LcdFillRec(control->X1,control->Y1,control->X2,control->Y2,1,control->BgColor);
    
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
    LcdFillRec(control->X1+1, control->Y1+1, control->X2-1,    control->Y2-1,   1, control->BgColor);
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

//���ؿؼ�ID��
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
        if(x>=x1+GuiCtrl[i].X1-1 && x<=x1+GuiCtrl[i].X2+1 && y>=y1+GuiCtrl[i].Y1-1 && y<=y1+GuiCtrl[i].Y2+1 && GuiCtrl[i].Dimmed ==0 && GuiCtrl[i].Visible ==1 && GuiCtrl[i].ItemType!=TYPE_DECORATION)
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
    LcdClearRect(panel->BgColor,panel->X1, panel->Y1,panel->X2, panel->Y2);
    
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
        //��䱳����
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
//0 δ˯��  1 ǳ˯��  2 ��˯��
int IsSleepMode()
{
    return (int)bSleepMode;
}

int ExitSleepMode()
{
    bSleepMode = 0;
    PWM1_duty_cycle(50);
    return 1;
}
int EnterSleepMode(u8 sleepmode)
{
    if(sleepmode ==1)
    {
        PWM1_duty_cycle(252);
        bSleepMode = 1;
    }
    else 
    {
        PWM1_duty_cycle(255);
        bSleepMode = 2;
    }
    return 1;
}


int ProcessSystemEvents ()
{
    static u16 keyDownControlId = 0 ; 
    u16 keyUpControlId = 0 ; 
    static u8 recentMotion[2*MOTION_DETECTION_NUMBER];
    static u8 recentMotionIndex = 0;
    static long lastTouchTmr = 400;
    u16 ph;
    
    
     ////PEN_STATUS: ����״̬,16��,4��,8��,1��,2��,64 ����
    if (PEN_STATUS & 0x20)//����
    {
        PEN_STATUS = 0;
        keyDownControlId = _getActiveControl(PENX, PENY);
        //���»�û������
        if(keyDownControlId)
        {
            //2015-10-31 �ݲ�֧�ְ���֮���Ч��������
            //Ĭ�ϰ���Ч��
            if(GuiCtrl[keyDownControlId].ControlId!= NULL && (GuiCtrl[keyDownControlId].ItemType ==TYPE_COMMANDBUTTON || GuiCtrl[keyDownControlId].ItemType ==TYPE_STRING) )//�ؼ���֧�ְ���֮���Ч��
            {
                if(UpperPanelId)
                    _drawControl(&GuiCtrl[keyDownControlId],&GuiCtrl[UpperPanelId],1);
                else
                    _drawControl(&GuiCtrl[keyDownControlId],&GuiCtrl[BasePanelId],1);
            }
            else//panel ��֧�ְ��µ�Ĭ��Ч��
            {
            }
            
            if(GuiCtrl[keyDownControlId].Callback != NULL)
            {
                (*(controlCallbackType)GuiCtrl[keyDownControlId].Callback)(GetActivePanel(), GuiCtrl[keyDownControlId].ControlId, EVENT_LEFT_CLICK,NULL,PENX,PENY);
            }
            else//������û�лص�����
            {
            }
        }
        else//δ������ָ���ؼ�
        {
        }
        /*
        long i=0;
        do//�ȴ������ɿ�
        {
            i++;
            Waitus(2200);//
            if(i>2200) break;
        }while(PEN_STATUS==32);
        */
    }
    if(PEN_STATUS & 0x40)//����
    {
        u16 controlId;
        PEN_STATUS = 0;
        controlId = _getActiveControl(PENX, PENY);
        //���»�û������
        if(keyDownControlId)
        {            
            if(GuiCtrl[controlId].Callback != NULL)
            {
                (*(controlCallbackType)GuiCtrl[controlId].Callback)(GetActivePanel(), GuiCtrl[controlId].ControlId, EVENT_LONG_PRESS,NULL,PENX,PENY);
            }
            else//������û�лص�����
            {
            }
        }
        else//δ������ָ���ؼ�
        {
        }
    }
    
    if(PEN_STATUS & 0x1F)
    {
        lastTouchTmr = GetSystemTime();
        if(IsSleepMode())
        {
            PENX = 800;
            PENY = 480;
            PEN_STATUS = 0;
            ExitSleepMode();
            return 0;
        }
        
        
        if(PEN_STATUS == 16)
        {
            keyUpControlId = _getActiveControl(PENX, PENY);
            
            
            //2015-10-31 �ݲ�֧�ְ���֮���Ч��������
            if(keyDownControlId)
            {
                //Ĭ�ϵ���Ч��
                if(GuiCtrl[keyDownControlId].ControlId!= NULL && (GuiCtrl[keyDownControlId].ItemType ==TYPE_COMMANDBUTTON || GuiCtrl[keyDownControlId].ItemType ==TYPE_STRING))//�ؼ���֧�ְ���֮���Ч��
                {
                    if(UpperPanelId)
                        _drawControl(&GuiCtrl[keyDownControlId],&GuiCtrl[UpperPanelId],0);
                    else
                        _drawControl(&GuiCtrl[keyDownControlId],&GuiCtrl[BasePanelId],0);
                }
                else//panel ��֧�ְ��µ�Ĭ��Ч��
                {
                }
            }
            
            //���µ�����
            if(keyUpControlId)
            {
                if(GuiCtrl[keyDownControlId].Callback != NULL)
                {
                    (*(controlCallbackType)GuiCtrl[keyDownControlId].Callback)(GetActivePanel(), GuiCtrl[keyDownControlId].ControlId, EVENT_LEFT_CLICK_UP,NULL,PENX,PENY);
                    if(keyUpControlId == keyDownControlId)
                    {
                        (*(controlCallbackType)GuiCtrl[keyUpControlId].Callback)(GetActivePanel(), GuiCtrl[keyUpControlId].ControlId, EVENT_COMMIT,NULL,PENX,PENY);
                    }
                    
                }
                else//������û�лص�����
                {
                }
                
            }
            else//δ������ָ���ؼ�
            {
            }
        }
        else
        {
            //////����״̬,32���£�16��,4��,8��,1��,2��
            if(PEN_STATUS ==4)
            {
                if(GetActivePanel() == LoadPanel(PANEL_4))
                {
                    ph = LoadPanel(PANEL_8);
                    SetCtrlVal(ph,PANEL_8_STRING, "");
                    InstallPopup(ph);
                }
                else if(ENABLE_SLIDE_SWIFT)
                {
                    int panel = _getNextPanelHandle(BasePanelId);
                    DisplayPanel(panel);
                }
                debug("��\r\n");
            }
            else if(PEN_STATUS ==8)
            {
                if(ENABLE_SLIDE_SWIFT)
                {
                    int panel = _getPrevPanelHandle(BasePanelId);
                    DisplayPanel(panel);
                }
                debug("�һ�\r\n");
            }
            else if(PEN_STATUS ==1)
                debug("�ϻ�\r\n");
            else if(PEN_STATUS ==2)
                debug("�»�\r\n");
            recentMotion[recentMotionIndex]=PEN_STATUS;
            recentMotion[recentMotionIndex+MOTION_DETECTION_NUMBER]=PEN_STATUS;
            recentMotionIndex++;
            if(recentMotionIndex>=MOTION_DETECTION_NUMBER)
                recentMotionIndex=0;
            if(PEN_STATUS == 4)//��ȷ��
            {
                MotionDetectedCallback(recentMotion+recentMotionIndex);
            }
        }
        keyDownControlId = 0;
        PENX = 800;
        PENY = 480;
        PEN_STATUS = 0;
        
    }
    else
    {
        if(IsSleepMode()==0 && (signed long)GetSystemTime() - lastTouchTmr >SCREEN_PROTECTION_TIME)
        {
            EnterSleepMode(1);
        }
        if(IsSleepMode()==1 && (signed long)GetSystemTime() - lastTouchTmr  >SCREEN_PROTECTION_TIME*1.2)
        {
            EnterSleepMode(2);
        }
    }
    if(UpperPanelId)
    {
        if(GuiCtrl[UpperPanelId].Callback != NULL)
        {
            (*(controlCallbackType)GuiCtrl[UpperPanelId].Callback)(UpperPanelId, NULL, EVENT_IDLE,NULL,0,0);
        }
        else//û�����ص�����
        {
        }
    }
    if(GuiCtrl[BasePanelId].Callback != NULL)
    {
        (*(controlCallbackType)GuiCtrl[BasePanelId].Callback)(BasePanelId, NULL, EVENT_IDLE,NULL,0,0);
    }
    else//û�����ص�����
    {
    }
    return 0;
}

void RunUserInterface()
{
    while(bMainUserInterfaceKeepRunning)
    {
        ProcessSystemEvents ();
        //KeyParse();//��ť����
    }
}

void QuitUserInterface()
{
    BasePanelId = 0;
    bMainUserInterfaceKeepRunning = 0;
}
//�������0 ����ԭ������뼰ʱ�˳�
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
        ProcessSystemEvents();
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
    else//û�����ص�����
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
    else//û�����ص�����
    {
    }
    statusPanelRefreshNeeded = 1;
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
int InstallPopup(u16 panelHandle)
{
    int i = 0;
    Gui_Control_Type_Def* panelControl;
    
    if(UpperPanelId)
    {
        //debug("���ڳ�ͻ��popup panel\n");
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
    else//û�����ص�����
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
    else//û�����ص�����
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


