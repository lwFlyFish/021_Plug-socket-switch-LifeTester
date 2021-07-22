#ifndef __SysConfig_H
#define __SysConfig_H	1
//
///////////////////////////////////////////////////////////////////////////

#include "mcu.h"
#include "stdio.h"
#include "string.h"
//#include "dtype.h"

#include <malloc.h>

#define SYS_VER					"02.02.04"		//软件版本


//#include "SysTick.h"
///////////////////////////////////////////////////////////////////////////	 
//系统参数宏定义-lyy
//////////////////////////////////////////////////////////////////////////
#define USE_SPI2_DMA			0		//0：不使用SPI2 DMA		1：使用DMA
#define DWG_EN					0		//0：关闭看门狗			1：打开看门狗
#define READ_PROTECTION_EN 		0	//0：关读保护			1：开读保护

//选择
#define RAMP_UART2 				0		//0：不映射uart2    	1：映射uart2

//RTC选择
#define USE_PCF8563 			1		//0：使用片内RTC     	1：使用片外PCF8563时钟

//#define FULL_RAMP_UART3_FOR232		0  //

///////////////////////////////////////////////////////////////////////////
//系统数据类型定义
///////////////////////////////////////////////////////////////////////////
/*/???
#ifndef bool
typedef enum{FALSE=0,TRUE =!FALSE}bool;
#endif
*/

#define CASE					break;case
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
#ifndef NULL
#define NULL					0
#endif

#ifndef TRUE
#define TRUE					1
#endif

#ifndef FALSE
#define FALSE					0
#endif

//
#define LIMIT(a, x, b)			if((x)<=(a))(x)=(a);else if((x)>=(b))(x)=(b);
#define MIN(a, b)				((a < b) ? (a) : (b))
#define MAX(a, b)				((a > b) ? (a) : (b))
#define ABS(x)					((x > 0) ? (x) : (-x))

///////////////////////////////////////////////////////////////////////////
//#define Conn(x,y)    x##y   //连接x,y
//#define ToChar(x)    #@x    //加单引号
//#define ToString(x)  #x     //加双引号


///////////////////////////////////////////////////////////////////////////
#include "math.h"
#include <string.h>

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
#ifndef BOOL
typedef uint8_t	BOOL;
#endif

#ifndef BYTE
typedef uint8_t	BYTE;
#endif

#ifndef byte
typedef int8_t	byte;
#endif
#ifndef CBYTE
typedef const uint8_t CBYTE;
#endif

#ifndef UINT16
typedef uint16_t UINT16;
#endif
#ifndef int16
typedef int16_t	int16;
#endif
#ifndef UINT32
typedef uint32_t UINT32;
#endif
#ifndef int32
typedef int32_t int32;
#endif

#ifndef s32
typedef int32_t s32;
#endif
#ifndef s16
typedef int16_t s16;
#endif
#ifndef s8
typedef int8_t  s8;
#endif

#ifndef u32
typedef uint32_t u32;
#endif
#ifndef u16
typedef uint16_t u16;
#endif
#ifndef u8
typedef uint8_t u8;
#endif

#define U16_MAX 0xFFFF
#define U32_MAX 0xFFFFFFFF
#define S32_MAX (2^32)-1

///////////////////////////////////////////////////////////////////////////
//#define
///////////////////////////////////////////////////////////////////////////
//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
// Attribute
///////////////////////////////////////////////////////////////////////////
//public:

	extern RCC_ClockType RCC_ClocksStatus;

	extern USART_Type *g_pPrintf;



//private:
	



///////////////////////////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////////////////////////
//public:
	//以下为汇编函数-------------------------------------------------------
	//执行WFI指令
	void WFI_SET(void);
	//关闭所有中断
	void INTX_DISABLE(void);
	//开启所有中断
	void INTX_ENABLE(void);
	//设置堆栈地址
	void MSR_MSP(u32 addr);
	
	//系统初始化
	extern void SysInitHardwareProc(void);

	//系统加解密处理
	void SysLockProtectedProc(BYTE nType);

	void NVICx_INIT(IRQn_Type Irqn, u8 NVIC_PreemptionPriority, u8 NVIC_SubPriority);

//private:

	//看门狗
	static void	SysWDGInitProc(u8 IWDG_Prescaler,u16 Reload);
	//
	static void ConfigSysRccClockProc(void);
	//
	static void ConfigSysNVICInfoProc(void);
	//
//	static void PVU_CheckStatus(void);
	//
	//static void InitSysEXTILineProc(void);

	//初始化系统状态
	void SysLedBlink(void);


#endif



