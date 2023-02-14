
#ifndef _POWER_SERVICE_H_
#define _POWER_SERVICE_H_

typedef struct
{
    u8 ch_state[2];
    float ch_vset[2];
    float ch_iset[2];
    float ch_vrt[2];
    float ch_irt[2];
    u32 updateCheck;
}PowerStatusDef;

extern PowerStatusDef gPS;

int UpdatePowerParam();
void PowerParamReset();
int DigitalPowerInit();


#endif