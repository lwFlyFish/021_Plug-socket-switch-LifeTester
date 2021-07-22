///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

#include "SysConfig.h"
#include "RealDatabase.h"
#include "SysUart1.h"
#include "SysUart1ProtocolProc.h"
#include "SysUart1CommProc.h"

///////////////////////////////////////////////////////////////////////////
// ȫ�ֱ�����ʼ��
///////////////////////////////////////////////////////////////////////////

//���ջ������ǿձ�־  �� = 0x01 �� = 0x00
static volatile u8 m_nSP1InBufSign = 0x00;


///////////////////////////////////////////////////////////////////////////
//Function Name: void SP1_SysCommProc(void)
//Function:      ͨ�ſ���
//Describtion:   
//Param:         ��
//return:        ��
//example:      
// 5S��ʱ�ѻ�
// �ѻ���1S�������
// ���10msͨ�����ݼ��
// ���50ms���½��
///////////////////////////////////////////////////////////////////////////
void SP1_SysCommProc(void)
{
	u16 nResult = 0;
   
    //���ݽ��ղ�����
	if (g_nSysCommTimeoutCtrl_SP1 > 0)
	{
		g_nSysCommTimeoutCtrl_SP1--;
	}
	
	//����1-ͨ�Ź��̵ȴ���ʱ
	if (g_nSysCommDelayTime_SP1 > 0)
	{
		g_nSysCommDelayTime_SP1--;
	}
	
	//����1-�ѻ�����״̬���
	if (g_nSysCommStatusCheck_SP1 > 0)
	{
		g_nSysCommStatusCheck_SP1--;
	}
	//�ѻ��󴮿ڳ�ʼ��
    if (g_nSysInitComm1Timer > 0)
	{
		g_nSysInitComm1Timer--;
	}
    
    //�õ���ǰ���յ����� - ���2ms�ж�1��
	/*/�������� DMA1_Channel5->CNDTR �ݼ�
	//if (DMA1_Channel5->CNDTR > UART1_RECVDATA_LEN)
	{
		SP1_CommStatusInitProc();
		return;
	}
	//���ݸ���
	if (DMA1_Channel5->CNDTR < 5)
	{
		SP1_CommStatusInitProc();
		return;
	}*/
	
	//�õ���ǰʵ�ʽ��յ��ֽ���
	m_nSP1RecvDataCount = UART1_GetRecvDataLen();
	
	//�ж��Ƕ���������
	if (m_nSP1RecvDataCount > 0)
	{
		//������
		m_nSP1InBufSign = 0x01;
	}
	else
	{	
       //������
		m_nSP1InBufSign = 0x00;
	}
	
	//���������ж��ն˵�ǰͨ��״̬-����Ƿ��ѻ�
	if (m_nSP1InBufSign == 0x00)
	{
		//�ж��ѻ�״̬
		switch (g_nSysCommStatusStep_SP1)
		{
		case 0:
      //5000ms��ʱ
			g_nSysCommStatusCheck_SP1 = 500;
			g_nSysCommStatusStep_SP1 = 1;
			break;
		
		case 1:
			//
			if (g_nSysCommStatusCheck_SP1 == 0)
			{
				//������, ����Ϊ�ѻ�״̬
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

	//ͨ��״̬���Ĵ�������
	g_nSysCommStatusCheck_SP1 = 0;
	g_nSysCommStatusStep_SP1 = 0;
	//ϵͳͨ��״̬ ����
	g_nDevCommStatus_SP1 = DEVONLINE;
	
	//���ݽ��ղ�����-�������
	if (g_nSysCommTimeoutTag_SP1 == 1)
	{
		//50ms��ʱ
		if (g_nSysCommTimeoutCtrl_SP1 == 0)
		{
			SP1_CommStatusInitProc();
			return;
		}
	}
	
	//10ms����1��
	if (g_nSysCommDelayTime_SP1 == 0)
	{
		//�������ݹ���
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
		
		// ���ݽ��ղ�����-�������
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
//Function:      ��ָ��λ��ȡ������Ϣ
//Describtion:   
//Param:         ��
//return:        ��
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
//ͨ��״̬��ʼ��
void SP1_CommStatusInitProc(void)
{
	//����״̬��ʼ��
	m_nSP1InBufSign = 0x00;

	m_nSP1RecvDataCount = 0x00;

	g_nSysCommDelayTime_SP1 = 0;

	//ͨ�ų�ʱ�Ĵ���
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










