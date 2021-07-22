///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

#include "SysConfig.h"
#include "RealDatabase.h"
#include "SysUart1.h"
#include "SysUart1ProtocolProc.h"
#include "SysUart1CommProc.h"

///////////////////////////////////////////////////////////////////////////
// 全局变量初始化
///////////////////////////////////////////////////////////////////////////

//接收缓冲区非空标志  有 = 0x01 无 = 0x00
static volatile u8 m_nSP1InBufSign = 0x00;


///////////////////////////////////////////////////////////////////////////
//Function Name: void SP1_SysCommProc(void)
//Function:      通信控制
//Describtion:   
//Param:         无
//return:        无
//example:      
// 5S超时脱机
// 脱机后1S间隔重连
// 间隔10ms通信数据检测
// 间隔50ms重新解包
///////////////////////////////////////////////////////////////////////////
void SP1_SysCommProc(void)
{
	u16 nResult = 0;
   
    //数据接收不完整
	if (g_nSysCommTimeoutCtrl_SP1 > 0)
	{
		g_nSysCommTimeoutCtrl_SP1--;
	}
	
	//串口1-通信过程等待延时
	if (g_nSysCommDelayTime_SP1 > 0)
	{
		g_nSysCommDelayTime_SP1--;
	}
	
	//串口1-脱机联机状态检查
	if (g_nSysCommStatusCheck_SP1 > 0)
	{
		g_nSysCommStatusCheck_SP1--;
	}
	//脱机后串口初始化
    if (g_nSysInitComm1Timer > 0)
	{
		g_nSysInitComm1Timer--;
	}
    
    //得到当前接收的数量 - 间隔2ms判断1次
	/*/有数据则 DMA1_Channel5->CNDTR 递减
	//if (DMA1_Channel5->CNDTR > UART1_RECVDATA_LEN)
	{
		SP1_CommStatusInitProc();
		return;
	}
	//数据干扰
	if (DMA1_Channel5->CNDTR < 5)
	{
		SP1_CommStatusInitProc();
		return;
	}*/
	
	//得到当前实际接收的字节数
	m_nSP1RecvDataCount = UART1_GetRecvDataLen();
	
	//判断是都有数据来
	if (m_nSP1RecvDataCount > 0)
	{
		//有数据
		m_nSP1InBufSign = 0x01;
	}
	else
	{	
       //无数据
		m_nSP1InBufSign = 0x00;
	}
	
	//无数据则判断终端当前通信状态-检测是否脱机
	if (m_nSP1InBufSign == 0x00)
	{
		//判断脱机状态
		switch (g_nSysCommStatusStep_SP1)
		{
		case 0:
      //5000ms延时
			g_nSysCommStatusCheck_SP1 = 500;
			g_nSysCommStatusStep_SP1 = 1;
			break;
		
		case 1:
			//
			if (g_nSysCommStatusCheck_SP1 == 0)
			{
				//无数据, 则置为脱机状态
				g_nDevCommStatus_SP1 = DEVOFFLINE;
				g_nSysCommStatusStep_SP1 = 2;
                g_nSysInitComm1Timer = 1000;
				SP1_CommStatusInitProc();
			}
			break;
		
		case 2:
			//
			if (g_nSysInitComm1Timer == 0)
			{
				//1s
				SP1_CommStatusInitProc();
                g_nSysInitComm1Timer = 100;
                g_bRecvStatus_SP1 = FALSE;
			}
			break;
		}
		
		return;
	}

	//通信状态检查寄存器清零
	g_nSysCommStatusCheck_SP1 = 0;
	g_nSysCommStatusStep_SP1 = 0;
	//系统通信状态 联机
	g_nDevCommStatus_SP1 = DEVONLINE;
	
	//数据接收不完整-解包出错
	if (g_nSysCommTimeoutTag_SP1 == 1)
	{
		//50ms超时
		if (g_nSysCommTimeoutCtrl_SP1 == 0)
		{
			SP1_CommStatusInitProc();
			return;
		}
	}
	
	//10ms接收1次
	if (g_nSysCommDelayTime_SP1 == 0)
	{
		//接收数据过程
		nResult = SP1_RecvProc();
	}
	else
	{
		return;
	}

	//
	switch (nResult)
	{
	//Continue
	case SYNSTEP:
	case CMDSTEP:
	case DATASTEP:
		//10ms
		g_nSysCommDelayTime_SP1 = 1;
		
		// 数据接收不完整-解包出错
		if (g_nSysCommTimeoutTag_SP1 == 0)
		{
			g_nSysCommTimeoutTag_SP1 = 1;
			g_nSysCommTimeoutCtrl_SP1 = 50;
		}
		break;
	//Sucess
	case SUCESS_COMM :
		SP1_CommStatusInitProc();
	break;

	//Failure
	case FAILURE:
		SP1_CommStatusInitProc();
	break;
	}
}

///////////////////////////////////////////////////////////////////////////
//Function Name: s16 SP1_GetComMsgFromPos(u16 nCurPos)
//Function:      从指定位置取队列信息
//Describtion:   
//Param:         无
//return:        无
//example:       
///////////////////////////////////////////////////////////////////////////
s16 SP1_GetComMsgFromPos(u16 nCurPos)
{
	s16 nResult = -1;

	//if (nCurPos >= m_nSP1RecvDataCount)
	if(nCurPos >= UART1_GetRecvDataLen())
	{
		return nResult;
	}

	nResult = 0;
	nResult = g_arrUart1RecvBuf[nCurPos];
	
    return nResult;
}

///////////////////////////////////////////////////////////////////////////
//通信状态初始化
void SP1_CommStatusInitProc(void)
{
	//接收状态初始化
	m_nSP1InBufSign = 0x00;

	m_nSP1RecvDataCount = 0x00;

	g_nSysCommDelayTime_SP1 = 0;

	//通信超时寄存器
	g_nSysCommTimeoutTag_SP1 = 0;
	g_nSysCommTimeoutCtrl_SP1 = 0;

	//
	SP1_SetSysCommStepProc();

	//memset((char*)g_arrUart1RecvBuf,0,UART1_RECVDATA_LEN);

	UART1_ClcRecvDataLen();

	g_nSP1RecvDelay = 0;
	g_nSP1CommStep = 0;
	g_bRecvStatus_SP1 = FALSE;
}

///////////////////////////////////////////////////////////////////////////










