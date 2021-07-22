///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
#include "SysUART3ProtocolProc.h"

#include "24cxx.h"
#include "RealDatabase.h"
#include "SaveParamProc.h"
#include "SysAtomVarProc.h"
#include "SysLCD_ShowInfoProc.h"
#include "SysLcd_PageFirst.h"
#include "SysLcd_PageInfo.h"
#include "SysLcd_PageParam.h"
#include "SysLcd_PageTest.h"
#include "SysUART3.h"
#include "SysUART3CommProc.h"
#include "pcf8563.h"
#include "systick.h"
///////////////////////////////////////////////////////////////////////////

//�ն˵�ַ
int16 g_nAddr_SP3 = 0;

//
static BYTE	 m_nCurStep_SP3 = SYNSTEP;
static u16	 m_nCmd_SP3		= 0;
static int16 m_nLen_SP3		= 0;

//
static int16 m_nPageID	 = 0;
static int16 m_nCtrlID	 = 0;
static BYTE	 m_nCtrlType = 0;

static BYTE m_nBtnType	   = 0;
static BYTE m_nReverseInfo = 0;

static BYTE m_nEndCount			  = 0;
static BYTE m_nRecvScreenInfoStep = 0;

u32 g_nSysTempValue	   = 0;
u8	g_nSysTempPointPos = 0;

///////////////////////////////////////////////////////////////////////////

void SP3_SetSysCommStepProc(void) { m_nCurStep_SP3 = SYNSTEP; }

BYTE SP3_RecvProc(void)
{
    BYTE nRetValue = SYNSTEP;

    //
    switch(m_nCurStep_SP3)
    {
    //ͬ��ͷ
    case SYNSTEP:
        //
        m_nCurStep_SP3 = SP3_SynProc();
        nRetValue	   = m_nCurStep_SP3;
        if(m_nCurStep_SP3 == FAILURE || m_nCurStep_SP3 == SUCESS_COMM)
        {
            m_nCurStep_SP3 = SYNSTEP;
            return nRetValue;
        }

        //�ӿ�ͨ��Ч��, ֱ�ӽ���������ȡ
        if(m_nCurStep_SP3 != CMDSTEP) { break; }

    //������
    case CMDSTEP:
        //
        m_nCurStep_SP3 = SP3_CmdProc();
        nRetValue	   = m_nCurStep_SP3;
        if(m_nCurStep_SP3 == FAILURE || m_nCurStep_SP3 == SUCESS_COMM)
        {
            m_nCurStep_SP3 = SYNSTEP;
            return nRetValue;
        }

        //�ӿ�ͨ��Ч��, ֱ�ӽ���������ȡ
        if(m_nCurStep_SP3 != DATASTEP) { break; }

    //���ݰ�
    case DATASTEP:
        //
        m_nCurStep_SP3 = SP3_DataProc();
        nRetValue	   = m_nCurStep_SP3;
        if(m_nCurStep_SP3 == FAILURE || m_nCurStep_SP3 == SUCESS_COMM)
        {
            m_nCurStep_SP3 = SYNSTEP;
            return nRetValue;
        }
        break;

    //
    default: m_nCurStep_SP3 = SYNSTEP; break;
    }

    return m_nCurStep_SP3;
}

///////////////////////////////////////////////////////////////////////////

//ͬ��ͷ
static BYTE SP3_SynProc(void)
{
    //
    BYTE  arrSynTab[1] = {0xEE};
    BYTE  i			   = 0;
    int16 nTemp;

    //
    for(i = 0; i < 1; i++)
    {
        nTemp = SP3_GetComMsgFromPos(i);

        if(nTemp == -1) { return SYNSTEP; }

        //
        if(nTemp == arrSynTab[i])
        {
            // nFirstError = 0;
            continue;
        }
        else
        {
            //��1������Ϊ��ַ���󷵻ص�
            if(g_nSysAddrError_SP3 == 1)
            {
                if(i > 1) { return CMDSTEP; }
                else
                {
                    return SUCESS_COMM;
                }
            }
            else
            {
                //ͬ��ͷ�������½���
                return SUCESS_COMM;
            }
        }
    }

    //
    return CMDSTEP;
}

//�������
static BYTE SP3_CmdProc(void)
{
    int16 nTemp0 = 0, nTemp1 = 0;

    //Ĭ�ϵ�ַƥ��
    g_nSysAddrError_SP3 = 0;

    // Cmd
    nTemp0 = SP3_GetComMsgFromPos(1);
    nTemp1 = SP3_GetComMsgFromPos(2);
    if((nTemp0 == -1) || (nTemp1 == -1)) { return CMDSTEP; }
    m_nCmd_SP3 = ConfigMulitBytesToValueProc(nTemp1, nTemp0, 0, 0);

    //����ID
    nTemp0 = SP3_GetComMsgFromPos(3);
    nTemp1 = SP3_GetComMsgFromPos(4);
    if((nTemp0 == -1) || (nTemp1 == -1)) { return CMDSTEP; }
    m_nPageID = ConfigMulitBytesToValueProc(nTemp1, nTemp0, 0, 0);

    //�ؼ�ID
    nTemp0 = SP3_GetComMsgFromPos(5);
    nTemp1 = SP3_GetComMsgFromPos(6);
    if((nTemp0 == -1) || (nTemp1 == -1)) { return CMDSTEP; }
    m_nCtrlID = ConfigMulitBytesToValueProc(nTemp1, nTemp0, 0, 0);

    //����
    nTemp0 = SP3_GetComMsgFromPos(7);
    if(nTemp0 == -1) { return CMDSTEP; }
    m_nCtrlType = nTemp0;

    //
    m_nLen_SP3 = 0;
    if(m_nLen_SP3 >= UART3_RECVDATA_LEN)
    {
        //
        return FAILURE;
    }

    //
    m_nEndCount			  = 0;
    m_nRecvScreenInfoStep = 0;

    //
    return DATASTEP;
}

//���ݽ���
static BYTE SP3_DataProc(void)
{
    int16 nTemp = 0;

    UINT16 i = 0x00, nCount = 0;
    BYTE   nReturnValue = FAILURE;

    //
    nTemp = m_nReverseInfo;

    //�ж�����������ȷ, �յ���β.
    nCount = SP3_GetRecvDataCountProc();

    //
    if(nCount <= 7) { return DATASTEP; }
    else
    {
        switch(m_nRecvScreenInfoStep)
        {
        case 0:
            //�ȴ���ȡ��β
            for(i = 8; i < nCount; i++)
            {
                nTemp = SP3_GetComMsgFromPos(i);
                //
                if(nTemp == -1) { return DATASTEP; }

                //��ť�ؼ�
                if(m_nCtrlType == 0x10)
                {
                    switch(i)
                    {
                    //��ť����
                    case 8: m_nBtnType = nTemp; continue;

                    //��������
                    case 9: m_nReverseInfo = nTemp; continue;

                    default:
                        // m_nBtnType = 0;
                        // m_nReverseInfo = 0;
                        break;
                    }
                }
                else
                {
                    // 0x11��ʼ 0x00�����м���ASCII��
                    m_nBtnType	   = 0;
                    m_nReverseInfo = 0;
                }
            }

            if(i == nCount) { m_nRecvScreenInfoStep = 1; }
            return DATASTEP;

        case 1: break;
        }

        //�ж�֡β
        for(i = 0; i < 4; i++)
        {
            nTemp = SP3_GetComMsgFromPos(nCount - 4 + i);
            //
            if(nTemp == -1) { return DATASTEP; }

            // FF FC FF FF
            switch(i)
            {
            // 0xFF
            case 0:
                if(nTemp == 0xFF) { m_nEndCount = 1; }
                else
                {
                    m_nEndCount = 0;
                }
                break;
            // 0xFC
            case 1:
                if(nTemp == 0xFC) { m_nEndCount = 2; }
                else
                {
                    m_nEndCount = 0;
                }
                break;
            // 0xFF
            case 2:
                if(nTemp == 0xFF) { m_nEndCount = 3; }
                else
                {
                    m_nEndCount = 0;
                }
                break;
            // 0xFF
            case 3:
                //
                if(nTemp == 0xFF) { m_nEndCount = 4; }
                else
                {
                    m_nEndCount = 0;
                }
                break;
            }
        }

        //��ǰ������, ��û���յ���β
        if((m_nEndCount == 0) || (m_nEndCount < 4))
        // if (m_nEndCount == 0)
        {
            //�ް�β, ����ʧ��
            return DATASTEP;
        }
        else
        {
            //������ȡ����
            m_nEndCount = 0;
        }
    }

    //
    switch(m_nCmd_SP3)
    {
    //��ʾ���͵���̬�ؼ�ָ��
    case 0xB110: break;

    //��ʾ���յ���̬�ؼ�ָ��
    case 0xB111: break;

    //��ʾ���յ�ͼ��ؼ�ָ��
    case 0xB126: break;
    }

    if(m_nBtnType == 0x00) {}

    //
    switch(m_nCtrlType)
    {
    //��ť�ؼ�
    case 0x10:
        if(1 == SP3_GetComMsgFromPos(nCount - 5)) { nReturnValue = SysPageBtnProc(); }
		break;

    //�ı��ؼ����� - ���ղ��������ı�ֵ
    case 0x11: nReturnValue = SysPageTextInfoProc(nCount); break;

    // ͼ��ؼ�
	case 0x01:
        if(1 == SP3_GetComMsgFromPos(nCount - 6)) { nReturnValue = SysPageIconProc(SP3_GetComMsgFromPos(nCount - 5)); }
		break;
    }

    Set_SysStaticFreshTimeCtlrValueProc(20);
    //��װˢ����ʾ��ֵ 60ms
    Set_SysDisplayTimeCtlrValueProc(60);

    //
    return nReturnValue;
}
//////////////////////////////////////////////////////////////////////////
u8 SP3_GetCtrlID(void) { return m_nCtrlID; }


///////////////////////////////���Ͷ�Ӧ�ؼ�����////////////////////////////////////
//���Ʒ�������һ��
extern BYTE SP3_SendBuzzerCmdProc(u8 nEnWait)
{
    volatile BYTE *pBuf		= g_arrUart3SendBuf;
    u8			   nCount	= 0;
    u32			   nTimeOut = 0;

    // Head
    *(pBuf + nCount++) = 0xEE;

    // Cmd
    *(pBuf + nCount++) = 0x61;
    *(pBuf + nCount++) = 0x0A;

    *(pBuf + nCount++) = 0x27;
    *(pBuf + nCount++) = 0xA8;

    // Lpc
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFC;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFF;

    SP3_SendBytesInfoProc((u8 *)&g_arrUart3SendBuf, nCount);

    //�ȴ����ͽ���
    while((!SP3_GetSysSendStatusProc()) && (nEnWait == 1) && (nTimeOut++ < 60000))
        ;

    return SUCESS_COMM;
}

//�л�����
extern BYTE SP3_SendSysChangePageCmdProc(UINT16 nPageID, u8 nEnWait)
{
    volatile BYTE *pBuf		= g_arrUart3SendBuf;
    u8			   nCount	= 0;
    u32			   nTimeOut = 0;

    // Head
    *(pBuf + nCount++) = 0xEE;

    // Cmd
    *(pBuf + nCount++) = 0xB1;
    *(pBuf + nCount++) = 0x00;

    //����ID ���ֽ���ǰ,���ֽ��ں�
    *(pBuf + nCount++) = (BYTE)(nPageID >> 8);
    *(pBuf + nCount++) = (BYTE)(nPageID);

    //��дָ�븴λ
    g_nWrPos = 0x00;
    g_nRdPos = 0x00;

    // Lpc
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFC;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFF;

    SP3_SendBytesInfoProc((u8 *)&g_arrUart3SendBuf, nCount);

    //�ȴ����ͽ���
    while((!SP3_GetSysSendStatusProc()) && (nEnWait == 1) && (nTimeOut++ < 60000))
        ;

    //��ȡ��ǰҳ
    g_nSysShowType = (TFT_PAGE_DEF)nPageID;

    //�����¿���ͨ������
    // ChangePageSetAIInitCmd(nPageID);

    return SUCESS_COMM;
}

//����Icon״̬
extern BYTE SP3_SendSetSysIconStatusCmdProc(UINT16 nPageID, UINT16 nCtrlID, BYTE nBtnStaus, u8 nEnWait)
{
    BYTE		   nCount	= 0;
    volatile BYTE *pBuf		= NULL;
    u32			   nTimeOut = 0;

    //ָ���ͻ�����
    pBuf = g_arrUart3SendBuf;

    // Head
    *(pBuf + nCount++) = 0xEE;

    // Cmd
    *(pBuf + nCount++) = 0xB1;
    *(pBuf + nCount++) = 0x23;

    //����ID ���ֽ���ǰ,���ֽ��ں�
    *(pBuf + nCount++) = (BYTE)(nPageID >> 8);
    *(pBuf + nCount++) = (BYTE)(nPageID);

    //�ؼ�ID ���ֽ���ǰ,���ֽ��ں�
    *(pBuf + nCount++) = (BYTE)(nCtrlID >> 8);
    *(pBuf + nCount++) = (BYTE)(nCtrlID);

    //��ť״̬
    *(pBuf + nCount++) = nBtnStaus;

    //
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFC;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFF;

    //��дָ�븴λ
    g_nWrPos = 0x00;
    g_nRdPos = 0x00;

    SP3_SendBytesInfoProc((u8 *)&g_arrUart3SendBuf, nCount);

    //�ȴ����ͽ���
    while((!SP3_GetSysSendStatusProc()) && (nEnWait == 1) && (nTimeOut++ < 60000))
        ;

    return SUCESS_COMM;
}

/*
������ʾ
EE B1    32    00 0B     00 0E    00     00 02     00 64     FF FC FF FF
pCURVE_DEF:
UINT16 nPageID;
UINT16 nCtrlID;
CURVE_DISP_TYPE_DEF emDispType;
u8 nCh;
u8 nDataLen;
u16 arrData[4];
*/
extern BYTE SP3_SendCurveDispCmdProc(pCURVE_DEF pCurveInfo, u8 nEnWait)
{
    //  BYTE nChCount = 0;
    BYTE		   nCount	= 0;
    volatile BYTE *pBuf		= g_arrUart3SendBuf;	//ָ���ͻ�����
    u32			   nTimeOut = 0;

    LIMIT(0, pCurveInfo->nCh, 7);

    // EE B1
    // 32/35 DispType
    // 00 0B ҳid
    // 00 0E �ؼ�id
    // 00    ͨ��ֵ0-7
    // 00 02 ��ֵ����
    // 00 64 ��ֵ
    // FF FC FF FF
    // Head
    *(pBuf + nCount++) = 0xEE;

    // Cmd
    *(pBuf + nCount++) = 0xB1;

    //��ʾ����
    switch(pCurveInfo->emDispType)
    {
    case Front:
        //��ǰ����ʾ
        *(pBuf + nCount++) = 0x32;
        break;
    case Back:
        //�Ӻ����ʾ
        *(pBuf + nCount++) = 0x35;
        break;
    }

    //����ID ���ֽ���ǰ,���ֽ��ں�
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nPageID >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nPageID);

    //�ؼ�ID ���ֽ���ǰ,���ֽ��ں�
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nCtrlID >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nCtrlID);

    //ͨ��
    *(pBuf + nCount++) = pCurveInfo->nCh;

    //���ݳ���
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nDataLen >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nDataLen);

    //����
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->arrData[pCurveInfo->nCh] >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->arrData[pCurveInfo->nCh]);

    // Lpc
    //*pBuf = nLpc;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFC;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFF;

    SP3_SendBytesInfoProc((u8 *)&g_arrUart3SendBuf, nCount);

    //�ȴ����ͽ���
    while((!SP3_GetSysSendStatusProc()) && (nEnWait == 1) && (nTimeOut++ < 60000))
        ;

    return SUCESS_COMM;
}

//�����Ǳ�ؼ�ֵ
extern BYTE SP3_SendSetMeterValueCmdProc(UINT16 nPageID, UINT16 nCtrlID, int32 nMeterValue, u8 nEnWait)
{
    BYTE		   i		= 0;
    volatile BYTE *pBuf		= NULL;
    u32			   nTimeOut = 0;

    //ָ���ͻ�����
    pBuf = g_arrUart3SendBuf;

    // Head
    *pBuf++ = 0xEE;

    // Cmd
    *pBuf++ = 0xB1;
    *pBuf++ = 0x10;

    //����ID ���ֽ���ǰ,���ֽ��ں�
    *pBuf++ = (BYTE)(nPageID >> 8);
    *pBuf++ = (BYTE)(nPageID);

    //�ؼ�ID ���ֽ���ǰ,���ֽ��ں�
    *pBuf++ = (BYTE)(nCtrlID >> 8);
    *pBuf++ = (BYTE)(nCtrlID);

    //�ؼ���ֵ
    for(i = 0; i < 4; i++) { *pBuf++ = (BYTE)(nMeterValue >> ((3 - i) * 8)); }

    //��дָ�븴λ
    g_nWrPos = 0x00;
    g_nRdPos = 0x00;

    // Lpc
    //*pBuf = nLpc;
    *pBuf++ = 0xFF;
    *pBuf++ = 0xFC;
    *pBuf++ = 0xFF;
    *pBuf	= 0xFF;

    SP3_SendBytesInfoProc((u8 *)&g_arrUart3SendBuf, 1 + 2 + 2 + 2 + 4 + 4);

    //�ȴ����ͽ���
    while((!SP3_GetSysSendStatusProc()) && (nEnWait == 1) && (nTimeOut++ < 60000))
        ;

    return SUCESS_COMM;
}

//ˢ���ı��ؼ���ֵ
extern BYTE SP3_SendSetSysTextValueCmdProc(UINT16 nPageID, UINT16 nCtrlID, BYTE nLen, BYTE *pTextValueInfoAry, u8 nEnWait)
{
    BYTE		   i		= 0;
    volatile BYTE *pBuf		= NULL;
    u32			   nTimeOut = 0;

    //ָ���ͻ�����
    pBuf = g_arrUart3SendBuf;

    // Head
    *pBuf++ = 0xEE;

    // Cmd
    *pBuf++ = 0xB1;
    *pBuf++ = 0x10;

    //����ID ���ֽ���ǰ,���ֽ��ں�
    *pBuf++ = (BYTE)(nPageID >> 8);
    *pBuf++ = (BYTE)(nPageID);

    //�ؼ�ID ���ֽ���ǰ,���ֽ��ں�
    *pBuf++ = (BYTE)(nCtrlID >> 8);
    *pBuf++ = (BYTE)(nCtrlID);

    //�ؼ���ֵ
    for(i = 0; i < nLen; i++) { *pBuf++ = pTextValueInfoAry[i]; }

    //��дָ�븴λ
    g_nWrPos = 0x00;
    g_nRdPos = 0x00;

    // Lpc
    //*pBuf = nLpc;
    *pBuf++ = 0xFF;
    *pBuf++ = 0xFC;
    *pBuf++ = 0xFF;
    *pBuf	= 0xFF;

    SP3_SendBytesInfoProc((u8 *)&g_arrUart3SendBuf, 1 + 2 + 2 + 2 + nLen + 4);

    //�ȴ����ͽ���
    while((!SP3_GetSysSendStatusProc()) && (nEnWait == 1) && (nTimeOut++ < 60000))
        ;

    return SUCESS_COMM;
}


////////////////////////////////�����ı�//////////////////////////////////////////

//�����ϴ����ı� 0x2EС����, ʱ����ж��С����
extern u8 SysAsciiCodeToTimeValueProc(u16 nCount)
{
    //  u32 nResult;
    u16 nLen = 0;
    u16 i	 = 0;
    u8	nValue;
    // u8 nPointPos = 0;
    u8 nIndex = 0;

    //�����ڴ�
    //    u8 *p = mymalloc(14);

    // lw-20190918
    u8 p[100];
    // ASCII��ĳ���
    // nPointPos = 0;
    nLen = nCount - 8 - 4 - 1;

    //
    nIndex = 0;
    //
    for(i = 0; i < nLen; i++)
    {
        nValue = SP3_GetComMsgFromPos(8 + i);

        //С����
        if(nValue == 0x2E)
        {
            // nPointPos = nLen - i - 1;
            continue;
        }
        //����
        else
        {
            //ȥ���ո�  �ո�Ĭ��'0'
            if(nValue == ' ') { nValue = '0'; }
            *(p + nIndex) = nValue - '0';
        }
        nIndex++;
    }

    //
    g_stRtcTime.nSecond = *(p + 12) * 10 + *(p + 13);
    g_stRtcTime.nMinute = *(p + 10) * 10 + *(p + 11);
    g_stRtcTime.nHour	= *(p + 8) * 10 + *(p + 9);
    g_stRtcTime.nDay	= *(p + 6) * 10 + *(p + 7);
    g_stRtcTime.nMonth	= *(p + 4) * 10 + *(p + 5);
    g_stRtcTime.nYear	= *(p)*1000 + *(p + 1) * 100 + *(p + 2) * 10 + *(p + 3);

    PCF8563_SetTime(g_stRtcTime.nYear, g_stRtcTime.nMonth, g_stRtcTime.nDay, g_stRtcTime.nHour, g_stRtcTime.nMinute, g_stRtcTime.nSecond, 1);

    //�ͷ��ڴ�
    // myfree(p);

    return SUCESS_COMM;
}

//�����ϴ����ı� 0x2EС����
/*
UINT16 nCount:����
BYTE nDesPointPos:С��λ
volatile UINT32* pDesValue:�õ�����
volatile BYTE* pDesPointPos: �õ�С��λ
BYTE nPointType:0��nDesPointPos  1�������С����
*/
extern void SysAsciiCodeToValueProc(UINT16 nCount, BYTE nDesPointPos, volatile UINT32 *pDesValue, volatile BYTE *pDesPointPos, BYTE nPointType)
{
    BYTE   arrTemp[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT16 nLen		   = 0;
    UINT16 i = 0, j = 0;
    BYTE   nValue;

    BYTE   nIndex	   = 0;
    float  ftTempValue = 0.0;
    UINT32 nResult	   = 0;
    int32  nBaseValue  = 10;
    BYTE   nPointPos   = 0;

    u8 nSignedFlag = 0;

    // ASCII��ĳ���
    nPointPos = 0;
    nLen	  = nCount - 8 - 4 - 1;

    //
    nIndex = 0;
    for(i = 0; i < nLen; i++)
    {
        nValue = SP3_GetComMsgFromPos(8 + i);

        //����
        if(nValue == 0x2D)
        {
            nSignedFlag = 1;
            continue;
        }

        //С����
        if(nValue == 0x2E)
        {
            nPointPos = nLen - i - 1;
            continue;
        }

        //����
        else
        {
            //ȥ���ո�  �ո�Ĭ��'0'
            if(nValue == ' ') { nValue = '0'; }
            arrTemp[nIndex] = nValue - '0';
        }

        //
        nIndex++;
    }

    // nPointType =1 ʱ С��λ��ȡ���������С��λ lyy180728
    if(nPointType == 1) { nDesPointPos = nPointPos; }

    //����nDesPointPosλС��
    if(nPointPos >= nDesPointPos) { nIndex = nIndex - nPointPos + nDesPointPos; }
    else
    {
        //����С��λ��С��Ŀ��Ҫ��λ��
        nIndex = nIndex + (nDesPointPos - nPointPos);
    }

    //
    nBaseValue = 1;
    nResult	   = 0;
    for(i = 0; i < nIndex; i++)
    {
        //???
        for(j = 0; j < (nIndex - i - 1); j++) { nBaseValue *= 10; }

        ftTempValue = nBaseValue * arrTemp[i];
        nResult += ftTempValue;

        //
        nBaseValue = 1;

        // nResult += arrTemp[nIndex-i-1] * nBaseValue;
        // nBaseValue *= 10;
        // pow������8λ�����о�����ʧ
        // nResult += (arrTemp[i] * pow(10, nIndex-i-1));
    }

    /*/
    g_nGValue = nResult;
    //g_nGPointPos = nPointPos;
    g_nGPointPos = nDesPointPos;
    */

    //�������λ
    if(nSignedFlag == 1)
    {
        //����λ
        nBaseValue = -nResult;
        *pDesValue = nBaseValue;
    }
    else
    {
        *pDesValue = nResult;
    }

    if(nPointType == 0x00) { *pDesPointPos = nDesPointPos; }
    else
    {
        *pDesPointPos = nPointPos;
    }
}

//////////////////////////����ҳ�Ŀؼ�//////////////////////////////////////////////

/*
Һ������ť������
*/
static u8 SysPageBtnProc(void)
{
    BYTE nReturnValue = FAILURE;

    /*/�����л�, ��ʾ��ǰ���µ��ǻ����л���ť
    case 0x00:
    //��������, ��ʾ��ǰ���µ��ǿ��ذ�ť
    case 0x01:
    //��������, ��ʾ��ǰ���µİ�����ť
    case 0x02:
    break;
    */

    switch(m_nPageID)
    {
    case FIRST_PAGE: nReturnValue = SysLCDTFT_PageFirstButtonProc(m_nCtrlID); break;

    case WORK_PAGE: nReturnValue = SysLCDTFT_PageTestButtonProc(m_nCtrlID); break;

    case PARAM_PAGE: nReturnValue = SysLCDTFT_PageParamButtonProc(m_nCtrlID); break;

    case INFO_PAGE: nReturnValue = SysLCDTFT_PageInfoButtonProc(m_nCtrlID); break;

    default: break;
    }
    return nReturnValue;
}

/*
�ı�������
*/
static u8 SysPageTextInfoProc(u8 nCount)
{
    BYTE nReturnValue = FAILURE;
    switch(m_nPageID)
    {
    case FIRST_PAGE: nReturnValue = SysLCDTFT_PageFirstTextProc(nCount, m_nCtrlID); break;

    case WORK_PAGE: nReturnValue = SysLCDTFT_PageTestTextProc(nCount, m_nCtrlID); break;

    case PARAM_PAGE: nReturnValue = SysLCDTFT_PageParamTextProc(nCount, m_nCtrlID); break;

	case INFO_PAGE:
        // nReturnValue = SysLCDTFT_PageInfoTextProc(nCount);
        break;

    default: break;
    }
    return nReturnValue;
}

u8 SysPageIconProc(u8 nCount)
{
	BYTE nReturnValue = FAILURE;
    switch(m_nPageID)
    {
    case FIRST_PAGE:
        // nReturnValue = SysLCDTFT_PageFirstTextProc(nCount);
		break;

    case WORK_PAGE: nReturnValue = SysLCDTFT_PageTestIconProc(nCount, m_nCtrlID); break;

    case PARAM_PAGE: nReturnValue = SysLCDTFT_PageParamIconProc(nCount, m_nCtrlID); break;

    case INFO_PAGE: nReturnValue = SysLCDTFT_PageInfoIconProc(nCount, m_nCtrlID); break;

    default: break;
    }

    return nReturnValue;
}

//////////////////////////////////////////////////////////////////////////

/*
EE �� B1 33 SCREEN_ID CONTROL_ID CHANNEL�� FF FC FF FF
���ָ��ͨ��������
CHANNEL(1 ���ֽ�)���� 8 ������ͨ������ŷ�Χ�� 0~7��
*/
extern BYTE SP3_SendCurveClearCmdProc(pCURVE_DEF pCurveInfo, u8 nEnWait)
{
    //  BYTE nChCount = 0;
    BYTE		   nCount	= 0;
    volatile BYTE *pBuf		= g_arrUart3SendBuf;	//ָ���ͻ�����
    u32			   nTimeOut = 0;

    LIMIT(0, pCurveInfo->nCh, 7);

    *(pBuf + nCount++) = 0xEE;

    // Cmd
    *(pBuf + nCount++) = 0xB1;

    //���
    *(pBuf + nCount++) = 0x33;

    //����ID ���ֽ���ǰ,���ֽ��ں�
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nPageID >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nPageID);

    //�ؼ�ID ���ֽ���ǰ,���ֽ��ں�
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nCtrlID >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nCtrlID);

    //ͨ��
    *(pBuf + nCount++) = pCurveInfo->nCh;

    // Lpc
    //*pBuf = nLpc;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFC;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFF;

    SP3_SendBytesInfoProc((u8 *)&g_arrUart3SendBuf, nCount);

    //�ȴ����ͽ���
    while((!SP3_GetSysSendStatusProc()) && (nEnWait == 1) && (nTimeOut++ < 60000))
        ;

    return SUCESS_COMM;
}

/*
��������
EE �� B1 34   SCREEN_ID   CONTROL_ID    XOFFSET XMUL    YOFFSET  YMUL�� FF FC FF FF
EE B1    34    00 0B       00 0E        00 00   00 64   00 00    01 72  FF FC FF FF
ָ����ֱ/ˮƽ������/ƽ��
XOFFSET (2 ���ֽ�)��ˮƽƫ�����ݵ���������Ϊ��������Ϊ��
XMUL (2 ���ֽ�)��ˮƽ����ϵ������λ 0.01
YOFFSET (2 ���ֽ�)����ֱƫ����ֵ������Ϊ��������Ϊ��
YMUL (2 ���ֽ�)����ֱ����ϵ������λ 0.01
�������������ļ��㹫ʽ���� N �����������ֵΪ V
X ���� = (N-XOFFSET)*XMUL*0.01
Y ���� = (V-YOFFSET)*YMUL*0.01
*/
extern BYTE SP3_SendCurveZoomCmdProc(pCURVE_DEF pCurveInfo, u8 nEnWait)
{
    //  BYTE nChCount = 0;
    BYTE		   nCount	= 0;
    volatile BYTE *pBuf		= g_arrUart3SendBuf;	//ָ���ͻ�����
    u32			   nTimeOut = 0;

    LIMIT(0, pCurveInfo->nCh, 7);

    // EE B1
    // 32/35 DispType
    // 00 0B ҳid
    // 00 0E �ؼ�id
    // 00    ͨ��ֵ0-7
    // 00 02 ��ֵ����
    // 00 64 ��ֵ
    // FF FC FF FF
    // Head
    *(pBuf + nCount++) = 0xEE;

    // Cmd
    *(pBuf + nCount++) = 0xB1;

    //����
    *(pBuf + nCount++) = 0x34;

    //����ID ���ֽ���ǰ,���ֽ��ں�
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nPageID >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nPageID);

    //�ؼ�ID ���ֽ���ǰ,���ֽ��ں�
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nCtrlID >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nCtrlID);

    //ˮƽƫ��
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->stZoomInfo.nXOffect >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->stZoomInfo.nXOffect);
    //ˮƽ���ű���
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->stZoomInfo.nXMUL >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->stZoomInfo.nXMUL);

    //��ֱƫ��
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->stZoomInfo.nYOffect >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->stZoomInfo.nYOffect);
    //��ֱ���ű���
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->stZoomInfo.nYMUL >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->stZoomInfo.nYMUL);

    // Lpc
    //*pBuf = nLpc;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFC;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFF;

    SP3_SendBytesInfoProc((u8 *)&g_arrUart3SendBuf, nCount);

    //�ȴ����ͽ���
    while((!SP3_GetSysSendStatusProc()) && (nEnWait == 1) && (nTimeOut++ < 60000))
        ;

    return SUCESS_COMM;
}
