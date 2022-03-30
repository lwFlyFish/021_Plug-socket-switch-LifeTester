///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

#include "main.h"

#include "PCF8563.H"
#include "RealDatabase.h"
#include "SaveParamProc.h"
#include "SysAtomVarProc.h"
#include "SysConfig.h"
#include "SysDIProc.h"
#include "SysDOInfoProc.h"
#include "SysGpioInit.h"
#include "SysLCD_PageFirst.h"
#include "SysLCD_PageTest.h"
#include "SysLCD_ShowInfoProc.h"
#include "SysTestProc.h"
#include "SysTick.h"
#include "SysUART3.h"
#include "SysUart3CommProc.h"
/*
小容量产品是指闪存存储器容量在16K至32K字节之间的STM32F101xx、
STM32F102xx和STM32F103xx微控制器。
中容量产品是指闪存存储器容量在64K至128K字节之间的STM32F101xx、
STM32F102xx和STM32F103xx微控制器。
大容量产品是指闪存存储器容量在256K至512K字节之间的STM32F101xx和STM32F103xx微控制器。
互联型产品是指STM32F105xx和STM32F107xx微控制器
*/

///////////////////////////////////////////////////////////////////////////
//**函数信息 : int main (void)
//**功能描述 :
//**输入参数 :
//**输出参数 :
///////////////////////////////////////////////////////////////////////////
// static u8 m_nRtcMinuteTemp = 60;
// static u8 m_nRtcSecondTemp = 60;
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    //系统硬件初始化
    SysInitHardwareProc();

    //初始化系统状态
    // InitSysStatusProc();

    //进入主界面
    SysTFTLcd_PageFirstGotoProc();

    //
    while(TRUE)
    {
        // 10ms
        if(Get_SysProcTimerCtrlValueProc() == 0)
        {
            //触屏通信
            SP3_SysCommProc();

            //
            SysGetDIValue();

            //进入试验
            SysTestCtrlProc();

            EE_ReadSysConfigInfo();

            //重装初值 10ms
            Set_SysProcTimerCtrlValueProc(10);
        }

        // 10ms 
        if(Get_LedRefreshValueProc() == 0)
        {
            //重装初值 10ms
            Set_LedRefreshValuePro(10);
        }

        if(Get_SysStaticFreshTimeCtrlValueProc() == 0)
        {
            //
            SysStaticParamRealTimeFreshProc();
            //重装初值 20ms
            Set_SysStaticFreshTimeCtlrValueProc(20);
        }

        if(Get_SysDisplayTimeCtrlValueProc() == 0)
        {
            SysTFTLCD_RealTimeFreshSysInfoProc();

            Set_SysDisplayTimeCtlrValueProc(TFT_REFRESH_DELAY_TIME);
        }

        // 1s
        if(Get_SysLedBlinkValueProc() == 0)
        {
            // 1000ms
            SysLedBlink();
            Set_SysLedBlinkValuePro(150);
        }

        // RTC时间
        if(g_nRtcReadTimerCount == 0)
        {
#if(USE_PCF8563 == 1)
            PCF8563_ReadTime(&g_stRtcTime);
            g_nPageFirstTimeShowFlag = 1;
#endif
            //
            g_nRtcReadTimerCount = 500;
        }

//
#if(DWG_EN == 1)
        //喂狗
        IWDG_ReloadCounter();
#endif
    }
}
