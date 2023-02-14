
#include "includes.h"



#define BUILD_YEAR_CH0 (__DATE__[ 7 ])
#define BUILD_YEAR_CH1 (__DATE__[ 8 ])
#define BUILD_YEAR_CH2 (__DATE__[ 9 ])
#define BUILD_YEAR_CH3 (__DATE__[10 ])

#define BUILD_MONTH_IS_JAN (__DATE__[ 0 ] == 'J' && __DATE__[ 1 ] == 'a' && __DATE__[ 2 ] == 'n')
#define BUILD_MONTH_IS_FEB (__DATE__[ 0 ] == 'F')
#define BUILD_MONTH_IS_MAR (__DATE__[ 0 ] == 'M' && __DATE__[ 1 ] == 'a' && __DATE__[ 2 ] == 'r')
#define BUILD_MONTH_IS_APR (__DATE__[ 0 ] == 'A' && __DATE__[ 1 ] == 'p')
#define BUILD_MONTH_IS_MAY (__DATE__[ 0 ] == 'M' && __DATE__[ 1 ] == 'a' && __DATE__[ 2 ] == 'y')
#define BUILD_MONTH_IS_JUN (__DATE__[ 0 ] == 'J' && __DATE__[ 1 ] == 'u' && __DATE__[ 2 ] == 'n')
#define BUILD_MONTH_IS_JUL (__DATE__[ 0 ] == 'J' && __DATE__[ 1 ] == 'u' && __DATE__[ 2 ] == 'l')
#define BUILD_MONTH_IS_AUG (__DATE__[ 0 ] == 'A' && __DATE__[ 1 ] == 'u')
#define BUILD_MONTH_IS_SEP (__DATE__[ 0 ] == 'S')
#define BUILD_MONTH_IS_OCT (__DATE__[ 0 ] == 'O')
#define BUILD_MONTH_IS_NOV (__DATE__[ 0 ] == 'N')
#define BUILD_MONTH_IS_DEC (__DATE__[ 0 ] == 'D')


#define BUILD_MONTH_CH0 \
        ((BUILD_MONTH_IS_OCT || BUILD_MONTH_IS_NOV || BUILD_MONTH_IS_DEC) ? '1' : '0')

#define BUILD_MONTH_CH1 \
        ( \
          ( BUILD_MONTH_IS_JAN ) ? '1' : \
          ( BUILD_MONTH_IS_FEB ) ? '2' : \
          ( BUILD_MONTH_IS_MAR ) ? '3' : \
          ( BUILD_MONTH_IS_APR ) ? '4' : \
          ( BUILD_MONTH_IS_MAY ) ? '5' : \
          ( BUILD_MONTH_IS_JUN ) ? '6' : \
          ( BUILD_MONTH_IS_JUL ) ? '7' : \
          ( BUILD_MONTH_IS_AUG ) ? '8' : \
          ( BUILD_MONTH_IS_SEP ) ? '9' : \
          ( BUILD_MONTH_IS_OCT ) ? '0' : \
          ( BUILD_MONTH_IS_NOV ) ? '1' : \
          ( BUILD_MONTH_IS_DEC ) ? '2' : \
          /* error default */    '?' \
        )

#define BUILD_DAY_CH0 ((__DATE__[ 4 ] >= '0') ? (__DATE__[ 4 ]) : '0')
#define BUILD_DAY_CH1 (__DATE__[ 5 ])


#define BUILD_HOUR_CH0 (__TIME__[ 0 ])
#define BUILD_HOUR_CH1 (__TIME__[ 1 ])

#define BUILD_MIN_CH0 (__TIME__[ 3 ])
#define BUILD_MIN_CH1 (__TIME__[ 4 ])

#define BUILD_SEC_CH0 (__TIME__[ 6 ])
#define BUILD_SEC_CH1 (__TIME__[ 7 ])

// π”√£∫
//const  char SoftwareVersionInfo[] __at( 0x4011 ) = {'M', 'D', 'S', '0', '2', '0', '.', BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3, BUILD_MONTH_CH0, BUILD_MONTH_CH1, BUILD_DAY_CH0, BUILD_DAY_CH1, BUILD_HOUR_CH0, BUILD_HOUR_CH1, BUILD_MIN_CH0, BUILD_MIN_CH1, '\0'};


typedef struct
{
    char userName[30];
    char userId[30];
    u8 userSex;//0 men, 1 women
    u8 userAge;
    char userPassword[30];
    u8 userStatus;
    u8 startUpCounter;
    u32 randomCheck;
    u32 updateCheck;
}UserConfigDef;


typedef struct
{
    u8 debug_mode;    
    u8 beep_after_ins;      
    u8 auto_reset_interval;     
    u8 start_up_music;      
    u32 uart_baud_rate_default[5];    
    u32 can_baud_rate_default[2];
    u32 i2c_baud_rate_default;
    u8 resetStatus;    
    char userString[64];
    u32 chipEncrypted;
    u32 ins_exec_interval;
    u32 updateCheck;
}SystemStatusDef;



extern SystemStatusDef sys;

int StoreSystemParam();
int GetSystemParam();
int LoadDefaultSystemParam();
int ResetSystemInformation();
int PrintSystemParam(char* str);


extern UserConfigDef user;
int StoreUserParam(u8 userIndex);
int GetUserParam(u8 userIndex);
int ResetUserInformation();
int LoadDefaultUserParam();

