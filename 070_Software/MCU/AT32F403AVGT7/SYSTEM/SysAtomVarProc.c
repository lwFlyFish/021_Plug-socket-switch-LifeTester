///////////////////////////////////////////////////////////////////////////
//
//File Name : SysAtomVarProc
//Compiler Type : ICCAVR 7.22
//Date : 2008/10/16
//Designer : spy
//ChipType : M128
//Clock : 14.7456MHz
// 
//Hardware Interface : 
//  SCLK - PF6
//  I/O  - PF5
//	RST  - PF4
//  
//
//Function : 
//           
//
///////////////////////////////////////////////////////////////////////////

#include "SysConfig.h"
#include "SysAtomVarProc.h"

#include "RealDatabase.h"
#include "SysTick.h"
///////////////////////////////////////////////////////////////////////////
//
extern UINT32 Get_SysCommDelayTimeCtrl(void)
{
	UINT32 nTemp;

	do 
	{
		nTemp = g_nSysCommDelayTime_SP2;
	} 
	while (nTemp != g_nSysCommDelayTime_SP2);

	return nTemp;
}

extern UINT32 Get_SysCommDelayTimeCtrl_SP1(void)
{
	UINT32 nTemp;

	do 
	{
		nTemp = g_nSysCommDelayTime_SP1;
	} 
	while (nTemp != g_nSysCommDelayTime_SP1);

	return nTemp;
}

///////////////////////////////////////////////////////////////////////////
extern UINT16 Get_SysCommTimeoutCtrl(void)
{
	UINT16 nTemp;

	do 
	{
		nTemp = g_nSysCommTimeoutCtrl_SP2;
	} 
	while (nTemp != g_nSysCommTimeoutCtrl_SP2);

	return nTemp;
}

extern UINT16 Get_SysCommTimeoutCtrl_SP1(void)
{
	UINT16 nTemp;

	do 
	{
		nTemp = g_nSysCommTimeoutCtrl_SP1;
	} 
	while (nTemp != g_nSysCommTimeoutCtrl_SP1);

	return nTemp;
}

///////////////////////////////////////////////////////////////////////////
extern UINT16 Get_SysTimerCtrlValueProc(void)
{
	UINT16 nTemp;

	do 
	{
		nTemp = g_nSysTimerCtrl;
	} 
	while (nTemp != g_nSysTimerCtrl);

	return nTemp;
}

extern UINT16 Get_SysTurnBackTimeCtrlValueProc(void)
{
	UINT16 nTemp;

	do 
	{
		nTemp = g_nSysTurnBackTimeCtrl;
	} 
	while (nTemp != g_nSysTurnBackTimeCtrl);

	return nTemp;
}

///////////////////////////////////////////////////////////////////////////
extern BYTE Get_SysProcTimerCtrlValueProc(void)
{
	BYTE nTemp;

	do
	{
		nTemp = g_nSysProcTimerCtrl;
	} 
	while (nTemp != g_nSysProcTimerCtrl);

	return nTemp;
}

extern void Set_SysProcTimerCtrlValueProc(BYTE nTimeValue)
{
	g_nSysProcTimerCtrl = nTimeValue;
}

extern BYTE Get_SysStaticFreshTimeCtrlValueProc(void)
{
	BYTE nTemp;

	do
	{
		nTemp = g_nSysStaticFreshTimerCtrl;
	} 
	while (nTemp != g_nSysStaticFreshTimerCtrl);

	return nTemp;
}

extern void Set_SysStaticFreshTimeCtlrValueProc(BYTE nTimeValue)
{
	g_nSysStaticFreshTimerCtrl = nTimeValue;
}

extern BYTE Get_SysDisplayTimeCtrlValueProc(void)
{
	BYTE nTemp;

	do
	{
		nTemp = g_nSysDisplayTimeCtrl;
	} 
	while (nTemp != g_nSysDisplayTimeCtrl);

	return nTemp;
}

extern void Set_SysDisplayTimeCtlrValueProc(BYTE nTimeValue)
{
	g_nSysDisplayTimeCtrl = nTimeValue;
}


extern u16 Get_LedRefreshValueProc(void)
{
	u16 nTemp;

	do
	{
		nTemp = g_nSysLedRunCount;
	} while (nTemp != g_nSysLedRunCount);

	return nTemp;
}

extern void Set_LedRefreshValuePro(u16 nTimeValue)
{
	g_nSysLedRunCount = nTimeValue;
}

extern u16 Get_SysLedBlinkValueProc(void)
{
	u16 nTemp;

	do
	{
		nTemp = g_nSysLedBlinkCount;
	} while (nTemp != g_nSysLedBlinkCount);

	return nTemp;
}

extern void Set_SysLedBlinkValuePro(u16 nTimeValue)
{
	g_nSysLedBlinkCount = nTimeValue;
}



///////////////////////////////////////////////////////////////////////////

