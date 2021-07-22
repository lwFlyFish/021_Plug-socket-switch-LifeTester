#include "SysLCD_PageTest.h"

#include "SysDOInfoProc.h"
#include "SysLCD_PageFirst.h"
#include "SysLCD_PageInfo.h"
#include "SysLCD_PageParam.h"
#include "SysLCD_ShowInfoProc.h"
#include "SysTestProc.h"
#include "SysUart3CommProc.h"
#include "SysUart3ProtocolProc.h"

#include <string.h>

TFT_Test_Show_Flag_T g_stTestShowFlag;
TFT_Test_Value_T	 g_stTftTestValue;
static uint8_t		 m_nTitleShowFlag = 0;
uint8_t				 g_nFlagMeanWhile = 0;
uint8_t				 g_nFlagAgain	  = 0;

/// refresh display
void SysLCDTFT_PageTestShowInfoProc(void)
{
    uint16_t nPage	 = WORK_PAGE;
	uint16_t nPageID = WORK_PAGE;
	uint16_t nItemID;
	uint16_t nCtrlID;
    uint8_t	 flag = 0;

    for(uint8_t i = 0; i < TFT_Test_Control_Show_Number; i++)
	{
		switch(i)
        {
        case 0:	   //����
			nCtrlID = TFT_Test_Text_Title;
            if(m_nTitleShowFlag == 1)
			{
                flag			 = 1;
                m_nTitleShowFlag = 0;
                switch(g_nTestType)
				{
                case TEST_TYPE_SWITCH: SP3_SendSetSysTextValueCmdProc(nPageID, nCtrlID, 12, (BYTE *)"�������鴰��", 1); break;

                case TEST_TYPE_SOCKET: SP3_SendSetSysTextValueCmdProc(nPageID, nCtrlID, 16, (BYTE *)"��ͷ�������鴰��", 1); break;

                default: break;
				}
			}
			break;
        case 1:	   //ͬʱ����/��������ˢ��
            if(g_stTestShowFlag.nWorkMode == 1)
			{
                flag	= 1;
				nItemID = TFT_Test_Text_WorkMethod;
                switch(g_stTestValue.nWorkMode)
				{
                case TEST_MODE_MEAN_WHILE:
                    SP3_SendSetSysTextValueCmdProc(nPageID, nItemID, 8, (BYTE *)"ͬʱ����", 1);
                    g_stTestShowFlag.nWorkMode = 0;
                    break;

                case TEST_MODE_SEQUENCE:
                    SP3_SendSetSysTextValueCmdProc(nPageID, nItemID, 8, (BYTE *)"��������", 1);
                    g_stTestShowFlag.nWorkMode = 0;
                    break;

                default: break;
				}
			}
			break;

        case 2:	   //����״̬ˢ��
            if(g_stTestShowFlag.nWorkState == 1)
			{
                flag	= 1;
				nItemID = TFT_Test_Text_WorkState;
                switch(g_stTestValue.nStatus)
				{
                case TEST_STATUS_START:
                    SP3_SendSetSysTextValueCmdProc(nPage, nItemID, 4, (BYTE *)"����", 1);
                    g_stTestShowFlag.nWorkState = 0;
                    break;

                case TEST_STATUS_STOP:
                    SP3_SendSetSysTextValueCmdProc(nPage, nItemID, 4, (BYTE *)"ֹͣ", 1);
                    g_stTestShowFlag.nWorkState = 0;
                    break;
                }
			}
			break;

        case 3:	   //����ͼ��ˢ��
            if(g_stTestShowFlag.nStart == 1)
			{
                flag	= 1;
				nItemID = TFT_Test_Icon_Start;
				SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, g_stTestValue.nStatus, 1);
                g_stTestShowFlag.nStart = 0;
			}
			break;

        case 4:	   //�Զ����鹤λһˢ��
            if(g_stTestShowFlag.arrAutomatic[TEST_SELECT_1] == 1)
			{
                flag	= 1;
				nItemID = TFT_Test_Icon_Automatic_One;
				SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, g_stTftTestValue.arrAutomatic[0], 1);
                g_stTestShowFlag.arrAutomatic[TEST_SELECT_1] = 0;
			}
			break;

        case 5:	   //�Զ����鹤λ��ˢ��
            if(g_stTestShowFlag.arrAutomatic[TEST_SELECT_2] == 1)
			{
                flag	= 1;
				nItemID = TFT_Test_Icon_Automatic_Two;
				SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, g_stTftTestValue.arrAutomatic[1], 1);
                g_stTestShowFlag.arrAutomatic[TEST_SELECT_2] = 0;
			}
			break;

        case 6:	   //�Զ����鹤λ��ˢ��
            if(g_stTestShowFlag.arrAutomatic[TEST_SELECT_3] == 1)
			{
                flag	= 1;
				nItemID = TFT_Test_Icon_Automatic_Thr;
				SP3_SendSetSysIconStatusCmdProc(nPage, nItemID, g_stTftTestValue.arrAutomatic[2], 1);
                g_stTestShowFlag.arrAutomatic[TEST_SELECT_3] = 0;
			}
			break;

        case 7:	   //�ֶ����鹤λһˢ��
            if(g_stTestShowFlag.arrManual[TEST_SELECT_1] == 1)
			{
                flag	= 1;
				nItemID = TFT_Test_Icon_Manual_One;

                g_stTestShowFlag.arrManual[TEST_SELECT_1] = 0;
                SP3_SendSetSysIconStatusCmdProc(TFT_PAGE_TYPE_TEST, nItemID, g_stTftTestValue.arrManual[TEST_SELECT_1], 1);
			}
			break;

        case 8:	   //�ֶ����鹤λ��ˢ��
            if(g_stTestShowFlag.arrManual[TEST_SELECT_2] == 1)
			{
                flag	= 1;
				nItemID = TFT_Test_Icon_Manual_Two;

                g_stTestShowFlag.arrManual[TEST_SELECT_2] = 0;
                SP3_SendSetSysIconStatusCmdProc(TFT_PAGE_TYPE_TEST, nItemID, g_stTftTestValue.arrManual[TEST_SELECT_2], 1);
			}
			break;

        case 9:	   //�ֶ����鹤λ��ˢ��
            if(g_stTestShowFlag.arrManual[TEST_SELECT_3] == 1)
			{
                flag	= 1;
				nItemID = TFT_Test_Icon_Manual_Thr;

                g_stTestShowFlag.arrManual[TEST_SELECT_3] = 0;
                SP3_SendSetSysIconStatusCmdProc(TFT_PAGE_TYPE_TEST, nItemID, g_stTftTestValue.arrManual[TEST_SELECT_3], 1);
			}
			break;

        default: break;
		}

        if(1 == flag) { break; }
	}
}

/// read text value
BYTE SysLCDTFT_PageTestTextProc(BYTE nCount, uint16_t _id) { return SUCESS_COMM; }

/// read button value
BYTE SysLCDTFT_PageTestButtonProc(uint16_t _id)
{
	BYTE nReturnValue = FAILURE;

    switch(_id)
	{
	//����������ô���
    case 10:
        if(TEST_STATUS_STOP == g_stTestValue.nStatus) { SysTFTLcd_PageParamGotoProc(); }
		nReturnValue = SUCESS_COMM;
		break;

	//����������
    case 11:
        if(TEST_STATUS_STOP == g_stTestValue.nStatus) { SysTFTLcd_PageFirstGotoProc(); }
		nReturnValue = SUCESS_COMM;
		break;

	//�����ش���
    case 12:
        SysTFTLcd_PageInfoGotoProc();
		nReturnValue = SUCESS_COMM;
		break;

	//ѡ��ͬʱ/��������ģʽ��ť
    case 13:
        if(TEST_STATUS_STOP == g_stTestValue.nStatus)
        {
            g_stTestShowFlag.nWorkMode = 1;
            g_stTestValue.nWorkMode++;
            g_stTestValue.nWorkMode %= TEST_MODE_NUMBER;
        }
		nReturnValue = SUCESS_COMM;
		break;

    default: break;
    }

	return nReturnValue;
}

BYTE SysLCDTFT_PageTestIconProc(BYTE nCount, uint16_t _id)
{
    BYTE nReturnValue = SUCESS_COMM;

	switch(_id)
	{
	//����ͼ��
    case 3:
		//
        g_stTestShowFlag.nStart		= 1;
        g_stTestShowFlag.nWorkState = 1;

        switch(nCount)
        {
        case TFT_STATUS_ICON_DISABLE:
            if(TEST_STATUS_STOP == g_stTestValue.nStatus) { g_stTestValue.nStep = TEST_STEP_START; }
            break;

        case TFT_STATUS_ICON_ENABLE:
            if(TEST_STATUS_START == g_stTestValue.nStatus) { g_stTestValue.nStep = TEST_STEP_STOP; }
            break;

        default: break;
        }
		break;

    case 4:	   //�Զ����鹤λһͼ��
        SysTestAutoIconProc(TEST_SELECT_1, nCount);
        break;

    case 5:	   //�Զ����鹤λ��ͼ��
        SysTestAutoIconProc(TEST_SELECT_2, nCount);
        break;

    case 6:	   //�Զ����鹤λ��ͼ��
        SysTestAutoIconProc(TEST_SELECT_3, nCount);
        break;

    case 7:	   //�ֶ����鹤λһͼ��
        SysTestManualIconProc(TEST_SELECT_1, nCount);
        break;

    case 8:	   //�ֶ����鹤λ��ͼ��
        SysTestManualIconProc(TEST_SELECT_2, nCount);
        break;

    case 9:	   //�ֶ����鹤λ��ͼ��
        SysTestManualIconProc(TEST_SELECT_3, nCount);
        break;

    default: break;
	}

	return nReturnValue;
}

/// go to param page
void SysTFTLcd_PageTestGotoProc(void)
{
    m_nTitleShowFlag							 = 1;
    g_stTestShowFlag.nStart						 = 1;
    g_stTestShowFlag.arrAutomatic[TEST_SELECT_1] = 1;
    g_stTestShowFlag.arrAutomatic[TEST_SELECT_2] = 1;
    g_stTestShowFlag.arrAutomatic[TEST_SELECT_3] = 1;
    g_stTestShowFlag.arrManual[TEST_SELECT_1]	 = 1;
    g_stTestShowFlag.arrManual[TEST_SELECT_2]	 = 1;
    g_stTestShowFlag.arrManual[TEST_SELECT_3]	 = 1;
    g_stTestShowFlag.nWorkMode					 = 1;
    g_stTestShowFlag.nWorkState					 = 1;

	SP3_SendSysChangePageCmdProc(TFT_PAGE_TYPE_TEST, 1);
}
