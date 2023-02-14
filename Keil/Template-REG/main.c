#include "stm32f10x.h"

int main(void)
{
    //IOPB EN 使能GPIO B时钟
    *( unsigned int * )0x40021018 |= (1 << 3 );
    //IOPB push-pull 配置GPIO B输出模式
    *( unsigned int * )0x40010C00 |= ((1) << (4*0) );
    //控制ODR寄存器
    //*( unsigned int * )0x40010C0C &= ~(1<<0); 
	  *( unsigned int * )0x40010C0C = 0xfffffffe;
}

void SystemInit(void)
{
    //函数体为空，目的骗过编译器不报错
}
