#ifndef __I2C_EE_H
#define __I2C_EE_H

#include "includes.h"
#include "stm32f10x_i2c.h"

#define EEPROM_I2Cx                   I2C1
#define EEPROM_I2C_APBxClock_FUN      RCC_APB1PeriphClockCmd
#define EEPROM_I2C_CLK                RCC_APB1Periph_I2C1
#define EEPROM_I2C_GPIO_APBxClock_FUN RCC_APB2PeriphClockCmd
#define EEPROM_I2C_GPIO_CLK           RCC_APB2Periph_GPIOB
#define EEPROM_I2C_SCL_PORT           GPIOB
#define EEPROM_I2C_SCL_PIN            GPIO_Pin_6
#define EEPROM_I2C_SDA_PORT           GPIOB
#define EEPROM_I2C_SDA_PIN            GPIO_Pin_7

/* STM32 I2C 快速模式 */
#define I2C_Speed 400000 //*

/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define I2Cx_OWN_ADDRESS7 0x0A

/* AT24C01/02每页有8个字节 */
#define I2C_PageSize 8

/* EEPROM Addresses defines */
#define EEPROM_ADDRESS 0xA0   /* E2 = 0 */

void EEPROM_I2C_Init(void);
uint8_t EEPROM_WriteByte(uint8_t addr, uint8_t data);
uint8_t EEPROM_ReadByte(uint8_t addr);
uint8_t I2C_EE_BufferRead(uint8_t *pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
uint8_t I2C_EE_PageWrite(uint8_t *pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite);

#endif
