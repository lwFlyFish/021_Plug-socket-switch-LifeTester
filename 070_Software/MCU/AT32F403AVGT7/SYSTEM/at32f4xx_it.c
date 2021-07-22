/**************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  *************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  *************************************************************************
  */

///////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////
#include "RealDatabase.h"

///////////////////////////////////////////////////////////////////////////

void NMI_Handler(void)
{

}


void HardFault_Handler(void)
{
     /* Go to infinite loop when Hard Fault exception occurs */
     while(1)
     {
     }
}

void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while(1)
    {

    }
}


void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while(1)
    {

    }
}

void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while(1)
    {

    }
}

void SVC_Handler(void)
{

}

void DebugMon_Handler(void)
{

}

void PendSV_Handler(void)
{

}

///////////////////////////////////////////////////////////////////////////

void WWDG_IRQHandler(void)
{
    ;
}

void PVD_IRQHandler(void)
{
    ;
}

void TAMPER_IRQHandler(void)
{
    ;
}


void FLASH_IRQHandler(void)
{
    ;
}

void RCC_CRS_IRQHandler(void)
{
    ;
}

void EXTI0_IRQHandler(void)
{
    ;
}

void EXTI1_IRQHandler(void)
{
    ;
}

void EXTI2_IRQHandler(void)
{
    ;
}

void EXTI3_IRQHandler(void)
{
    ;
}

void EXTI4_IRQHandler(void)
{
    ;
}

// void DMA1_Channel1_IRQHandler(void)
// {
//  ;
// }

void ADC1_IRQHandler(void)
{
    ;
}

void CAN1_RX_IRQHandler(void)
{
    ;
}

void EXTI9_5_IRQHandler(void)
{
    ;
}

void TIM1_BRK_IRQHandler(void)
{
    ;
}

void TIM1_UP_IRQHandler(void)
{
    ;
}

void TIM1_TRG_COM_IRQHandler(void)
{
    ;
}

void TIM1_CC_IRQHandler(void)
{
    ;
}

void TIM2_IRQHandler(void)
{
    ;
}

void TIM3_IRQHandler(void)
{
    ;
}

void TIM4_IRQHandler(void)
{
    ;
}

void I2C1_IRQHandler(void)
{
    ;
}

void I2C2_IRQHandler(void)
{
    ;
}

void SPI1_IRQHandler(void)
{
    ;
}

void SPI2_IRQHandler(void)
{
    ;
}

void UART1_IRQHandler(void)
{
    ;
}

void UART2_IRQHandler(void)
{
    ;
}


void EXTI15_10_IRQHandler(void)
{
    ;
}

void RTCAlarm_IRQHandler(void)
{
    ;
}

void USB_WKUP_IRQHandler(void)
{
    ;
}

void TIM8_BRK_IRQHandler(void)
{
    ;
}

void TIM8_UP_IRQHandler(void)
{
    ;
}

void TIM8_TRG_COM_IRQHandler(void)
{
    ;
}

void TIM8_CC_IRQHandler(void)
{
    ;
}

void SDIO_IRQHandler(void)
{
    ;
}

void TIM5_IRQHandler(void)
{
    ;
}

void SPI3_IRQHandler(void)
{
    ;
}

void UART4_IRQHandler(void)
{
    ;
}

void UART5_IRQHandler(void)
{
    ;
}

void TIM6_IRQHandler(void)
{
    ;
}

void TIM7_IRQHandler(void)
{
    ;
}


void DMA2_Channel4_IRQHandler(void)
{
    ;
}


void ETHERNET_MAC_IRQHandler(void)
{
    ;
}

void COMP1_2_IRQHandler(void)
{
    ;
}

void USB_OTG_FS_IRQHandler(void)
{
    ;
}



void AES_IRQHandler(void)
{
    ;
}

void TRNG_IRQHandler(void)
{
    ;
}

void UART7_IRQHandler(void)
{
    ;
}

void UART8_IRQHandler(void)
{
    ;
}


//void RTC_IRQHandler(void)
//{
//
//}



/*
//???4
void TIM4_IRQHandler(void)
{
    //?????TIM??????:TIM ???
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        //??TIMx???????:TIM ???
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}*/

///////////////////////////////////////////////////////////////////////////
//1ms??
///////////////////////////////////////////////////////////////////////////
void SysTick_Handler(void)
{
    //??led
    if(g_nSysLedRunCount > 0)
    {
        g_nSysLedRunCount--;
    }
    //??led
    if(g_nSysLedBlinkCount > 0)
    {
        g_nSysLedBlinkCount--;
    }

    //
    if(g_nAdcPeriodFilterCount > 0)
    {
        g_nAdcPeriodFilterCount--;
    }

    //
    if(g_nRtcReadTimerCount > 0)
    {
        g_nRtcReadTimerCount--;
    }

    ///////////////////////////////////////////////////////////////////////
    //?????
    if(g_nSysProcTimerCtrl > 0)
    {
        g_nSysProcTimerCtrl--;
    }

    if(g_nSysStaticFreshTimerCtrl > 0)
    {
        g_nSysStaticFreshTimerCtrl--;
    }

    //???????
    if(g_nSysDisplayTimeCtrl > 0)
    {
        g_nSysDisplayTimeCtrl--;
    }

    //1200ms??
    if(g_nSysTurnBackTimeCtrl > 0)
    {
        g_nSysTurnBackTimeCtrl--;
    }

    //////////////////////////////////////uart1

    //
    if(g_nSP1RecvDelay > 0)
    {
        g_nSP1RecvDelay--;
    }

    ///////////////////////////////////////uart2
    //
    if(g_nSP2RecvDelay > 0)
    {
        g_nSP2RecvDelay--;
    }
}


