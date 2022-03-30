///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
#include "SysConfig.h"
#include "at32f4xx.h"
#include "SysGpioInit.h"

///////////////////////////////////////////////////////////////////////////
// GPIO 配置
///////////////////////////////////////////////////////////////////////////
void GPIO_Configuration(void)
{
	//PA13 14 15默认是JTAG口, 做普通IO需禁用
	//AFIO->MAPR &= ~(0x07 << 24);    //清除b26:b24
	//AFIO->MAPR |= 0x04 << 24;       //设定JTAG无效, SWD无效

	//禁止JTAG   SWD可以用PA15 可用作IO
	GPIO_PinsRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	//禁用JTAG SWD
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);

	InitSysAllPortStatusProc();

// 	// Port A
// 	InitSysPortAStatusProc();
// 
// 	// Port B
// 	InitSysPortBStatusProc();
// 
// 	// Port C
// 	InitSysPortCStatusProc();
// 
// 	// Port D
// 	InitSysPortDStatusProc();
// 	
// 	// Port E
// 	InitSysPortEStatusProc();
// 
// 	// Port F
// 	InitSysPortFStatusProc();
// 
// 	// Port G
// 	InitSysPortGStatusProc();

	/*/???
	//GPIO_Mode_IN_FLOATING 浮空输入,就相当于此端口在默认情况下什么都不接, 呈高阻态
	1、浮空输入_IN_FLOATING -- 浮空输入, 可以做KEY识别, RX1
	2、带上拉输入_IPU -- IO内部上拉电阻输入
	3、带下拉输入_IPD -- IO内部下拉电阻输入
	4、模拟输入_AIN -- 应用ADC模拟输入, 或者低功耗下省电
	5、开漏输出_OUT_OD -- IO输出0接GND, IO输出1, 悬空, 需要外接上拉电阻, 才能实现输出高电平。当输出为1时, IO口的状态由上拉电阻拉高电平, 但由于是开漏输出模式, 这样IO口也就可以由外部电路改变为低电平或不变. 可以读IO输入电平变化, 实现C51的IO双向功能
	6、推挽输出_OUT_PP -- IO输出0-接GND, IO输出1 -接VCC, 读输入值是未知的
	7、复用功能的推挽输出_AF_PP -- 片内外设功能(I2C的SCL,SDA)
	8、复用功能的开漏输出_AF_OD -- 片内外设功能(TX1,MOSI,MISO.SCK.SS)
	*/
}

const GPIO_CONFIG_DEF m_arrConfigBuf[] = {
	{ SYS_LED_PORT,		SYS_LED_PINS,		GPIO_MaxSpeed_50MHz,GPIO_Mode_OUT_PP,HIGH},
	{ TM_DIO_PORT,		TM_DIO_PINS,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_FLOATING,HIGH},				//
	{ TM_CLK_PORT,		TM_CLK_PINS,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_FLOATING,HIGH},				//
	{ TM_CS_PORT,		TM_CS_PINS,			GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_FLOATING,HIGH},				//
	{ SDA_PORT,		SDA_PINS,GPIO_MaxSpeed_50MHz,GPIO_Mode_OUT_PP,HIGH},
	{ SCL_PORT,		SCL_PINS,GPIO_MaxSpeed_50MHz,GPIO_Mode_OUT_PP,HIGH},
	{ EEPROM_CS_PORT	,		EEPROM_CS_PINS,GPIO_MaxSpeed_50MHz,GPIO_Mode_OUT_PP,HIGH},
	{ RS485_DIR_PORT	,		RS485_DIR_PINS,GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_FLOATING,LOW},		//

	//out
	{ OUTPUT_PORT1,			OUTPUT_PINS1,		GPIO_MaxSpeed_50MHz,GPIO_Mode_OUT_PP,HIGH},			
	{ OUTPUT_PORT2,			OUTPUT_PINS2,		GPIO_MaxSpeed_50MHz,GPIO_Mode_OUT_PP,HIGH},			
	{ OUTPUT_PORT3,			OUTPUT_PINS3,		GPIO_MaxSpeed_50MHz,GPIO_Mode_OUT_PP,HIGH},			
	{ OUTPUT_PORT4,			OUTPUT_PINS4,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_FLOATING,HIGH},
	{ OUTPUT_PORT5,			OUTPUT_PINS5,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_FLOATING,HIGH},
	{ OUTPUT_PORT6,			OUTPUT_PINS6,		GPIO_MaxSpeed_50MHz,GPIO_Mode_OUT_PP,HIGH},			
	{ OUTPUT_PORT7,			OUTPUT_PINS7,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_FLOATING,HIGH},
	{ OUTPUT_PORT8,			OUTPUT_PINS8,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_FLOATING,HIGH},
	{ OUTPUT_PORT9,			OUTPUT_PINS9,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_FLOATING,HIGH},
	{ OUTPUT_PORT10,		OUTPUT_PINS10,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_FLOATING,HIGH},
	{ OUTPUT_PORT11,		OUTPUT_PINS11,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_FLOATING,HIGH},
	{ OUTPUT_PORT12,		OUTPUT_PINS12,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_FLOATING,HIGH},
	{ OUTPUT_PORT13,		OUTPUT_PINS13,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_FLOATING,HIGH},
	{ OUTPUT_PORT14,		OUTPUT_PINS14,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_FLOATING,HIGH},
	{ OUTPUT_PORT15,		OUTPUT_PINS15,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_FLOATING,HIGH},

	//in
	{ INPUT_PORT1 ,			INPUT_PINS1 ,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT2 ,			INPUT_PINS2 ,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT3 ,			INPUT_PINS3 ,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT4	,			INPUT_PINS4 ,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT5	,			INPUT_PINS5 ,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT6	,			INPUT_PINS6 ,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT7	,			INPUT_PINS7 ,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT8	,			INPUT_PINS8 ,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT9	,			INPUT_PINS9 ,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT10,			INPUT_PINS10,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT11,			INPUT_PINS11,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT12,			INPUT_PINS12,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT13,			INPUT_PINS13,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT14,			INPUT_PINS14,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT15,			INPUT_PINS15,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT16,			INPUT_PINS16,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT17,			INPUT_PINS17,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT18,			INPUT_PINS18,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT19,			INPUT_PINS19,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT20,			INPUT_PINS20,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},
	{ INPUT_PORT21,			INPUT_PINS21,		GPIO_MaxSpeed_50MHz,GPIO_Mode_IN_PU,HIGH},

};

/*
只用于 普通IO口配置
*/
static void InitSysAllPortStatusProc(void)
{
	u8 nNum = sizeof(m_arrConfigBuf) / sizeof(GPIO_CONFIG_DEF);
	u8 nCount = 0;
	GPIO_InitType GPIO_InitStructure;
	
	for (nCount = 0;nCount < nNum;nCount++)
	{
		if(m_arrConfigBuf[nCount].GPIO_Port == GPIOA)
		{
				RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA ,ENABLE);   
		}
		if(m_arrConfigBuf[nCount].GPIO_Port == GPIOB)
		{
				RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB ,ENABLE);   
		}
		if(m_arrConfigBuf[nCount].GPIO_Port == GPIOC)
		{
				RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOC ,ENABLE);   
		}
		if(m_arrConfigBuf[nCount].GPIO_Port == GPIOD)
		{
				RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOD ,ENABLE);   
		}
		if(m_arrConfigBuf[nCount].GPIO_Port == GPIOE)
		{
				RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOE ,ENABLE);   
		}
	
		GPIO_InitStructure.GPIO_Pins = m_arrConfigBuf[nCount].GPIO_Pin;
		GPIO_InitStructure.GPIO_MaxSpeed = m_arrConfigBuf[nCount].GPIO_Speed;
		GPIO_InitStructure.GPIO_Mode = m_arrConfigBuf[nCount].GPIO_Mode;
		GPIO_Init(m_arrConfigBuf[nCount].GPIO_Port, &GPIO_InitStructure);

		if (m_arrConfigBuf[nCount].GPIO_Out == HIGH)
		{
			GPIO_SetBits(m_arrConfigBuf[nCount].GPIO_Port, m_arrConfigBuf[nCount].GPIO_Pin);
		}
		else if (m_arrConfigBuf[nCount].GPIO_Out == LOW)
		{
			GPIO_ResetBits(m_arrConfigBuf[nCount].GPIO_Port, m_arrConfigBuf[nCount].GPIO_Pin);
		}

	}

}





