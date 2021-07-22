///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

#include "RealDataBase.h"
#include "stdio.h"

#include "SysUart3.h"
///////////////////////////////////////////////////////////////////////////

u8 g_arrUart3SendBuf[UART3_SENDDATA_LEN];
u8 g_arrUart3RecvBuf[UART3_RECVDATA_LEN];

volatile BYTE g_nSendStatus_SP3 = 0;

///////////////////////////////////////////////////////////////////////////
void SysUart3InitProc(u32 nBound)
{
    //GPIO端口设置
    GPIO_InitType GPIO_InitStructure;
    USART_InitType UART_InitStructure;

    #if(FULL_RAMP_UART3_FOR232 == 0)
    //使能UART3
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_USART3, ENABLE);

    //使能UART3, GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOD, ENABLE);

    //映射
    GPIO_PinsRemapConfig(GPIO_FullRemap_USART3, ENABLE);

    //UART3_TX   GPIOD.8
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_8;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    //复用推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    //初始化
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //UART3_RX GPIOD.9
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_9;
    //浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    //初始化
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    #else
    //使能UART3
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_USART3, ENABLE);

    //使能UART3, GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE);

    //UART3_TX   GPIOB.10
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_10;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    //复用推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    //初始化
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //UART3_RX GPIOB.11
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_11;
    //浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    //初始化
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    #endif

    //UART 初始化设置
    //串口波特率
    UART_InitStructure.USART_BaudRate = nBound;
    //字长为8位数据格式
    UART_InitStructure.USART_WordLength = USART_WordLength_8b;
    //一个停止位
    UART_InitStructure.USART_StopBits = USART_StopBits_1;
    //无奇偶校验位
    UART_InitStructure.USART_Parity = USART_Parity_No;
    //无硬件数据流控制
    UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    //收发模式
    UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    //初始化串口3
    USART_Init(USART3, &UART_InitStructure);
    //UART3 DMA Enable
    USART_DMACmd(USART3, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);
    //开启串口接受中断
    //UART_ITConfig(UART2, UART_IT_RXIEN, ENABLE);
    //使能串口3
    USART_Cmd(USART3, ENABLE);

    Uart3DMAInitProc();
}

//发送单字节
void SP3_SendByteInfoProc(u8 nSendInfo)
{
    volatile BYTE *pBuf = g_arrUart3SendBuf;

    //指向发送缓冲区
    //pBuf = g_arrUART2SendBuff;
    g_nSendStatus_SP3 = 0;
    //
    *pBuf++ = nSendInfo;

    //DMA发送方式
    SP3_DmaSendDataProc(1);
}

void SP3_SendBytesInfoProc(u8 *pSendInfo, u16 nSendCount)
{
    u16 i = 0;
    volatile BYTE *pBuf = g_arrUart3SendBuf;

    //指向发送缓冲区
    //pBuf = g_arrUART2SendBuff;
    g_nSendStatus_SP3 = 0;

    for(i = 0; i < nSendCount; i++)
    {
        //
        *pBuf++ = pSendInfo[i];
    }

    //DMA发送方式
    SP3_DmaSendDataProc(nSendCount);
}

void SP3_DmaSendDataProc(u16 nSendLen)
{
    //清DMA发送完成标志
    DMA_ClearFlag(DMA1_FLAG_GL2 | DMA1_FLAG_TC2 | DMA1_FLAG_HT2 | DMA1_FLAG_ERR2);

    //停止DMA
    DMA_ChannelEnable(DMA1_Channel2, DISABLE);

    // DMAy Channelx CNDTR Configuration
    // Write to DMAy Channelx CNDTR
    DMA1_Channel2->TCNT = (u32)nSendLen;
    // DMAy Channelx CPAR Configuration
    // Write to DMAy Channelx CPAR
    // DMA1_Channel7->CPAR = (u32) &(UART2->TDR);
    // DMAy Channelx CMAR Configuration
    // Write to DMAy Channelx CMAR
    DMA1_Channel2->CMBA = (u32)g_arrUart3SendBuf;

    g_nSendStatus_SP3 = 0;

    //启动DMA
    DMA_ChannelEnable(DMA1_Channel2, ENABLE);
}


u16 SP3_GetRecvDataCountProc(void)
{
    return (UART3_RECVDATA_LEN - DMA1_Channel3->TCNT);
}

extern void UART3_ClcRecvDataLen(void)
{
    //停止DMA
    DMA_ChannelEnable(DMA1_Channel3, DISABLE);

    //配置DMA传输发送数据量
    DMA1_Channel3->TCNT = UART3_RECVDATA_LEN;
    /* Write to DMAy Channelx CMAR */
    DMA1_Channel3->CMBA = (u32)g_arrUart3RecvBuf;

    //启动DMA
    DMA_ChannelEnable(DMA1_Channel3, ENABLE);
}


static void Uart3DMAInitProc(void)
{
    //
    DMA_InitType DMA_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_DMA1, ENABLE);

    //DMA3 UART3 Receive
    DMA_Reset(DMA1_Channel3);

    //
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) & (USART3->DT);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)g_arrUart3RecvBuf;
    DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALSRC;
    DMA_InitStructure.DMA_BufferSize = UART3_RECVDATA_LEN;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
    DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
    DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;
    DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
    //
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
    DMA_INTConfig(DMA1_Channel3, DMA_INT_TC, ENABLE);

    //Enable DAM13
    DMA_ChannelEnable(DMA1_Channel3, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //DMA2 UART3 Send
    DMA_Reset(DMA1_Channel2);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) & (USART3->DT);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)g_arrUart3SendBuf;
    //数据传输方向, 从内存读取发送到外设
    DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALDST;
    DMA_InitStructure.DMA_BufferSize = UART3_SENDDATA_LEN;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
    DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
    DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;
    //DMA_MODE_CIRCULAR;
    DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
    //
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);

    DMA_INTConfig(DMA1_Channel2, DMA_INT_TC, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************
* Function Name  : DMAChannel6_IRQHandler
* Description    : This function handles DMA Stream 6 interrupt request.
* Input          : None
* Output         : None
* Return         : None
**************************************************************************/
void DMA1_Channel3_IRQHandler(void)
{
    //uart3 接收
    if(DMA_GetITStatus(DMA1_INT_TC3))
    {
        DMA_ClearITPendingBit(DMA1_FLAG_GL3 | DMA1_FLAG_TC3 | DMA1_FLAG_HT3 | DMA1_FLAG_ERR3);
        //停止DMA
        DMA_ChannelEnable(DMA1_Channel3, DISABLE);

        //配置DMA传输发送数据量
        DMA1_Channel3->TCNT = UART3_RECVDATA_LEN;
        /* Write to DMAy Channelx CMAR */
        DMA1_Channel3->CMBA = (u32)g_arrUart3RecvBuf;

        //启动DMA
        DMA_ChannelEnable(DMA1_Channel3, ENABLE);
    }

    //
    //g_nSys485CommStatusShift_SP1 = 0;
}

/**************************************************************************
* Function Name  : DMAChannel7_IRQHandler
* Description    : This function handles DMA Stream 7 interrupt request.
* Input          : None
* Output         : None
* Return         : None
**************************************************************************/
void DMA1_Channel2_IRQHandler(void)
{
    //uart3 发送
    if(DMA_GetITStatus(DMA1_INT_TC2))
    {
        DMA_ClearITPendingBit(DMA1_FLAG_GL2 | DMA1_FLAG_TC2 | DMA1_FLAG_HT2 | DMA1_FLAG_ERR2);

        //串口2DMA发送完成标志
        g_nSysUsart3SendOver_Flag = 1;
        //g_nUsart2Status = USART2DATASTEP;
        //
        //g_nSys485CommStatusShift_SP1 = 0;
        g_nSendStatus_SP3 = 1;
    }
}

void UART3_IRQHandler(void)
{
    ;
}







