
#ifndef _INCLUDES_H_
#define _INCLUDES_H_


//œµÕ≥ ±÷”
#define SYSTEM_CLOCK 72000000UL





/*************************************************************************/
/* Global definations                                                    */
/*************************************************************************/
#define UART1_BUFFER_SIZE 100
#define UART2_BUFFER_SIZE 100
#define UART3_BUFFER_SIZE 2500
#define UART4_BUFFER_SIZE 100
#define UART5_BUFFER_SIZE 100

#define CAN_BUFFER_SIZE 10

/*************************************************************************/
/* Standard header files                                                 */
/*************************************************************************/
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "ctype.h"
#include "math.h"
#include "stddef.h"

/*************************************************************************/
/* 3rd party common Files                                                */
/*************************************************************************/
#include "cJSON.h"

/*************************************************************************/
/* CPU specified files                                                   */
/*************************************************************************/
#include "datatype.h"
#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_it.h"
#include "stm32f10x_can.h"

/*************************************************************************/
/* debug specified files                                                 */
/*************************************************************************/
#include "debug_STM32.h"

/*************************************************************************/
/* Driver Files                                                          */
/*************************************************************************/
#include "WinTimer.h"
#include "QuadDAC.h"
#include "Timer4.h"
#include "Timer1.h"
#include "GPIO.h"
#include "SPIFlash.h"
#include "TypeC.h"
#include "OnChipADC.h"
#include "InternalFlash.h"
#include "Beep.h"
#include "LedDriver.h"
#include "CAN.h"

////USB
//usb lib
#include "usb_type.h"
#include "usb_regs.h"
#include "usb_def.h"
#include "usb_core.h"
#include "usb_init.h"
#include "usb_mem.h"
#include "usb_int.h"


#include "usb_prop.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "platform_config.h"
#include "usb_pwr.h"
#include "usb_istr.h"
#include "usb_mem.h"
#include "usb_conf.h"



/*************************************************************************/
/* GUI Files                                                             */
/*************************************************************************/
#include "SystemGui.h"
#include "MainPanel.h"

/*************************************************************************/
/* Upper level Files                                                     */
/*************************************************************************/
#include "UserTask.h"
#include "TFCEvent.h"
#include "TFCError.h"

/*************************************************************************/
/* Applications                                                          */
/*************************************************************************/
#include "UartSwitch.h"
#include "TFCThumb.h"
#include "HostComm.h"
#include "FsMode.h"

extern u32 gCPULoadTimerCounter;


#endif
