#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "includes.h"
#include "stm32f10x_spi.h"

#define SPI_FLASH_PageSize         256
#define SPI_FLASH_PerWritePageSize 256
#define sFLASH_ID                  0XEF4017 // W25Q64

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

#define SPI_FLASH_CS_LOW()           GPIO_ResetBits(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN)
#define SPI_FLASH_CS_HIGH()          GPIO_SetBits(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN)

// FLASH命令定义
#define W25X_WriteEnable      0x06
#define W25X_WriteDisable     0x04
#define W25X_ReadStatusReg    0x05
#define W25X_WriteStatusReg   0x01
#define W25X_ReadData         0x03
#define W25X_FastReadData     0x0B
#define W25X_FastReadDual     0x3B
#define W25X_PageProgram      0x02
#define W25X_BlockErase       0xD8
#define W25X_SectorErase      0x20
#define W25X_ChipErase        0xC7
#define W25X_PowerDown        0xB9
#define W25X_ReleasePowerDown 0xAB
#define W25X_DeviceID         0xAB
#define W25X_ManufactDeviceID 0x90
#define W25X_JedecDeviceID    0x9F
#define Dummy_Byte            0x00

/*SPI接口定义-结尾****************************/

void SPI_FLASH_Init(void);
uint8_t SPI_FLASH_SendByte(uint8_t byte);
uint32_t SPI_FLASH_ReadJEDECID(void);
void SPI_FLASH_BufferRead(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_ChipErase(void);
void SPI_Flash_WAKEUP(void);

#endif
