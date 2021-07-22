///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
#include "SysDIProc.h"

#include "RealDatabase.h"
#include "SysLCD_ShowInfoProc.h"
#include "mcu.h"
///////////////////////////////////////////////////////////////////////////

static struct
{
    GPIO_Type *GPIO;

    uint16_t Pin;

    uint16_t count;
} DI_T[DI_Number];

uint8_t DI_Flag[DI_Number];

void SysDIInit(void)
{
    GPIO_InitType GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_GPIOC | RCC_APB2PERIPH_GPIOD | RCC_APB2PERIPH_GPIOE, ENABLE);

    DI_T[0].GPIO = GPIOD;
    DI_T[0].Pin  = GPIO_Pins_10;

    DI_T[1].GPIO = GPIOD;
    DI_T[1].Pin  = GPIO_Pins_9;

    DI_T[2].GPIO = GPIOD;
    DI_T[2].Pin  = GPIO_Pins_8;

    DI_T[3].GPIO = GPIOC;
    DI_T[3].Pin  = GPIO_Pins_9;

    DI_T[4].GPIO = GPIOC;
    DI_T[4].Pin  = GPIO_Pins_8;

    DI_T[5].GPIO = GPIOD;
    DI_T[5].Pin  = GPIO_Pins_15;

    DI_T[6].GPIO = GPIOD;
    DI_T[6].Pin  = GPIO_Pins_14;

    DI_T[7].GPIO = GPIOD;
    DI_T[7].Pin  = GPIO_Pins_13;

    DI_T[8].GPIO = GPIOD;
    DI_T[8].Pin  = GPIO_Pins_11;

    DI_T[9].GPIO = GPIOC;
    DI_T[9].Pin  = GPIO_Pins_11;

    DI_T[10].GPIO = GPIOE;
    DI_T[10].Pin  = GPIO_Pins_4;

    DI_T[11].GPIO = GPIOC;
    DI_T[11].Pin  = GPIO_Pins_10;

    DI_T[12].GPIO = GPIOA;
    DI_T[12].Pin  = GPIO_Pins_12;

    DI_T[13].GPIO = GPIOA;
    DI_T[13].Pin  = GPIO_Pins_11;

    DI_T[14].GPIO = GPIOA;
    DI_T[14].Pin  = GPIO_Pins_8;

    DI_T[15].GPIO = GPIOD;
    DI_T[15].Pin  = GPIO_Pins_7;

    DI_T[16].GPIO = GPIOD;
    DI_T[16].Pin  = GPIO_Pins_6;

    DI_T[17].GPIO = GPIOD;
    DI_T[17].Pin  = GPIO_Pins_5;

    DI_T[18].GPIO = GPIOD;
    DI_T[18].Pin  = GPIO_Pins_4;

    DI_T[19].GPIO = GPIOD;
    DI_T[19].Pin  = GPIO_Pins_3;

    DI_T[20].GPIO = GPIOD;
    DI_T[20].Pin  = GPIO_Pins_2;

    for(int i = 0; i < DI_Number; i++)
    {
        GPIO_InitStructure.GPIO_Pins     = DI_T[i].Pin;
        GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
        GPIO_InitStructure.GPIO_Mode     = GPIO_Mode_IN_FLOATING;
        GPIO_Init(DI_T[i].GPIO, &GPIO_InitStructure);

        DI_T[i].count = 0;
    }
}



//获取输入信号变量
//在主循环中调用，如果DI1接24V-，仿真就能看到DI_Flag[0]为1，否则为0.依次类推
void SysGetDIValue(void)
{
    for(int i = 0; i < DI_Number ; i++)
    {
        if(DI_Signal_Set == GPIO_ReadInputDataBit(DI_T[i].GPIO, DI_T[i].Pin))
        {
            if(DI_T[i].count < DI_Filter_Time)
            {
                DI_T[i].count = DI_Filter_Time;
            }
            else if(DI_T[i].count < (2 * DI_Filter_Time))
            {
                DI_T[i].count++;
            }
            else
            {
                if((DI_Flag[i]& DI_Flag_Set) != DI_Flag_Set)
                {
                    DI_Flag[i] = DI_Flag_Set;
                }
            }
        }
        else
        {
            if(DI_T[i].count > DI_Filter_Time)
            {
                DI_T[i].count = DI_Filter_Time;
            }
            else if(DI_T[i].count > 0)
            {
                DI_T[i].count--;
            }
            else
            {
                if((DI_Flag[i]| DI_Flag_Reset) != DI_Flag_Reset)
                {
                    DI_Flag[i] = DI_Flag_Reset;
                }
            }
        }
    }
}

