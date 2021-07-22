#ifndef __SysUart1ProtocolProc_H
#define __SysUart1ProtocolProc_H  1
//

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
// Attribute
///////////////////////////////////////////////////////////////////////////
// public:
	//�豸��ַ
	extern s16 g_nAddr_SP1;
// private:
	//Comm
	static u8 m_nCurStep_SP1;
	static s16 m_nCmd_SP1;
	//static s16 m_nLen_SP1;

///////////////////////////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////////////////////////
//public:
	//
    
    extern void SP1_SetSysCommStepProc(void);
	extern u8 SP1_RecvProc(void);
    
//private:
	//
	static u8 Syn1Proc(void);
	static u8 Cmd1Proc(void);
	static u8 Data1Proc(void);

	static u8 RequestRealParam(void);
#endif
///////////////////////////////////////////////////////////////////////////





