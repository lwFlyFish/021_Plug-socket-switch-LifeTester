#ifndef __SYSLCD_PAGEINFO_H
#define __SYSLCD_PAGEINFO_H

#include "RealDatabase.h"

//监控界面文本框ID
typedef enum
{
	TFT_Info_Title = 0,
	TFT_Info_One_SetCount,
	TFT_Info_One_Countting,
	TFT_Info_One_ElecCount,
	TFT_Info_One_FaultCount,
	TFT_Info_Two_SetCount,
	TFT_Info_Two_Countting,
	TFT_Info_Two_ElecCount,
	TFT_Info_Two_FaultCount,
	TFT_Info_Thr_SetCount,
	TFT_Info_Thr_Countting,
	TFT_Info_Thr_ElecCount,
	TFT_Info_Thr_FaultCount,
	TFT_Info_One_RunState,
	TFT_Info_One_ElecDetection,
	TFT_Info_Two_RunState,
	TFT_Info_Two_ElecDetection,
	TFT_Info_Thr_RunState,
	TFT_Info_Thr_ElecDetection,
	TFT_Info_One_FaultCle,
	TFT_Info_Two_FaultCle,
	TFT_Info_Thr_FaultCle,
	TFT_Info_Text_Number
} TFT_Info_Show_Text_E;

typedef enum
{
	TFT_Info_Return_Button = 22,
} TFT_Info_Button_E;

typedef struct
{
    uint8_t nDevStatus;			   ///< 设备运行状态
                                   ///< 电流检测，采用参数页面数值
                                   ///< 设定次数，采用参数页面数值
    uint32_t nRealCount;		   ///< 当前次数
    uint32_t nRealCurrentCount;	   ///< 电流次数
    uint32_t nRealErrorCount;	   ///< 故障次数
    uint8_t	 nFlagErrorClear;	   ///< 点击故障清除之后置1，试验开始前置0
} TFT_INFO_VALUE_T;				   ///< 监控页面的变量

typedef struct
{
    uint8_t nDevStatus;			  ///< 设备运行状态
    uint8_t nCurrentCheck;		  ///< 电流检测
    uint8_t nSetCount;			  ///< 设定次数，采用参数页面数值
    uint8_t nRealCount;			  ///< 当前次数
    uint8_t nRealCurrentCount;	  ///< 电流次数
    uint8_t nRealErrorCount;	  ///< 故障次数
    uint8_t nFlagErrorClear;	  ///<故障清除
} TFT_INFO_SHOW_FLAG_T;			  ///< 监控页面的刷新标志


extern TFT_INFO_VALUE_T		g_stPageInfoValue[TEST_SELECT_NUMBER];
extern TFT_INFO_SHOW_FLAG_T g_stPageInfoShowFlag[TEST_SELECT_NUMBER];

void SysLCDTFT_PageInfoShowInfoProc(void);
// BYTE SysLCDTFT_PageInfoTextProc(BYTE nCount);
BYTE SysLCDTFT_PageInfoButtonProc(uint16_t _id);
void SysTFTLcd_PageInfoGotoProc(void);
BYTE SysLCDTFT_PageInfoIconProc(BYTE nCount, uint16_t _id);

#endif
