

#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_

/***DEF+**********************************************************************/
/* Data type redefinition.                                                   */
/***DEF-**********************************************************************/
typedef void              VOID;
typedef unsigned long     DWORD,ULONG,U32,uint32,UINT32;
typedef long              LONG, S32,int32,INT32;
typedef int               INT;
typedef unsigned int      UINT;
typedef unsigned short    WORD, U16,USHORT,WCHAR,uint16,UINT16;
typedef short             S16, SHORT,int16,INT16;
typedef unsigned char     BYTE, U8, UCHAR, BOOL, uchar,uint8,UINT8;
typedef char              S8, CHAR;
typedef float             FLOAT;

typedef void (*pCallback)(void);
typedef int (*pTFCCallback)(cJSON* root,cJSON* retObj);


struct	key
{
	uchar	direction;
	uchar	keypstate;
	uchar	rate;
};
typedef struct key keystate;



#ifndef NULL
#define NULL ((void*)0)  
#endif

#endif