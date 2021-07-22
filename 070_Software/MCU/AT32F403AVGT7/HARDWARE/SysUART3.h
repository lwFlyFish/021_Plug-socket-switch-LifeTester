#ifndef __SYSUART3_H
#define __SYSUART3_H        1
//
///////////////////////////////////////////////////////////////////////////
#include "SysGpioInit.h"

#define FULL_RAMP_UART3_FOR232      1  //0:用做485  1用作232

#define UART3_SENDDATA_LEN              200
#define UART3_RECVDATA_LEN              200


#if(FULL_RAMP_UART3_FOR232 == 0)

#define RS485B_IN                                   RS485B_DIR_PORT->BRE = RS485B_DIR_PINS;
#define RS485B_OUT                              RS485B_DIR_PORT->BSRE = RS485B_DIR_PINS;

#endif

///////////////////////////////////////////////////////////////////////////
// Attribute
///////////////////////////////////////////////////////////////////////////
//public:
extern u8 g_arrUart3SendBuf[UART3_SENDDATA_LEN];
extern u8 g_arrUart3RecvBuf[UART3_RECVDATA_LEN];
extern volatile BYTE g_nSendStatus_SP3;



//private:





///////////////////////////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////////////////////////
//public:
//初始化UART2
extern void SysUart3InitProc(u32 nBound);

//发送单字节
extern void SP3_SendByteInfoProc(u8 nSendInfo);
//发送多字节
extern void SP3_SendBytesInfoProc(u8 *pSendInfo, u16 nSendCount);
//DMA发送方式
extern void SP3_DmaSendDataProc(u16 nSendLen);

extern void Usart3DmaRxInit(void);

extern u16 SP3_GetRecvDataCountProc(void);
extern void UART3_ClcRecvDataLen(void);

//private:
static void Uart3DMAInitProc(void);




#endif





