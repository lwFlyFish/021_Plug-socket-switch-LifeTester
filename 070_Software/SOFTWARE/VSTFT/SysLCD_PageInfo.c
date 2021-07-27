#include "SysLCD_PageInfo.h"

#include "SysLCD_PageParam.h"
#include "SysLCD_PageTest.h"
#include "SysLCD_ShowInfoProc.h"
#include "SysTestProc.h"
#include "SysUart3CommProc.h"
#include "SysUart3ProtocolProc.h"

TFT_INFO_VALUE_T	 g_stPageInfoValue[TEST_SELECT_NUMBER];
TFT_INFO_SHOW_FLAG_T g_stPageInfoShowFlag[TEST_SELECT_NUMBER];

static uint8_t m_nTitleShowFlag = 0;
static uint8_t m_nAllShowFlag	= 1;

static void SysIconProc(TEST_SELECT_E _select, uint8_t _flag);

/// refresh display
void SysLCDTFT_PageInfoShowInfoProc(void)
{
    uint16_t nPage	 = INFO_PAGE;
	uint16_t nPageID = INFO_PAGE;
	uint16_t nItemID;
	uint16_t nCtrlID;
    uint8_t	 flag = 0;

    for(uint8_t i = 0; i < TFT_Info_Text_Number; i++)
	{
		switch(i)
        {
        case 0:
            nCtrlID = TFT_Info_Title;
            if(m_nTitleShowFlag == 1)
            {
                flag			 = 1;
                m_nTitleShowFlag = 0;
                switch(g_nTestType)
				{
                case TEST_TYPE_SWITCH: SP3_SendSetSysTextValueCmdProc(nPageID, nCtrlID, 12, (BYTE *)"开关监控窗口", 1); break;

                case TEST_TYPE_SOCKET: SP3_SendSetSysTextValueCmdProc(nPageID, nCtrlID, 16, (BYTE *)"插头插座监控窗口", 1); break;

                default: break;
				}
            }
			break;

        case 1:	   //工位一运行状态
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_1].nDevStatus)
            {
                flag	= 1;
                nItemID = TFT_Info_One_RunState;
                if((TEST_STATUS_START == g_stTestValue.nStatus) && (TFT_STATUS_ICON_ENABLE == g_stTftTestValue.arrAutomatic[TEST_SELECT_1]) && (0 == g_stTestInfo[TEST_SELECT_1].nEndFlag))
                { SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, TFT_STATUS_ICON_ENABLE, 1); }
                else
                {
                    SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, TFT_STATUS_ICON_DISABLE, 1);
                }

                g_stPageInfoShowFlag[TEST_SELECT_1].nDevStatus = 0;
            }
            break;

        case 2:	   //工位二运行状态
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_2].nDevStatus)
            {
                flag	= 1;
                nItemID = TFT_Info_Two_RunState;
                if((TEST_STATUS_START == g_stTestValue.nStatus) && (TFT_STATUS_ICON_ENABLE == g_stTftTestValue.arrAutomatic[TEST_SELECT_2]) && (0 == g_stTestInfo[TEST_SELECT_2].nEndFlag))
                { SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, TFT_STATUS_ICON_ENABLE, 1); }
                else
                {
                    SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, TFT_STATUS_ICON_DISABLE, 1);
                }

                g_stPageInfoShowFlag[TEST_SELECT_2].nDevStatus = 0;
            }
            break;

        case 3:	   //工位三运行状态
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_3].nDevStatus)
            {
                flag	= 1;
                nItemID = TFT_Info_Thr_RunState;
                if((TEST_STATUS_START == g_stTestValue.nStatus) && (TFT_STATUS_ICON_ENABLE == g_stTftTestValue.arrAutomatic[TEST_SELECT_3]) && (0 == g_stTestInfo[TEST_SELECT_3].nEndFlag))
                { SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, TFT_STATUS_ICON_ENABLE, 1); }
                else
                {
                    SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, TFT_STATUS_ICON_DISABLE, 1);
                }

                g_stPageInfoShowFlag[TEST_SELECT_3].nDevStatus = 0;
            }
            break;

        case 4:	   //工位一电流检测
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_1].nCurrentCheck)
            {
                flag	= 1;
                nItemID = TFT_Info_One_ElecDetection;
                SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, g_stParamValue[g_nTestType][TEST_SELECT_1].nCurrentDetection, 1);
                g_stPageInfoShowFlag[TEST_SELECT_1].nCurrentCheck = 0;
            }
            break;

        case 5:	   //工位二电流检测
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_2].nCurrentCheck)
            {
                flag	= 1;
                nItemID = TFT_Info_Two_ElecDetection;
                SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, g_stParamValue[g_nTestType][TEST_SELECT_2].nCurrentDetection, 1);
                g_stPageInfoShowFlag[TEST_SELECT_2].nCurrentCheck = 0;
            }
            break;

        case 6:	   //工位三电流检测
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_3].nCurrentCheck)
            {
                flag	= 1;
                nItemID = TFT_Info_Thr_ElecDetection;
                SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, g_stParamValue[g_nTestType][TEST_SELECT_3].nCurrentDetection, 1);
                g_stPageInfoShowFlag[TEST_SELECT_3].nCurrentCheck = 0;
            }
            break;

        case 7:	   //工位一设定次数
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_1].nSetCount)
            {
                flag	= 1;
                nItemID = TFT_Info_One_SetCount;
                SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, g_stParamValue[g_nTestType][TEST_SELECT_1].nTestCount, 7);
                g_stPageInfoShowFlag[TEST_SELECT_1].nSetCount = 0;
			}
            break;

        case 8:	   //工位二设定次数
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_2].nSetCount)
            {
                flag	= 1;
                nItemID = TFT_Info_Two_SetCount;
                SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, g_stParamValue[g_nTestType][TEST_SELECT_2].nTestCount, 7);
                g_stPageInfoShowFlag[TEST_SELECT_2].nSetCount = 0;
            }
            break;

        case 9:	   //工位三设定次数
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_3].nSetCount)
            {
                flag	= 1;
                nItemID = TFT_Info_Thr_SetCount;
                SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, g_stParamValue[g_nTestType][TEST_SELECT_3].nTestCount, 7);
                g_stPageInfoShowFlag[TEST_SELECT_3].nSetCount = 0;
            }
            break;

        case 10:	//工位一当前次数
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_1].nRealCount)
			{
                flag	= 1;
                nItemID = TFT_Info_One_Countting;
                SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, g_stTestInfo[TEST_SELECT_1].nTestCount, 7);
                g_stPageInfoShowFlag[TEST_SELECT_1].nRealCount = 0;
			}
            break;

        case 11:	//工位二当前次数
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_2].nRealCount)
            {
                flag	= 1;
                nItemID = TFT_Info_Two_Countting;
                SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, g_stTestInfo[TEST_SELECT_2].nTestCount, 7);
                g_stPageInfoShowFlag[TEST_SELECT_2].nRealCount = 0;
            }
            break;

        case 12:	//工位三当前次数
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_3].nRealCount)
            {
                flag	= 1;
                nItemID = TFT_Info_Thr_Countting;
                SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, g_stTestInfo[TEST_SELECT_3].nTestCount, 7);
                g_stPageInfoShowFlag[TEST_SELECT_3].nRealCount = 0;
            }
            break;

        case 13:	//工位一电流次数
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_1].nRealCurrentCount)
            {
                uint32_t count = 0;
                flag		   = 1;
                nItemID		   = TFT_Info_One_ElecCount;
                if(TFT_STATUS_ICON_ENABLE == g_stParamValue[g_nTestType][TEST_SELECT_1].nCurrentDetection) { count = g_stTestInfo[TEST_SELECT_1].nTestCount - g_stTestInfo[TEST_SELECT_1].nFaultCount; }
                SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, count, 7);
                g_stPageInfoShowFlag[TEST_SELECT_1].nRealCurrentCount = 0;
            }
            break;

        case 14:	//工位二电流次数
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_2].nRealCurrentCount)
            {
                uint32_t count = 0;
                flag		   = 1;
                nItemID		   = TFT_Info_Two_ElecCount;
                if(TFT_STATUS_ICON_ENABLE == g_stParamValue[g_nTestType][TEST_SELECT_2].nCurrentDetection) { count = g_stTestInfo[TEST_SELECT_2].nTestCount - g_stTestInfo[TEST_SELECT_2].nFaultCount; }
                SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, count, 7);
                g_stPageInfoShowFlag[TEST_SELECT_2].nRealCurrentCount = 0;
            }
            break;

        case 15:	//工位三电流次数
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_3].nRealCurrentCount)
            {
                uint32_t count = 0;
                flag		   = 1;
                nItemID		   = TFT_Info_Thr_ElecCount;
                if(TFT_STATUS_ICON_ENABLE == g_stParamValue[g_nTestType][TEST_SELECT_3].nCurrentDetection) { count = g_stTestInfo[TEST_SELECT_3].nTestCount - g_stTestInfo[TEST_SELECT_3].nFaultCount; }
                SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, count, 7);
                g_stPageInfoShowFlag[TEST_SELECT_3].nRealCurrentCount = 0;
            }
            break;

        case 16:	//工位一故障次数
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_1].nRealErrorCount)
            {
                flag	= 1;
                nItemID = TFT_Info_One_FaultCount;
                SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, g_stTestInfo[TEST_SELECT_1].nFaultCount, 7);
                g_stPageInfoShowFlag[TEST_SELECT_1].nRealErrorCount = 0;
            }
            break;

        case 17:	//工位二故障次数
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_2].nRealErrorCount)
            {
                flag	= 1;
                nItemID = TFT_Info_Two_FaultCount;
                SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, g_stTestInfo[TEST_SELECT_2].nFaultCount, 7);
                g_stPageInfoShowFlag[TEST_SELECT_2].nRealErrorCount = 0;
            }
            break;

        case 18:	//工位三故障次数
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_3].nRealErrorCount)
            {
                flag	= 1;
                nItemID = TFT_Info_Thr_FaultCount;
                SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, g_stTestInfo[TEST_SELECT_3].nFaultCount, 7);
                g_stPageInfoShowFlag[TEST_SELECT_3].nRealErrorCount = 0;
            }
            break;

        case 19:	//工位一故障清除
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_1].nFlagErrorClear)
            {
                flag	= 1;
                nItemID = TFT_Info_One_FaultCle;
                SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, g_stPageInfoValue[TEST_SELECT_1].nFlagErrorClear, 1);
                g_stPageInfoShowFlag[TEST_SELECT_1].nFlagErrorClear = 0;
			}
            break;

        case 20:	//工位二故障清除
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_2].nFlagErrorClear)
			{
                flag	= 1;
                nItemID = TFT_Info_Two_FaultCle;
                SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, g_stPageInfoValue[TEST_SELECT_2].nFlagErrorClear, 1);
                g_stPageInfoShowFlag[TEST_SELECT_2].nFlagErrorClear = 0;
            }
            break;

        case 21:	//工位三故障清除
            if(1 == g_stPageInfoShowFlag[TEST_SELECT_3].nFlagErrorClear)
            {
                flag	= 1;
                nItemID = TFT_Info_Thr_FaultCle;
                SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, g_stPageInfoValue[TEST_SELECT_3].nFlagErrorClear, 1);
                g_stPageInfoShowFlag[TEST_SELECT_3].nFlagErrorClear = 0;
			}

            if(1 == m_nAllShowFlag) { m_nAllShowFlag = 0; }
            break;

        default: break;
        }

        if((1 == flag) && (0 == m_nAllShowFlag)) { break; }
    }
}

BYTE SysLCDTFT_PageInfoIconProc(BYTE nCount, uint16_t _id)
{
    BYTE nReturnValue = SUCESS_COMM;

    switch(_id)
    {
    case 19:	//工位一故障清除图标
        SysIconProc(TEST_SELECT_1, nCount);
        break;

    case 20:	//工位二故障清除图标
        SysIconProc(TEST_SELECT_2, nCount);
        break;

    case 21:	//工位三故障清除图标
        SysIconProc(TEST_SELECT_3, nCount);
        break;

    default: break;
	}
	return nReturnValue;
}

/// read button value
BYTE SysLCDTFT_PageInfoButtonProc(uint16_t _id)
{
	BYTE nReturnValue = FAILURE;

    switch(_id)
	{
    case TFT_Info_Return_Button:	//返回按钮
        SysTFTLcd_PageTestGotoProc();
        nReturnValue = SUCESS_COMM;
        break;

    default: break;
	}
	return nReturnValue;
}


/// go to param page
void SysTFTLcd_PageInfoGotoProc(void)
{
	m_nTitleShowFlag = 1;
    m_nAllShowFlag	 = 1;

    for(uint8_t i = 0; i < TEST_SELECT_NUMBER; i++)
	{
        g_stPageInfoShowFlag[i].nSetCount		  = 1;
        g_stPageInfoShowFlag[i].nRealCount		  = 1;
        g_stPageInfoShowFlag[i].nRealCurrentCount = 1;
        g_stPageInfoShowFlag[i].nRealErrorCount	  = 1;
        g_stPageInfoShowFlag[i].nDevStatus		  = 1;
        g_stPageInfoShowFlag[i].nCurrentCheck	  = 1;
        g_stPageInfoShowFlag[i].nFlagErrorClear	  = 1;
	}
    SP3_SendSysChangePageCmdProc(TFT_PAGE_TYPE_INFO, 1);
}

// 图标动作时，刷新数据。flag是图标返回的数值
static void SysIconProc(TEST_SELECT_E _select, uint8_t _flag)
{
    g_stPageInfoShowFlag[_select].nFlagErrorClear = 1;

    if(TEST_STATUS_STOP == g_stTestValue.nStatus) { return; }

    switch(_flag)
    {
    case TFT_STATUS_ICON_DISABLE: g_stPageInfoValue[_select].nFlagErrorClear = TFT_STATUS_ICON_ENABLE; break;

    case TFT_STATUS_ICON_ENABLE: g_stPageInfoValue[_select].nFlagErrorClear = TFT_STATUS_ICON_DISABLE; break;

    default: break;
    }
}
