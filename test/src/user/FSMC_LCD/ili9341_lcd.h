#ifndef __ILI9341_LCD_H
#define __ILI9341_LCD_H

#include "includes.h"
#include "stm32f10x_fsmc.h"

#define SPI_FLASH_PageSize         256
#define SPI_FLASH_PerWritePageSize 256
#define sFLASH_ID                  0XEF4017 // W25Q64

/*SPI接口定义-开头****************************/
#define FLASH_SPIx              SPI1
#define FLASH_SPI_APBxClock_FUN RCC_APB2PeriphClockCmd
#define FLASH_SPI_CLK           RCC_APB2Periph_SPI1


#endif
