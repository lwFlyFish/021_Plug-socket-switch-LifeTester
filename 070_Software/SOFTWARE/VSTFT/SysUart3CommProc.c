///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

#include "SysConfig.h"
#include "SysUart3CommProc.h"

#include "SysUART3.h"

#include "RealDatabase.h"
#include "SysUart3ProtocolProc.h"
///////////////////////////////////////////////////////////////////////////
// ȫ�ֱ�����ʼ��
///////////////////////////////////////////////////////////////////////////

//���ջ������ǿձ�־  �� = 0x01 �� = 0x00
static volatile BYTE m_nSP3InBufSign = 0x00;
//���ն�����Ч���ݼ�����
static volatile UINT16 m_nSP3RecvDataCount = 0;

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
//Function Name: void SP3_SysCommProc(void)
//Function:      ͨ�ſ���
//Describtion:
//Param:         ��
//return:        ��
//example:
///////////////////////////////////////////////////////////////////////////
// CmdProc���ճ���ʱ��Ϊ�ֿ��ж�
// 2S��ʱ�ѻ�
// �ѻ���500mS�������
// ���20msͨ�����ݼ��
// ���50ms���½��
///////////////////////////////////////////////////////////////////////////
void SP3_SysCommProc(void)
{
    UINT16 nResult = 0;
    //  u8 nCount = 0;

    //���ݽ��ղ�����
    if(g_nSysCommTimeoutCtrl_SP3 > 0)
    {
        g_nSysCommTimeoutCtrl_SP3--;
    }

    //����1-ͨ�Ź��̵ȴ���ʱ
    if(g_nSysCommDelayTime_SP3 > 0)
    {
        g_nSysCommDelayTime_SP3--;
    }

    //����1-�ѻ�����״̬���
    if(g_nSysCommStatusCheck_SP3 > 0)
    {
        g_nSysCommStatusCheck_SP3--;
    }
    //�ѻ��󴮿ڳ�ʼ��
    if(g_nSysInitComm2Timer > 0)
    {
        g_nSysInitComm2Timer--;
    }

    //�õ���ǰ���յ����� - ���2ms�ж�1��
    if(SP3_GetRecvDataCountProc() > UART3_RECVDATA_LEN)
    {
        SP3_CommStatusInitProc();
        return;
    }

    //�õ�ʵ�ʽ��յ��ֽ���
    m_nSP3RecvDataCount = SP3_GetRecvDataCountProc();//UART3_RECVDATA_LEN - DMA1_Channel6->CNDTR);

    //�ж��Ƿ���������
    if(m_nSP3RecvDataCount > 0)
    {
        //������
        m_nSP3InBufSign = 0x01;
    }
    else
    {
        //������
        m_nSP3InBufSign = 0x00;
    }

    //���������ж��ն˵�ǰͨ��״̬-����Ƿ��ѻ�
    if(m_nSP3InBufSign == 0x00)
    {
        //�ж��ѻ�״̬
        switch(g_nSysCommStatusStep_SP3)
        {
        case 0:
            //2000ms��ʱ
            g_nSysCommStatusCheck_SP3 = 200;
            g_nSysCommStatusStep_SP3 = 1;
            break;

        case 1:
            //
            if(g_nSysCommStatusCheck_SP3 == 0)
            {
                //������, ����Ϊ�ѻ�״̬
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

    //ͨ��״̬���Ĵ�������
    g_nSysCommStatusCheck_SP3 = 0;
    g_nSysCommStatusStep_SP3 = 0;
    //ϵͳͨ��״̬ ����
    g_nDevCommStatus_SP3 = DEVONLINE;

    //ϵͳ��ʱ�ж�
    if(g_nSysCommTimeoutTag_SP3 == 1)
    {
        //500ms��ʱ 10ms��ʱ
        if(g_nSysCommTimeoutCtrl_SP3 == 0)
        {
            //��ʱ
            SP3_CommStatusInitProc();
            return;
        }
    }

    //20ms����1��
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

        // ���ݽ��ղ�����-�������
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
//Function:      ��ָ��λ��ȡ������Ϣ
//Describtion:
//Param:         ��
//return:        ��
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
//ͨ��״̬��ʼ��
void SP3_CommStatusInitProc(void)
{
    //����״̬��ʼ��
    m_nSP3InBufSign = 0x00;
    //
    m_nSP3RecvDataCount = 0x00;

    //
    g_nSysCommDelayTime_SP3 = 0;

    //ͨ�ų�ʱ�Ĵ���
    g_nSysCommTimeoutTag_SP3 = 0;
    g_nSysCommTimeoutCtrl_SP3 = 0;

    m_nCurStep_SP3 = SYNSTEP;
	memset((char *)g_arrUart3RecvBuf, 0, UART3_RECVDATA_LEN);
    UART3_ClcRecvDataLen();

}



