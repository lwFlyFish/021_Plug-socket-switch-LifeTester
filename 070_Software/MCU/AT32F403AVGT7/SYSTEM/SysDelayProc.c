///////////////////////////////////////////////////////////////////////////
// LED����
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
// ������  : Delay
// ����    : ��ʱ(ms)
// ����    : d:msֵ
// ���    : ��
// ����ֵ  : �� 
// ˵��    : ?��������?��?��?Timer2?������?��2��������?1o��??��???��y���䨺��??��?
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
















