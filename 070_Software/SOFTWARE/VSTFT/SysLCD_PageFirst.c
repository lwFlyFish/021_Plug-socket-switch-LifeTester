#include "SysLCD_PageFirst.h"
#include "SysLCD_ShowInfoProc.h"
#include "SysUart3ProtocolProc.h"
#include "SysLCD_PageTest.h"

#include <string.h>

uint8_t g_nPageFirstTimeShowFlag=0;

/// refresh display
void SysLCDTFT_PageFirstShowInfoProc(void)
{
	if(1==g_nPageFirstTimeShowFlag){SysShow_SysTimeInfoProc(TFT_PAGE_TYPE_FIRST, 2, g_stRtcTime);}
}

BYTE SysLCDTFT_PageFirstTextProc(BYTE nCount, uint16_t _id)
{
	BYTE nReturnValue = SUCESS_COMM;
    switch (_id)
    {
    case 2: ///< 时间
        SysAsciiCodeToTimeValueProc(nCount);
		g_nPageFirstTimeShowFlag=1;
      break;

    default:nReturnValue =  FAILURE;
			break;
    }
    return nReturnValue;
}


/// read button value
BYTE SysLCDTFT_PageFirstButtonProc(uint16_t _id)
{
	BYTE nReturnValue = FAILURE;

    switch (_id)
    {
    //显示试验界面
    case TFT_First_Switch_Button:
		g_nTestType = TEST_TYPE_SWITCH;
		SysTFTLcd_PageTestGotoProc();
		nReturnValue = SUCESS_COMM;
		break;
	
	case TFT_First_Socket_Button:
		g_nTestType = TEST_TYPE_SOCKET;
		SysTFTLcd_PageTestGotoProc();
		nReturnValue = SUCESS_COMM;
		break;

	default:nReturnValue = FAILURE;
		break;
    }
    return nReturnValue;
}


/// go to param page
void SysTFTLcd_PageFirstGotoProc(void)
{
    SP3_SendSysChangePageCmdProc(TFT_PAGE_TYPE_FIRST,1);
	// 刷新标志位置1，首页不用
}



