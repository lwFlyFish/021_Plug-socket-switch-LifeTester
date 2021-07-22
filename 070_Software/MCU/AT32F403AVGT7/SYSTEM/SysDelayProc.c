///////////////////////////////////////////////////////////////////////////
// LED处理
///////////////////////////////////////////////////////////////////////////

#include "SysConfig.h"
#include "SysDelayProc.h"
///////////////////////////////////////////////////////////////////////////
extern void SysDelayMSProc(UINT16 nTime)
{
	int16 i;

	while(nTime--)
	{
		i=12000;//
		while(i--);
	}
}

extern void SysDelayUSProc(u32 nTime) 
{    
	u32 i = 0;   
	
	while (nTime--) 
	{   
		i = 10;  
		while (i--)
		{
			__NOP();
		}
	}    
}

///////////////////////////////////////////////////////////////////////////
// 函数名  : Delay
// 描述    : 延时(ms)
// 输入    : d:ms值
// 输出    : 无
// 返回值  : 无 
// 说明    : ?óê±ê?à?ó?Timer2?¨ê±?÷2úéúμ?1oá??μ???êyà′êμ??μ?
///////////////////////////////////////////////////////////////////////////
//extern void SysDelayMS(UINT32 d)
//{
//	g_nSysSocketTimer_Counter = 0; 
//	
//	//
//	while (g_nSysSocketTimer_Counter < d);
//}

//???
extern void SysDelay_MS(u32 nDelay)
{
	u32 i;
	
	for (i=0; i<nDelay; i++)
	{
		__NOP();
	}
}
















