///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
#include "mcu.h"
#include "SysConfig.h"
#include "SysGpioInit.h"
#include "SysTick.h"
#include "malloc.h"	
#include "PCF8563.H"
#include "SysUart3.h"
#include "SysDIInfoProc.h"
#include "24cxx.h"
#include "SysDOInfoProc.h"
#include "SysDIProc.h"

///////////////////////////////////////////////////////////////////////////

RCC_ClockType RCC_ClocksStatus;

USART_Type *g_pPrintf = (USART_Type *)USART1;

///////////////////////////////////////////////////////////////////////////
//系统硬件初始化
void SysInitHardwareProc(void)
{
//	u16 nCount = 0;
	
	//配置外设时钟
	//ConfigSysRccClockProc();

	//保护
	//SysLockProtectedProc(READ_PROTECTION_EN);

	//得到系统时钟 测试用
	RCC_GetClocksFreq(&RCC_ClocksStatus);
	
	//初始化GPIO
	GPIO_Configuration();

	//系统中断信息配置 通道, 优先级等信息
	ConfigSysNVICInfoProc();

	//初始化内存池使用内存管理
	SysMemInit();
	
	
	SysUart3InitProc(57600);
	
	SysDOInitProc();
	SysDIInit();
	
	//配置SysTick
	ConfigSysTickInfoProc();
	
		
#if USE_PCF8563 == 0
	//RTC初始化
	while((RTC_Init()==1)&&(nCount++ <=500));
#else
	//printf_SoftUart("/******************************/\r\n");
	if(PCF8563_InitProc())
	{
		//g_stDebugInfo.bPCF8563Tag = 0;
		//printf_SoftUart("PCF8563 Init Fault\r\n");
	}
	else
	{
		//g_stDebugInfo.bPCF8563Tag = 1;
		//printf_SoftUart("PCF8563 Init Successd\r\n");
	}
#endif
	

#if(DWG_EN==1)
	//看门狗
	SysWDGInitProc(IWDG_Prescaler_16, 0xfff);
#endif

}

//系统加解密处理
void SysLockProtectedProc(BYTE nType)
{
	//设置读保护:
	if (nType == 0x01)
	{
		//这些函数在stm32f10x_flash里面;
		//注意:调用上面这个库的时候需在#include "stm32f10x_flash.h"前加#define_FLASH_PROG; 否则报(没有定义)错;
		if (FLASH_GetReadProtectStatus() != SET)
		{ 
			//解锁FALSH设置读保护
			FLASH_Unlock(); 
			FLASH_ReadProtectConfig(ENABLE);
		}
	}
	//解除读保护
	else
	{
		//
		if (FLASH_GetReadProtectStatus() != RESET)
		{
			//
			FLASH_Unlock();
			//
			FLASH_ReadProtectConfig(DISABLE);
		}
	}
}

/*/独立看门狗初始化
static void	SysWDGInitProc(u8 IWDG_Prescaler,u16 Reload)
{
	//RCC_LSICmd(ENABLE);
	//while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
	
	//PVU_CheckStatus();

	//写入0x5555,用于允许狗狗寄存器写入功能
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	//狗狗时钟分频,40K/256=156HZ(6.4ms)
	//狗狗时钟分频,40K/16=2496HZ(400us)
	IWDG_SetPrescaler(IWDG_Prescaler);
	
	//PVU_CheckStatus();
	//IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	//喂狗时间 5s/6.4MS=781. 注意不能大于0xFFF
	//喂狗时间 5s/400us=12500. 注意不能大于0xFFF
	IWDG_SetReload(Reload);	

	//喂狗
	IWDG_ReloadCounter();

	//使能
	IWDG_Enable();
}
*/
///////////////////////////////////////////////////////////////////////////
/**
brief
eg.
NVICx_INIT(EXTI9_5_IRQn,0,1);
*/
void NVICx_INIT(IRQn_Type Irqn, u8 NVIC_PreemptionPriority, u8 NVIC_SubPriority)
{

	NVIC_InitType NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = Irqn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/*/配置外设时钟 to 72MHz
static void ConfigSysRccClockProc(void)
{
	ErrorStatus HSEStartUpStatus;

	//时钟管理重置
	RCC_DeInit();

	///打开外部晶振
	RCC_HSEConfig(RCC_HSE_ON);
	//等待外部晶振就绪
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

	//等待外部晶振就绪
	HSEStartUpStatus = RCC_WaitForHSEStartUp();     
	if (HSEStartUpStatus == SUCCESS)    
	{   
		//flash读取缓冲, 加速
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  
		//flash操作的延时
		FLASH_SetLatency(FLASH_Latency_2);
		
		//-----------------------------------------------------------------
		//AHB使用系统时钟
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		//注:AHB主要负责外部存储器时钟。
		//PB2负责AD, I/O, 高级TIM, 串口1
		//APB2(高速)为HCLK
		RCC_PCLK2Config(RCC_HCLK_Div1);

		//APB1负责DA, USB, SPI, I2C, CAN, 串口2345, 普通TIM
		//APB1(低速)为HCLK的一半   
		RCC_PCLK1Config(RCC_HCLK_Div2);

		//???
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);

		//倍频设置---------------------------------------------------------
		*(vu32 *)0x40022000 = 0x01;
		//PLLCLK = 8MHz * 9 = 72 MHz
		RCC_PLLConfig (RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		//启动PLL
		RCC_PLLCmd(ENABLE);
		//等待PLL启动
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		
		//将PLL设置为系统时钟源
		RCC_SYSCLKConfig (RCC_SYSCLKSource_PLLCLK);
		//等待系统时钟源的启动
		while (RCC_GetSYSCLKSource() != 0x08);
	}

	//启动AHP设备
	RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_DMA1 | 	
						  RCC_AHBPERIPH_DMA2 | 
						  RCC_AHBPERIPH_SRAM,    
						  ENABLE); 
	
	//启动ABP2设备
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA | 	   
							RCC_APB2Periph_GPIOB | 	   
							RCC_APB2Periph_GPIOC | 	   
							RCC_APB2Periph_GPIOD |	   
							RCC_APB2Periph_GPIOE |	   
							RCC_APB2Periph_SPI1  |
							RCC_APB2Periph_ADC1  |	   
							RCC_APB2Periph_ADC2  |	   
							RCC_APB2Periph_TIM1  |
							RCC_APB2Periph_TIM8  | 
							RCC_APB2Periph_USART1|	   
							RCC_APB2Periph_AFIO,
							ENABLE);
	
	//启动ABP1设备
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 |
							RCC_APB1Periph_TIM3 |
							RCC_APB1Periph_TIM4 |
							RCC_APB1PERIPH_USART2 |
							RCC_APB1Periph_USART3 |
							RCC_APB1Periph_UART4 | 
							RCC_APB1Periph_UART5 | 
							RCC_APB1Periph_PWR | 
							RCC_APB1Periph_SPI2 | 
							RCC_APB1Periph_SPI3 | 
							RCC_APB1Periph_DAC | 
							RCC_APB1Periph_BKP,							 
							ENABLE);
}*/


//系统中断配置 - 开中断
static void ConfigSysNVICInfoProc(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

/**
brief  
eg.
PVU_CheckStatus();
*/
/*static void PVU_CheckStatus(void)
{
	while (1)
	{
		if (IWDG_GetFlagStatus(IWDG_FLAG_PVU) == RESET)
		{
			break;
		}
	}
}*/

///////////////////////////////////////////////////////////////////////////



// hard fault handler in C,  
// with stack frame location as input parameter  
void hard_fault_handler_c(unsigned int * hardfault_args)  
{  
	unsigned int stacked_r0;  
	unsigned int stacked_r1;  
	unsigned int stacked_r2;  
	unsigned int stacked_r3;  
	unsigned int stacked_r12;  
	unsigned int stacked_lr;  
	unsigned int stacked_pc;  
	unsigned int stacked_psr;  
	 
	
	stacked_r0=stacked_r0;
	stacked_r1=stacked_r1;
	stacked_r2=stacked_r2;
	stacked_r3=stacked_r3;
	stacked_r12=stacked_r12;
	stacked_lr=stacked_lr;
	stacked_pc=stacked_pc;
	stacked_psr=stacked_psr;
	
	
	stacked_r0 = ((unsigned long) hardfault_args[0]);  
	stacked_r1 = ((unsigned long) hardfault_args[1]);  
	stacked_r2 = ((unsigned long) hardfault_args[2]);  
	stacked_r3 = ((unsigned long) hardfault_args[3]);  
	 
	stacked_r12 = ((unsigned long) hardfault_args[4]);  
	stacked_lr = ((unsigned long) hardfault_args[5]);  
	stacked_pc = ((unsigned long) hardfault_args[6]);  
	stacked_psr = ((unsigned long) hardfault_args[7]);  
		 
	while(1)
	{
		;
	}
	 
} 

///////////////////////////////////////////////////////////////////////////
//系统led闪烁
///////////////////////////////////////////////////////////////////////////
void SysLedBlink(void)
{
	//反转LED
	SYS_LED_PORT->OPTDT ^= SYS_LED_PINS;
}







