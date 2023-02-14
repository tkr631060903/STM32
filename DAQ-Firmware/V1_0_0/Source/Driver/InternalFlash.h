#ifndef _INTERNAL_FLASH_H_
#define _INTERNAL_FLASH_H_



#define FLASH_PAGE_SIZE    	((u16)0x400)//1KB
#define StartAddr  		((u32)0x00020000-FLASH_PAGE_SIZE)
#define EndAddr    		((u32)0x00020000)


void Intenal_Flash_test(void);
int InternalFlashOnePageWrite(char* str, u8 numOfByte);
int InternalFlashOnePageRead(char* str, u8 numOfByte);


#endif