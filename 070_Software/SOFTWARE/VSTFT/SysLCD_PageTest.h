#ifndef __SYSLCD_PAGETEST_H
#define __SYSLCD_PAGETEST_H

#include "RealDatabase.h"
#include "SysConfig.h"

//试验界面控件ID
typedef enum
{
	TFT_Test_Text_Title = 0,
    TFT_Test_Text_WorkMethod,			  //工作方式
    TFT_Test_Text_WorkState,			  //工作状态
    TFT_Test_Icon_Start,				  //启动
    TFT_Test_Icon_Automatic_One,		  //工位一自动
    TFT_Test_Icon_Automatic_Two,		  //工位二自动
    TFT_Test_Icon_Automatic_Thr,		  //工位三自动
    TFT_Test_Icon_Manual_One,			  //工位一手动
    TFT_Test_Icon_Manual_Two,			  //工位二手动
    TFT_Test_Icon_Manual_Thr,			  //工位三手动
    TFT_Test_Control_Show_Number = 10,	  //刷新控件的数量

    TFT_Test_Button_Param = 10,	   //参数设置界面
    TFT_Test_Button_First,		   //欢迎界面
    TFT_Test_Button_Info,		   //监控界面
    TFT_Test_Button_WorkMethod,	   //工作方式

} TFT_Test_Control_E;

typedef struct
{
	uint8_t nWorkMode;
	uint8_t nWorkState;
	uint8_t nStart;
    uint8_t arrAutomatic[TEST_SELECT_NUMBER];
    uint8_t arrManual[TEST_SELECT_NUMBER];
} TFT_Test_Show_Flag_T;

typedef struct
{
    uint8_t nWorkMode;	   ///< 试验模式，同时或依次
    uint8_t nWorkState;	   ///< 工作状态
    uint8_t nStep;
	uint8_t arrAutomatic[TEST_SELECT_NUMBER];
	uint8_t arrManual[TEST_SELECT_NUMBER];
} TFT_Test_Value_T;


extern uint8_t g_nFlagMeanWhile;	//同时工作开始试验标志位
extern uint8_t g_nFlagAgain;		//轮流工作开始试验标志位

extern TFT_Test_Value_T		g_stTftTestValue;
extern TFT_Test_Show_Flag_T g_stTestShowFlag;

void SysLCDTFT_PageTestShowInfoProc(void);
BYTE SysLCDTFT_PageTestTextProc(BYTE nCount, uint16_t _id);
BYTE SysLCDTFT_PageTestButtonProc(uint16_t _id);
BYTE SysLCDTFT_PageTestIconProc(BYTE nCount, uint16_t _id);
void SysTFTLcd_PageTestGotoProc(void);


#endif
