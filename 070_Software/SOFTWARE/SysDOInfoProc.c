#include "SysConfig.h"
#include "SysDOInfoProc.h"

#include "RealDatabase.h"
///////////////////////////////////////////////////////////////////////////
struct
{
    GPIO_Type *GPIO;
    uint16_t Pin;
} DO_T[DO_Number];


void SysDOInitProc(void)
{
    GPIO_InitType GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_GPIOB | RCC_APB2PERIPH_GPIOD | RCC_APB2PERIPH_GPIOE, ENABLE);

    DO_T[0].GPIO = GPIOE;
    DO_T[0].Pin = GPIO_Pins_5;

    DO_T[1].GPIO = GPIOA;
    DO_T[1].Pin = GPIO_Pins_0;

    DO_T[2].GPIO = GPIOE;
    DO_T[2].Pin = GPIO_Pins_9;

    DO_T[3].GPIO = GPIOB;
    DO_T[3].Pin = GPIO_Pins_3;

    DO_T[4].GPIO = GPIOB;
    DO_T[4].Pin = GPIO_Pins_4;

    DO_T[5].GPIO = GPIOB;
    DO_T[5].Pin = GPIO_Pins_5;

    DO_T[6].GPIO = GPIOB;
    DO_T[6].Pin = GPIO_Pins_6;

    DO_T[7].GPIO = GPIOB;
    DO_T[7].Pin = GPIO_Pins_7;

    DO_T[8].GPIO = GPIOB;
    DO_T[8].Pin = GPIO_Pins_8;

    DO_T[9].GPIO = GPIOA;
    DO_T[9].Pin = GPIO_Pins_15;

    DO_T[10].GPIO = GPIOD;
    DO_T[10].Pin = GPIO_Pins_12;

    DO_T[11].GPIO = GPIOE;
    DO_T[11].Pin = GPIO_Pins_2;

    DO_T[12].GPIO = GPIOE;
    DO_T[12].Pin = GPIO_Pins_1;

    DO_T[13].GPIO = GPIOE;
    DO_T[13].Pin = GPIO_Pins_0;

    DO_T[14].GPIO = GPIOB;
    DO_T[14].Pin = GPIO_Pins_9;

    for(int i = 0; i < DO_Number; i++)
    {
        GPIO_InitStructure.GPIO_Pins = DO_T[i].Pin;
        GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
        GPIO_Init(DO_T[i].GPIO, &GPIO_InitStructure);
        GPIO_SetBits(DO_T[i].GPIO, DO_T[i].Pin);
    }
}


//DO输出信号调用次函数，_port：0代表DO1，1代表DO2，依次类推
//_flag：0代表无输出(电压不确定)，1代表有输出(测量输出口与EX_V+电压为24V)
void SysDOCtrlProc(uint8_t _port,uint8_t _flag)
{
    if(1 == _flag)
	{
		GPIO_ResetBits(DO_T[_port].GPIO, DO_T[_port].Pin);
	}
	else
	{
		GPIO_SetBits(DO_T[_port].GPIO, DO_T[_port].Pin);
	}
}








