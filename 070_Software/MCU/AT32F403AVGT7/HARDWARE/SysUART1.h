#ifndef __SYSUART1_H
#define	__SYSUART1_H			1
//
///////////////////////////////////////////////////////////////////////////
#include "SysConfig.h"

///////////////////////////////////////////////////////////////////////////

#define RAMP_UART1_GPIO			1

#define UART1_SENDDATA_LEN 		500
#define UART1_RECVDATA_LEN 		500

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
// Attribute
///////////////////////////////////////////////////////////////////////////
//public:
	//
extern volatile u8 g_arrUart1SendBuf[UART1_SENDDATA_LEN];
extern volatile u8 g_arrUart1RecvBuf[UART1_RECVDATA_LEN];
extern u16 UART1_GetRecvDataLen(void);
extern void UART1_ClcRecvDataLen(void);

	
//private:





///////////////////////////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////////////////////////
//public:
	//初始化UART1
	extern void SysUart1InitProc(u32 nBound);

#if(USE_SPI2_DMA == 0)
	//发送单字节-----------------------------------------------------------
	extern void SP1_SendByteInfoProc(u8 nSendInfo);
	//发送多字节
	extern void SP1_SendBytesInfoProc(u8* pSendInfo, u16 nSendCount);
	//DMA发送方式
	extern void SP1_DmaSendDataProc(u16 nSendLen);

	extern u16 UART1_GetRecvDataLen(void);
	extern void UART1_ClcRecvDataLen(void);

//private:
	//
	static void Uart1DMAInitProc(void);

#endif


#endif








