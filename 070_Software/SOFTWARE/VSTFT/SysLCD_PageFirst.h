#ifndef __SYSLCD_PAGEFIRST_H
#define __SYSLCD_PAGEFIRST_H

#include "SysConfig.h"

// ��ӭ���水ťID
typedef enum
{
    TFT_First_Switch_Button = 0,	//�������鰴ť
    TFT_First_Socket_Button,		//��ͷ�������鰴ť

    TFT_First_Button_Number
} TFT_First_Button_E;

extern uint8_t g_nPageFirstTimeShowFlag;

void SysLCDTFT_PageFirstShowInfoProc(void);
BYTE SysLCDTFT_PageFirstTextProc(BYTE nCount, uint16_t _id);
BYTE SysLCDTFT_PageFirstButtonProc(uint16_t _id);
void SysTFTLcd_PageFirstGotoProc(void);

#endif
