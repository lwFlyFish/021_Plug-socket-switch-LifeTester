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

//终端地址
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
    //同步头
    case SYNSTEP:
        //
        m_nCurStep_SP3 = SP3_SynProc();
        nRetValue	   = m_nCurStep_SP3;
        if(m_nCurStep_SP3 == FAILURE || m_nCurStep_SP3 == SUCESS_COMM)
        {
            m_nCurStep_SP3 = SYNSTEP;
            return nRetValue;
        }

        //加快通信效率, 直接进入命令收取
        if(m_nCurStep_SP3 != CMDSTEP) { break; }

    //命令字
    case CMDSTEP:
        //
        m_nCurStep_SP3 = SP3_CmdProc();
        nRetValue	   = m_nCurStep_SP3;
        if(m_nCurStep_SP3 == FAILURE || m_nCurStep_SP3 == SUCESS_COMM)
        {
            m_nCurStep_SP3 = SYNSTEP;
            return nRetValue;
        }

        //加快通信效率, 直接进入数据收取
        if(m_nCurStep_SP3 != DATASTEP) { break; }

    //数据包
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

//同步头
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
            //上1次是因为地址错误返回的
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
                //同步头不对重新接收
                return SUCESS_COMM;
            }
        }
    }

    //
    return CMDSTEP;
}

//命令接收
static BYTE SP3_CmdProc(void)
{
    int16 nTemp0 = 0, nTemp1 = 0;

    //默认地址匹配
    g_nSysAddrError_SP3 = 0;

    // Cmd
    nTemp0 = SP3_GetComMsgFromPos(1);
    nTemp1 = SP3_GetComMsgFromPos(2);
    if((nTemp0 == -1) || (nTemp1 == -1)) { return CMDSTEP; }
    m_nCmd_SP3 = ConfigMulitBytesToValueProc(nTemp1, nTemp0, 0, 0);

    //画面ID
    nTemp0 = SP3_GetComMsgFromPos(3);
    nTemp1 = SP3_GetComMsgFromPos(4);
    if((nTemp0 == -1) || (nTemp1 == -1)) { return CMDSTEP; }
    m_nPageID = ConfigMulitBytesToValueProc(nTemp1, nTemp0, 0, 0);

    //控件ID
    nTemp0 = SP3_GetComMsgFromPos(5);
    nTemp1 = SP3_GetComMsgFromPos(6);
    if((nTemp0 == -1) || (nTemp1 == -1)) { return CMDSTEP; }
    m_nCtrlID = ConfigMulitBytesToValueProc(nTemp1, nTemp0, 0, 0);

    //类型
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

//数据接收
static BYTE SP3_DataProc(void)
{
    int16 nTemp = 0;

    UINT16 i = 0x00, nCount = 0;
    BYTE   nReturnValue = FAILURE;

    //
    nTemp = m_nReverseInfo;

    //判断整包接收正确, 收到包尾.
    nCount = SP3_GetRecvDataCountProc();

    //
    if(nCount <= 7) { return DATASTEP; }
    else
    {
        switch(m_nRecvScreenInfoStep)
        {
        case 0:
            //等待收取包尾
            for(i = 8; i < nCount; i++)
            {
                nTemp = SP3_GetComMsgFromPos(i);
                //
                if(nTemp == -1) { return DATASTEP; }

                //按钮控件
                if(m_nCtrlType == 0x10)
                {
                    switch(i)
                    {
                    //按钮属性
                    case 8: m_nBtnType = nTemp; continue;

                    //保留属性
                    case 9: m_nReverseInfo = nTemp; continue;

                    default:
                        // m_nBtnType = 0;
                        // m_nReverseInfo = 0;
                        break;
                    }
                }
                else
                {
                    // 0x11开始 0x00结束中间是ASCII码
                    m_nBtnType	   = 0;
                    m_nReverseInfo = 0;
                }
            }

            if(i == nCount) { m_nRecvScreenInfoStep = 1; }
            return DATASTEP;

        case 1: break;
        }

        //判断帧尾
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

        //当前包收完, 但没有收到包尾
        if((m_nEndCount == 0) || (m_nEndCount < 4))
        // if (m_nEndCount == 0)
        {
            //无包尾, 返回失败
            return DATASTEP;
        }
        else
        {
            //整包收取结束
            m_nEndCount = 0;
        }
    }

    //
    switch(m_nCmd_SP3)
    {
    //表示发送的组态控件指令
    case 0xB110: break;

    //表示接收的组态控件指令
    case 0xB111: break;

    //表示接收的图标控件指令
    case 0xB126: break;
    }

    if(m_nBtnType == 0x00) {}

    //
    switch(m_nCtrlType)
    {
    //按钮控件
    case 0x10:
        if(1 == SP3_GetComMsgFromPos(nCount - 5)) { nReturnValue = SysPageBtnProc(); }
		break;

    //文本控件类型 - 接收不定长的文本值
    case 0x11: nReturnValue = SysPageTextInfoProc(nCount); break;

    // 图表控件
	case 0x01:
        if(1 == SP3_GetComMsgFromPos(nCount - 6)) { nReturnValue = SysPageIconProc(SP3_GetComMsgFromPos(nCount - 5)); }
		break;
    }

    Set_SysStaticFreshTimeCtlrValueProc(20);
    //重装刷新显示初值 60ms
    Set_SysDisplayTimeCtlrValueProc(60);

    //
    return nReturnValue;
}
//////////////////////////////////////////////////////////////////////////
u8 SP3_GetCtrlID(void) { return m_nCtrlID; }


///////////////////////////////发送对应控件数据////////////////////////////////////
//控制蜂蜜器响一声
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

    //等待发送结束
    while((!SP3_GetSysSendStatusProc()) && (nEnWait == 1) && (nTimeOut++ < 60000))
        ;

    return SUCESS_COMM;
}

//切换画面
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

    //画面ID 高字节在前,低字节在后
    *(pBuf + nCount++) = (BYTE)(nPageID >> 8);
    *(pBuf + nCount++) = (BYTE)(nPageID);

    //读写指针复位
    g_nWrPos = 0x00;
    g_nRdPos = 0x00;

    // Lpc
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFC;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFF;

    SP3_SendBytesInfoProc((u8 *)&g_arrUart3SendBuf, nCount);

    //等待发送结束
    while((!SP3_GetSysSendStatusProc()) && (nEnWait == 1) && (nTimeOut++ < 60000))
        ;

    //获取当前页
    g_nSysShowType = (TFT_PAGE_DEF)nPageID;

    //设置温控器通信命令
    // ChangePageSetAIInitCmd(nPageID);

    return SUCESS_COMM;
}

//设置Icon状态
extern BYTE SP3_SendSetSysIconStatusCmdProc(UINT16 nPageID, UINT16 nCtrlID, BYTE nBtnStaus, u8 nEnWait)
{
    BYTE		   nCount	= 0;
    volatile BYTE *pBuf		= NULL;
    u32			   nTimeOut = 0;

    //指向发送缓冲区
    pBuf = g_arrUart3SendBuf;

    // Head
    *(pBuf + nCount++) = 0xEE;

    // Cmd
    *(pBuf + nCount++) = 0xB1;
    *(pBuf + nCount++) = 0x23;

    //画面ID 高字节在前,低字节在后
    *(pBuf + nCount++) = (BYTE)(nPageID >> 8);
    *(pBuf + nCount++) = (BYTE)(nPageID);

    //控件ID 高字节在前,低字节在后
    *(pBuf + nCount++) = (BYTE)(nCtrlID >> 8);
    *(pBuf + nCount++) = (BYTE)(nCtrlID);

    //按钮状态
    *(pBuf + nCount++) = nBtnStaus;

    //
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFC;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFF;

    //读写指针复位
    g_nWrPos = 0x00;
    g_nRdPos = 0x00;

    SP3_SendBytesInfoProc((u8 *)&g_arrUart3SendBuf, nCount);

    //等待发送结束
    while((!SP3_GetSysSendStatusProc()) && (nEnWait == 1) && (nTimeOut++ < 60000))
        ;

    return SUCESS_COMM;
}

/*
曲线显示
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
    volatile BYTE *pBuf		= g_arrUart3SendBuf;	//指向发送缓冲区
    u32			   nTimeOut = 0;

    LIMIT(0, pCurveInfo->nCh, 7);

    // EE B1
    // 32/35 DispType
    // 00 0B 页id
    // 00 0E 控件id
    // 00    通道值0-7
    // 00 02 数值长度
    // 00 64 数值
    // FF FC FF FF
    // Head
    *(pBuf + nCount++) = 0xEE;

    // Cmd
    *(pBuf + nCount++) = 0xB1;

    //显示类型
    switch(pCurveInfo->emDispType)
    {
    case Front:
        //从前端显示
        *(pBuf + nCount++) = 0x32;
        break;
    case Back:
        //从后端显示
        *(pBuf + nCount++) = 0x35;
        break;
    }

    //画面ID 高字节在前,低字节在后
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nPageID >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nPageID);

    //控件ID 高字节在前,低字节在后
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nCtrlID >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nCtrlID);

    //通道
    *(pBuf + nCount++) = pCurveInfo->nCh;

    //数据长度
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nDataLen >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nDataLen);

    //数据
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->arrData[pCurveInfo->nCh] >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->arrData[pCurveInfo->nCh]);

    // Lpc
    //*pBuf = nLpc;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFC;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFF;

    SP3_SendBytesInfoProc((u8 *)&g_arrUart3SendBuf, nCount);

    //等待发送结束
    while((!SP3_GetSysSendStatusProc()) && (nEnWait == 1) && (nTimeOut++ < 60000))
        ;

    return SUCESS_COMM;
}

//设置仪表控件值
extern BYTE SP3_SendSetMeterValueCmdProc(UINT16 nPageID, UINT16 nCtrlID, int32 nMeterValue, u8 nEnWait)
{
    BYTE		   i		= 0;
    volatile BYTE *pBuf		= NULL;
    u32			   nTimeOut = 0;

    //指向发送缓冲区
    pBuf = g_arrUart3SendBuf;

    // Head
    *pBuf++ = 0xEE;

    // Cmd
    *pBuf++ = 0xB1;
    *pBuf++ = 0x10;

    //画面ID 高字节在前,低字节在后
    *pBuf++ = (BYTE)(nPageID >> 8);
    *pBuf++ = (BYTE)(nPageID);

    //控件ID 高字节在前,低字节在后
    *pBuf++ = (BYTE)(nCtrlID >> 8);
    *pBuf++ = (BYTE)(nCtrlID);

    //控件的值
    for(i = 0; i < 4; i++) { *pBuf++ = (BYTE)(nMeterValue >> ((3 - i) * 8)); }

    //读写指针复位
    g_nWrPos = 0x00;
    g_nRdPos = 0x00;

    // Lpc
    //*pBuf = nLpc;
    *pBuf++ = 0xFF;
    *pBuf++ = 0xFC;
    *pBuf++ = 0xFF;
    *pBuf	= 0xFF;

    SP3_SendBytesInfoProc((u8 *)&g_arrUart3SendBuf, 1 + 2 + 2 + 2 + 4 + 4);

    //等待发送结束
    while((!SP3_GetSysSendStatusProc()) && (nEnWait == 1) && (nTimeOut++ < 60000))
        ;

    return SUCESS_COMM;
}

//刷新文本控件的值
extern BYTE SP3_SendSetSysTextValueCmdProc(UINT16 nPageID, UINT16 nCtrlID, BYTE nLen, BYTE *pTextValueInfoAry, u8 nEnWait)
{
    BYTE		   i		= 0;
    volatile BYTE *pBuf		= NULL;
    u32			   nTimeOut = 0;

    //指向发送缓冲区
    pBuf = g_arrUart3SendBuf;

    // Head
    *pBuf++ = 0xEE;

    // Cmd
    *pBuf++ = 0xB1;
    *pBuf++ = 0x10;

    //画面ID 高字节在前,低字节在后
    *pBuf++ = (BYTE)(nPageID >> 8);
    *pBuf++ = (BYTE)(nPageID);

    //控件ID 高字节在前,低字节在后
    *pBuf++ = (BYTE)(nCtrlID >> 8);
    *pBuf++ = (BYTE)(nCtrlID);

    //控件的值
    for(i = 0; i < nLen; i++) { *pBuf++ = pTextValueInfoAry[i]; }

    //读写指针复位
    g_nWrPos = 0x00;
    g_nRdPos = 0x00;

    // Lpc
    //*pBuf = nLpc;
    *pBuf++ = 0xFF;
    *pBuf++ = 0xFC;
    *pBuf++ = 0xFF;
    *pBuf	= 0xFF;

    SP3_SendBytesInfoProc((u8 *)&g_arrUart3SendBuf, 1 + 2 + 2 + 2 + nLen + 4);

    //等待发送结束
    while((!SP3_GetSysSendStatusProc()) && (nEnWait == 1) && (nTimeOut++ < 60000))
        ;

    return SUCESS_COMM;
}


////////////////////////////////解析文本//////////////////////////////////////////

//解析上传的文本 0x2E小数点, 时间会有多个小数点
extern u8 SysAsciiCodeToTimeValueProc(u16 nCount)
{
    //  u32 nResult;
    u16 nLen = 0;
    u16 i	 = 0;
    u8	nValue;
    // u8 nPointPos = 0;
    u8 nIndex = 0;

    //申请内存
    //    u8 *p = mymalloc(14);

    // lw-20190918
    u8 p[100];
    // ASCII码的长度
    // nPointPos = 0;
    nLen = nCount - 8 - 4 - 1;

    //
    nIndex = 0;
    //
    for(i = 0; i < nLen; i++)
    {
        nValue = SP3_GetComMsgFromPos(8 + i);

        //小数点
        if(nValue == 0x2E)
        {
            // nPointPos = nLen - i - 1;
            continue;
        }
        //数字
        else
        {
            //去掉空格  空格默认'0'
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

    //释放内存
    // myfree(p);

    return SUCESS_COMM;
}

//解析上传的文本 0x2E小数点
/*
UINT16 nCount:长度
BYTE nDesPointPos:小数位
volatile UINT32* pDesValue:得到参数
volatile BYTE* pDesPointPos: 得到小数位
BYTE nPointType:0：nDesPointPos  1检测输入小数点
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

    // ASCII码的长度
    nPointPos = 0;
    nLen	  = nCount - 8 - 4 - 1;

    //
    nIndex = 0;
    for(i = 0; i < nLen; i++)
    {
        nValue = SP3_GetComMsgFromPos(8 + i);

        //负数
        if(nValue == 0x2D)
        {
            nSignedFlag = 1;
            continue;
        }

        //小数点
        if(nValue == 0x2E)
        {
            nPointPos = nLen - i - 1;
            continue;
        }

        //数字
        else
        {
            //去掉空格  空格默认'0'
            if(nValue == ' ') { nValue = '0'; }
            arrTemp[nIndex] = nValue - '0';
        }

        //
        nIndex++;
    }

    // nPointType =1 时 小数位数取决于输入的小数位 lyy180728
    if(nPointType == 1) { nDesPointPos = nPointPos; }

    //保留nDesPointPos位小数
    if(nPointPos >= nDesPointPos) { nIndex = nIndex - nPointPos + nDesPointPos; }
    else
    {
        //输入小数位数小于目的要求位数
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
        // pow函数在8位机上有精度损失
        // nResult += (arrTemp[i] * pow(10, nIndex-i-1));
    }

    /*/
    g_nGValue = nResult;
    //g_nGPointPos = nPointPos;
    g_nGPointPos = nDesPointPos;
    */

    //处理符号位
    if(nSignedFlag == 1)
    {
        //符号位
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

//////////////////////////所有页的控件//////////////////////////////////////////////

/*
液晶屏按钮处理函数
*/
static u8 SysPageBtnProc(void)
{
    BYTE nReturnValue = FAILURE;

    /*/画面切换, 表示当前按下的是画面切换按钮
    case 0x00:
    //开关描述, 表示当前按下的是开关按钮
    case 0x01:
    //按键描述, 表示当前按下的按键按钮
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
文本处理函数
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
EE 【 B1 33 SCREEN_ID CONTROL_ID CHANNEL】 FF FC FF FF
清空指定通道的数据
CHANNEL(1 个字节)：共 8 个数据通道，编号范围（ 0~7）
*/
extern BYTE SP3_SendCurveClearCmdProc(pCURVE_DEF pCurveInfo, u8 nEnWait)
{
    //  BYTE nChCount = 0;
    BYTE		   nCount	= 0;
    volatile BYTE *pBuf		= g_arrUart3SendBuf;	//指向发送缓冲区
    u32			   nTimeOut = 0;

    LIMIT(0, pCurveInfo->nCh, 7);

    *(pBuf + nCount++) = 0xEE;

    // Cmd
    *(pBuf + nCount++) = 0xB1;

    //清除
    *(pBuf + nCount++) = 0x33;

    //画面ID 高字节在前,低字节在后
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nPageID >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nPageID);

    //控件ID 高字节在前,低字节在后
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nCtrlID >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nCtrlID);

    //通道
    *(pBuf + nCount++) = pCurveInfo->nCh;

    // Lpc
    //*pBuf = nLpc;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFC;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFF;

    SP3_SendBytesInfoProc((u8 *)&g_arrUart3SendBuf, nCount);

    //等待发送结束
    while((!SP3_GetSysSendStatusProc()) && (nEnWait == 1) && (nTimeOut++ < 60000))
        ;

    return SUCESS_COMM;
}

/*
曲线缩放
EE 【 B1 34   SCREEN_ID   CONTROL_ID    XOFFSET XMUL    YOFFSET  YMUL】 FF FC FF FF
EE B1    34    00 0B       00 0E        00 00   00 64   00 00    01 72  FF FC FF FF
指定垂直/水平的缩放/平移
XOFFSET (2 个字节)：水平偏移数据点数，左移为正，右移为负
XMUL (2 个字节)：水平缩放系数，单位 0.01
YOFFSET (2 个字节)：垂直偏移数值，下移为正，上移为负
YMUL (2 个字节)：垂直缩放系数，单位 0.01
采样点与坐标点的计算公式：第 N 个采样点的数值为 V
X 坐标 = (N-XOFFSET)*XMUL*0.01
Y 坐标 = (V-YOFFSET)*YMUL*0.01
*/
extern BYTE SP3_SendCurveZoomCmdProc(pCURVE_DEF pCurveInfo, u8 nEnWait)
{
    //  BYTE nChCount = 0;
    BYTE		   nCount	= 0;
    volatile BYTE *pBuf		= g_arrUart3SendBuf;	//指向发送缓冲区
    u32			   nTimeOut = 0;

    LIMIT(0, pCurveInfo->nCh, 7);

    // EE B1
    // 32/35 DispType
    // 00 0B 页id
    // 00 0E 控件id
    // 00    通道值0-7
    // 00 02 数值长度
    // 00 64 数值
    // FF FC FF FF
    // Head
    *(pBuf + nCount++) = 0xEE;

    // Cmd
    *(pBuf + nCount++) = 0xB1;

    //缩放
    *(pBuf + nCount++) = 0x34;

    //画面ID 高字节在前,低字节在后
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nPageID >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nPageID);

    //控件ID 高字节在前,低字节在后
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nCtrlID >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->nCtrlID);

    //水平偏移
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->stZoomInfo.nXOffect >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->stZoomInfo.nXOffect);
    //水平缩放比例
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->stZoomInfo.nXMUL >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->stZoomInfo.nXMUL);

    //垂直偏移
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->stZoomInfo.nYOffect >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->stZoomInfo.nYOffect);
    //垂直缩放比例
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->stZoomInfo.nYMUL >> 8);
    *(pBuf + nCount++) = (BYTE)(pCurveInfo->stZoomInfo.nYMUL);

    // Lpc
    //*pBuf = nLpc;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFC;
    *(pBuf + nCount++) = 0xFF;
    *(pBuf + nCount++) = 0xFF;

    SP3_SendBytesInfoProc((u8 *)&g_arrUart3SendBuf, nCount);

    //等待发送结束
    while((!SP3_GetSysSendStatusProc()) && (nEnWait == 1) && (nTimeOut++ < 60000))
        ;

    return SUCESS_COMM;
}
