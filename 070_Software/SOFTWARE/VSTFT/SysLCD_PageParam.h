#ifndef __SYSLCD_PAGEPARAM_H
#define __SYSLCD_PAGEPARAM_H

#include "RealDatabase.h"


//参数界面按钮ID
typedef enum
{
	TFT_Param_WorkStationOne_Elec_Button = 26,
	TFT_Param_WorkStationTwo_Elec_Button,
	TFT_Param_WorkStationThr_Elec_Button,
	TFT_Param_Return_Button,

	TFT_Param_Button_Number
} TFT_Param_Button_E;

//参数界面文本框ID
typedef enum
{
	TFT_Param_Title = 0,
	TFT_Param_One_TestCount,
	TFT_Param_One_TurnTime,
	TFT_Param_One_LeaveTime,
	TFT_Param_One_ShortTime,
	TFT_Param_One_BrokenTime,
	TFT_Param_One_FaultCount,
	TFT_Param_One_CurrentDetectionIcon,
	TFT_Param_Two_TestCount,
	TFT_Param_Two_TurnTime,
	TFT_Param_Two_LeaveTime,
	TFT_Param_Two_ShortTime,
	TFT_Param_Two_BrokenTime,
	TFT_Param_Two_FaultCount,
	TFT_Param_Two_CurrentDetectionIcon,
	TFT_Param_Thr_TestCount,
	TFT_Param_Thr_TurnTime,
	TFT_Param_Thr_LeaveTime,
	TFT_Param_Thr_ShortTime,
	TFT_Param_Thr_BrokenTime,
	TFT_Param_Thr_FaultCount,
	TFT_Param_Thr_CurrentDetectionIcon,

	TFT_Param_Text_Number,
} TFT_Param_Show_Text_E;

//设定参数结构体
typedef struct
{
    uint32_t nTestCount;		   ///< 试验次数
    uint32_t nTurnTime;			   ///< 接通时间
    uint32_t nLeaveTime;		   ///< 断开时间
    uint32_t nShortTime;		   ///< 短路时间
    uint32_t nBrokenTime;		   ///< 断路时间
    uint32_t nFaultCount;		   ///< 故障次数
    uint8_t	 nCurrentDetection;	   ///< 电流检测
} TFT_Param_Value_T;

//设定参数刷新结构体
typedef struct
{
	uint8_t nTestCount;
	uint8_t nTurnTime;
	uint8_t nLeaveTime;
	uint8_t nShortTime;
	uint8_t nBrokenTime;
	uint8_t nFaultCount;
    uint8_t nCurrentDetectionIcon;
} TFT_Param_Show_Flag_T;

typedef enum
{
    TFT_PARAM_SAVE_INDEX_TEST_COUNT = 0,
    TFT_PARAM_SAVE_INDEX_TURN_TIME,
    TFT_PARAM_SAVE_INDEX_LEAVE_TIME,
    TFT_PARAM_SAVE_INDEX_SHORT_TIME,
    TFT_PARAM_SAVE_INDEX_BROKEN_TIME,
    TFT_PARAM_SAVE_INDEX_FAULT_COUNT,
    TFT_PARAM_SAVE_INDEX_CURRENT_DETECT,
} TFT_PARAM_SAVE_INDEX_E;


extern TFT_Param_Value_T	 g_stParamValue[TEST_TYPE_NUMBER][TEST_SELECT_NUMBER];
extern TFT_Param_Show_Flag_T g_stPageParamSaveFlag[TEST_TYPE_NUMBER][TEST_SELECT_NUMBER];

void SysLCDTFT_PageParamShowInfoProc(void);
BYTE SysLCDTFT_PageParamTextProc(BYTE nCount, uint16_t nId);
BYTE SysLCDTFT_PageParamButtonProc(uint16_t _nId);
void SysTFTLcd_PageParamGotoProc(void);
BYTE SysLCDTFT_PageParamIconProc(BYTE nCount, uint16_t _id);


#endif
