#ifndef __SysDIProc_H
#define __SysDIProc_H 1

#include "SysConfig.h"
#include "at32f4xx.h"

#define DI_Filter_Time 5

#define DI_Flag_Set   1
#define DI_Flag_Reset 0

#define DI_Signal_Set Bit_SET    // Bit_SET

#define DI_Key_Click (0x02)

#define DI_Number 21

extern uint8_t DI_Flag[DI_Number];

extern void SysDIInit(void);
extern void SysGetDIValue(void);

#endif
