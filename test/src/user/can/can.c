#include "can.h"

static void CAN_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能CAN时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    // 使能CAN引脚相关时钟,AFIO引脚复用重定义
    RCC_APB2PeriphClockCmd(CAN_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
    // 重映射引脚
    GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = CAN_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(CAN_TX_GPIO_PROT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = CAN_RX_GPIO_PIN;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(CAN_RX_GPIO_PORT, &GPIO_InitStructure);
}

static void CAN_Mode_Config(void)
{

    CAN_InitTypeDef CAN_InitTypeStruct;
    CAN_InitTypeStruct.CAN_ABOM = ENABLE;
    CAN_InitTypeStruct.CAN_AWUM = ENABLE;
    // 回环模式,调试或验证CAN时候使用
    CAN_InitTypeStruct.CAN_Mode = CAN_Mode_LoopBack;
    // 错误重传
    CAN_InitTypeStruct.CAN_NART = ENABLE;
    CAN_InitTypeStruct.CAN_RFLM = ENABLE;
    CAN_InitTypeStruct.CAN_TTCM = DISABLE;
    // 按照ID优先级发送
    CAN_InitTypeStruct.CAN_TXFP = DISABLE;
    // 配置成1Mbps,ss=1 bs1=5 bs2=3 位时间宽度为(1+5+3) 波特率即为时钟周期tq*(1+3+5)
    CAN_InitTypeStruct.CAN_BS1 = CAN_BS1_5tq; // BTR-TS1 时间段1 占用了5个时间单元
    CAN_InitTypeStruct.CAN_BS2 = CAN_BS2_3tq; // BTR-TS1 时间段2 占用了3个时间单元
    CAN_InitTypeStruct.CAN_SJW = CAN_SJW_2tq; // BTR-SJW 重新同步跳跃宽度 2个时间单元
    /* CAN Baudrate = 1 MBps (1MBps已为stm32的CAN最高速率) (CAN 时钟频率为 APB 1 = 36 MHz) */
    // BTR-BRP 波特率分频器  定义了时间单元的时间长度 36/(1+5+3)/4=1 Mbps
    CAN_InitTypeStruct.CAN_Prescaler = 4;
    CAN_Init(CAN1, &CAN_InitTypeStruct);
}

static void CAN_Filter_Config(void)
{
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    /*CAN筛选器初始化*/
    CAN_FilterInitStructure.CAN_FilterNumber = 0; // 筛选器组0
    // 工作在掩码模式
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    // 筛选器位宽为单个32位。
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    /* 使能筛选器，按照标志符的内容进行比对筛选，
        扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */
    // 要筛选的ID高位，第0位保留，第1位为RTR标志，第2位为IDE标志，从第3位开始是EXID
    CAN_FilterInitStructure.CAN_FilterIdHigh = ((PASS_ID << 3 | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF0000) >> 16;
    // 要筛选的ID低位
    CAN_FilterInitStructure.CAN_FilterIdLow = (PASS_ID << 3 | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF;
    // 筛选器高16位每位必须匹配
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;
    // 筛选器低16位每位必须匹配
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xFFFF;
    // 筛选器被关联到FIFO0
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    // 使能筛选器
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
    /*CAN通信中断使能*/
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

void CAN_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel                   = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void CAN_Config(void)
{
    CAN_GPIO_Config();
    CAN_Mode_Config();
    CAN_Filter_Config();
    CAN_NVIC_Config();
}
