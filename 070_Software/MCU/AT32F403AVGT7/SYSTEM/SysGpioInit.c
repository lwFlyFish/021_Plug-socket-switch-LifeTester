///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
#include "SysConfig.h"
#include "at32f4xx.h"
#include "SysGpioInit.h"

///////////////////////////////////////////////////////////////////////////
// GPIO ����
///////////////////////////////////////////////////////////////////////////
void GPIO_Configuration(void)
{
	//PA13 14 15Ĭ����JTAG��, ����ͨIO�����
	//AFIO->MAPR &= ~(0x07 << 24);    //���b26:b24
	//AFIO->MAPR |= 0x04 << 24;       //�趨JTAG��Ч, SWD��Ч

	//��ֹJTAG   SWD������PA15 ������IO
	GPIO_PinsRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	//����JTAG SWD
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
	//GPIO_Mode_IN_FLOATING ��������,���൱�ڴ˶˿���Ĭ�������ʲô������, �ʸ���̬
	1����������_IN_FLOATING -- ��������, ������KEYʶ��, RX1
	2������������_IPU -- IO�ڲ�������������
	3������������_IPD -- IO�ڲ�������������
	4��ģ������_AIN -- Ӧ��ADCģ������, ���ߵ͹�����ʡ��
	5����©���_OUT_OD -- IO���0��GND, IO���1, ����, ��Ҫ�����������, ����ʵ������ߵ�ƽ�������Ϊ1ʱ, IO�ڵ�״̬�������������ߵ�ƽ, �������ǿ�©���ģʽ, ����IO��Ҳ�Ϳ������ⲿ��·�ı�Ϊ�͵�ƽ�򲻱�. ���Զ�IO�����ƽ�仯, ʵ��C51��IO˫����
	6���������_OUT_PP -- IO���0-��GND, IO���1 -��VCC, ������ֵ��δ֪��
	7�����ù��ܵ��������_AF_PP -- Ƭ�����蹦��(I2C��SCL,SDA)
	8�����ù��ܵĿ�©���_AF_OD -- Ƭ�����蹦��(TX1,MOSI,MISO.SCK.SS)
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
ֻ���� ��ͨIO������
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





