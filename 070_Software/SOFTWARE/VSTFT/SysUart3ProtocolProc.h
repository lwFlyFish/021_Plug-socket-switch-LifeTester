#ifndef __SysUart3ProtocolProc_H
#define __SysUart3ProtocolProc_H 1
//
///////////////////////////////////////////////////////////////////////////
#include "SysConfig.h"

#define TEMP_MAX 450	// 45.0��
#define HUMI_MAX 700	// 70.0%RH

typedef enum
{
    Front = 0,	  //��ǰ��
    Back  = 1,	  //�Ӻ��
} CURVE_DISP_TYPE_DEF;

typedef struct
{
    s16 nXOffect;	 //ˮƽƫ��
    u16 nXMUL;		 //ˮƽ����
    s16 nYOffect;	 //��ֱƫ��
    u16 nYMUL;		 //��ֱ����
} CURVE_ZOOM_DEF;

//��Ҫ�����λ��
typedef struct
{
    CURVE_ZOOM_DEF stCurveInfo;	   //�Ŵ���С��ƫ��
    s16			   nHmax;		   //��ֱ�������
    s16			   nHmin;		   //��ֱ������С
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
//�豸��ַ
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
//���Ʒ�������һ��
extern BYTE SP3_SendBuzzerCmdProc(u8 nEnWait);
//�л�����
extern BYTE SP3_SendSysChangePageCmdProc(UINT16 nPageID, u8 EnWait);
//����ͼ��״̬
extern BYTE SP3_SendSetSysIconStatusCmdProc(UINT16 nPageID, UINT16 nCtrlID, BYTE nBtnStaus, u8 nEnWait);
//�����Ǳ�ؼ�ֵ
extern BYTE SP3_SendSetMeterValueCmdProc(UINT16 nPageID, UINT16 nCtrlID, int32 nMeterValue, u8 nEnWait);
//ˢ���ı��ؼ���ֵ
extern BYTE SP3_SendSetSysTextValueCmdProc(UINT16 nPageID, UINT16 nCtrlID, BYTE nLen, BYTE *pTextValueInfoAry, u8 nEnWait);
//����Ļ����λ����ʾָ�����ı�
extern BYTE SP3_SendStringInfoCmdProc(UINT16 nXPos, UINT16 nYPos, BYTE nBack, BYTE nFont, char *pText, u8 nEnWait);
//ˢ������ϵͳ�����Ϣ
extern BYTE SP3_SendSetSysCourseInfoCmdProc(BYTE nEnable, UINT16 nXPos, UINT16 nYPos, BYTE nLen, BYTE nWidth, u8 nEnWait);
//��ʷ����
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

//�����ϴ����ı� 0x2EС����
extern void SysAsciiCodeToValueProc(UINT16 nCount, BYTE nDesPointPos, volatile UINT32 *pDesValue, volatile BYTE *pDesPointPos, BYTE nPointType);
extern void SysAsciiCodeToValueProc0(UINT16 nCount, volatile s16 *pDesValue);

extern void ChangePageSetAIInitCmd(u8 nPage);
extern BYTE SysTestResultPageDataRecord(void);
// private:
//
static BYTE SP3_SynProc(void);
static BYTE SP3_CmdProc(void);
static BYTE SP3_DataProc(void);

//�����ϴ����ı� 0x2EС����
extern u8 SysAsciiCodeToTimeValueProc(u16 nCount);
//�����ϴ����ı�
// static void SysAsciiCodeToValueProc(u16 nCount, u8 nDesPointPos);

//��ҳ��������
static BYTE SysFirstPageBtnProc(void);
static BYTE SysWorkPageBtnProc(void);
static BYTE SysCalibrationPageBtnProc(void);
//���ҳ�水������
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
