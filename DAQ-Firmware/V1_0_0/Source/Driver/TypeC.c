

#include "includes.h"


const char orKey[24]={166,237,184,196,143,228,159,20,187,107,92,4,210,110,40,100,76,4,164,44,241,124,157,179};
#define CHECK_BYTES 24

void Get_Device_Guid(char* str)
{
    u32 Device_Id0,Device_Id1,Device_Id2;
    u32 tmp[8];
    
    tmp[0]=0x1FFFF7E8-0x12345678;
    tmp[1]=0x12345678;
    tmp[0]=tmp[0];
    tmp[1]=tmp[1];
    
    tmp[2]=0x1FFFF7EC-0x07654321;
    tmp[3]=0x07654321;
    tmp[2]=tmp[2];
    tmp[3]=tmp[3];
    
    tmp[4]=0x1FFFF7F0-0x12436587;
    tmp[5]=0x12436587;
    tmp[4]=tmp[4];
    tmp[5]=tmp[5];
    
    Device_Id0 = *(vu32*)(tmp[0]+tmp[1]);
    Device_Id1 = *(vu32*)(tmp[2]+tmp[3]);
    Device_Id2 = *(vu32*)(tmp[4]+tmp[5]);
    
    sprintf(str,"%08x%08x%08x",Device_Id0,Device_Id1,Device_Id2);
    return ;
}

//
int InitSPIFlashCode()
{
    char str[24];
    char content[0x100];
    int i;
    int j;
    
    Get_Device_Guid(str);
    for (i = 0;i<0x100;i++)
    {
        content[i] = GetRandomCharFromADC();
    }
    
    for(i=0;i<24;i++)
    {
        content[i+0x80]= str[i]^orKey[i];
    }
    
    sFLASH_EraseSector(0x00);
    sFLASH_WritePage((uint8_t*)content,0x00,0x100);
    
    //do something others
    for (j=0;j<10;j++)
    {
        for (i = 0;i<0x100;i++)
        {
            content[i] = GetRandomCharFromADC();
        }
        for(i=0x30;i<24;i++)
        {
            content[i] = str[i]^0xab+0xc2;
        }
        sFLASH_WritePage((uint8_t*)content,0x100+j*0x100,0x100);
    }
    return 1;
}


//return : 1 ok; 2 init; 0 wrong
int SPIFlashVarify()
{
    char str[24];
    
    Get_Device_Guid(str);   
    int i;
    for(i=0;i<24;i++)
    {
        str[i]^= orKey[i];
    }
    
    u8 Rx_Buffer[64];
    sFLASH_ReadBuffer(Rx_Buffer, 0x80, 64);
    //sFLASH_ReadBuffer(Rx_Buffer, 0xd000, 64);
    for(i=0;i<CHECK_BYTES;i++)
    {
        if(Rx_Buffer[i]!=str[i])
            break;
    }
    if(i<CHECK_BYTES)//¿ÉÄÜ¼ÙµÄ
    {
        if(sys.chipEncrypted==0x24678)
        {
            return 0;
        }
        else
        {
            InitSPIFlashCode();
            sys.chipEncrypted=0x24678;
            StoreSystemParam();
            debug("New chip detected, successfully encrypted.\r\n");
            return 2;
        }
    }
    else
    {
        if(sys.chipEncrypted==0)
        {
            InitSPIFlashCode();
            sys.chipEncrypted=0x24678;
            StoreSystemParam();
            debug("Low security level chip detected, successfully upgraded.\r\n");
            return 3;
        }
        return 1;
    }
}