///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

#include "SysConfig.h"
#include "SysUart3CommProc.h"

#include "SysUART3.h"

#include "RealDatabase.h"
#include "SysUart3ProtocolProc.h"
///////////////////////////////////////////////////////////////////////////
// 全局变量初始化
///////////////////////////////////////////////////////////////////////////

//接收缓冲区非空标志  有 = 0x01 无 = 0x00
static volatile BYTE m_nSP3InBufSign = 0x00;
//接收队列有效数据计数器
static volatile UINT16 m_nSP3RecvDataCount = 0;

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
//Function Name: void SP3_SysCommProc(void)
//Function:      通信控制
//Describtion:
//Param:         无
//return:        无
//example:
///////////////////////////////////////////////////////////////////////////
// CmdProc中收长度时改为分开判断
// 2S超时脱机
// 脱机后500mS间隔重连
// 间隔20ms通信数据检测
// 间隔50ms重新解包
///////////////////////////////////////////////////////////////////////////
void SP3_SysCommProc(void)
{
    UINT16 nResult = 0;
    //  u8 nCount = 0;

    //数据接收不完整
    if(g_nSysCommTimeoutCtrl_SP3 > 0)
    {
        g_nSysCommTimeoutCtrl_SP3--;
    }

    //串口1-通信过程等待延时
    if(g_nSysCommDelayTime_SP3 > 0)
    {
        g_nSysCommDelayTime_SP3--;
    }

    //串口1-脱机联机状态检查
    if(g_nSysCommStatusCheck_SP3 > 0)
    {
        g_nSysCommStatusCheck_SP3--;
    }
    //脱机后串口初始化
    if(g_nSysInitComm2Timer > 0)
    {
        g_nSysInitComm2Timer--;
    }

    //得到当前接收的数量 - 间隔2ms判断1次
    if(SP3_GetRecvDataCountProc() > UART3_RECVDATA_LEN)
    {
        SP3_CommStatusInitProc();
        return;
    }

    //得到实际接收的字节数
    m_nSP3RecvDataCount = SP3_GetRecvDataCountProc();//UART3_RECVDATA_LEN - DMA1_Channel6->CNDTR);

    //判断是否有数据来
    if(m_nSP3RecvDataCount > 0)
    {
        //有数据
        m_nSP3InBufSign = 0x01;
    }
    else
    {
        //无数据
        m_nSP3InBufSign = 0x00;
    }

    //无数据则判断终端当前通信状态-检测是否脱机
    if(m_nSP3InBufSign == 0x00)
    {
        //判断脱机状态
        switch(g_nSysCommStatusStep_SP3)
        {
        case 0:
            //2000ms延时
            g_nSysCommStatusCheck_SP3 = 200;
            g_nSysCommStatusStep_SP3 = 1;
            break;

        case 1:
            //
            if(g_nSysCommStatusCheck_SP3 == 0)
            {
                //无数据, 则置为脱机状态
                g_nDevCommStatus_SP3 = DEVOFFLINE;
                g_nSysCommStatusStep_SP3 = 2;
                g_nSysInitComm2Timer = 50;
                SP3_CommStatusInitProc();
            }
            break;

        case 2:
            //
            if(g_nSysInitComm2Timer == 0)
            {
                //1s
                SP3_CommStatusInitProc();
                g_nSysInitComm2Timer = 50;
                g_bRecvStatus_SP3 = FALSE;
            }
            break;
        }

        return;
    }

    //通信状态检查寄存器清零
    g_nSysCommStatusCheck_SP3 = 0;
    g_nSysCommStatusStep_SP3 = 0;
    //系统通信状态 联机
    g_nDevCommStatus_SP3 = DEVONLINE;

    //系统超时判断
    if(g_nSysCommTimeoutTag_SP3 == 1)
    {
        //500ms超时 10ms定时
        if(g_nSysCommTimeoutCtrl_SP3 == 0)
        {
            //超时
            SP3_CommStatusInitProc();
            return;
        }
    }

    //20ms接收1次
    if(g_nSysCommDelayTime_SP3 == 0)
    {
        nResult = SP3_RecvProc();
    }
    else
    {
        return;
    }

    //
    switch(nResult)
    {
    //Continue
    case SYNSTEP:
    case CMDSTEP:
    case DATASTEP:
        //20ms
        g_nSysCommDelayTime_SP3 = 1;

        // 数据接收不完整-解包出错
        if(g_nSysCommTimeoutTag_SP3 == 0)
        {
            g_nSysCommTimeoutTag_SP3 = 1;
            g_nSysCommTimeoutCtrl_SP3 = 20;
        }
        break;

    //Sucess
    case SUCESS_COMM:
        //
        SP3_CommStatusInitProc();
        break;

    //Failure
    case FAILURE:
        //
        SP3_CommStatusInitProc();
        break;
    }
}

//


//
BYTE SP3_GetSysSendStatusProc(void)
{
    return g_nSendStatus_SP3;
}

///////////////////////////////////////////////////////////////////////////
//Function Name: int16 SP3_GetComMsgFromPos(UINT16 nCurPos)
//Function:      从指定位置取队列信息
//Describtion:
//Param:         无
//return:        无
//example:
///////////////////////////////////////////////////////////////////////////
int16 SP3_GetComMsgFromPos(UINT16 nCurPos)
{
    int16 nResult = -1;

    //
    if(nCurPos >= m_nSP3RecvDataCount)
    {
        return nResult;
    }

    nResult = 0x00;
    nResult = g_arrUart3RecvBuf[nCurPos];
    //nResult &= 0x00FF;

    return nResult;
}

///////////////////////////////////////////////////////////////////////////
//通信状态初始化
void SP3_CommStatusInitProc(void)
{
    //接收状态初始化
    m_nSP3InBufSign = 0x00;
    //
    m_nSP3RecvDataCount = 0x00;

    //
    g_nSysCommDelayTime_SP3 = 0;

    //通信超时寄存器
    g_nSysCommTimeoutTag_SP3 = 0;
    g_nSysCommTimeoutCtrl_SP3 = 0;

    m_nCurStep_SP3 = SYNSTEP;
	memset((char *)g_arrUart3RecvBuf, 0, UART3_RECVDATA_LEN);
    UART3_ClcRecvDataLen();

}



