#ifndef __RealDatabase_H
#define __RealDatabase_H 1
//
///////////////////////////////////////////////////////////////////////////
// �õ��ṹ���������Ե�ַ
///////////////////////////////////////////////////////////////////////////
//���ﶨ���OffsetAddr�����Ҫ��MyField��ƫ�ơ�����ǿ��ת����Ľṹָ����ô�����������ʽṹ���ֶΣ���ʵ������ʽ����û��Ҳ���������MyField�ֶΡ�
// ANSI C��׼�����κ�ֵΪ0�ĳ�����ǿ��ת�����κ�һ�����͵�ָ�룬����ת�������һ��NULLָ�룬���((MyStruct*)0)�Ľ������һ������ΪMyStruct*��NULLָ�롣
//����������NULLָ��������MyStruct�ĳ�Ա��Ȼ�ǷǷ��ģ���&(((MyStruct*)0)->MyField)����ͼ�������ȡMyField�ֶ����ݣ�
//�������Ǽ��㵱�ṹ��ʵ������ַΪ((MyStruct*)0)ʱMyField�ֶεĵ�ַ�������ı����������Ͳ����ɷ���MyField�Ĵ��룬
//�������Ǹ���MyStruct���ڴ沼�ֺͽṹ��ʵ����ַ�ڱ����ڼ������(����)��ַ����������ȫ������ͨ��NULLָ������ڴ�����⡣
//����Ϊ��ַ��ֵΪ0�����������ַ��ֵ�����ֶ�����ڽṹ���ַ��ƫ�ơ�
//
//��������������һ��Ҫʵ����һ��MyStruct���󣬲�����ֵ���ڱ����ڽ��У�û�������ڸ�����
//ʵ�����������ñ��������յ������������Ϣ���ڱ����ڼ���ĳЩֵ�ķ���������C++����к����е�(��̬)Ԫ��̼������ƣ�
//ֻ����C++����Ա��������ģ�弼���ڱ�������ɷǳ����ӵļ��㣬��ȱ��ģ��֧�ֵ�ANSI C���ⷽ���������Ҫ����ࡣ

#define OffsetAddr(struc, e) (size_t) & (((struc *)0)->e)
///////////////////////////////////////////////////////////////////////////

#include "SysConfig.h"

#include <stdint.h>
#include <string.h>
///////////////////////////////////////////////////////////////////////////
#define MAXCOUNTER 60000

#define TEMP_CH_NUM 4	  //�¶ȵ��� 4·AI7048
#define RESULT_NUM	20	  //ʵ������������

#define Motor_Step_PWM_Off_Reset (0)
#define Motor_Step_PWM_Off_Set	 (0xFFFF)
#define Motor_Step_PWM_Off		 Motor_Step_PWM_Off_Set

// #define FIFO_NUM		30
// #define REAL_BUF_NUM	10
//
// //�������
// #define MAXTESTPOINGCOUNT 5

///////////////////////////////////////////////////////////////////////////
//ͨ�Ź���
//Ϊ�˲���MM32X103.H��typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
//����  �޸�SUCESS�궨��ΪSUCESS_COMM
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

//���ò���-eeprom����
typedef struct
{
    u8 nSysSlaveCommAddr;
    u8 nTempPoint;
    u8 nHumiPoint;
    u8 nWindSpeedPoint;
    u8 nScalePoint;

    s16 arrTemp[TEMP_CH_NUM];	 //���õ��¶�
    u16 nHumi;
    u16 nWindSpeed;

    s16 nColdWaterStartVPT;	   //��Դ������ֵ

    u16 nLampTime;

} CONFIG_PARAM_DEF;

//ʱ��ṹ��
typedef struct
{
    vu8 nHour;
    vu8 nMinute;
    vu8 nSecond;

    //������������
    vu8	 nMonth;
    vu8	 nDay;
    vu8	 nWeek;
    vu16 nYear;

} RTC_TIME_DEF;
//�����ṹ��
extern RTC_TIME_DEF g_stRtcTime;

//ϵͳͨ��������Ϣ
typedef struct tagCmdInfo
{
    //����״̬
    BOOL nStatus;

    BYTE nCmd;
    //
    BYTE nType;

} CMDINFO;

typedef struct MyStruct
{
    u8 nWaterInfo;		// 0����  1 עˮ  2ȱˮ
    u8 nWaterBucket;	// 0���� 1����
    u8 nTempOver;		// 0���� 1����
} WARNING_INFO_DEF;

//��������-ʵʱ��������
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
    u32				 nTestTime;			   // min��λ
    char			 arrScaleValue[15];	   //ָ����ƽ�����ݵ�ַ
} TEST_PARAM_DEF;

// pid-EEPROM����
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


//����PID�����ı�־
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
    u16 nTime;	   //��ʱ��
    u16 nDelay;	   //�ݼ�ʱ��
} DELAY_INFO_DEF;

///////////////////////////////////////////////////////////////////////////
// Attribute
///////////////////////////////////////////////////////////////////////////
// public:
///////////////////////////////////////////////////////////////////////////////////////////////////
//����ҳ
typedef enum
{
	TFT_PAGE_TYPE_FIRST = 0,
	TFT_PAGE_TYPE_TEST,
	TFT_PAGE_TYPE_PARAM,
	TFT_PAGE_TYPE_INFO,

	TFT_PAGE_TYPE_NUMBER
} TFT_PAGE_TYPE_E;

//����/��ͷ����ˢ��ѡ��
typedef enum
{
	TEST_TYPE_SWITCH = 0,
	TEST_TYPE_SOCKET,

	TEST_TYPE_NUMBER
} TEST_TYPE_E;

//��λ
typedef enum
{
    TEST_SELECT_1 = 0,
	TEST_SELECT_2,
	TEST_SELECT_3,

	TEST_SELECT_NUMBER
} TEST_SELECT_E;


//ͬʱ������������ˢ��ѡ��
typedef enum
{
    TEST_MODE_MEAN_WHILE = 0,	 //ͬʱ����
    TEST_MODE_SEQUENCE,			 //��������

	TEST_MODE_NUMBER
} TFT_WorkNumber_Number_E;

//���鹤��ֹͣ����ˢ��ѡ��
typedef enum
{
    TEST_STATUS_STOP = 0,	 //ֹͣ����
    TEST_STATUS_START,		 //���鹤��

	TEST_STATUS_NUMBER
} TEST_STATUS_E;


//�Զ� �ֶ�
typedef enum
{
	TEST_MODE_ACTION_AUTO = 0,
	TEST_MODE_ACTION_MANUAL,

	TEST_MODE_ACTION_NUMBER
} TEST_MODE_ACTION_E;

//ͼ��״̬
typedef enum
{
	TFT_STATUS_ICON_DISABLE = 0,
	TFT_STATUS_ICON_ENABLE,

    TFT_STATUS_ICON_NUMBER
} TFT_STATUS_ICON_E;


////////////////////////����//////////////////////////
extern TEST_TYPE_E g_nTestType;
extern uint8_t	   g_nFlagMeanWhile_Again;
extern uint8_t	   g_nFlagTest_Stop;


//ϵͳ״̬��־
//	extern SYSSTATUSTAGINFO g_nSysStatusTagInfo;

//�޸�ʱ���־
extern u8 g_nSetSysTimeFlag;
//
//	extern SYSSPECIALSTATUSINFO g_nSysSpecialStatusInfo;

extern u8 g_nFillStaticInfoFlag;


// ADת��ģʽ
extern volatile BYTE g_nADConvertMode;

//ϵͳ�˵�������־
//	extern SYSMENUFLAG m_stSysMenuFlag;

//ʵʱˢ�±�־λ
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

extern uint8_t g_nMotor2Status_Step;	// 0Ĭ�ϣ�1����״̬��2����������3����
extern uint8_t g_nMotor3Status_Step;
extern uint8_t g_nGasInputDisableFlag;
extern uint8_t g_nScaleReadEnd_bsa224s;
extern uint8_t g_nScaleReadStart_bsa224s;
extern uint8_t g_nScaleCount;
extern uint8_t g_nEnableScaleZeroFlag;
extern uint8_t g_nScaleCommFlag_bsa224s;


//����������в������
extern BYTE g_nTestStepFlag;

//
extern volatile UINT16 g_nSysTimerCtrl;

// lyy new
extern volatile u16 g_nSysLedRunCount;
extern volatile u16 g_nSysLedBlinkCount;
extern volatile u16 g_nAdcPeriodFilterCount;
extern volatile u16 g_nRtcReadTimerCount;
extern volatile u16 g_nStepMotorFsmCount;
//ϵͳ����
extern volatile BYTE g_nSysProcTimerCtrl;

//ϵͳ��ʾʱ��Ƭ
extern volatile BYTE g_nSysDisplayTimeCtrl;
// 1200ms��ת��ʱ
extern volatile UINT16 g_nSysTurnBackTimeCtrl;

//
extern volatile BYTE g_nSysStaticFreshTimerCtrl;

//�豸��ǰ����
// extern DEVCURPARAMINFO g_stDevCurParam;
//�豸�ر����
// extern DEVSPECIALPARAMINFO g_stDevSpecialParam;
// 4��λ�궨���� - 1x/2x/5x/10x�궨�������� - 372�ֽ�
// 	extern SYSBDPARAMINFO g_stSysBDParamInfo[4];
// 	//������ - 220�ֽ�
// 	extern TESTRESULTINFO g_stTestResultInfo[1]; //�������ṹ����
// 	//ϵͳʱ�䱣�� - 10�ֽ�
// 	extern SYSTIMEPROTECTINFO g_stSysTimeProtectInfo;

extern volatile int32 g_nCurForceBaseValue;
extern volatile int32 g_nCurForceValue;

//ͨ�ű���-------------------------------------------------------------
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
//ͨ��״̬ 0:�ǽ���״̬, 1:����״̬
extern volatile u8 g_bRecvStatus_SP1;
//ͨ��״̬
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
//ͨ��״̬ 0:�ǽ���״̬, 1:����״̬
extern volatile u8 g_bRecvStatus_SP2;
//ͨ��״̬
extern volatile CMDINFO g_stSysCmdInfo_SP2;
extern volatile s16		g_nSP2RecvDelay;
//�������ݳ�ʱ����
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
//ͨ��״̬ 0:�ǽ���״̬, 1:����״̬
extern volatile u8 g_bRecvStatus_SP3;
//ͨ��״̬
extern volatile CMDINFO g_stSysCmdInfo_SP3;
extern volatile s16		g_nSP3RecvDelay;
//�������ݳ�ʱ����
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
//ͨ��״̬ 0:�ǽ���״̬, 1:����״̬
extern volatile u8 g_bRecvStatus_SP4;
//ͨ��״̬
extern volatile CMDINFO g_stSysCmdInfo_SP4;
extern volatile s16		g_nSP4RecvDelay;
//�������ݳ�ʱ����
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
//ͨ��״̬ 0:�ǽ���״̬, 1:����״̬
extern volatile u8 g_bRecvStatus_SP5;
//ͨ��״̬
extern volatile CMDINFO g_stSysCmdInfo_SP5;
extern volatile s16		g_nSP5RecvDelay;
//�������ݳ�ʱ����
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

//ͨ��״̬ 0:�ǽ���״̬, 1:����״̬
extern volatile u8 g_bRecvStatus_SP6;
//ͨ��״̬
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


// 485��
extern volatile u8 g_nSysUsart3RecvOver_Flag;
extern volatile u8 g_nSysUsart3SendOver_Flag;
extern volatile u8 g_n485CommStatusShiftDelay_SP3;

extern volatile u8 g_nSysUsart6RecvOver_Flag;
extern volatile u8 g_nSysUsart6SendOver_Flag;
extern volatile u8 g_n485CommStatusShiftDelay_SP6;

extern volatile u8 g_nSysUsart4RecvOver_Flag;
extern volatile u8 g_nSysUsart4SendOver_Flag;
extern volatile u8 g_n485CommStatusShiftDelay_SP4;

//��дָ��
extern volatile BYTE g_nWrPos;
extern volatile BYTE g_nRdPos;
//
//	extern SYSPASSWORDINFO g_stSyInPassWordInfo;
// 	//��������
extern TEST_PARAM_DEF g_stRealParam;
// 	extern TEST_PARAM_BUF_DEF g_arrRealParamBuf[REAL_BUF_NUM];
// 	//���ò���
extern CONFIG_PARAM_DEF		g_stConfigParam;
extern CONFIG_PARAM_TAG_DEF g_stConfigTag;
//
// 	//PID����
extern PID_AI_PARAM_DEF	  g_stPIDAIParam;
extern PID_AI_SET_TAG_DEF g_stPIDAISetTag;
// 	extern RESULT_PARAM_DEF g_stResultParam[RESULT_NUM];


//�궨��Ϣ
//	extern BD_PARAM_DEF g_stBDParam;
extern u8 g_nBDCurPointCount;

// AIͨ��
extern u8 g_nCurCommAddr;
extern u8 g_nAICmdAckOkFlag;
//�����¿�����־
extern u8 g_nAISetBusy;

//��¼����Ľ������
extern u8 g_nResultItemTotal;
//���ҳ�����
extern u8 g_nResultItemCount;
//����������λ��
extern u8 g_nResultItemPoint;

// PID��������������
extern u8 g_nPIDAutoEndFlag;

extern DELAY_INFO_DEF g_stLampInfo, g_stColdWaterInfo, g_stAI7048DelayInfo;

extern char g_stWarningBuf[10];

//����ֵ���������
extern u16 g_nAbsEncoderValue;
extern u16 g_nReadAbsEncoderTag;
// private:


///////////////////////////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////////////////////////
// public:
//��װ���ֽڵ�������
extern int32 ConfigMulitBytesToValueProc(BYTE nFirstValue, BYTE nSeocndValue, BYTE nThirdValue, BYTE nFourthValue);

//
extern u8	SysDecToBCDProc(u8 nNum);
extern u8	SysBCDToDecProc(u8 bcd);	//ʮ����תBCD
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
