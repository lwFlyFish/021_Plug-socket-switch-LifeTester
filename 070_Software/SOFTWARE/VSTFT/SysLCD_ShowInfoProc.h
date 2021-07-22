#ifndef __SysLCD_ShowInfoProc_H__
#define __SysLCD_ShowInfoProc_H__ 1
#include "SysConfig.h"
#include "pcf8563.h"
typedef enum
{
    FIRST_PAGE = 0,
    WORK_PAGE,
    PARAM_PAGE,
	INFO_PAGE,
} TFT_PAGE_DEF;


///////////////////////////////////////////////////////////////////////////
// Attribute
///////////////////////////////////////////////////////////////////////////
//public:
//ϵͳ�˵���־ - ��������ʱ����λ�á����λ�úͲ����Ƚ����
//extern SYSMENUFLAG g_stSysMenuFlag;
//ϵͳ��ʾҳ������
extern TFT_PAGE_DEF g_nSysShowType;
//private:
//����\ʱ�ӻ���
static BYTE m_arrDateTimeInfo[7];

///////////////////////////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////////////////////////
//public:
//TFTLCDϵͳʵʱˢ��
extern void SysTFTLCD_RealTimeFreshSysInfoProc(void);

extern BYTE SysStaticParamRealTimeFreshProc(void);

//��ʾ��ҳ
extern void SysTFTLcd_ShowSysFirstPageProc(void);
extern void SysTFTLcd_ShowWorkPageProc(void);
extern void SysTFTLcd_ShowSaveResultPageProc(void);
//��ʾ���ڽ���
extern void SysTFTLcd_ShowAboutPageProc(void);
extern void SysTFTLcd_ShowBDPageProc(void);
extern void SysTFTLcd_ShowConfigPageProc(void);
extern void SysTFTLcd_ShowResultPageProc(void);
extern void SysTFTLcd_ShowPasswordPageProc(void);
extern void SysTFTLcd_ShowSysClearResultPageProc(void);
extern void SysTFTLcd_ShowEnterPageProc(void);
extern void SysTFTLcd_EndWarnEnterProc(void);
//

extern void SysLCDTFT_ShowFloatNumberProc(u16 nPageID, u16 nCtrlID, float fShowValue, u8 nMaxPointNum);
extern void SysLCDTFT_ShowNumberProc(u16 nPageID, u16 nCtrlID, BYTE nPointPos, u8 nLen, int32 nShowValue, BYTE nMaxDataLen);
extern void SysLCDTFT_ShowIconInfoProc(u16 nPageID, u16 nCtrlID, u8 nFrame);
//��ʾʱ��
extern void SysShow_SysTimeInfoProc(UINT16 nPageID, UINT16 nCtrlID, RTC_TIME_DEF stTime);
extern void SysShow_SysTimeStyle2(UINT16 nPageID, UINT16 nCtrlID, RTC_TIME_DEF stTime);

extern void Lcd_BuzzerCtrl(void);
//private:

static void SysLCDTFT_ShowConfigPageProc(void);
static void SysLCDTFT_ShowBDPageProc(void);
static void SysLCDTFT_ShowWorkPageInfoProc(void);
static void SysLCDTFT_ShowResultInfoProc(void);
static void SysLCDTFT_ShowSaveResultInfoProc(void);

//��ʾ���ҳ���
static void SysShow_VerProc(void);

#endif
///////////////////////////////////////////////////////////////////////////
//end
///////////////////////////////////////////////////////////////////////////
