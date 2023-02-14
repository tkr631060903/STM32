
#include "includes.h"


PowerStatusDef gPS;


/*
该函数由UserTask.c文件中的TFCEventCheck()函数每隔1秒调用一次。
当TFC收到PC传来的pwr_write或者pwr_read指令后也会立刻执行该函数。
*/
int UpdatePowerParam()
{
    int retVal = 1;
    //1. 将set参数写入LTC ... TBD.
    //  需要写入的参数为 gPS.ch_state[2],  gPS.ch_vset[2],  gPS.ch_iset[2].
    //  使用gPS.ch_vset[2],  gPS.ch_iset[2]这几个参数前需校验值是否小于0，大于或等于0方可将其配置写入LTC
    //  可能会需要使用函数 I2C1_ByteWrite() 或者 I2C1_BufferWrite()
    
    //2. 读取LTC的实测参数值及状态值 ... TBD，
    //   需要读出的参数为 gPS.ch_vrt[2],   gPS.ch_irt[2].
    //   若读gPS.ch_vrt[2],   gPS.ch_irt[2]的过程出现错误，请将对应值赋值为-9999,  显示屏上将显示 ---
    //   如果有错误，使用SetTFCError ()标注出来。错误类型有：
    //   ERROR_PWR_IN  ERROR_PWR_SELF  ERROR_PWR_OC0   ERROR_PWR_OC1   ERROR_PWR_SC0   ERROR_PWR_SC1
	//   举例：SetTFCError (ERROR_PWR_SELF);

    
    //3. 根据情况将数据更新到LCD上。
    if(sys.lcd_exist) retVal*= UpdatePowerDashboard(&gPS);
    return retVal;
}

/*
该函数由main.c文件中的main()函数在启动时调用，仅对TFC-PWR板生效。
*/
int DigitalPowerInit()
{
    int retVal = 1;

    //在此添加LTC初始化代码
    //可能会需要使用函数 I2C1_ByteWrite 或者 I2C1_BufferWrite
    //Ch0与Ch1的初始输出电压电流请保持为0V/0A，以保证跟UI显示一致
    
    
    return retVal;
}



void PowerParamReset()
{
    int i ;
    for(i=0;i<2;i++)
    {
        gPS.ch_state[i]=0;
        gPS.ch_vset[i]=-9999;
        gPS.ch_iset[i]=-9999;
        gPS.ch_vrt[i]=-9999;
        gPS.ch_irt[i]=-9999;
    }
    gPS.updateCheck = 1235396309;
}

