///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

#include "SysConfig.h"
#include "RealDatabase.h"

#include "SysTick.h"

///////////////////////////////////////////////////////////////////////////
//如果需要使用OS,则包括下面的头文件即可.
///////////////////////////////////////////////////////////////////////////

#if SYSTEM_SUPPORT_OS
	#include "includes.h"				 
#endif
///////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//开发板
//包括delay_us,delay_ms
///////////////////////////////////////////////////////////////////////////
//
#define OS_CRITICAL_METHOD
//1s/1000=1ms
#define OS_TICKS_PER_SEC    1000u  
///////////////////////////////////////////////////////////////////////////

//ms延时倍乘数,在ucos下,代表每个节拍的ms数
static u16 fac_ms = 0;
//us延时倍乘数
static u8  fac_us = 0;

///////////////////////////////////////////////////////////////////////////
//SYSTICK的时钟固定为HCLK时钟的1/8
void ConfigSysTickInfoProc(void)
{
	////SysTick时钟源为AHB HCLK时钟除以8, 即72/8 = 9MHZ
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	//SysTick_Config(SystemCoreClock / 1000);
	
	UINT16 nTemp = fac_ms;

	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
#ifdef OS_CRITICAL_METHOD 	
	u32 reload;
#endif

	fac_ms = nTemp;

	//选择外部时钟  HCLK/8
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	
	//为系统时钟的1/8  
	fac_us = SystemCoreClock / 8000000;	

	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	#ifdef OS_CRITICAL_METHOD 	
		//每秒钟的计数次数 单位为K	   
		reload = SystemCoreClock / 8000000;	

		//根据OS_TICKS_PER_SEC设定溢出时间
		reload *= 1000000 / OS_TICKS_PER_SEC;

		//reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右	
		fac_ms = 1000 / OS_TICKS_PER_SEC;//代表ucos可以延时的最少单位	   

		//开启SYSTICK中断
		SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   	

		//每1/OS_TICKS_PER_SEC秒中断一次
		SysTick->LOAD = reload; 	
		//开启SYSTICK    
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   	
	#else
		//非ucos下,代表每个ms需要的systick时钟数   
		fac_ms = (u16)fac_us * 1000;
	#endif 
}

///////////////////////////////////////////////////////////////////////////
//延时nus
//nus为要延时的us数.		    								   
void delay_us(u32 nus)
{		
	// 	u32 temp;	    	 
	// 	SysTick->LOAD=nus*fac_us; 					//时间加载	  		 
	// 	SysTick->VAL=0x00;        					//清空计数器
	// 	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数	  
	// 	do
	// 	{
	// 		temp=SysTick->CTRL;
	// 	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	// 	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	// 	SysTick->VAL =0X00;      					 //清空计数器	 

	u32 ticks;
	u32 told, tnow, tcnt = 0;

	//LOAD的值
	u32 reload = SysTick->LOAD;	

	//需要的节拍数
	ticks = nus*fac_us; 			
	tcnt = 0;

	//刚进入时的计数器值
	told = SysTick->VAL;        	

	while (1)
	{
		tnow = SysTick->VAL;

		if (tnow != told)
		{
			//这里注意一下SYSTICK是一个递减的计数器就可以了.
			if (tnow < told)
				tcnt += told - tnow;
			else 
				tcnt += reload - tnow + told;

			told = tnow;

			//时间超过/等于要延迟的时间,则退出.
			if (tcnt >= ticks)
				break;
		}
	};
}

//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	// 	u32 temp;		   
	// 	SysTick->LOAD=(u32)nms*fac_ms;				//时间加载(SysTick->LOAD为24bit)
	// 	SysTick->VAL =0x00;							//清空计数器
	// 	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数  
	// 	do
	// 	{
	// 		temp=SysTick->CTRL;
	// 	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	// 	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	// 	SysTick->VAL =0X00;       					//清空计数器	 

	while (nms--)
	{
		delay_us(990);
	}
} 

