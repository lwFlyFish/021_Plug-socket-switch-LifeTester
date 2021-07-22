#ifndef __SysConfig_H
#define __SysConfig_H	1
//
///////////////////////////////////////////////////////////////////////////

#include "mcu.h"
#include "stdio.h"
#include "string.h"
//#include "dtype.h"

#include <malloc.h>

#define SYS_VER					"02.02.04"		//����汾


//#include "SysTick.h"
///////////////////////////////////////////////////////////////////////////	 
//ϵͳ�����궨��-lyy
//////////////////////////////////////////////////////////////////////////
#define USE_SPI2_DMA			0		//0����ʹ��SPI2 DMA		1��ʹ��DMA
#define DWG_EN					0		//0���رտ��Ź�			1���򿪿��Ź�
#define READ_PROTECTION_EN 		0	//0���ض�����			1����������

//ѡ��
#define RAMP_UART2 				0		//0����ӳ��uart2    	1��ӳ��uart2

//RTCѡ��
#define USE_PCF8563 			1		//0��ʹ��Ƭ��RTC     	1��ʹ��Ƭ��PCF8563ʱ��

//#define FULL_RAMP_UART3_FOR232		0  //

///////////////////////////////////////////////////////////////////////////
//ϵͳ�������Ͷ���
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
//#define Conn(x,y)    x##y   //����x,y
//#define ToChar(x)    #@x    //�ӵ�����
//#define ToString(x)  #x     //��˫����


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
//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
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
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

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
	//����Ϊ��ຯ��-------------------------------------------------------
	//ִ��WFIָ��
	void WFI_SET(void);
	//�ر������ж�
	void INTX_DISABLE(void);
	//���������ж�
	void INTX_ENABLE(void);
	//���ö�ջ��ַ
	void MSR_MSP(u32 addr);
	
	//ϵͳ��ʼ��
	extern void SysInitHardwareProc(void);

	//ϵͳ�ӽ��ܴ���
	void SysLockProtectedProc(BYTE nType);

	void NVICx_INIT(IRQn_Type Irqn, u8 NVIC_PreemptionPriority, u8 NVIC_SubPriority);

//private:

	//���Ź�
	static void	SysWDGInitProc(u8 IWDG_Prescaler,u16 Reload);
	//
	static void ConfigSysRccClockProc(void);
	//
	static void ConfigSysNVICInfoProc(void);
	//
//	static void PVU_CheckStatus(void);
	//
	//static void InitSysEXTILineProc(void);

	//��ʼ��ϵͳ״̬
	void SysLedBlink(void);


#endif



