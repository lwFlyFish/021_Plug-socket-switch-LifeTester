#ifndef __SYS_TEST_PROC_H
#define __SYS_TEST_PROC_H

#include "RealDatabase.h"


typedef enum	///< 实验过程的步骤
{
    TEST_STEP_IDLE = 0,			   ///< 空闲状态
    TEST_STEP_STOP,				   ///< 停止状态
    TEST_STEP_START,			   ///< 点了启动，状态跳转到这里，然后初始化时间变量。。
    TEST_STEP_START_MEAN_WHILE,	   ///< 同时工作模式
    TEST_STEP_START_SEQUENCE,	   ///< 依次工作模式
    TEST_STEP_START_WARN,		   ///< 警告
} TEST_STEP_E;

typedef struct
{
    uint8_t nWorkMode;	  ///选择工作模式
    uint8_t nStatus;	  ///< 实验状态
    uint8_t nStep;		  ///< 工作步骤
    uint8_t nStepBkp;	  ///< 工作步骤
    uint8_t nSelect;	  ///< 轮流工作模式下的当前工位
} TEST_VALUE_T;

typedef struct
{
    uint8_t	 nModeStep;		 ///< 同时工作模式内的工作步骤
    uint8_t	 nValidFlag;	 ///< 电流检测模式下，是否发生故障
    uint8_t	 nEndFlag;		 ///< 所有动作完成标志
    uint32_t nRunTime;		 ///< 运行时间，单位是MS
    uint32_t nTestCount;	 ///< 当前试验次数
    uint32_t nValidTime;	 ///< 电流检测模式下，检测到有或无的时间
    uint32_t nFaultCount;	 ///< 错误次数
} TEST_INFO_T;


extern TEST_VALUE_T g_stTestValue;	  ///< 只用一个就行，不用数组
extern TEST_VALUE_T g_stTestModeValue;
extern TEST_INFO_T	g_stTestInfo[TEST_SELECT_NUMBER];

extern void SysTestCtrlProc(void);
extern void SysTestCtrlMeanWhile(void);	   //同时工作处理函数
extern void SysTestCtrlSequence(void);	   //轮流工作处理函数
extern void SysTestCtrlManual(void);	   //手动工作处理函数
extern void SysTestManualIconProc(TEST_SELECT_E _select, uint8_t _flag);
extern void SysTestAutoIconProc(TEST_SELECT_E _select, uint8_t _flag);

#endif
