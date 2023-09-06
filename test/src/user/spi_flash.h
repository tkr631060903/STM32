#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "includes.h"
#include "stm32f10x_spi.h"

/*SPI接口定义-开头****************************/
#define FLASH_SPIx              SPI1
#define FLASH_SPI_APBxClock_FUN RCC_APB2PeriphClockCmd
#define FLASH_SPI_CLK           RCC_APB2Periph_SPI1

// CS(NSS)引脚 片选选普通GPIO即可
#define FLASH_SPI_CS_APBxClock_FUN RCC_APB2PeriphClockCmd
#define FLASH_SPI_CS_CLK           RCC_APB2Periph_GPIOC
#define FLASH_SPI_CS_PORT          GPIOC
#define FLASH_SPI_CS_PIN           GPIO_Pin_0

// SCK引脚
#define FLASH_SPI_SCK_APBxClock_FUN RCC_APB2PeriphClockCmd
#define FLASH_SPI_SCK_CLK           RCC_APB2Periph_GPIOA
#define FLASH_SPI_SCK_PORT          GPIOA
#define FLASH_SPI_SCK_PIN           GPIO_Pin_5
// MISO引脚
#define FLASH_SPI_MISO_APBxClock_FUN RCC_APB2PeriphClockCmd
#define FLASH_SPI_MISO_CLK           RCC_APB2Periph_GPIOA
#define FLASH_SPI_MISO_PORT          GPIOA
#define FLASH_SPI_MISO_PIN           GPIO_Pin_6
// MOSI引脚
#define FLASH_SPI_MOSI_APBxClock_FUN RCC_APB2PeriphClockCmd
#define FLASH_SPI_MOSI_CLK           RCC_APB2Periph_GPIOA
#define FLASH_SPI_MOSI_PORT          GPIOA
#define FLASH_SPI_MOSI_PIN           GPIO_Pin_7

#define FLASH_SPI_CS_LOW()           GPIO_ResetBits(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN)
#define FLASH_SPI_CS_HIGH()          GPIO_SetBits(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN)

/*SPI接口定义-结尾****************************/

#endif
