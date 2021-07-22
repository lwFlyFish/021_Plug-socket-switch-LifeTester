///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

#include "SysConfig.h"
#include "SysUart1.h"

#include "RealDatabase.h"
#include "SysUart1CommProc.h"
#include "SysUart1ProtocolProc.h"

///////////////////////////////////////////////////////////////////////////
//终端地址

s16 g_nAddr_SP1 = 0;

static u8 m_nCurStep_SP1 = SYNSTEP;
static s16 m_nCmd_SP1 = 0;
s16 m_nLen_SP1 = 0;
///////////////////////////////////////////////////////////////////////////
//
void SP1_SetSysCommStepProc(void)
{
    m_nCurStep_SP1 = SYNSTEP;
}

u8 SP1_RecvProc(void)
{
    u8 nRetValue = SYNSTEP;
    switch(m_nCurStep_SP1)
    {
    //同步头
    case SYNSTEP:
        //
        m_nCurStep_SP1 = Syn1Proc();
        nRetValue = m_nCurStep_SP1;
        if(m_nCurStep_SP1 == FAILURE || m_nCurStep_SP1 == SUCESS_COMM)
        {
            m_nCurStep_SP1 = SYNSTEP;
            return nRetValue;
        }

        //加快通信效率, 直接进入命令收取
        if(m_nCurStep_SP1 != CMDSTEP)
        { break; }

    //命令字
    case CMDSTEP:
        //
        m_nCurStep_SP1 = Cmd1Proc();
        nRetValue = m_nCurStep_SP1;
        if(m_nCurStep_SP1 == FAILURE || m_nCurStep_SP1 == SUCESS_COMM)
        {
            m_nCurStep_SP1 = SYNSTEP;
            return nRetValue;
        }

        //加快通信效率, 直接进入数据收取
        if(m_nCurStep_SP1 != DATASTEP)
        { break; }

    //数据包
    case DATASTEP:
        //
        m_nCurStep_SP1 = Data1Proc();
        nRetValue = m_nCurStep_SP1;
        if(m_nCurStep_SP1 == FAILURE || m_nCurStep_SP1 == SUCESS_COMM)
        {
            m_nCurStep_SP1 = SYNSTEP;
            return nRetValue;
        }
        break;

    //
    default:
        m_nCurStep_SP1 = SYNSTEP;
        break;
    }
    return m_nCurStep_SP1;
}

///////////////////////////////////////////////////////////////////////////
//同步头
static u8 Syn1Proc(void)
{
    u8 arrSynTab[] = {0xED, 0xED, 0xF0}; //
    u8 i = 0;
    s16 nTemp;

    for(i = 0; i < 3; i++)
    {
        nTemp = SP1_GetComMsgFromPos(i);
        if(nTemp == -1)
        {
            return SYNSTEP;
        }

        if(nTemp == arrSynTab[i])
        {

            continue;
        }
        else
        {
            //上1次是因为地址错误返回的
            if(g_nSysAddrError_SP1 == 1)
            {
                if(i > 1)
                { return CMDSTEP; }
                else
                { return SUCESS_COMM ; }
            }
            else
            {
                //同步头不对重新接收
                return SUCESS_COMM ;
            }
        }
    }

    return CMDSTEP;
}

//命令接收
static u8 Cmd1Proc(void)
{
    s16 nFirstValue = 0;
    s16 nSecondValue = 0;
    //Addr
    g_nAddr_SP1 = SP1_GetComMsgFromPos(3);////
    if(g_nAddr_SP1 == -1)
    {
        //printf_SoftUart("Addr Error!\r\n");
        return CMDSTEP;
    }


    if(g_nAddr_SP1 != 0xFF)
    {
        //地址不匹配
        if(g_nAddr_SP1 != g_stConfigParam.nSysSlaveCommAddr)
        {
            g_nSysAddrError_SP1 = 1;
            //printf_SoftUart("Addr1 Error!\r\n");
            return SUCESS_COMM;
        }
        else
        {
            //地址匹配
            g_nSysAddrError_SP1 = 0;
        }
    }
    else
    {
        //地址匹配
        g_nSysAddrError_SP1 = 0;
    }

    //Cmd//-----功能字
    m_nCmd_SP1 = SP1_GetComMsgFromPos(4);
    if(m_nCmd_SP1 == -1)
    {
        //printf_SoftUart("Cmd Error!\r\n");
        return CMDSTEP;
    }

    //Data Len//-----数据长度
    nFirstValue = SP1_GetComMsgFromPos(5);
    if(nFirstValue == -1)
    {
        //printf_SoftUart("Len1 Error!\r\n");
        return CMDSTEP;
    }

    nSecondValue = SP1_GetComMsgFromPos(6);
    if(nSecondValue == -1)
    {
        //printf_SoftUart("Len2 Error!\r\n");
        return CMDSTEP;
    }
    m_nLen_SP1 = ConfigMulitBytesToValueProc(nFirstValue, nSecondValue, 0, 0);
    if(m_nLen_SP1 == -1)
    {
        //printf_SoftUart("CMDSTEP Error!\r\n");
        return CMDSTEP;
    }

    //超界
    if(m_nLen_SP1 >= UART1_RECVDATA_LEN)
    {
        //printf_SoftUart("Break Bounds Error!\r\n");
        return FAILURE;
    }

    return DATASTEP;
}

//数据接收
volatile u8 nLpc = 0x00, nLow = 0, nHih = 0;
volatile u16 i = 0x00;
static u8 Data1Proc(void)
{
    u8 nIUType = 0;
    u8 nReturnValue = FAILURE;
    //u8 nLpc = 0x00, nLow = 0, nHih = 0;
    s16 nTemp = 0;
    //u16 i = 0x00;
    //  u8 nData = 0;
    //  u8 nType = 0;
    //  u8 nIndex = 0;

    //计算地址 命令字 数据长度的校验码
    nLow = (BYTE)m_nLen_SP1;
    nHih = (BYTE)(m_nLen_SP1 >> 8);
    nLpc = (BYTE)((BYTE)g_nAddr_SP1 ^ (BYTE)m_nCmd_SP1 ^ nLow ^ nHih);

    for(i = 0; i < m_nLen_SP1; i++)
    {
        nTemp = g_arrUart1RecvBuf[i + 7]; // SP1_GetComMsgFromPos(i+7);
        if(nTemp == -1)
        {
            //printf_SoftUart("DATASTEP Error!\r\n");
            return DATASTEP;
        }
        if(i < m_nLen_SP1)
        {
            nLpc ^= nTemp;
        }
    }

    //判断校验和
    if(nLpc != g_arrUart1RecvBuf[i + 7])
    {
        //printf_SoftUart("nLpc Error!\r\n");
        //校验和错误
        return FAILURE;
    }

    //处理命令
    switch(m_nCmd_SP1)
    {
    //实时查询数据
    case REAL_CMD:
        nReturnValue = RequestRealParam();
        break;

    //起动试验
    case START_CMD:
        nReturnValue = HostStartCmdProc(SP1_GetComMsgFromPos(7));
        break;

    //停止实验
    case STOP_CMD:
        nReturnValue = HostStopCmdProc();
        break;

    //自整定PID
    case AUTO_PID_CMD:
        nReturnValue = HostAutoPidCmd((u8 *)(g_arrUart1RecvBuf + 7));
        break;

    //设置PID参数
    case SET_PID_CMD:
        nReturnValue = HostSetPidParamProc((u8 *)(g_arrUart1RecvBuf + 7));
        break;

    //设置系统参数
    case SET_SYSPARAM_CMD:
        nReturnValue = HostSetSysParam((u8 *)(g_arrUart1RecvBuf + 7));
        break;

    //读取系统参数
    case READ_SYSPARAM_CMD:
        nReturnValue = HostReadSysParam();
        break;

    //清零电压电流
    case RESET_SENSOR_CMD:
        nReturnValue = HostResetSensorProc(SP1_GetComMsgFromPos(7));
        break;

    //设置目标值
    case SET_IUBD_PARAM_CMD:
        nIUType = *(g_arrUart1RecvBuf + 7);
        if(nIUType == 0)
        {
            //设置电流
            nReturnValue = HostSetIbdParamProc((u8 *)(g_arrUart1RecvBuf + 8));
        }
        else if(nIUType == 1)
        {
            //设置电压
            nReturnValue = HostSetUbdParamProc((u8 *)(g_arrUart1RecvBuf + 8));
        }
        break;

    //标定电压电流
    case BD_IU_CMD:
        nIUType = *(g_arrUart1RecvBuf + 7);
        if(nIUType == 0)
        {
            //标电流
            nReturnValue = HostBDCurrentProc(SP1_GetComMsgFromPos(9), 1);
        }
        else if(nIUType == 1)
        {
            //标电压
            nReturnValue = HostBDVoltageProc(SP1_GetComMsgFromPos(9), 1);
        }
        break;

    //读取电压电流占空比信息
    case READ_DEBUG_INFO_CMD:
        nReturnValue = HostReadDebugInfoCmdProc();
        break;

    //命令错误
    default:
        SlaveAckHostSetCmd(0xFF, 0xFF);
        break;
    }

    return nReturnValue;
}

///////////////////////////////////////////////////////////////////////////

/*
实时查询数据
*/
static u8 RequestRealParam(void)
{
    u8 nPackStatus = 0;
    u8 nReturn = FAILURE;

    if(g_nPowerRMSFlag == 1)
    {
        nPackStatus = 0x04;
    }

    switch(nPackStatus)
    {
    //正常数据包
    case 0:
        nReturn = HostRequestRealParam(nPackStatus);
        //发送后清除
        g_nPowerRMSFlag = 0;
        break;

    case 0x04:
        nReturn = HostRequestRealParam(nPackStatus);
        //printf_SoftUart("PowerRMS!\r\n");
        //发送后清除
        g_nPowerRMSFlag = 0;
        break;

    default:
        nReturn = HostRequestRealParam(nPackStatus);
        break;
    }

    return nReturn;
}



















