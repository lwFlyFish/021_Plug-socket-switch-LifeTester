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
//系统菜单标志 - 参数设置时大项位置、光标位置和参数比较情况
//extern SYSMENUFLAG g_stSysMenuFlag;
//系统显示页面类型
extern TFT_PAGE_DEF g_nSysShowType;
//private:
//日期\时钟缓存
static BYTE m_arrDateTimeInfo[7];

///////////////////////////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////////////////////////
//public:
//TFTLCD系统实时刷新
extern void SysTFTLCD_RealTimeFreshSysInfoProc(void);

extern BYTE SysStaticParamRealTimeFreshProc(void);

//显示首页
extern void SysTFTLcd_ShowSysFirstPageProc(void);
extern void SysTFTLcd_ShowWorkPageProc(void);
extern void SysTFTLcd_ShowSaveResultPageProc(void);
//显示关于界面
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
//显示时间
extern void SysShow_SysTimeInfoProc(UINT16 nPageID, UINT16 nCtrlID, RTC_TIME_DEF stTime);
extern void SysShow_SysTimeStyle2(UINT16 nPageID, UINT16 nCtrlID, RTC_TIME_DEF stTime);

extern void Lcd_BuzzerCtrl(void);
//private:

static void SysLCDTFT_ShowConfigPageProc(void);
static void SysLCDTFT_ShowBDPageProc(void);
static void SysLCDTFT_ShowWorkPageInfoProc(void);
static void SysLCDTFT_ShowResultInfoProc(void);
static void SysLCDTFT_ShowSaveResultInfoProc(void);

//显示结果页编号
static void SysShow_VerProc(void);

#endif
///////////////////////////////////////////////////////////////////////////
//end
///////////////////////////////////////////////////////////////////////////
