

#include "includes.h"


int   Panel5SaveCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    int panelHandle;
    switch(event)
    {
    case EVENT_COMMIT:
        panelHandle = LoadPanel(PANEL_6);
        DisplayPanel(panelHandle);
        break;
    }
    return 1;
}

int EnterCalibFunction()
{
    char buf[25];
    static int errorCounter  = 0;
    int index =0;
    index = index;
    
    SetBusy(1);
    SetUartSwitch(FUNC_WEIGHT);
    SendData(PORT_SENSOR,"3--------33\n",12);
    SetBusy(0);
    
    int counter = 200;//��20ms����ɽ���
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
            //У�����
            return 0;
        }
    }
    else
    {
        //do nothing
        return 0;
    }
    
    //����յ��ȴ�50kg������˵��0kgУ������
    //�����ȴ�30��ֱ���յ����    
        
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
            //�����ȴ�4��ֱ���յ����
            if(buf[0]=='8' && buf[1]=='0' && buf[2]=='5' && buf[3]=='0')
            {
                SendData(PORT_SENSOR,"5--------35\n",12);
                SendData(PORT_SENSOR,"5--------35\n",12);
                //Show PANEL_7_PICTURE_8
                SetCtrlAttribute(LoadPanel(PANEL_7),PANEL_7_PICTURE_8,ATTR_VISIBLE,1);
                RefreshCtrl(LoadPanel(PANEL_7),PANEL_7_PICTURE_8);
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
            //У�����
            return 0;
        }
    }
    else
    {
        return 0;
    }
    
    //����յ��ȴ�100kg������˵��50kgУ������
    //�����ȴ�30��ֱ���յ����
    PEN_STATUS =0;
    if(AudioNumericVoiceDigit(50,0)==0);
    AudioSingleVoiceNoWait(19);
    
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
            //�����ȴ�4��ֱ���յ����
            if(buf[0]=='8' && buf[1]=='1' && buf[2]=='0' && buf[3]=='0')
            {
                SendData(PORT_SENSOR,"5--------35\n",12);
                SendData(PORT_SENSOR,"5--------35\n",12);
                //Show PANEL_7_PICTURE_8
                SetCtrlAttribute(LoadPanel(PANEL_7),PANEL_7_PICTURE_9,ATTR_VISIBLE,1);
                RefreshCtrl(LoadPanel(PANEL_7),PANEL_7_PICTURE_9);
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
            //У�����
            return 0;
        }
    }
    else
    {
        return 0;
    }
    
    //����յ��ȴ�150kg������˵��100kgУ������
    //�����ȴ�30��ֱ���յ����
    PEN_STATUS =0;
    if(AudioNumericVoiceDigit(100,0)==0);
    AudioSingleVoiceNoWait(19);
    
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
            //�����ȴ�4��ֱ���յ����
            if(buf[0]=='8' && buf[1]=='1' && buf[2]=='5' && buf[3]=='0')
            {
                SendData(PORT_SENSOR,"5--------35\n",12);
                SendData(PORT_SENSOR,"5--------35\n",12);
                //Show PANEL_7_PICTURE_8
                SetCtrlAttribute(LoadPanel(PANEL_7),PANEL_7_PICTURE_10,ATTR_VISIBLE,1);
                RefreshCtrl(LoadPanel(PANEL_7),PANEL_7_PICTURE_10);
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
            //У�����
            return 0;
        }
    }
    else
    {
        return 0;
    }
    
    //����յ��ȴ�200kg������˵��150kgУ������
    //�����ȴ�30��ֱ���յ����
    PEN_STATUS =0;
    if(AudioNumericVoiceDigit(150,0)==0);
    AudioSingleVoiceNoWait(19);
    
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
            //�����ȴ�4��ֱ���յ����
            if(buf[0]=='8' && buf[1]=='2' && buf[2]=='0' && buf[3]=='0')
            {
                SendData(PORT_SENSOR,"5--------35\n",12);
                SendData(PORT_SENSOR,"5--------35\n",12);
                //Show PANEL_7_PICTURE_8
                SetCtrlAttribute(LoadPanel(PANEL_7),PANEL_7_PICTURE_11,ATTR_VISIBLE,1);
                RefreshCtrl(LoadPanel(PANEL_7),PANEL_7_PICTURE_11);
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
            //У�����
            return 0;
        }
    }
    else
    {
        return 0;
    }
    
    
    //����յ�ok������˵��200kgУ������
    //�����ȴ�30��ֱ���յ����
    PEN_STATUS =0;
    if(AudioNumericVoiceDigit(200,0)==0);
    AudioSingleVoiceNoWait(19);
    
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
            //�����ȴ�4��ֱ���յ����
            if(buf[0]=='8' && buf[1]=='o' && buf[2]=='k' && buf[3]=='!')
            {
                SendData(PORT_SENSOR,"5--------35\n",12);
                SendData(PORT_SENSOR,"5--------35\n",12);
                //Show PANEL_7_PICTURE_8
                //SetCtrlVal(LoadPanel(PANEL_7),PANEL_7_PICTURE_3,0);
                SetCtrlAttribute(LoadPanel(PANEL_7),PANEL_7_PICTURE_3,ATTR_VISIBLE,1);
                RefreshCtrl(LoadPanel(PANEL_7),PANEL_7_PICTURE_3);
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
            //У�����
            return 0;
        }
    }
    else
    {
        return 0;
    }
    
    
    
    return 1;
}

int   Panel5Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    switch(event)
    {
    case EVENT_IDLE:
        //UpdateStatusPanel();
        break;
    case EVENT_BEFORE_PAINT:
        SetCtrlAttribute(LoadPanel(PANEL_7),PANEL_7_PICTURE_8,ATTR_VISIBLE,0);
        SetCtrlAttribute(LoadPanel(PANEL_7),PANEL_7_PICTURE_9,ATTR_VISIBLE,0);
        SetCtrlAttribute(LoadPanel(PANEL_7),PANEL_7_PICTURE_10,ATTR_VISIBLE,0);
        SetCtrlAttribute(LoadPanel(PANEL_7),PANEL_7_PICTURE_11,ATTR_VISIBLE,0);
        SetCtrlAttribute(LoadPanel(PANEL_7),PANEL_7_PICTURE_3,ATTR_VISIBLE,0);
        break;
    case EVENT_AFTER_PAINT:
        
        if(EnterCalibFunction()==0)
        {
            PEN_STATUS =0;
            DispErrorMsg(5);
            DisplayPanel(LoadPanel(PANEL_3));
        }
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
