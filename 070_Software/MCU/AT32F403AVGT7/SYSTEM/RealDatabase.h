#ifndef __RealDatabase_H
#define __RealDatabase_H 1
//
///////////////////////////////////////////////////////////////////////////
// 得到结构体变量的相对地址
///////////////////////////////////////////////////////////////////////////
//这里定义的OffsetAddr宏就是要的MyField的偏移。这样强制转换后的结构指针怎么可以用来访问结构体字段？其实这个表达式根本没有也不打算访问MyField字段。
// ANSI C标准允许任何值为0的常量被强制转换成任何一种类型的指针，并且转换结果是一个NULL指针，因此((MyStruct*)0)的结果就是一个类型为MyStruct*的NULL指针。
//如果利用这个NULL指针来访问MyStruct的成员当然是非法的，但&(((MyStruct*)0)->MyField)的意图并非想存取MyField字段内容，
//而仅仅是计算当结构体实例的首址为((MyStruct*)0)时MyField字段的地址。聪明的编译器根本就不生成访问MyField的代码，
//而仅仅是根据MyStruct的内存布局和结构体实例首址在编译期计算这个(常量)地址，这样就完全避免了通过NULL指针访问内存的问题。
//又因为首址的值为0，所以这个地址的值就是字段相对于结构体基址的偏移。
//
//如上做法避免了一定要实例化一个MyStruct对象，并且求值是在编译期进行，没有运行期负担。
//实际上这种利用编译器掌握的整个程序的信息以在编译期计算某些值的方法与现在C++编程中很流行的(静态)元编程技术类似，
//只不过C++程序员可以利用模板技术在编译期完成非常复杂的计算，而缺乏模板支持的ANSI C在这方面的能力则要弱许多。

#define OffsetAddr(struc, e) (size_t) & (((struc *)0)->e)
///////////////////////////////////////////////////////////////////////////

#include "SysConfig.h"

#include <stdint.h>
#include <string.h>
///////////////////////////////////////////////////////////////////////////
#define MAXCOUNTER 60000

#define TEMP_CH_NUM 4	  //温度点数 4路AI7048
#define RESULT_NUM	20	  //实验结果保存数量

#define Motor_Step_PWM_Off_Reset (0)
#define Motor_Step_PWM_Off_Set	 (0xFFFF)
#define Motor_Step_PWM_Off		 Motor_Step_PWM_Off_Set

// #define FIFO_NUM		30
// #define REAL_BUF_NUM	10
//
// //最大测点数
// #define MAXTESTPOINGCOUNT 5

///////////////////////////////////////////////////////////////////////////
//通信过程
//为了不与MM32X103.H中typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
//混淆  修改SUCESS宏定义为SUCESS_COMM
#define SYNSTEP	 0
#define CMDSTEP	 1
#define DATASTEP 2
//#define SUCESS				3
#define SUCESS_COMM 3
#define FAILURE		4

///////////////////////////////////////////////////////////////////////////

#define DEVOFFLINE 0
#define DEVONLINE  1

#define TFT_REFRESH_DELAY_TIME 25

#define EE_VALUE_INIT (0x66)

typedef struct
{
    u8 arrTempSetReg[TEMP_CH_NUM];
    u8 arrTempTimeout[TEMP_CH_NUM];
} CONFIG_PARAM_TAG_DEF;

//设置参数-eeprom保存
typedef struct
{
    u8 nSysSlaveCommAddr;
    u8 nTempPoint;
    u8 nHumiPoint;
    u8 nWindSpeedPoint;
    u8 nScalePoint;

    s16 arrTemp[TEMP_CH_NUM];	 //设置的温度
    u16 nHumi;
    u16 nWindSpeed;

    s16 nColdWaterStartVPT;	   //冷源启动差值

    u16 nLampTime;

} CONFIG_PARAM_DEF;

//时间结构体
typedef struct
{
    vu8 nHour;
    vu8 nMinute;
    vu8 nSecond;

    //公历日月年周
    vu8	 nMonth;
    vu8	 nDay;
    vu8	 nWeek;
    vu16 nYear;

} RTC_TIME_DEF;
//日历结构体
extern RTC_TIME_DEF g_stRtcTime;

//系统通信命令信息
typedef struct tagCmdInfo
{
    //命令状态
    BOOL nStatus;

    BYTE nCmd;
    //
    BYTE nType;

} CMDINFO;

typedef struct MyStruct
{
    u8 nWaterInfo;		// 0正常  1 注水  2缺水
    u8 nWaterBucket;	// 0正常 1干烧
    u8 nTempOver;		// 0正常 1过热
} WARNING_INFO_DEF;

//测量参数-实时测量更新
typedef struct
{
    u8				 arrTempSetStatus[TEMP_CH_NUM];
    u8				 nSHTErrFlag;
    u8				 nTestStatus;
    u8				 nLampStatus;
    u8				 nMotoStatus;
    u8				 nTempCtrlStatus;
    u8				 nHumiCtrlStatus;
    u8				 nColdWaterStatus;
    WARNING_INFO_DEF stWarning;
    s16				 arrTemp[TEMP_CH_NUM];
    u16				 nSHT15Temp;
    s16				 nDewPoint;
    u16				 nHumi;
    u16				 nWindSpeed;
    s32				 nScaleValue;
    u32				 nTestTime;			   // min单位
    char			 arrScaleValue[15];	   //指向天平的数据地址
} TEST_PARAM_DEF;

// pid-EEPROM保存
typedef struct
{
    u8	nPIDCh;
    u8	arrPPoint[TEMP_CH_NUM];
    u8	arrIPoint[TEMP_CH_NUM];
    u8	arrDPoint[TEMP_CH_NUM];
    u16 arrP[TEMP_CH_NUM];
    u16 arrI[TEMP_CH_NUM];
    u16 arrD[TEMP_CH_NUM];
} PID_AI_PARAM_DEF;


//设置PID参数的标志
typedef struct
{
    u8 arrPidSetPReg[TEMP_CH_NUM];
    u8 arrPidSetIReg[TEMP_CH_NUM];
    u8 arrPidSetDReg[TEMP_CH_NUM];
    u8 arrPidAutoReg[TEMP_CH_NUM];
    u8 arrPidSetPTimeout[TEMP_CH_NUM];
    u8 arrPidSetITimeout[TEMP_CH_NUM];
    u8 arrPidSetDTimeout[TEMP_CH_NUM];
    u8 arrPidAutoTimeout[TEMP_CH_NUM];
} PID_AI_SET_TAG_DEF;

typedef struct
{
    u16 nTime;	   //总时长
    u16 nDelay;	   //递减时间
} DELAY_INFO_DEF;

///////////////////////////////////////////////////////////////////////////
// Attribute
///////////////////////////////////////////////////////////////////////////
// public:
///////////////////////////////////////////////////////////////////////////////////////////////////
//所有页
typedef enum
{
	TFT_PAGE_TYPE_FIRST = 0,
	TFT_PAGE_TYPE_TEST,
	TFT_PAGE_TYPE_PARAM,
	TFT_PAGE_TYPE_INFO,

	TFT_PAGE_TYPE_NUMBER
} TFT_PAGE_TYPE_E;

//开关/插头插座刷新选择
typedef enum
{
	TEST_TYPE_SWITCH = 0,
	TEST_TYPE_SOCKET,

	TEST_TYPE_NUMBER
} TEST_TYPE_E;

//工位
typedef enum
{
    TEST_SELECT_1 = 0,
	TEST_SELECT_2,
	TEST_SELECT_3,

	TEST_SELECT_NUMBER
} TEST_SELECT_E;


//同时工作轮流工作刷新选择
typedef enum
{
    TEST_MODE_MEAN_WHILE = 0,	 //同时工作
    TEST_MODE_SEQUENCE,			 //轮流工作

	TEST_MODE_NUMBER
} TFT_WorkNumber_Number_E;

//试验工作停止工作刷新选择
typedef enum
{
    TEST_STATUS_STOP = 0,	 //停止工作
    TEST_STATUS_START,		 //试验工作

	TEST_STATUS_NUMBER
} TEST_STATUS_E;


//自动 手动
typedef enum
{
	TEST_MODE_ACTION_AUTO = 0,
	TEST_MODE_ACTION_MANUAL,

	TEST_MODE_ACTION_NUMBER
} TEST_MODE_ACTION_E;

//图标状态
typedef enum
{
	TFT_STATUS_ICON_DISABLE = 0,
	TFT_STATUS_ICON_ENABLE,

    TFT_STATUS_ICON_NUMBER
} TFT_STATUS_ICON_E;


////////////////////////变量//////////////////////////
extern TEST_TYPE_E g_nTestType;
extern uint8_t	   g_nFlagMeanWhile_Again;
extern uint8_t	   g_nFlagTest_Stop;


//系统状态标志
//	extern SYSSTATUSTAGINFO g_nSysStatusTagInfo;

//修改时间标志
extern u8 g_nSetSysTimeFlag;
//
//	extern SYSSPECIALSTATUSINFO g_nSysSpecialStatusInfo;

extern u8 g_nFillStaticInfoFlag;


// AD转换模式
extern volatile BYTE g_nADConvertMode;

//系统菜单操作标志
//	extern SYSMENUFLAG m_stSysMenuFlag;

//实时刷新标志位
// 	extern WORK_PAGE_TAG_DEF g_nWorkPageShowTag;
// 	extern PID_PAGE_TAG_DEF g_stPIDPageShowTag;
// 	extern CONFIG_PAGE_TAG_DEF g_stConfigPageShowTag;
// 	//
// 	extern STATICFILLTAG_WORKPAGEFIRST g_stSysStaticFillTag_WorkPageFirst;
// 	extern STATICFILLTAG_TESTPARAMPAGEFIRST g_stStaticFillTag_TestParamPageFirst;
// 	extern STATICFILLTAG_TESTPARAMPAGESECOND g_stStaticFillTag_TestParamPageSecond;
//
// 	extern STATICFILLTAG_TESTRESULTPAGEFIRST g_stStaticFillTag_TestResultPageFirst;
// 	extern STATICFILLTAG_TESTRESULTPAGESECOND g_stStaticFillTag_TestResultPageSecond;
// 	//
// 	extern STATICFILLTAG_RESULTSTATPAGEFIRST g_stStaticFillTag_ResultStatPageFirst;
// 	extern STATICFILLTAG_RESULTSTATPAGESECOND g_stStaticFillTag_ResultStatPageSecond;
// 	//
// 	extern STATICFILLTAG_CONFIGPARAMPAGEFIRST g_stStaticFillTag_ConfigParamParam;
// 	extern STATICFILLTAG_BDPAGEFIRST g_stStaticFillTag_BDPageParam;

extern uint8_t g_nMotor2Status_Step;	// 0默认，1发送状态，2控制启动，3结束
extern uint8_t g_nMotor3Status_Step;
extern uint8_t g_nGasInputDisableFlag;
extern uint8_t g_nScaleReadEnd_bsa224s;
extern uint8_t g_nScaleReadStart_bsa224s;
extern uint8_t g_nScaleCount;
extern uint8_t g_nEnableScaleZeroFlag;
extern uint8_t g_nScaleCommFlag_bsa224s;


//试验过程运行步骤控制
extern BYTE g_nTestStepFlag;

//
extern volatile UINT16 g_nSysTimerCtrl;

// lyy new
extern volatile u16 g_nSysLedRunCount;
extern volatile u16 g_nSysLedBlinkCount;
extern volatile u16 g_nAdcPeriodFilterCount;
extern volatile u16 g_nRtcReadTimerCount;
extern volatile u16 g_nStepMotorFsmCount;
//系统处理
extern volatile BYTE g_nSysProcTimerCtrl;

//系统显示时间片
extern volatile BYTE g_nSysDisplayTimeCtrl;
// 1200ms反转定时
extern volatile UINT16 g_nSysTurnBackTimeCtrl;

//
extern volatile BYTE g_nSysStaticFreshTimerCtrl;

//设备当前参数
// extern DEVCURPARAMINFO g_stDevCurParam;
//设备特别参数
// extern DEVSPECIALPARAMINFO g_stDevSpecialParam;
// 4档位标定参数 - 1x/2x/5x/10x标定参数数组 - 372字节
// 	extern SYSBDPARAMINFO g_stSysBDParamInfo[4];
// 	//试验结果 - 220字节
// 	extern TESTRESULTINFO g_stTestResultInfo[1]; //试验结果结构数组
// 	//系统时间保护 - 10字节
// 	extern SYSTIMEPROTECTINFO g_stSysTimeProtectInfo;

extern volatile int32 g_nCurForceBaseValue;
extern volatile int32 g_nCurForceValue;

//通信变量-------------------------------------------------------------
extern volatile u8 g_nSysCommTimerCtrl;
// UART1
extern volatile u16 g_nSysInitComm1Timer;
extern volatile u16 g_nSysCommTimeoutCtrl_SP1;
extern volatile u32 g_nSysCommDelayTime_SP1;
extern volatile s16 g_nSysCommStatusCheck_SP1;
extern volatile u8	g_nSysCommStatusStep_SP1;
extern volatile u8	g_nSysCommTimeoutTag_SP1;
extern volatile u8	g_nDevCommStatus_SP1;
extern volatile u8	g_nSysAddrError_SP1;
//通信状态 0:非接收状态, 1:接收状态
extern volatile u8 g_bRecvStatus_SP1;
//通信状态
extern volatile CMDINFO g_stSysCmdInfo_SP1;
extern volatile s16		g_nSP1RecvDelay;

// UART2
extern volatile u16 g_nSysInitComm2Timer;
extern volatile u16 g_nSysCommTimeoutCtrl_SP2;
extern volatile u32 g_nSysCommDelayTime_SP2;
extern volatile s16 g_nSysCommStatusCheck_SP2;
extern volatile u8	g_nSysCommStatusStep_SP2;
extern volatile u8	g_nSysCommTimeoutTag_SP2;
extern volatile u8	g_nDevCommStatus_SP2;
extern volatile u8	g_nSysAddrError_SP2;
//通信状态 0:非接收状态, 1:接收状态
extern volatile u8 g_bRecvStatus_SP2;
//通信状态
extern volatile CMDINFO g_stSysCmdInfo_SP2;
extern volatile s16		g_nSP2RecvDelay;
//请求数据超时计数
extern volatile u16 g_nSysRequestDataTimeoutTimer2;

// UART3
extern volatile u16 g_nSysInitComm3Timer;
extern volatile u16 g_nSysCommTimeoutCtrl_SP3;
extern volatile u32 g_nSysCommDelayTime_SP3;
extern volatile s16 g_nSysCommStatusCheck_SP3;
extern volatile u8	g_nSysCommStatusStep_SP3;
extern volatile u8	g_nSysCommTimeoutTag_SP3;
extern volatile u8	g_nDevCommStatus_SP3;
extern volatile u8	g_nSysAddrError_SP3;
//通信状态 0:非接收状态, 1:接收状态
extern volatile u8 g_bRecvStatus_SP3;
//通信状态
extern volatile CMDINFO g_stSysCmdInfo_SP3;
extern volatile s16		g_nSP3RecvDelay;
//请求数据超时计数
extern volatile u16 g_nSysRequestDataTimeoutTimer3;

// UAR4
extern volatile u16 g_nSysInitComm4Timer;
extern volatile u16 g_nSysCommTimeoutCtrl_SP4;
extern volatile u32 g_nSysCommDelayTime_SP4;
extern volatile s16 g_nSysCommStatusCheck_SP4;
extern volatile u8	g_nSysCommStatusStep_SP4;
extern volatile u8	g_nSysCommTimeoutTag_SP4;
extern volatile u8	g_nDevCommStatus_SP4;
extern volatile u8	g_nSysAddrError_SP4;
//通信状态 0:非接收状态, 1:接收状态
extern volatile u8 g_bRecvStatus_SP4;
//通信状态
extern volatile CMDINFO g_stSysCmdInfo_SP4;
extern volatile s16		g_nSP4RecvDelay;
//请求数据超时计数
extern volatile u16 g_nSysRequestDataTimeoutTimer4;

// UAR5
extern volatile u16 g_nSysInitComm5Timer;
extern volatile u16 g_nSysCommTimeoutCtrl_SP5;
extern volatile u32 g_nSysCommDelayTime_SP5;
extern volatile s16 g_nSysCommStatusCheck_SP5;
extern volatile u8	g_nSysCommStatusStep_SP5;
extern volatile u8	g_nSysCommTimeoutTag_SP5;
extern volatile u8	g_nDevCommStatus_SP5;
extern volatile u8	g_nSysAddrError_SP5;
//通信状态 0:非接收状态, 1:接收状态
extern volatile u8 g_bRecvStatus_SP5;
//通信状态
extern volatile CMDINFO g_stSysCmdInfo_SP5;
extern volatile s16		g_nSP5RecvDelay;
//请求数据超时计数
extern volatile u16 g_nSysRequestDataTimeoutTimer5;

// UART6
extern volatile u16 g_nSysInitComm6Timer;
extern volatile u16 g_nSysCommTimeoutCtrl_SP6;
extern volatile u32 g_nSysCommDelayTime_SP6;
extern volatile s16 g_nSysCommStatusCheck_SP6;
extern volatile u8	g_nSysCommStatusStep_SP6;
extern volatile u8	g_nSysCommTimeoutTag_SP6;
extern volatile u8	g_nDevCommStatus_SP6;
extern volatile u8	g_nSysAddrError_SP6;

//通信状态 0:非接收状态, 1:接收状态
extern volatile u8 g_bRecvStatus_SP6;
//通信状态
extern volatile CMDINFO g_stSysCmdInfo_SP6;
extern volatile s16		g_nSP6RecvDelay;
extern volatile u16		g_nSysRequestDataTimeoutTimer6;

///////////////////////////////////////////////////////////////////////
extern volatile u8 g_nUSART1_Tx_Char;
extern volatile u8 g_nUARTSendOver_Flag;

//
extern volatile u8 g_nSysAddrError_SP1;
extern volatile u8 g_nSysAddrError_SP2;
extern volatile u8 g_nSysAddrError_SP3;


// 485用
extern volatile u8 g_nSysUsart3RecvOver_Flag;
extern volatile u8 g_nSysUsart3SendOver_Flag;
extern volatile u8 g_n485CommStatusShiftDelay_SP3;

extern volatile u8 g_nSysUsart6RecvOver_Flag;
extern volatile u8 g_nSysUsart6SendOver_Flag;
extern volatile u8 g_n485CommStatusShiftDelay_SP6;

extern volatile u8 g_nSysUsart4RecvOver_Flag;
extern volatile u8 g_nSysUsart4SendOver_Flag;
extern volatile u8 g_n485CommStatusShiftDelay_SP4;

//读写指针
extern volatile BYTE g_nWrPos;
extern volatile BYTE g_nRdPos;
//
//	extern SYSPASSWORDINFO g_stSyInPassWordInfo;
// 	//测量参数
extern TEST_PARAM_DEF g_stRealParam;
// 	extern TEST_PARAM_BUF_DEF g_arrRealParamBuf[REAL_BUF_NUM];
// 	//设置参数
extern CONFIG_PARAM_DEF		g_stConfigParam;
extern CONFIG_PARAM_TAG_DEF g_stConfigTag;
//
// 	//PID参数
extern PID_AI_PARAM_DEF	  g_stPIDAIParam;
extern PID_AI_SET_TAG_DEF g_stPIDAISetTag;
// 	extern RESULT_PARAM_DEF g_stResultParam[RESULT_NUM];


//标定信息
//	extern BD_PARAM_DEF g_stBDParam;
extern u8 g_nBDCurPointCount;

// AI通信
extern u8 g_nCurCommAddr;
extern u8 g_nAICmdAckOkFlag;
//设置温控器标志
extern u8 g_nAISetBusy;

//记录保存的结果数量
extern u8 g_nResultItemTotal;
//结果页面计数
extern u8 g_nResultItemCount;
//最后结果保存的位置
extern u8 g_nResultItemPoint;

// PID参数自整定结束
extern u8 g_nPIDAutoEndFlag;

extern DELAY_INFO_DEF g_stLampInfo, g_stColdWaterInfo, g_stAI7048DelayInfo;

extern char g_stWarningBuf[10];

//绝对值编码器结果
extern u16 g_nAbsEncoderValue;
extern u16 g_nReadAbsEncoderTag;
// private:


///////////////////////////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////////////////////////
// public:
//组装多字节到长整形
extern int32 ConfigMulitBytesToValueProc(BYTE nFirstValue, BYTE nSeocndValue, BYTE nThirdValue, BYTE nFourthValue);

//
extern u8	SysDecToBCDProc(u8 nNum);
extern u8	SysBCDToDecProc(u8 bcd);	//十进制转BCD
extern u16	SysWordToBCDProc(u16 nNum);
extern u16	SysBCDToWordProc(u16 nBCD);
extern void SysDWordToBCDProc(u8 *pBCDStr, u32 nNum, u8 nLen);

extern void StringCat(char *pStr1, char *pStr2);
extern void StringCopy(char *pStr1, char *pStr2, unsigned char nLen);
extern u8	StringLen(char *pStr);
extern void MemSet(char *pStr, char nData, u8 nLen);

extern u16 GetSysCRCValueProc(u8 *pMsg, u16 nDataLen);

extern void			StrToHex(BYTE *pbDest, BYTE *pbSrc, int nLen);
extern void			HexToStr(BYTE *pbDest, BYTE *pbSrc, int nLen);
extern u8			GetDecNumber(int num);
extern u32			DecimalWeight(u8 nWeight);
extern float		IntToFloatProc(s32 nSource, u8 nPoints);
extern s32			FloatToIntProc(float nSource, u8 *pSourcePoints, u8 nLimitPointsMax);
extern RTC_TIME_DEF GetSoftWareBuildTargetTime(void);
// private:


#endif
///////////////////////////////////////////////////////////////////////////
