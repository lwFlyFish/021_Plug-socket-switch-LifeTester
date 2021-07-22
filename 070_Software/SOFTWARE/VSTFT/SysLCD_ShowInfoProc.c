///////////////////////////////////////////////////////////////////////////
//
// File Name : SysLCD_ShowInfoProc
// Compiler Type : ICCAVR 7.22
// Date : 2008/12/16
// Designer : spy
// ChipType : M32
// Clock : 14.7456MHz
//
// Hardware Interface :
//
//
//
// Function :
//
//
///////////////////////////////////////////////////////////////////////////

#include "SysLCD_ShowInfoProc.h"

#include "RealDatabase.h"
#include "SaveParamProc.h"
#include "SysUART3.h"
#include "SysUart3ProtocolProc.h"

#include "SysLCD_PageFirst.h"
#include "SysLCD_PageTest.h"
#include "SysLCD_PageParam.h"
#include "SysLCD_PageInfo.h"

#define DISP_HUMI_BUF_NUM 30


///////////////////////////////////////////////////////////////////////////

TFT_PAGE_DEF g_nSysShowType = FIRST_PAGE;

u16 g_nDispHumiValue = 0;

//��ǰˢ�����ݱ�־
BYTE g_nCurFreshDataFlag   = 0x00;
BYTE g_nCurFreshData_SFlag = 0x00;

// PIDҳˢ��
u8 g_nPIDPageCurFreshDataFlag          = 0;
u8 g_nPIDHumiPageCurFreshDataFlag      = 0;
u8 g_nPIDWindSpeedPageCurFreshDataFlag = 0;
//����ҳˢ��
u8 g_nConfigPageCurFreshDataFlag = 0;
//�궨ҳˢ��
u8 g_nBDPageCurFreshDataFlag = 0;

u8 g_nResultPageFillTag = 0;
u8 g_nBDPageFillTag     = 0;
u8 g_nHumiIconStatus    = 0;

///////////////////////////////////////////////////////////////////////////
// TFTLCDϵͳ��̬����ˢ��
extern BYTE SysStaticParamRealTimeFreshProc(void)
{
    BYTE nRetValue = 0;

    //�Ƿ�ˢ��
    if (g_nFillStaticInfoFlag == 0) 
		return 0; 
	
    //ϵͳ��ʾ������ʵʱ��ʾ
    switch (g_nSysShowType)
    {
    //��ҳ, ��ӭ��Ϣ/ϵͳ����ѡ����Ϣ(��ҳ����)
    case FIRST_PAGE: 
		break;
	
	case PARAM_PAGE: 
		break;

    default: 
		break;
    }

    return nRetValue;
}

//ʵʱˢ��
extern void SysTFTLCD_RealTimeFreshSysInfoProc(void)
{
    //ϵͳ��ʾ������ʵʱ��ʾ
    switch(g_nSysShowType)
    {
    //��ҳ, ��ӭ��Ϣ/ϵͳ����ѡ����Ϣ(��ҳ����)
    case FIRST_PAGE:
        //��ʾʱ��
        //SysShow_SysTimeInfoProc(FIRST_PAGE, 0x03, g_stRtcTime);

        SysLCDTFT_PageFirstShowInfoProc();
		break;
		
	case WORK_PAGE:
		SysLCDTFT_PageTestShowInfoProc();
		break;
		
	case PARAM_PAGE:
		SysLCDTFT_PageParamShowInfoProc();
		break;
		
	case INFO_PAGE:
		SysLCDTFT_PageInfoShowInfoProc();
		break;

    default: 
	break;
    }
}

//////////////////////////////�л�����///////////////////////////////////

//��ʾ��ҳ
extern void SysTFTLcd_ShowSysFirstPageProc(void)
{
    SP3_SendSysChangePageCmdProc(FIRST_PAGE, 1);
}

///////////////////////////////////////////////////////////////////
/*��ʾ����С������
u16 nPageID:ҳ��ID
u16 nCtrlID:�ؼ�id
float fShowValue:��ʾ����
nMaxPointNum:���С��λ��
*/
extern void SysLCDTFT_ShowFloatNumberProc(u16 nPageID, u16 nCtrlID, float fShowValue, u8 nMaxPointNum)
{
    //���������ʾ
    char  arrShowBuf[11] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n'};
    BYTE  nPointPos      = 0;
    BYTE  nLen           = 0;
    s32   nTemp          = 0;
    float fTemp          = 0.0;

    //����С�������õ�����ֵ
    nTemp = FloatToIntProc(fShowValue, &nPointPos, nMaxPointNum);

    //С������
    fTemp = IntToFloatProc(nTemp, nPointPos);

    //ת�����ַ�
    if(nPointPos == 0) { sprintf(arrShowBuf, "%d", (s32)fTemp); }
    else
    {
        switch(nPointPos)
        {
        case 1: sprintf(arrShowBuf, "%0.1f", fTemp); break;

        case 2: sprintf(arrShowBuf, "%0.2f", fTemp); break;

        case 3: sprintf(arrShowBuf, "%0.3f", fTemp); break;

        case 4: sprintf(arrShowBuf, "%0.4f", fTemp); break;

        case 5: sprintf(arrShowBuf, "%0.5f", fTemp); break;

        case 6: sprintf(arrShowBuf, "%0.6f", fTemp); break;

        case 7: sprintf(arrShowBuf, "%0.7f", fTemp); break;

        case 8: sprintf(arrShowBuf, "%0.8f", fTemp); break;
        default: sprintf(arrShowBuf, "%0.1f", fTemp); break;
        }
    }

    //����
    nLen = strlen(arrShowBuf);

    SP3_SendSetSysTextValueCmdProc(nPageID, nCtrlID, nLen, (u8 *)arrShowBuf, 1);
}

/*
u16 nPageID:ҳ��ID
u16 nCtrlID:�ؼ�id
BYTE nPointPos:С����
u8 nLen:���ݳ���
int32 nShowValue:��ʾ����
BYTE nMaxDataLen:������ݳ���
*/

extern void SysLCDTFT_ShowNumberProc(u16 nPageID, u16 nCtrlID, BYTE nPointPos, u8 nLen, int32 nShowValue, BYTE nMaxDataLen)
{
    //���������ʾ
    BYTE arrShowBuf[11] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n'};
    // BYTE arrShowBit[2] = {0x30, '\n'};

    BYTE i;
    // BYTE nCurXPos;
    BYTE nTotalLen, nBitNum = 0;
    // BYTE nColCoef;
    int32 nTemp;

    //ȷ������λ��
    arrShowBuf[nMaxDataLen] = '\n';
    nTemp                   = ABS(nShowValue);
    while(nTemp)
    {
        //����Ч���ݳ���
        nBitNum++;
        nTemp /= 10;
    }

    if(nBitNum == 0)
    {
        //����Ϊ0ʱ, ����1λ��Чλ
        nBitNum++;
    }

    //����С����λʱ�����ݳ���
    nTotalLen = nBitNum;

    //
    nTemp = ABS(nShowValue);

    //��С����
    if(nPointPos > 0)
    {
        //����С����
        arrShowBuf[nMaxDataLen - 1 - nPointPos] = '.';

        if(nTotalLen > nPointPos)
        {
            //���ݼ�С���㳤��
            nTotalLen++;
        }
        else
        {
            //���ݼ�С���㳤��
            nTotalLen = nPointPos + 2;
        }

        //
        for(i = 0; i < (nTotalLen - 1); i++)
        {
            if(i < nPointPos) { arrShowBuf[nMaxDataLen - 1 - i] = (nTemp % 10) | 0x30; }
            else
            {
                arrShowBuf[nMaxDataLen - 2 - i] = (nTemp % 10) | 0x30;
            }

            nTemp = nTemp / 10;
        }
    }
    //��С����
    else
    {
        //����(��λ��ǰ)   //�����������ǰ��ʼ���
        for(i = 0; i < nTotalLen; i++)
        {
            arrShowBuf[nMaxDataLen - i - 1] = (nTemp % 10) | 0x30;
            nTemp /= 10;
        }
    }

    //���+-��
    if(nShowValue < 0)
    {
        if(nMaxDataLen > nTotalLen) { arrShowBuf[nMaxDataLen - nTotalLen - 1] = '-'; }
    }

    SP3_SendSetSysTextValueCmdProc(nPageID, nCtrlID, nLen, arrShowBuf, 1);
}



void SysLCDTFT_ShowIconInfoProc(u16 nPageID, u16 nCtrlID, u8 nFrame)
{
    //�л�Icon����
    SP3_SendSetSysIconStatusCmdProc(nPageID, nCtrlID, nFrame, 1);
}

///////////////////////////////////��̬��Ϣ///////////////////////////////////////
//��ʾʱ��
void SysShow_SysTimeInfoProc(UINT16 nPageID, UINT16 nCtrlID, RTC_TIME_DEF stTime)
{
    //���������ʾ
    BYTE arrShowBuf[20] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n'};
    BYTE nLen           = 20;

    // Year
    arrShowBuf[0] = stTime.nYear / 1000 | 0x30;
    arrShowBuf[1] = stTime.nYear % 1000 / 100 | 0x30;
    arrShowBuf[2] = stTime.nYear % 100 / 10 | 0x30;
    arrShowBuf[3] = stTime.nYear % 10 | 0x30;
    //
    arrShowBuf[4] = '.';
    // Month
    arrShowBuf[5] = (stTime.nMonth / 10) | 0x30;
    arrShowBuf[6] = (stTime.nMonth % 10) | 0x30;
    //
    arrShowBuf[7] = '.';
    // Day
    arrShowBuf[8] = (stTime.nDay / 10) | 0x30;
    arrShowBuf[9] = (stTime.nDay % 10) | 0x30;
    //
    arrShowBuf[10] = '.';

    // Hour
    arrShowBuf[11] = (stTime.nHour / 10) | 0x30;
    arrShowBuf[12] = (stTime.nHour % 10) | 0x30;
    //
    arrShowBuf[13] = '.';
    // Minute
    arrShowBuf[14] = (stTime.nMinute / 10) | 0x30;
    arrShowBuf[15] = (stTime.nMinute % 10) | 0x30;

    //
    arrShowBuf[16] = '.';
    // Second
    arrShowBuf[17] = (stTime.nSecond / 10) | 0x30;
    arrShowBuf[18] = (stTime.nSecond % 10) | 0x30;

    //
    SP3_SendSetSysTextValueCmdProc(nPageID, nCtrlID, nLen, arrShowBuf, 1);
}

//��ʾʱ��
void SysShow_TestTime(UINT16 nPageID, UINT16 nCtrlID, u32 nTime)
{
    //���������ʾ
    BYTE arrShowBuf[20] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    //  BYTE nLen = 8;

    u16 nHour    = nTime / 60;
    u8  nMinuter = nTime % 60;

    //  //Year
    //  arrShowBuf[0] = nTime / 100 | 0x30;
    //  arrShowBuf[1] = nTime % 100 / 10 | 0x30;
    //  arrShowBuf[2] = nTime % 10 | 0x30;
    //  //
    //  arrShowBuf[3] = 'H';
    //  //Month
    //  arrShowBuf[4] = (nMinuter / 10) | 0x30;
    //  arrShowBuf[5] = (nMinuter % 10) | 0x30;
    //  arrShowBuf[6] = 'M';
    //  //
    //  arrShowBuf[6] = '\n';

    sprintf((char *)arrShowBuf, "����ʱ��:%03dʱ%02d��", nHour, nMinuter);
    //
    SP3_SendSetSysTextValueCmdProc(nPageID, nCtrlID, strlen((char *)arrShowBuf), arrShowBuf, 1);
}

void Lcd_BuzzerCtrl(void) { SP3_SendBuzzerCmdProc(1); }
