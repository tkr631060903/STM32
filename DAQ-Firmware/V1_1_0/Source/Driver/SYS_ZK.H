
#ifndef __SYS_ZK_H__
#define __SYS_ZK_H__



struct __ASC_ZK
{
	u8 code;
	u8 ENCODE[16];
};

struct __HZK_ZK
{
	char code[2];
	u8  HZCODE[32];
};

#endif
