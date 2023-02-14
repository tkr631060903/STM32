#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_


/** @addtogroup STM32100B_EVAL_LOW_LEVEL_M25P_FLASH_SPI
  * @{
  */
/**
  * @brief  M25P FLASH SPI Interface pins
    CS PA15, CLK PB3, MISO PB4, MOSI PB5
  */  
#define sFLASH_SPI                       SPI3
#define sFLASH_SPI_CLK                   RCC_APB1Periph_SPI3
#define sFLASH_SPI_SCK_PIN               GPIO_Pin_3                  /* PA.05 */
#define sFLASH_SPI_SCK_GPIO_PORT         GPIOB                       /* GPIOA */
#define sFLASH_SPI_SCK_GPIO_CLK          RCC_APB2Periph_GPIOB
#define sFLASH_SPI_MISO_PIN              GPIO_Pin_4                  /* PA.06 */
#define sFLASH_SPI_MISO_GPIO_PORT        GPIOB                       /* GPIOA */
#define sFLASH_SPI_MISO_GPIO_CLK         RCC_APB2Periph_GPIOB
#define sFLASH_SPI_MOSI_PIN              GPIO_Pin_5                  /* PA.07 */
#define sFLASH_SPI_MOSI_GPIO_PORT        GPIOB                       /* GPIOA */
#define sFLASH_SPI_MOSI_GPIO_CLK         RCC_APB2Periph_GPIOB
#define sFLASH_CS_PIN                    GPIO_Pin_15                  /* PA.04 */
#define sFLASH_CS_GPIO_PORT              GPIOA                       /* GPIOD */
#define sFLASH_CS_GPIO_CLK               RCC_APB2Periph_GPIOA


/* Function Prototypes */


#define sFLASH_CMD_WRITE          0x02  /*!< Write to Memory instruction */
#define sFLASH_CMD_WRSR           0x01  /*!< Write Status Register instruction */
#define sFLASH_CMD_WREN           0x06  /*!< Write enable instruction */
#define sFLASH_CMD_READ           0x03  /*!< Read from Memory instruction */
#define sFLASH_CMD_RDSR           0x05  /*!< Read Status Register instruction  */
#define sFLASH_CMD_RDID           0x9F  /*!< Read identification */
#define sFLASH_CMD_SE             0x20  /*!< Sector Erase instruction */
#define sFLASH_CMD_BE             0x52  /*!< Bulk Erase instruction */
#define sFLASH_CMD_CE             0xC7  /*!< Chip Erase instruction */

#define sFLASH_WIP_FLAG           0x01  /*!< Write In Progress (WIP) flag */

#define sFLASH_DUMMY_BYTE         0xA5
#define sFLASH_SPI_PAGESIZE       0x100

#define sFLASH_M25P128_ID         0x202018
#define sFLASH_M25P64_ID          0x202017


/** @defgroup STM32_EVAL_SPI_FLASH_Exported_Macros
  * @{
  */
/**
  * @brief  Select sFLASH: Chip Select pin low
  */
#define sFLASH_CS_LOW()       GPIO_ResetBits(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN)
/**
  * @brief  Deselect sFLASH: Chip Select pin high
  */
#define sFLASH_CS_HIGH()      GPIO_SetBits(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN)   

 
/**
  * @brief  High layer functions
  */
void sFLASH_LowLevel_DeInit(void);
uint8_t sFLASH_RecvByte();
void sFLASH_Rx_Init(void);
void SPI1_DMA_Configuration(u32 len);
void StartSPIDMATransfer();
void sFLASH_DeInit(void);
void sFLASH_Init(void);
void sFLASH_EraseSector(uint32_t SectorAddr);
void sFLASH_EraseBulk(void);
void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t sFLASH_ReadID(void);
void sFLASH_ReadUniqueID(char* str);
void sFLASH_StartReadSequence(uint32_t ReadAddr);

/**
  * @brief  Low layer functions
  */
uint8_t sFLASH_ReadByte(void);
uint8_t sFLASH_SendByte(uint8_t byte);
uint16_t sFLASH_SendHalfWord(uint16_t HalfWord);
void sFLASH_WriteEnable(void);
void sFLASH_WaitForWriteEnd(void);

void LCD_SetPoint(u16 x, u16 y, u16 color);
unsigned char SST25_HighSpeed_Read(unsigned long Dst); 


#endif