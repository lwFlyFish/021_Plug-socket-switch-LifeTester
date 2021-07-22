///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

#include "SysConfig.h" 
#include "SysUart1.h"
//#include "sysleddisp.h"
///////////////////////////////////////////////////////////////////////////

volatile u8 g_arrUart1SendBuf[UART1_SENDDATA_LEN];
volatile u8 g_arrUart1RecvBuf[UART1_RECVDATA_LEN];
///////////////////////////////////////////////////////////////////////////

#ifdef __GNUC__
 
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
		
#endif 
///////////////////////////////////////////////////////////////////////////

#ifdef USE_IAR
PUTCHAR_PROTOTYPE
{
  UART_SendData(UART1, (uint8_t) ch);
  while (UART_GetFlagStatus(UART1, UART_IT_TXIEN) == RESET);
  return ch;
}

#else
	#pragma import(__use_no_semihosting)             

///////////////////////////////////////////////////////////////////////////
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       

//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	u16 nTimeOut = 500;

	//UARTx->CR1 |= CR1_UE_Set; 使能UARTx
	if ((g_pPrintf->CTRL1 & 0x2000) == 0)
	{//没有使能UARTx
		nTimeOut = 0;
		return ch;
	}

	//等待发送  带超时判断
	while (((g_pPrintf->STS & 0x40) == 0))// && (nTimeOut-- ))
	{;
		//delay_us(10);
	}

	if(nTimeOut == 0)
	{
		nTimeOut = 0;
		return ch;
	}
	
	g_pPrintf->DT = (ch & (uint16_t)0x00FF);
	
	return ch;

}
#endif 

///////////////////////////////////////////////////////////////////////////
void SysUart1InitProc(u32 nBound)
{
	//GPIO端口设置
	GPIO_InitType GPIO_InitStructure;
	USART_InitType UART_InitStructure;

	//使能UART1
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1,ENABLE);

	//使能UART1, GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA, ENABLE);
	
	//UART1_TX   GPIOA.9 PA.9
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_9; 
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	//复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	//初始化GPIOA.9
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//UART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_10;
	//浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//初始化GPIOA.10
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
#ifdef __MM32F103_HD_H
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);
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
	//初始化串口1
	USART_Init(USART1, &UART_InitStructure); 

	//UART1 DMA Enable   
	USART_DMACmd(USART1,USART_DMAReq_Tx| USART_DMAReq_Rx,ENABLE);   
	//开启串口接受中断
	//UART_ITConfig(UART2, UART_IT_RXIEN, ENABLE);


	//使能串口1 
	USART_Cmd(USART1, ENABLE);                   
	
	Uart1DMAInitProc();

}

#if( USE_SPI2_DMA == 0)
//发送单字节
void SP1_SendByteInfoProc(u8 nSendInfo)
{
	volatile BYTE* pBuf = g_arrUart1SendBuf;

	//指向发送缓冲区
	//pBuf = g_arrUART2SendBuff;

	//
	*pBuf++ = nSendInfo;

	//DMA发送方式
	SP1_DmaSendDataProc(1);
}

void SP1_SendBytesInfoProc(u8* pSendInfo, u16 nSendCount)
{
	u16 i = 0;
	volatile BYTE* pBuf = g_arrUart1SendBuf;
	
	//指向发送缓冲区
	//pBuf = g_arrUART2SendBuff;
	
	for (i=0; i<nSendCount; i++)
	{
		*pBuf++ = pSendInfo[i];
	}
	
	//DMA发送方式
	SP1_DmaSendDataProc(nSendCount);

}

void SP1_DmaSendDataProc(u16 nSendLen)
{
	//清DMA发送完成标志
	DMA_ClearFlag(DMA1_FLAG_GL4 | DMA1_FLAG_TC4 | DMA1_FLAG_HT4 | DMA1_FLAG_ERR4);

	//停止DMA
	DMA_ChannelEnable(DMA1_Channel4, DISABLE);

	//DMAy Channelx CNDTR Configuration
	// Write to DMAy Channelx CNDTR
	DMA1_Channel4->TCNT = (u32)nSendLen;

	//DMAy Channelx CPAR Configuration
	//Write to DMAy Channelx CPAR
	//DMA1_Channel7->CPAR = (u32) &(UART2->TDR);
	//DMAy Channelx CMAR Configuration
	//Write to DMAy Channelx CMAR
	DMA1_Channel4->CMBA = (u32)g_arrUart1SendBuf;

	//启动DMA 
	DMA_ChannelEnable(DMA1_Channel4, ENABLE);
}

u16 UART1_GetRecvDataLen(void)
{
	return (UART1_RECVDATA_LEN - DMA1_Channel5->TCNT);
}

extern void UART1_ClcRecvDataLen(void)
{
	//停止DMA
	DMA_ChannelEnable(DMA1_Channel5, DISABLE);

	//配置DMA传输发送数据量
	DMA1_Channel5->TCNT = UART1_RECVDATA_LEN;
	/* Write to DMAy Channelx CMAR */
	DMA1_Channel5->CMBA = (u32)g_arrUart1RecvBuf;

	//启动DMA 
	DMA_ChannelEnable(DMA1_Channel5, ENABLE);
}

static void Uart1DMAInitProc(void)
{
	//
	DMA_InitType DMA_InitStructure;
	NVIC_InitType NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_DMA1, ENABLE);
	
	//DMA5 UART1 Receive
	DMA_Reset(DMA1_Channel5);

	//
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART1->DT);
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)g_arrUart1RecvBuf;
	DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALSRC;
	DMA_InitStructure.DMA_BufferSize = UART1_RECVDATA_LEN;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
	DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
	DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
	DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;
	DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;;
	DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;;
	DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
	//
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	DMA_INTConfig(DMA1_Channel5, DMA_INT_TC, ENABLE);

	//Enable DAM15
	DMA_ChannelEnable(DMA1_Channel5, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//DMA4 UART1 Send
	DMA_Reset(DMA1_Channel4);

	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) &(USART1->DT);
	DMA_InitStructure.DMA_MemoryBaseAddr =(u32)g_arrUart1SendBuf;
	//数据传输方向, 从内存读取发送到外设
	DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALDST;
	DMA_InitStructure.DMA_BufferSize = UART1_SENDDATA_LEN;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
	DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
	DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
	DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;
	//DMA_Mode_Circular;
	DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
	DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
	DMA_InitStructure.DMA_MTOM  = DMA_MEMTOMEM_DISABLE;
	//
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	
	DMA_INTConfig(DMA1_Channel4, DMA_INT_TC, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************
* Function Name  : DMAChannel4_IRQHandler
* Description    : This function handles DMA Stream 4 interrupt request.
* Input          : None
* Output         : None
* Return         : None
**************************************************************************/
void DMA1_Channel4_IRQHandler(void)
{
	//UART1 发送
	if (DMA_GetITStatus(DMA1_INT_TC4))
	{
		DMA_ClearITPendingBit(DMA1_FLAG_GL4 | DMA1_FLAG_TC4 | DMA1_FLAG_HT4 | DMA1_FLAG_ERR4);

		//串口2DMA发送完成标志
		//g_nSysUsart2SendOver_Flag = 1;
		//g_nUsart2Status = USART2DATASTEP;
		//
		//g_nSys485CommStatusShift_SP1 = 0;
	}
}

/**************************************************************************
* Function Name  : DMAChannel5_IRQHandler
* Description    : This function handles DMA Stream 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
**************************************************************************/
void DMA1_Channel5_IRQHandler(void)
{
	//UART1 接收
	if (DMA_GetITStatus(DMA1_INT_TC5))
	{
		DMA_ClearITPendingBit(DMA1_INT_TC5);
		//停止DMA
		DMA_ChannelEnable(DMA1_Channel5, DISABLE);

		//配置DMA传输发送数据量
		DMA1_Channel5->TCNT = UART1_RECVDATA_LEN;

		// Write to DMAy Channelx CMAR
		DMA1_Channel5->CMBA = (u32)g_arrUart1RecvBuf;

		//启动DMA 
		DMA_ChannelEnable(DMA1_Channel5, ENABLE);
	}	
	//
	//g_nSys485CommStatusShift_SP1 = 0;
}

u16 UART1_GetRecvDataLen(void)
{
    return (UART1_RECVDATA_LEN - DMA1_Channel5->TCNT);
}

void UART1_ClcRecvDataLen(void)
{
	DMA_ChannelEnable(DMA1_Channel5, DISABLE);
    //配置DMA接收数据量
    DMA1_Channel5->TCNT = UART1_RECVDATA_LEN;
    //
    DMA_ChannelEnable(DMA1_Channel5, ENABLE);
}

#endif














