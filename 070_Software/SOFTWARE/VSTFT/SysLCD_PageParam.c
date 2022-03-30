#include "SysLCD_PageParam.h"

#include "SaveParamProc.h"
#include "SysLCD_PageTest.h"
#include "SysLCD_ShowInfoProc.h"
#include "SysUart3CommProc.h"
#include "SysUart3ProtocolProc.h"


TFT_Param_Value_T			 g_stParamValue[TEST_TYPE_NUMBER][TEST_SELECT_NUMBER];
static TFT_Param_Show_Flag_T m_arrParamShowFlag[TEST_SELECT_NUMBER];
static uint8_t				 m_nTitleShowFlag = 0;
static uint8_t				 m_nAllShowFlag	  = 0;

static void SysIconProc(TEST_SELECT_E _select, uint8_t _flag);

// refresh display
void SysLCDTFT_PageParamShowInfoProc(void)
{
	uint16_t nPageID = PARAM_PAGE;
    uint16_t nPage	 = PARAM_PAGE;
	uint16_t nItemID;
	uint16_t nCtrlID;
    uint8_t	 flag = 0;

    for(uint8_t i = 0; i < TFT_Param_Text_Number; i++)
	{
		switch(i)
        {
        case 0:	   //标题
			nCtrlID = TFT_Param_Title;
            if(m_nTitleShowFlag == 1)
			{
                flag			 = 1;
                m_nTitleShowFlag = 0;
                switch(g_nTestType)
				{
                case TEST_TYPE_SWITCH: SP3_SendSetSysTextValueCmdProc(nPageID, nCtrlID, 16, (BYTE *)"开关参数设置窗口", 1); break;

                case TEST_TYPE_SOCKET: SP3_SendSetSysTextValueCmdProc(nPageID, nCtrlID, 20, (BYTE *)"插头插座参数设置窗口", 1); break;

                default: break;
				}
			}
			break;

        case 1:	   //工位一试验次数
            if(1 == m_arrParamShowFlag[TEST_SELECT_1].nTestCount)
			{
                flag	= 1;
				nItemID = TFT_Param_One_TestCount;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, g_stParamValue[g_nTestType][TEST_SELECT_1].nTestCount, 7);
				m_arrParamShowFlag[TEST_SELECT_1].nTestCount = 0;
			}
			break;

        case 2:	   //工位一接通时间
            if(1 == m_arrParamShowFlag[TEST_SELECT_1].nTurnTime)
			{
                flag	= 1;
				nItemID = TFT_Param_One_TurnTime;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 1, 7, g_stParamValue[g_nTestType][TEST_SELECT_1].nTurnTime, 7);
				m_arrParamShowFlag[TEST_SELECT_1].nTurnTime = 0;
			}
			break;

        case 3:	   //工位一断开时间
            if(1 == m_arrParamShowFlag[TEST_SELECT_1].nLeaveTime)
			{
                flag	= 1;
				nItemID = TFT_Param_One_LeaveTime;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 1, 7, g_stParamValue[g_nTestType][TEST_SELECT_1].nLeaveTime, 7);
				m_arrParamShowFlag[TEST_SELECT_1].nLeaveTime = 0;
			}
			break;

        case 4:	   //工位一短路时间
            if(1 == m_arrParamShowFlag[TEST_SELECT_1].nShortTime)
			{
                flag	= 1;
				nItemID = TFT_Param_One_ShortTime;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 1, 7, g_stParamValue[g_nTestType][TEST_SELECT_1].nShortTime, 7);
				m_arrParamShowFlag[TEST_SELECT_1].nShortTime = 0;
			}
			break;

        case 5:	   //工位一断路时间
            if(1 == m_arrParamShowFlag[TEST_SELECT_1].nBrokenTime)
			{
                flag	= 1;
				nItemID = TFT_Param_One_BrokenTime;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 1, 7, g_stParamValue[g_nTestType][TEST_SELECT_1].nBrokenTime, 7);
				m_arrParamShowFlag[TEST_SELECT_1].nBrokenTime = 0;
			}
			break;

        case 6:	   //工位一故障次数
            if(1 == m_arrParamShowFlag[TEST_SELECT_1].nFaultCount)
			{
                flag	= 1;
				nItemID = TFT_Param_One_FaultCount;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, g_stParamValue[g_nTestType][TEST_SELECT_1].nFaultCount, 7);
				m_arrParamShowFlag[TEST_SELECT_1].nFaultCount = 0;
			}
			break;

        case 7:	   //工位一电流检测图标
            if(1 == m_arrParamShowFlag[TEST_SELECT_1].nCurrentDetectionIcon)
			{
                flag	= 1;
				nItemID = TFT_Param_One_CurrentDetectionIcon;
				SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, g_stParamValue[g_nTestType][TEST_SELECT_1].nCurrentDetection, 1);
				m_arrParamShowFlag[TEST_SELECT_1].nCurrentDetectionIcon = 0;
			}
			break;

        case 8:	   //工位二试验次数
            if(1 == m_arrParamShowFlag[TEST_SELECT_2].nTestCount)
			{
                flag	= 1;
				nItemID = TFT_Param_Two_TestCount;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, g_stParamValue[g_nTestType][TEST_SELECT_2].nTestCount, 7);
				m_arrParamShowFlag[TEST_SELECT_2].nTestCount = 0;
			}
			break;

        case 9:	   //工位二接通时间
            if(1 == m_arrParamShowFlag[TEST_SELECT_2].nTurnTime)
			{
                flag	= 1;
				nItemID = TFT_Param_Two_TurnTime;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 1, 7, g_stParamValue[g_nTestType][TEST_SELECT_2].nTurnTime, 7);
				m_arrParamShowFlag[TEST_SELECT_2].nTurnTime = 0;
			}
			break;

        case 10:	//工位二断开时间
            if(1 == m_arrParamShowFlag[TEST_SELECT_2].nLeaveTime)
			{
                flag	= 1;
				nItemID = TFT_Param_Two_LeaveTime;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 1, 7, g_stParamValue[g_nTestType][TEST_SELECT_2].nLeaveTime, 7);
				m_arrParamShowFlag[TEST_SELECT_2].nLeaveTime = 0;
			}
			break;

        case 11:	//工位二短路时间
            if(1 == m_arrParamShowFlag[TEST_SELECT_2].nShortTime)
			{
                flag	= 1;
				nItemID = TFT_Param_Two_ShortTime;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 1, 7, g_stParamValue[g_nTestType][TEST_SELECT_2].nShortTime, 7);
				m_arrParamShowFlag[TEST_SELECT_2].nShortTime = 0;
			}
			break;

        case 12:	//工位二断路时间
            if(1 == m_arrParamShowFlag[TEST_SELECT_2].nBrokenTime)
			{
                flag	= 1;
				nItemID = TFT_Param_Two_BrokenTime;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 1, 7, g_stParamValue[g_nTestType][TEST_SELECT_2].nBrokenTime, 7);
				m_arrParamShowFlag[TEST_SELECT_2].nBrokenTime = 0;
			}
			break;

        case 13:	//工位二故障次数
            if(1 == m_arrParamShowFlag[TEST_SELECT_2].nFaultCount)
			{
                flag	= 1;
				nItemID = TFT_Param_Two_FaultCount;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, g_stParamValue[g_nTestType][TEST_SELECT_2].nFaultCount, 7);
				m_arrParamShowFlag[TEST_SELECT_2].nFaultCount = 0;
			}
			break;

        case 14:	//工位二电流检测图标
            if(1 == m_arrParamShowFlag[TEST_SELECT_2].nCurrentDetectionIcon)
			{
                flag	= 1;
				nItemID = TFT_Param_Two_CurrentDetectionIcon;
				SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, g_stParamValue[g_nTestType][TEST_SELECT_2].nCurrentDetection, 1);
				m_arrParamShowFlag[TEST_SELECT_2].nCurrentDetectionIcon = 0;
			}
			break;

        case 15:	//工位三试验次数
            if(1 == m_arrParamShowFlag[TEST_SELECT_3].nTestCount)
			{
                flag	= 1;
				nItemID = TFT_Param_Thr_TestCount;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, g_stParamValue[g_nTestType][TEST_SELECT_3].nTestCount, 7);
				m_arrParamShowFlag[TEST_SELECT_3].nTestCount = 0;
			}
			break;

        case 16:	//工位三接通时间
            if(1 == m_arrParamShowFlag[TEST_SELECT_3].nTurnTime)
			{
                flag	= 1;
				nItemID = TFT_Param_Thr_TurnTime;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 1, 7, g_stParamValue[g_nTestType][TEST_SELECT_3].nTurnTime, 7);
				m_arrParamShowFlag[TEST_SELECT_3].nTurnTime = 0;
			}
			break;

        case 17:	//工位三断开时间
            if(1 == m_arrParamShowFlag[TEST_SELECT_3].nLeaveTime)
			{
                flag	= 1;
				nItemID = TFT_Param_Thr_LeaveTime;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 1, 7, g_stParamValue[g_nTestType][TEST_SELECT_3].nLeaveTime, 7);
				m_arrParamShowFlag[TEST_SELECT_3].nLeaveTime = 0;
			}
			break;

        case 18:	//工位三短路时间
            if(1 == m_arrParamShowFlag[TEST_SELECT_3].nShortTime)
			{
                flag	= 1;
				nItemID = TFT_Param_Thr_ShortTime;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 1, 7, g_stParamValue[g_nTestType][TEST_SELECT_3].nShortTime, 7);
				m_arrParamShowFlag[TEST_SELECT_3].nShortTime = 0;
			}
			break;

        case 19:	//工位三短路时间
            if(1 == m_arrParamShowFlag[TEST_SELECT_3].nBrokenTime)
			{
                flag	= 1;
				nItemID = TFT_Param_Thr_BrokenTime;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 1, 7, g_stParamValue[g_nTestType][TEST_SELECT_3].nBrokenTime, 7);
				m_arrParamShowFlag[TEST_SELECT_3].nBrokenTime = 0;
			}
			break;

        case 20:	//工位三故障次数
            if(1 == m_arrParamShowFlag[TEST_SELECT_3].nFaultCount)
			{
                flag	= 1;
				nItemID = TFT_Param_Thr_FaultCount;
				SysLCDTFT_ShowNumberProc(nPage, nItemID, 0, 7, g_stParamValue[g_nTestType][TEST_SELECT_3].nFaultCount, 7);
				m_arrParamShowFlag[TEST_SELECT_3].nFaultCount = 0;
			}
			break;

        case 21:	//工位三电流检测图标
            if(1 == m_arrParamShowFlag[TEST_SELECT_3].nCurrentDetectionIcon)
			{
                flag	= 1;
				nItemID = TFT_Param_Thr_CurrentDetectionIcon;
				SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, g_stParamValue[g_nTestType][TEST_SELECT_3].nCurrentDetection, 1);
				m_arrParamShowFlag[TEST_SELECT_3].nCurrentDetectionIcon = 0;
			}

            if(1 == m_nAllShowFlag) { m_nAllShowFlag = 0; }
			break;

        default: break;
		}

        if((1 == flag) && (0 == m_nAllShowFlag)) { break; }
	}
}

// read text value
BYTE SysLCDTFT_PageParamTextProc(BYTE nCount, uint16_t _id)
{
    BYTE		  nReturnValue = SUCESS_COMM;
    uint8_t		  nTmp		   = 0;
    uint32_t	  nValue	   = 0;
    TEST_SELECT_E select	   = TEST_SELECT_1;

    TFT_Param_Value_T *	   st	= NULL;
    TFT_Param_Show_Flag_T *flag = NULL;

    switch(_id)
    {
    case TFT_Param_One_TestCount:
    case TFT_Param_One_TurnTime:
    case TFT_Param_One_LeaveTime:
    case TFT_Param_One_ShortTime:
    case TFT_Param_One_BrokenTime:
    case TFT_Param_One_FaultCount:
        select = TEST_SELECT_1;
        st	   = &g_stParamValue[g_nTestType][TEST_SELECT_1];
        flag   = &m_arrParamShowFlag[TEST_SELECT_1];
        break;

    case TFT_Param_Two_TestCount:
    case TFT_Param_Two_TurnTime:
    case TFT_Param_Two_LeaveTime:
    case TFT_Param_Two_ShortTime:
    case TFT_Param_Two_BrokenTime:
    case TFT_Param_Two_FaultCount:
        select = TEST_SELECT_2;
        st	   = &g_stParamValue[g_nTestType][TEST_SELECT_2];
        flag   = &m_arrParamShowFlag[TEST_SELECT_2];
        break;

    case TFT_Param_Thr_TestCount:
    case TFT_Param_Thr_TurnTime:
    case TFT_Param_Thr_LeaveTime:
    case TFT_Param_Thr_ShortTime:
    case TFT_Param_Thr_BrokenTime:
    case TFT_Param_Thr_FaultCount:
        select = TEST_SELECT_3;
        st	   = &g_stParamValue[g_nTestType][TEST_SELECT_3];
        flag   = &m_arrParamShowFlag[TEST_SELECT_3];
        break;
    }

    switch(_id)
    {
    case TFT_Param_One_TestCount:	 ///<试验次数
    case TFT_Param_Two_TestCount:
    case TFT_Param_Thr_TestCount:
        flag->nTestCount = 1;
        SysAsciiCodeToValueProc(nCount, 0, (u32 *)&nValue, (u8 *)&nTmp, 0x00);

        if(nValue != st->nTestCount)
        {
            st->nTestCount = nValue;
            SysParamSaveProc(g_nTestType, select, TFT_PARAM_SAVE_INDEX_TEST_COUNT);
        }
        break;

    case TFT_Param_One_TurnTime:	///< 接通时间
    case TFT_Param_Two_TurnTime:
    case TFT_Param_Thr_TurnTime:
        flag->nTurnTime = 1;
        SysAsciiCodeToValueProc(nCount, 1, (u32 *)&nValue, (u8 *)&nTmp, 0x00);

        if(nValue != st->nTurnTime)
        {
            st->nTurnTime = nValue;
            SysParamSaveProc(g_nTestType, select, TFT_PARAM_SAVE_INDEX_TURN_TIME);
        }
        break;

    case TFT_Param_One_LeaveTime:	 ///< 断开时间
    case TFT_Param_Two_LeaveTime:
    case TFT_Param_Thr_LeaveTime:
        flag->nLeaveTime = 1;
        SysAsciiCodeToValueProc(nCount, 1, (u32 *)&nValue, (u8 *)&nTmp, 0x00);

        if(nValue != st->nLeaveTime)
        {
            st->nLeaveTime = nValue;
            SysParamSaveProc(g_nTestType, select, TFT_PARAM_SAVE_INDEX_LEAVE_TIME);
        }
        break;

    case TFT_Param_One_ShortTime:	 ///< 短路时间
    case TFT_Param_Two_ShortTime:
    case TFT_Param_Thr_ShortTime:
        flag->nShortTime = 1;
        SysAsciiCodeToValueProc(nCount, 1, (u32 *)&nValue, (u8 *)&nTmp, 0x00);

        if(nValue != st->nShortTime)
        {
            st->nShortTime = nValue;
            SysParamSaveProc(g_nTestType, select, TFT_PARAM_SAVE_INDEX_SHORT_TIME);
        }
        break;

    case TFT_Param_One_BrokenTime:	  ///< 断路时间
    case TFT_Param_Two_BrokenTime:
    case TFT_Param_Thr_BrokenTime:
        flag->nBrokenTime = 1;
        SysAsciiCodeToValueProc(nCount, 1, (u32 *)&nValue, (u8 *)&nTmp, 0x00);

        if(nValue != st->nBrokenTime)
        {
            st->nBrokenTime = nValue;
            SysParamSaveProc(g_nTestType, select, TFT_PARAM_SAVE_INDEX_BROKEN_TIME);
        }
        break;

    case TFT_Param_One_FaultCount:	  ///< 故障次数
    case TFT_Param_Two_FaultCount:
    case TFT_Param_Thr_FaultCount:
        flag->nFaultCount = 1;
        SysAsciiCodeToValueProc(nCount, 0, (u32 *)&nValue, (u8 *)&nTmp, 0x00);

        if(nValue != st->nFaultCount)
        {
            st->nFaultCount = nValue;
            SysParamSaveProc(g_nTestType, select, TFT_PARAM_SAVE_INDEX_FAULT_COUNT);
        }
        break;

    default: nReturnValue = FAILURE; break;
    }

	return nReturnValue;
}

// read button value
BYTE SysLCDTFT_PageParamButtonProc(uint16_t nId)
{
	BYTE nReturnValue = FAILURE;

    switch(nId)
	{
	case TFT_Param_Return_Button:
        SysTFTLcd_PageTestGotoProc();
        nReturnValue = SUCESS_COMM;
		break;

    default: break;
	}

	return nReturnValue;
}

BYTE SysLCDTFT_PageParamIconProc(BYTE nCount, uint16_t _id)
{
    BYTE nReturnValue = SUCESS_COMM;

	switch(_id)
	{
    case TFT_Param_One_CurrentDetectionIcon: SysIconProc(TEST_SELECT_1, nCount); break;

    case TFT_Param_Two_CurrentDetectionIcon: SysIconProc(TEST_SELECT_2, nCount); break;

    case TFT_Param_Thr_CurrentDetectionIcon: SysIconProc(TEST_SELECT_3, nCount); break;

    default: nReturnValue = FAILURE; break;
	}

	return nReturnValue;
}


/// go to param page
void SysTFTLcd_PageParamGotoProc(void)
{
	m_nTitleShowFlag = 1;
    m_nAllShowFlag	 = 1;

    for(uint8_t i = 0; i < TEST_SELECT_NUMBER; i++)
	{
        m_arrParamShowFlag[i].nTestCount			= 1;
        m_arrParamShowFlag[i].nTurnTime				= 1;
        m_arrParamShowFlag[i].nLeaveTime			= 1;
        m_arrParamShowFlag[i].nShortTime			= 1;
        m_arrParamShowFlag[i].nBrokenTime			= 1;
        m_arrParamShowFlag[i].nFaultCount			= 1;
		m_arrParamShowFlag[i].nCurrentDetectionIcon = 1;
	}

    SP3_SendSysChangePageCmdProc(TFT_PAGE_TYPE_PARAM, 1);
}

// 图标动作时，刷新数据。flag是图标返回的数值
static void SysIconProc(TEST_SELECT_E _select, uint8_t _flag)
{
    m_arrParamShowFlag[_select].nCurrentDetectionIcon = 1;

    switch(_flag)
    {
    case TFT_STATUS_ICON_DISABLE: g_stParamValue[g_nTestType][_select].nCurrentDetection = TFT_STATUS_ICON_ENABLE; break;

    case TFT_STATUS_ICON_ENABLE: g_stParamValue[g_nTestType][_select].nCurrentDetection = TFT_STATUS_ICON_DISABLE; break;

    default: break;
    }

    SysParamSaveProc(g_nTestType, _select, TFT_PARAM_SAVE_INDEX_CURRENT_DETECT);
}
