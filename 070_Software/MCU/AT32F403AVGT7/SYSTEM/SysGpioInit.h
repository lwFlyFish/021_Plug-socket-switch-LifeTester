#ifndef __SysGpioInit_H
#define __SysGpioInit_H	  1
//
///////////////////////////////////////////////////////////////////////////
#include "at32f4xx.h"

typedef enum 
{
	DEF = 0,
	LOW = 1,
	HIGH = 2,
}GPIO_OUT_DEF;

typedef struct
{
	GPIO_Type *		GPIO_Port;
	uint16_t			GPIO_Pin;
	GPIOMaxSpeed_Type	GPIO_Speed;
	GPIOMode_Type	GPIO_Mode;
	GPIO_OUT_DEF		GPIO_Out;
}GPIO_CONFIG_DEF;

///////////////////////////////////////////////////////////////////////////
//#define
///////////////////////////////////////////////////////////////////////////
#define SYS_LED_PORT			GPIOE
#define SYS_LED_PINS			GPIO_Pins_7
#define SYS_LED_ON				SYS_LED_PORT->BRE  = SYS_LED_PINS
#define SYS_LED_OFF				SYS_LED_PORT->BSRE = SYS_LED_PINS

//TM1620
#define TM_CLK_PORT				GPIOC
#define TM_CLK_PINS				GPIO_Pins_12
#define TM_CS_PORT				GPIOD
#define TM_CS_PINS				GPIO_Pins_1
#define TM_DIO_PORT				GPIOD
#define TM_DIO_PINS				GPIO_Pins_0

// DAC
#define DAC1_PORT					GPIOA
#define DAC1_PINS					GPIO_Pins_4

#define DAC2_PORT					GPIOA
#define DAC3_PINS					GPIO_Pins_5

//485 DIR
#define RS485_DIR_PORT			GPIOA
#define RS485_DIR_PINS			GPIO_Pins_1

//IIC
#define SDA_PORT					GPIOC
#define SDA_PINS					GPIO_Pins_5
#define SCL_PORT					GPIOB
#define SCL_PINS					GPIO_Pins_0
#define EEPROM_CS_PORT		GPIOC
#define EEPROM_CS_PINS		GPIO_Pins_4

//output
#define OUTPUT_PORT1			GPIOE					
#define OUTPUT_PINS1			GPIO_Pins_5

#define OUTPUT_PORT2			GPIOA
#define OUTPUT_PINS2			GPIO_Pins_0		//TIM2_CH1 TIM5_CH1

#define OUTPUT_PORT3			GPIOE
#define OUTPUT_PINS3			GPIO_Pins_9		//TIM1_CH1

#define OUTPUT_PORT4			GPIOB
#define OUTPUT_PINS4			GPIO_Pins_3		//TIM2_CH2

#define OUTPUT_PORT5			GPIOB
#define OUTPUT_PINS5			GPIO_Pins_4		//TIM3_CH1

#define OUTPUT_PORT6			GPIOB
#define OUTPUT_PINS6			GPIO_Pins_5		//TIM3_CH2

#define OUTPUT_PORT7			GPIOB
#define OUTPUT_PINS7			GPIO_Pins_6		//TIM4_CH1

#define OUTPUT_PORT8			GPIOB
#define OUTPUT_PINS8			GPIO_Pins_7		//TIM4_CH2

#define OUTPUT_PORT9			GPIOB
#define OUTPUT_PINS9			GPIO_Pins_8		//TIM3_CH1

#define OUTPUT_PORT10			GPIOA
#define OUTPUT_PINS10			GPIO_Pins_15		//TIM4_CH3

#define OUTPUT_PORT11			GPIOD
#define OUTPUT_PINS11			GPIO_Pins_12		//TIM4_CH1

#define OUTPUT_PORT12			GPIOE
#define OUTPUT_PINS12			GPIO_Pins_2

#define OUTPUT_PORT13			GPIOE
#define OUTPUT_PINS13			GPIO_Pins_1

#define OUTPUT_PORT14			GPIOE
#define OUTPUT_PINS14			GPIO_Pins_0

#define OUTPUT_PORT15			GPIOB
#define OUTPUT_PINS15			GPIO_Pins_9		//TIM4_CH4

//pwm out 
#define PWM_PORT1					OUTPUT_PORT2	
#define PWM_PINS1					OUTPUT_PINS2	//TIM5_CH1
#define DIR_PORT1					OUTPUT_PORT1
#define DIR_PINS1					OUTPUT_PINS1

#define PWM_PORT2					OUTPUT_PORT3
#define PWM_PINS2					OUTPUT_PINS3	//TIM1_CH1
#define DIR_PORT2					OUTPUT_PORT6
#define DIR_PINS2					OUTPUT_PINS6

#define PWM_PORT3					OUTPUT_PORT4
#define PWM_PINS3					OUTPUT_PINS4	//TIM2_CH2
#define DIR_PORT3					OUTPUT_PORT5
#define DIR_PINS3					OUTPUT_PINS5

#define PWM_PORT4					OUTPUT_PORT7
#define PWM_PINS4					OUTPUT_PINS7	//TIM4_CH1
#define DIR_PORT4					OUTPUT_PORT8
#define DIR_PINS4					OUTPUT_PINS8


//input
#define INPUT_PORT1					GPIOD
#define INPUT_PINS1					GPIO_Pins_10

#define INPUT_PORT2					GPIOD
#define INPUT_PINS2					GPIO_Pins_9

#define INPUT_PORT3					GPIOD
#define INPUT_PINS3					GPIO_Pins_8

#define INPUT_PORT4					GPIOC
#define INPUT_PINS4					GPIO_Pins_9

#define INPUT_PORT5					GPIOC
#define INPUT_PINS5					GPIO_Pins_8

#define INPUT_PORT6					GPIOD
#define INPUT_PINS6					GPIO_Pins_15

#define INPUT_PORT7					GPIOD
#define INPUT_PINS7					GPIO_Pins_14

#define INPUT_PORT8					GPIOD
#define INPUT_PINS8					GPIO_Pins_13

#define INPUT_PORT9					GPIOD
#define INPUT_PINS9					GPIO_Pins_11

#define INPUT_PORT10				GPIOC
#define INPUT_PINS10				GPIO_Pins_11

#define INPUT_PORT11				GPIOE
#define INPUT_PINS11				GPIO_Pins_11

#define INPUT_PORT12				GPIOC
#define INPUT_PINS12				GPIO_Pins_10

#define INPUT_PORT13				GPIOA
#define INPUT_PINS13				GPIO_Pins_12

#define INPUT_PORT14				GPIOA
#define INPUT_PINS14				GPIO_Pins_11

#define INPUT_PORT15				GPIOA
#define INPUT_PINS15				GPIO_Pins_8

#define INPUT_PORT16				GPIOD
#define INPUT_PINS16				GPIO_Pins_7

#define INPUT_PORT17				GPIOD
#define INPUT_PINS17				GPIO_Pins_6

#define INPUT_PORT18				GPIOD
#define INPUT_PINS18				GPIO_Pins_5

#define INPUT_PORT19				GPIOD
#define INPUT_PINS19				GPIO_Pins_4

#define INPUT_PORT20				GPIOD
#define INPUT_PINS20				GPIO_Pins_3

#define INPUT_PORT21				GPIOD
#define INPUT_PINS21				GPIO_Pins_2


///////////////////////////////////////////////////////////////////////////
// Attribute
///////////////////////////////////////////////////////////////////////////
//public:



//private:




///////////////////////////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////////////////////////
//public:
	//GPIO≈‰÷√
	extern void GPIO_Configuration(void);


//private:
	//
	static void InitSysPortAStatusProc(void);
	static void InitSysPortBStatusProc(void);
	static void InitSysPortCStatusProc(void);
	static void InitSysPortDStatusProc(void);
	static void InitSysPortEStatusProc(void);
	static void InitSysPortFStatusProc(void);
	static void InitSysPortGStatusProc(void);
	static void InitSysAllPortStatusProc(void);

#endif

