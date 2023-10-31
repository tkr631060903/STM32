/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"     /* Obtains integer types */
#include "diskio.h" /* Declarations of disk functions */
#include "includes.h"
#include "spi_flash.h"
#include "sdio_sdcard.h"

/* Definitions of physical drive number for each drive */
#define ATA          0 // 预留SD卡
#define SPI_FLASH    1 // 外部SPI FLASH

#define SD_BLOCKSIZE 512

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
    BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat = STA_NOINIT;
    switch (pdrv) {
        case ATA: // SD卡
            stat &= ~STA_NOINIT;
            return stat;
        case SPI_FLASH:
            /* SPI Flash状态检测：读取SPI Flash 设备ID */
            if (SPI_FLASH_ReadJEDECID() == sFLASH_ID) {
                /* 设备ID读取结果正确 */
                stat &= ~STA_NOINIT;
            } else {
                /* 设备ID读取结果错误 */
                stat = STA_NODISK;
            }
            return stat;
    }
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
    BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat = STA_NOINIT;
    switch (pdrv) {
        case ATA:
            if (SD_Init() == SD_OK) {
                // disk_status(ATA);
                stat &= ~STA_NOINIT;
            } else {
                stat = STA_NOINIT;
            }
            return stat;
        case SPI_FLASH:
            /* 初始化SPI Flash */
            SPI_FLASH_Init();
            /* 延时一小段时间 */
            SysTick_Delay_ms(500);
            /* 唤醒SPI Flash */
            SPI_Flash_WAKEUP();
            /* 获取SPI Flash芯片状态 */
            stat = disk_status(SPI_FLASH);
            return stat;
    }
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
    BYTE pdrv,    /* Physical drive nmuber to identify the drive */
    BYTE *buff,   /* Data buffer to store read data */
    LBA_t sector, /* Start sector in LBA */
    UINT count    /* Number of sectors to read */
)
{
    DRESULT res;
    SD_Error SD_state = SD_OK;
    switch (pdrv) {
        case ATA:
            if ((DWORD)buff & 3) {
                DRESULT res = RES_OK;
                DWORD scratch[SD_BLOCKSIZE / 4];
                while (count--) {
                    res = disk_read(ATA, (void *)scratch, sector++, 1);
                    if (res != RES_OK) {
                        break;
                    }
                    memcpy(buff, scratch, SD_BLOCKSIZE);
                    buff += SD_BLOCKSIZE;
                }
                return res;
            }
            SD_state = SD_ReadMultiBlocks(buff, sector * SD_BLOCKSIZE, SD_BLOCKSIZE, count);
            if (SD_state == SD_OK) {
                /* Check if the Transfer is finished */
                SD_state = SD_WaitReadOperation();
                while (SD_GetStatus() != SD_TRANSFER_OK)
                    ;
            }
            if (SD_state != SD_OK)
                res = RES_PARERR;
            else
                res = RES_OK;   
            return res;
        case SPI_FLASH:
            /* 扇区偏移2MB，外部Flash文件系统空间放在SPI Flash后面6MB空间 */
            sector += 512;
            SPI_FLASH_BufferRead(buff, sector << 12, count << 12);
            return RES_OK;
    }
    return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(
    BYTE pdrv,        /* Physical drive nmuber to identify the drive */
    const BYTE *buff, /* Data to be written */
    LBA_t sector,     /* Start sector in LBA */
    UINT count        /* Number of sectors to write */
)
{
    DRESULT res;
    uint32_t write_addr;
    SD_Error SD_state = SD_OK;
    switch (pdrv) {
        case ATA:
            if ((DWORD)buff & 3) {
                DRESULT res = RES_OK;
                DWORD scratch[SD_BLOCKSIZE / 4];
                while (count--) {
                    memcpy(scratch, buff, SD_BLOCKSIZE);
                    res = disk_write(ATA, (void *)scratch, sector++, 1);
                    if (res != RES_OK) {
                        break;
                    }
                    buff += SD_BLOCKSIZE;
                }
                return res;
            }
            SD_state = SD_WriteMultiBlocks((uint8_t *)buff, (uint64_t)sector * SD_BLOCKSIZE, SD_BLOCKSIZE, count);
            if (SD_state == SD_OK) {
                /* Check if the Transfer is finished */
                SD_state = SD_WaitWriteOperation();
                /* Wait until end of DMA transfer */
                while (SD_GetStatus() != SD_TRANSFER_OK)
                    ;
            }
            if (SD_state != SD_OK)
                res = RES_PARERR;
            else
                res = RES_OK;
            return res;
        case SPI_FLASH:
            /* 扇区偏移2MB，外部Flash文件系统空间放在SPI Flash后面6MB空间 */
            sector += 512;
            write_addr = sector << 12;
            SPI_FLASH_SectorErase(write_addr);
            SPI_FLASH_BufferWrite((uint8_t *)buff, write_addr, count << 12);
            return RES_OK;
    }
    return RES_PARERR;
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
extern SD_CardInfo SDCardInfo;

DRESULT disk_ioctl(
    BYTE pdrv, /* Physical drive nmuber (0..) */
    BYTE cmd,  /* Control code */
    void *buff /* Buffer to send/receive control data */
)
{
    DRESULT res;
    switch (pdrv) {
        case ATA:
            switch (cmd) {
                // Get R/W sector size (WORD)
                case GET_SECTOR_SIZE:
                    *(WORD *)buff = SD_BLOCKSIZE;
                    break;
                // Get erase block size in unit of sector (DWORD)
                case GET_BLOCK_SIZE:
                    *(DWORD *)buff = 1;
                    break;

                case GET_SECTOR_COUNT:
                    *(DWORD *)buff = SDCardInfo.CardCapacity / SDCardInfo.CardBlockSize;
                    break;
                case CTRL_SYNC:
                    break;
            }
            res = RES_OK;
            return res;
        case SPI_FLASH:
            switch (cmd) {
                /* 扇区数量：1536*4096/1024/1024=6(MB) */
                case GET_SECTOR_COUNT:
                    *(DWORD *)buff = 1536;
                    break;
                /* 扇区大小  */
                case GET_SECTOR_SIZE:
                    *(WORD *)buff = 4096;
                    break;
                /* 同时擦除扇区个数 */
                case GET_BLOCK_SIZE:
                    *(DWORD *)buff = 1;
                    break;
            }
            return RES_OK;
    }
    return RES_PARERR;
}

__weak DWORD get_fattime(void)
{
    /* 返回当前时间戳 */
    return ((DWORD)(2015 - 1980) << 25) /* Year 2015 */
           | ((DWORD)1 << 21)           /* Month 1 */
           | ((DWORD)1 << 16)           /* Mday 1 */
           | ((DWORD)0 << 11)           /* Hour 0 */
           | ((DWORD)0 << 5)            /* Min 0 */
           | ((DWORD)0 >> 1);           /* Sec 0 */
}
