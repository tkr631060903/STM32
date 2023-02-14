
#include "includes.h"


/*
typedef struct
{
    char userName[30];
    char userId[30];
    u8 userSex;//0 men, 1 women
    u8 userAge;
    char userPassword[30];
    u8 userStatus;
    u8 startUpCounter;
    u32 randomCheck;
    u32 updateCheck;
}UserConfigDef;
*/

UserConfigDef user={
    "",
    "",
    0,
    30,
    "",
    0,
    0,
    0,
    0
};


/*
typedef struct
{
    u8 debug_mode;    
    u8 beep_after_ins;      
    u8 auto_reset_interval;     
    u8 start_up_music;      
    u32 uart_baud_rate_default[5];    
    u32 can_baud_rate_default[2];
    u32 i2c_baud_rate_default;
    u8 resetStatus;    
    char userString[64];
    u32 chipEncrypted;
    u32 ins_exec_interval;
    u32 updateCheck;
}SystemStatusDef;
*/

SystemStatusDef sys={
    1,   //bat voltage
    1,
    0,
    0,
    {115200,115200,115200,115200,115200},
    {500,500},
    400000,
    RCC_FLAG_SFTRST,
    "",
    0,
    50,
    0,
    0
};

u8 gUART3Bond = 0;


int PrintSystemParam(char* str)
{
    sprintf(str,"debug_mode:%d, beep_after_ins:%d,auto_reset_interval:%d,start_up_music:%d,uart_baud_rate_default:%d/%d/%d/%d/%d,can_baud_rate_default:%d/%d,i2c_baud_rate_default:%d,ins_exec_interval:%d.",sys.debug_mode,sys.beep_after_ins,sys.auto_reset_interval,sys.start_up_music,
            sys.uart_baud_rate_default[0],sys.uart_baud_rate_default[1],sys.uart_baud_rate_default[2],sys.uart_baud_rate_default[3],sys.uart_baud_rate_default[4],sys.can_baud_rate_default[0],sys.can_baud_rate_default[1],sys.i2c_baud_rate_default,sys.ins_exec_interval);
    return 1;
}


int StoreSystemParam()
{
    sys.updateCheck = 1235396309;
    if(InternalFlashOnePageWrite((char*)(&sys),sizeof(SystemStatusDef)))
    {
        return 1;
    }
    else
        return 0;
}



int GetSystemParam()
{
    if(InternalFlashOnePageRead((char*)(&sys),sizeof(SystemStatusDef)))
    {
        return 1;
    }
    else
        return 0;
}


int ResetSystemInformation()
{
    int i;
    sys.debug_mode = 1;
    sys.beep_after_ins = 0;
    sys.auto_reset_interval = 0;
    sys.start_up_music = 0;
    for(i=0;i<5;i++)
        sys.uart_baud_rate_default[i]=115200;
    for(i=0;i<2;i++)
        sys.can_baud_rate_default[i]=500;
    sys.i2c_baud_rate_default = 400000;
    sys.resetStatus = RCC_FLAG_SFTRST;
    strcpy(sys.userString,"");
    sys.chipEncrypted = 0;
    sys.ins_exec_interval =50;
    sys.lcd_exist =0;
    sys.updateCheck = 1235396309;
    return 1;
}


int LoadDefaultSystemParam()
{
    if(sys.updateCheck != 1235396309)//如果user没有初始化
    {
        if(GetSystemParam())//IAP读取成功
        {
            if(sys.updateCheck != 1235396309 || sys.i2c_baud_rate_default>1000000 || sys.ins_exec_interval>1000 || sys.ins_exec_interval<10)//存的信息也不对
            {
                ResetSystemInformation();
                StoreSystemParam();
                debug("New chip or new config version detected, new params successfully created.\r\n");
            }
            return 1;
        }
        else    //IAP信息读取失败
        {

            return 0;
        }
    }
    else
    {
        //do nothing
    }
    return 1;
}



//usreIndex:0-12
int StoreUserParam(u8 userIndex)
{
    if(userIndex>12) userIndex = 12;
    user.updateCheck = 1235396309;
    user.randomCheck = GetRandomCharFromADC() + (GetRandomCharFromADC()<<8)+(GetRandomCharFromADC()<<16)+(GetRandomCharFromADC()<<24);
    sFLASH_EraseSector(0x2000+userIndex*0x1000);
    sFLASH_WriteBuffer((uint8_t*)(&user),0x2000+userIndex*0x1000,sizeof(UserConfigDef));
    
    UserConfigDef tmpUser;
    tmpUser.randomCheck =0;
    sFLASH_ReadBuffer((uint8_t*)(&tmpUser),0x2000+userIndex*0x1000,sizeof(UserConfigDef));
    if(user.randomCheck == tmpUser.randomCheck)
        return 1;
    else return 0;
}

int GetUserParam(u8 userIndex)
{
    if(userIndex>12) userIndex = 12;
    sFLASH_ReadBuffer((uint8_t*)(&user),0x2000+userIndex*0x1000,sizeof(UserConfigDef));
    if(user.updateCheck == 1235396309)
    {
        return 1;
    }
    else
        return 0;
}

int ResetUserInformation()
{
    strcpy(user.userName,"");
    strcpy(user.userId,"");
    user.userSex = 0;
    user.userAge = 30;
    strcpy(user.userPassword,"");
    user.userStatus = 0;
    user.startUpCounter = 0;
    user.randomCheck = 0;
    user.updateCheck = 0;
    return 1;
}

int LoadDefaultUserParam()
{
    if(user.updateCheck != 1235396309)//如果user没有初始化
    {
        if(!GetUserParam(0))//IAP读取成功
        {
            debug("Empty user information, new user created.\r\n");
            ResetUserInformation();
            StoreUserParam(0);
        }
        else
        {
            user.startUpCounter ++;
            StoreUserParam(0);
        }
    }
    return 1;
}


