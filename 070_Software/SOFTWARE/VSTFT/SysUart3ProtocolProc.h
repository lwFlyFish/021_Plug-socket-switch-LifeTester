#ifndef __SysUart3ProtocolProc_H
#define __SysUart3ProtocolProc_H 1
//
///////////////////////////////////////////////////////////////////////////
#include "SysConfig.h"

#define TEMP_MAX 450	// 45.0度
#define HUMI_MAX 700	// 70.0%RH

typedef enum
{
    Front = 0,	  //从前端
    Back  = 1,	  //从后端
} CURVE_DISP_TYPE_DEF;

typedef struct
{
    s16 nXOffect;	 //水平偏移
    u16 nXMUL;		 //水平缩放
    s16 nYOffect;	 //垂直偏移
    u16 nYMUL;		 //垂直缩放
} CURVE_ZOOM_DEF;

//需要保存的位置
typedef struct
{
    CURVE_ZOOM_DEF stCurveInfo;	   //放大缩小和偏移
    s16			   nHmax;		   //垂直方向最大
    s16			   nHmin;		   //垂直方向最小
} CURVE_INFO_SAVE_DEF, *pCURVE_INFO_SAVE_DEF;

typedef struct
{
    CURVE_DISP_TYPE_DEF emDispType;
    u8					nCh;
    u8					nDataLen;
    UINT16				nPageID;
    UINT16				nCtrlID;
    CURVE_ZOOM_DEF		stZoomInfo;
    s16					nHmax;
    s16					nHmin;
    u16					arrData[8];
} CURVE_DEF, *pCURVE_DEF;
extern CURVE_DEF g_stTempCurveInfo;

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
// Attribute
///////////////////////////////////////////////////////////////////////////
// public:
//设备地址
extern int16 g_nAddr_SP3;
extern u16	 g_arrRealSensorValue_U[3];
extern u16	 g_arrRealSensorValue_I[3];
// private:
// Comm
static BYTE m_nCurStep_SP3;

static u16	 m_nCmd_SP3;
static int16 m_nLen_SP3;

//
static int16 m_nPageID;
static int16 m_nCtrlID;
static BYTE	 m_nCtrlType;

//
static BYTE m_nBtnType;
static BYTE m_nReverseInfo;

static BYTE m_nEndCount;
static BYTE m_nRecvScreenInfoStep;


///////////////////////////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////////////////////////
// public:
//
extern u8 SP3_GetCtrlID(void);

extern void SP3_SetSysCommStepProc(void);
extern BYTE SP3_RecvProc(void);
//控制蜂蜜器响一声
extern BYTE SP3_SendBuzzerCmdProc(u8 nEnWait);
//切换画面
extern BYTE SP3_SendSysChangePageCmdProc(UINT16 nPageID, u8 EnWait);
//设置图标状态
extern BYTE SP3_SendSetSysIconStatusCmdProc(UINT16 nPageID, UINT16 nCtrlID, BYTE nBtnStaus, u8 nEnWait);
//设置仪表控件值
extern BYTE SP3_SendSetMeterValueCmdProc(UINT16 nPageID, UINT16 nCtrlID, int32 nMeterValue, u8 nEnWait);
//刷新文本控件的值
extern BYTE SP3_SendSetSysTextValueCmdProc(UINT16 nPageID, UINT16 nCtrlID, BYTE nLen, BYTE *pTextValueInfoAry, u8 nEnWait);
//在屏幕任意位置显示指定的文本
extern BYTE SP3_SendStringInfoCmdProc(UINT16 nXPos, UINT16 nYPos, BYTE nBack, BYTE nFont, char *pText, u8 nEnWait);
//刷新设置系统光标信息
extern BYTE SP3_SendSetSysCourseInfoCmdProc(BYTE nEnable, UINT16 nXPos, UINT16 nYPos, BYTE nLen, BYTE nWidth, u8 nEnWait);
//历史曲线
extern BYTE SP3_SendSetSysHistoryCurveCmdProc(UINT16 nPageID, UINT16 nCtrlID, BYTE nChNum, u16 *pData, u8 nEnWait);
extern BYTE SP3_SendHistoryCurveEnCmsProc(UINT16 nPageID, UINT16 nCtrlID, BYTE nEn, u8 nEnWait);
extern BYTE SP3_SendClearHistoryCurveCmsProc(UINT16 nPageID, UINT16 nCtrlID, BYTE nChNum, u8 nEnWait);
extern BYTE SP3_SendHistoryCurveFullScreenCmdProc(UINT16 nPageID, UINT16 nCtrlID, u8 nEnWait);
extern BYTE SP3_SendHistoryCurveHTimeCmdProc(UINT16 nPageID, UINT16 nCtrlID, u16 nTime, u8 nEnWait);
extern BYTE SP3_SendHistoryCurveVTimeCmdProc(UINT16 nPageID, UINT16 nCtrlID, u32 nMax, u32 nMin, u8 nEnWait);
extern BYTE SP3_SendDataRecordCmdProc(BYTE nPageID, UINT16 nCtrlID, BYTE *pTextValue, u8 nEnWait);
extern BYTE SP3_SendClearDataRecordCmdProc(BYTE nPageID, UINT16 nCtrlID, u8 nEnWait);

extern BYTE SP3_SendCurveClearCmdProc(pCURVE_DEF pCurveInfo, u8 nEnWait);
extern BYTE SP3_SendCurveZoomCmdProc(pCURVE_DEF pCurveInfo, u8 nEnWait);
extern BYTE SP3_SendCurveDispCmdProc(pCURVE_DEF pCurveInfo, u8 nEnWait);


//
// extern void SysLCDTFT_ShowSysTimeValueProc(UINT16 nPageID, UINT16 nCtrlID, UINT16 nSecond, UINT16 nMinute, UINT16 nHour, UINT16 nDay, UINT16 nMonth, UINT16 nYear);

//解析上传的文本 0x2E小数点
extern void SysAsciiCodeToValueProc(UINT16 nCount, BYTE nDesPointPos, volatile UINT32 *pDesValue, volatile BYTE *pDesPointPos, BYTE nPointType);
extern void SysAsciiCodeToValueProc0(UINT16 nCount, volatile s16 *pDesValue);

extern void ChangePageSetAIInitCmd(u8 nPage);
extern BYTE SysTestResultPageDataRecord(void);
// private:
//
static BYTE SP3_SynProc(void);
static BYTE SP3_CmdProc(void);
static BYTE SP3_DataProc(void);

//解析上传的文本 0x2E小数点
extern u8 SysAsciiCodeToTimeValueProc(u16 nCount);
//解析上传的文本
// static void SysAsciiCodeToValueProc(u16 nCount, u8 nDesPointPos);

//首页按键处理
static BYTE SysFirstPageBtnProc(void);
static BYTE SysWorkPageBtnProc(void);
static BYTE SysCalibrationPageBtnProc(void);
//结果页面按键处理
static BYTE SysTestResultPageBtnProc(void);
static BYTE SysConfigParamPageBtnProc(void);
static BYTE SysPwdPageBtnProc(void);
static BYTE SysAboutPageBtnProc(void);

static BYTE SysWorkPageIconProc(void);

//
static BYTE SysConfigPageTextInfoProc(BYTE nCount);
static BYTE SysSaveResultPageTextInfoProc(BYTE nCount);
static BYTE SysFirstPageTextInfoProc(BYTE nCount);
static BYTE SysWorkPageTextInfoProc(BYTE nCount);
static BYTE SysCalibrationPageTextInfoProc(BYTE nCount);

static u8 SysPageBtnProc(void);
static u8 SysPageTextInfoProc(u8 nCount);
static u8 SysPageIconProc(u8 nCount);
static u8 SysPageSelectControlsProc(void);

static void ExecSysBDVoltageProc(void);
static void ExecSysBDCurrentProc(void);
#endif
///////////////////////////////////////////////////////////////////////////
