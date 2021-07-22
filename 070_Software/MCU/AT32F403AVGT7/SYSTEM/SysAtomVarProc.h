#ifndef __SysAtomVarProc_H__
#define __SysAtomVarProc_H__ 1
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// Attribute
///////////////////////////////////////////////////////////////////////////
// public:





// private:





///////////////////////////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////////////////////////
// public:
	//
	extern UINT32 Get_SysCommDelayTimeCtrl(void);
	extern UINT32 Get_SysCommDelayTimeCtrl_SP1(void);

	//
	extern UINT16 Get_SysCommTimeoutCtrl(void);
	extern UINT16 Get_SysCommTimeoutCtrl_SP1(void);

	//
	extern UINT16 Get_SysTimerCtrlValueProc(void);
	extern UINT16 Get_SysTurnBackTimeCtrlValueProc(void);

	extern BYTE Get_SysProcTimerCtrlValueProc(void);
	extern void Set_SysProcTimerCtrlValueProc(BYTE nTimeValue);

	extern BYTE Get_SysStaticFreshTimeCtrlValueProc(void);
	extern void Set_SysStaticFreshTimeCtlrValueProc(BYTE nTimeValue);

	extern BYTE Get_SysDisplayTimeCtrlValueProc(void);
	extern void Set_SysDisplayTimeCtlrValueProc(BYTE nTimeValue);

	extern u16 Get_LedRefreshValueProc(void);
	extern void Set_LedRefreshValuePro(u16 nTimeValue);

	extern u16 	Get_SysLedBlinkValueProc(void);
	extern void Set_SysLedBlinkValuePro(u16 nTimeValue);
	
// private:





#endif
///////////////////////////////////////////////////////////////////////////
//end
///////////////////////////////////////////////////////////////////////////




