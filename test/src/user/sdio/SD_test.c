#include "sdio_sdcard.h"
#include "stm32f10x.h"
#include "includes.h"

#define BLOCK_SIZE        512 /* Block Size in Bytes */
#define NUMBER_OF_BLOCKS  10  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE (BLOCK_SIZE * NUMBER_OF_BLOCKS)

typedef enum {
    FAILED = 0,
    PASSED = !FAILED
} TestStatus;

uint8_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
SD_Error Status = SD_OK;
uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];

static void SD_EraseTest(void);
void SD_SingleBlockTest(void);
static TestStatus eBuffercmp(uint8_t *pBuffer, uint32_t BufferLength);
static void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset);
TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint32_t BufferLength);
void SD_MultiBlockTest(void);

void SD_Test(void)
{
    // SDIO和SD卡初始化
    SD_Error Status = SD_OK;
    if ((Status = SD_Init()) != SD_OK) {
        printf("Init error\n");
    } else {
        printf("Init succeed\n");
    }

    // SD卡擦除测试
    SD_EraseTest();
    // single block 读写测试
    SD_SingleBlockTest();
    // muti block 读写测试
    SD_MultiBlockTest();
}

static void SD_EraseTest(void)
{
    /*------------------- 块擦除 -------------------------------*/
    if (Status == SD_OK) {
        /* 擦除 NumberOfBlocks 个块每个块长度为512字节 */
        Status = SD_Erase(0x00, (BLOCK_SIZE * NUMBER_OF_BLOCKS));
    }
    if (Status == SD_OK) {
        Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
        /* 等待传输完成 */
        Status = SD_WaitReadOperation();
        /* 检查传输是否正常*/
        while (SD_GetStatus() != SD_TRANSFER_OK)
            ;
    }
    /* 校验数据 */
    if (Status == SD_OK) {
        EraseStatus = eBuffercmp(Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
    }
    if (EraseStatus == PASSED) {
        printf("SD卡擦除测试成功！\n");
    } else {
        printf("SD卡擦除测试失败！\n");
        printf("温馨提示：部分SD卡不支持擦除测试，若SD卡能通过下面的single读写测试，即表示SD卡能够正常使用。\n");
    }
}

void SD_SingleBlockTest(void)
{
    /*------------------- 块 读写 --------------------------*/
    /* 向数组填充要写入的数据*/
    Fill_Buffer(Buffer_Block_Tx, BLOCK_SIZE, 0x320F);

    if (Status == SD_OK) {
        /* 把512个字节写入到SD卡的0地址 */
        Status = SD_WriteBlock(Buffer_Block_Tx, 0x00, BLOCK_SIZE);
        /* 检查传输 */
        Status = SD_WaitWriteOperation();
        while (SD_GetStatus() != SD_TRANSFER_OK)
            ;
    }
    if (Status == SD_OK) {
        /* 从SD卡的0地址读取512个字节 */
        Status = SD_ReadBlock(Buffer_Block_Rx, 0x00, BLOCK_SIZE);
        /* 检查传输 */
        Status = SD_WaitReadOperation();
        while (SD_GetStatus() != SD_TRANSFER_OK)
            ;
    }
    /* 校验读出的数据是否与写入的数据一致 */
    if (Status == SD_OK) {
        TransferStatus1 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE);
    }
    if (TransferStatus1 == PASSED) {
        printf("Single block 测试成功！\n");
    } else {
        printf("Single block 测试失败，请确保SD卡正确接入开发板，或换一张SD卡测试！\n");
    }
}

/**
 * @brief  Tests the SD card Multiple Blocks operations.
 * @param  None
 * @retval None
 */
void SD_MultiBlockTest(void)
{
    /*--------------- Multiple Block Read/Write ---------------------*/
    /* Fill the buffer to send */
    Fill_Buffer(Buffer_MultiBlock_Tx, MULTI_BUFFER_SIZE, 0x0);
    if (Status == SD_OK) {
        /* Write multiple block of many bytes on address 0 */
        Status = SD_WriteMultiBlocks(Buffer_MultiBlock_Tx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
        /* Check if the Transfer is finished */
        Status = SD_WaitWriteOperation();
        while (SD_GetStatus() != SD_TRANSFER_OK)
            ;
    }
    if (Status == SD_OK) {
        /* Read block of many bytes from address 0 */
        Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
        /* Check if the Transfer is finished */
        Status = SD_WaitReadOperation();
        while (SD_GetStatus() != SD_TRANSFER_OK)
            ;
    }
    /* Check the correctness of written data */
    if (Status == SD_OK) {
        TransferStatus2 = Buffercmp(Buffer_MultiBlock_Tx, Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
    }

    if (TransferStatus2 == PASSED) {
        printf("Multi block 测试成功！\n");
    } else {
        printf("Multi block 测试失败，请确保SD卡正确接入开发板，或换一张SD卡测试！\n");
    }
}

/**
 * @brief  Fills buffer with user predefined data.
 * @param  pBuffer: pointer on the Buffer to fill
 * @param  BufferLength: size of the buffer to fill
 * @param  Offset: first value to fill on the Buffer
 * @retval None
 */
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
    uint16_t index = 0;

    /* Put in global buffer same values */
    for (index = 0; index < BufferLength; index++) {
        pBuffer[index] = index + Offset;
    }
}

/**
 * @brief  Compares two buffers.
 * @param  pBuffer1, pBuffer2: buffers to be compared.
 * @param  BufferLength: buffer's length
 * @retval PASSED: pBuffer1 identical to pBuffer2
 *         FAILED: pBuffer1 differs from pBuffer2
 */
TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint32_t BufferLength)
{
    while (BufferLength--) {
        if (*pBuffer1 != *pBuffer2) {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
}

TestStatus eBuffercmp(uint8_t *pBuffer, uint32_t BufferLength)
{
    while (BufferLength--) {
        /* In some SD Cards the erased state is 0xFF, in others it's 0x00 */
        if ((*pBuffer != 0xFF) && (*pBuffer != 0x00)) {
            return FAILED;
        }

        pBuffer++;
    }

    return PASSED;
}
