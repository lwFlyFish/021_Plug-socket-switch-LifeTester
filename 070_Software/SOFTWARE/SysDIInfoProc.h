#ifndef __SysDIInfoProc_H
#define __SysDIInfoProc_H	1
//
///////////////////////////////////////////////////////////////////////////



#define SYSDINUM 	4  	 

/*
水泵工作状态
*/
typedef struct
{
	u8 nStatus; // 0 正常   1上水   2缺水
	u8 nDelay;
	u8 nTimeOut;

}PUMP_INFO_DEF;

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
// Attribute
///////////////////////////////////////////////////////////////////////////
//public:
	extern u16 DI_Value;
	extern u32 m_arrSysDI_DT[SYSDINUM];	 
	extern u16 DI_Value_Changed_Time;
	extern PUMP_INFO_DEF g_stPumpInfo;



//private:
	//



///////////////////////////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////////////////////////
//public:
	
	extern void key1ShortPress(void);

	extern void key1LongPress(void);

	extern void key2ShortPress(void);

	extern void key2LongPress(void);

	extern void key3ShortPress(void);

	extern void key3LongPress(void);

	extern void key4ShortPress(void);

	extern void key4LongPress(void);

	//输入处理
	extern void DI_InputInfoProc(void);

	//输入端口初始化
	extern void DI_PortInitProc(void);

	extern void Pump_TimeOutCheckProc(void);

	extern void Pump_CheckWaterOver(void);
//private:



#endif






