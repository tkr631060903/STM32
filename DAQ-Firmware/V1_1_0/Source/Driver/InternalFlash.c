

#include "includes.h"


/*
    char buf[64]={0};
    for(i=0;i<64;i++)
        buf[i] = i;
    if(InternalFlashOnePageWrite(buf,64)==0)
        debug("flash failed");
    else
        debug("flash ok");
    char str[32];
    InternalFlashOnePageRead(str,24);
    if(str[2]==2)
    {
        debug("flash ok2");
    }
    else
    {
        debug("flash failed2");
    }
    */



/**************************************************************************************************************
函数描述 : 内部FLASH读写擦除测试
内部FLASH写操作之前要擦除,在地址0x08008000到0x0800C000之间全部写入0x55555555
写入后读回校验

***************************************************************************************************************/
void Intenal_Flash_test(void)
{
    typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
    u32 EraseCounter = 0x00, Address = 0x00;
    u32 Data;
    __IO u32 NbrOfPage = 0x00;
    volatile FLASH_Status FLASHStatus;
    volatile TestStatus MemoryProgramStatus;
    
    debug("Flash start\r\n");
    
    FLASHStatus = FLASH_COMPLETE;
    MemoryProgramStatus = PASSED;
    Data = 0x00000001;
    
    //Program Erase controller在复位后是上锁状态，所以Unlock the Flash Program Erase controller
    FLASH_Unlock();
    
    //Define the number of page to be erased
    NbrOfPage = (EndAddr - StartAddr) / FLASH_PAGE_SIZE;
    
    //Clear All pending flags
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
    
    //Erase the FLASH pages
    for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
    {
        FLASHStatus = FLASH_ErasePage(StartAddr + (FLASH_PAGE_SIZE * EraseCounter));
    }
    
    //FLASH Word program of data 0x55555555 at addresses defined by StartAddr and EndAddr
    Address = StartAddr;
    
    while((Address < EndAddr) && (FLASHStatus == FLASH_COMPLETE))
    {
        FLASHStatus = FLASH_ProgramWord(Address, Data);
        Data++;
        Address = Address + 4;
    }
    
    //Check the corectness of written data
    Address = StartAddr;
    
    //while((Address < EndAddr) && (MemoryProgramStatus != 0))
    while(Address < EndAddr)
    {
        debug("%d\r\n",(*(__IO u32*) Address));
        
    	Address += 4;
    }
    if (MemoryProgramStatus)
        debug("Flash WR Success!\r\n");
}

int InternalFlashOnePageWrite(char* str, u8 numOfByte)
{
    u32 Address = 0x00;
    //__IO u32 NbrOfPage = 0x00;
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    int verifySuccess = 1;
    
    //Program Erase controller在复位后是上锁状态，所以Unlock the Flash Program Erase controller
    FLASH_Unlock();
    
    //Clear All pending flags
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
    FLASHStatus = FLASH_ErasePage(StartAddr);
    
    Address = StartAddr;
    while((Address < StartAddr+ FLASH_PAGE_SIZE) && (FLASHStatus == FLASH_COMPLETE) && Address < StartAddr + numOfByte)
    {
        FLASHStatus = FLASH_ProgramWord(Address, (uint32_t)(*(uint32_t*)(str+Address-StartAddr)));
        
        Address = Address + 4;
    }
    //FLASH_Lock();
    
    //while((Address < EndAddr) && (MemoryProgramStatus != 0))
    Address = StartAddr;
    while(Address < StartAddr+FLASH_PAGE_SIZE && Address < StartAddr+numOfByte)
    {
        u32 temp;
        temp = (*(__IO long*) Address);
        if( temp!= *(long*)(str+Address-StartAddr))
        {
            verifySuccess = 0;
            break;
        }
        
    	Address += 4;
    }
    if(verifySuccess)
        return 1;
    else
        return 0;
}


int InternalFlashOnePageRead(char* str, u8 numOfByte)
{
    u32 Address = StartAddr;;
    while(Address < StartAddr+FLASH_PAGE_SIZE && Address < StartAddr+numOfByte)
    {
        *(long*)(str+Address-StartAddr) = (*(__IO long*) Address);
    	Address += 4;
    }
    return 1;
}


