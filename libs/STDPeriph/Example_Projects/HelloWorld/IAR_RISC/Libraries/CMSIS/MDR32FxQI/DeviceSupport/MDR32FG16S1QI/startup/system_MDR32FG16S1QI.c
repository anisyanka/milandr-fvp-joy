/**
  * FILE system_MDR32FG16S1QI.c
  */


/** @addtogroup __CMSIS CMSIS
  * @{
  */

/** @addtogroup MDR32FG16S1QI MDR32FG16S1QI
  * @{
  */

/** @addtogroup MDR32FG16S1QI_System MDR32FG16S1QI System
  * @{
  */

#include "MDR32FG16S1QI.h"
#include "MDR32FxQI_config.h"

/** @addtogroup __MDR32FG16S1QI_System_Private_Defines MDR32FG16S1QI System Private Defines
  * @{
  */

#if defined (__ARMCC_VERSION) /* ARM Compiler */
    extern uint32_t __Vectors;
    #define __VECTOR_TABLE_ADDRESS &__Vectors
#elif defined (__ICCARM__) /* IAR Compiler */
    extern uint32_t __vector_table;
    #define __VECTOR_TABLE_ADDRESS &__vector_table
#endif

/** @} */ /* End of group __MDR32FG16S1QI_System_Private_Defines */

/** @addtogroup __MDR32FG16S1QI_System_Private_Variables MDR32FG16S1QI System Private Variables
  * @{
  */

/*******************************************************************************
*  Clock Definitions
*******************************************************************************/
uint32_t SystemCoreClock = (uint32_t)8000000;   /*!< System Clock Frequency (Core Clock)
                                                 *   default value */

/** @} */ /* End of group __MDR32FG16S1QI_System_Private_Variables */

/** @addtogroup __MDR32FG16S1QI_System_Private_Functions MDR32FG16S1QI System Private Functions
  * @{
  */

/**
  * @brief  Update SystemCoreClock according to Clock Register Values
  * @note   None
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
    uint32_t cpu_c1_freq, cpu_c2_freq, cpu_c3_freq;
    uint32_t pll_mul;

    /* Compute CPU_CLK frequency */

    /* Determine CPU_C1 frequency */
    if ((MDR_RST_CLK->CPU_CLOCK & (uint32_t)0x00000002) == (uint32_t)0x00000002)
    {
        cpu_c1_freq = HSE_Value;
    }
    else
    {
        cpu_c1_freq = HSI_Value;
    }

    if ((MDR_RST_CLK->CPU_CLOCK & (uint32_t)0x00000001) == (uint32_t)0x00000001)
    {
        cpu_c1_freq /= 2;
    }

    /* Determine CPU_C2 frequency */
    cpu_c2_freq = cpu_c1_freq;

    if ((MDR_RST_CLK->CPU_CLOCK & (uint32_t)0x00000004) == (uint32_t)0x00000004)
    {
        /* Determine CPU PLL output frequency */
        pll_mul = ((MDR_RST_CLK->PLL_CONTROL >> 8) & (uint32_t)0x0F) + 1;
        cpu_c2_freq *= pll_mul;
    }

    /*Select CPU_CLK from HSI, CPU_C3, LSE, LSI cases */
    switch ((MDR_RST_CLK->CPU_CLOCK >> 8) & (uint32_t)0x03)
    {
        case 0 :
            /* HSI */
            SystemCoreClock = HSI_Value;
            break;
        case 1 :
            /* CPU_C3 */
            /* Determine CPU_C3 frequency */
            cpu_c3_freq = cpu_c2_freq / ((MDR_RST_CLK->CPU_CLOCK >> 4 & (uint32_t)0x0F) + 1);
            SystemCoreClock = cpu_c3_freq;
            break;
        case 2 :
            /* LSE */
            SystemCoreClock = LSE_Value;
            break;
        default : /* case 3 */
            /* LSI */
            SystemCoreClock = LSI_Value;
            break;
    }
}

/**
  * @brief  Setup the microcontroller system RST clock configuration
  *         to the default reset state. Setup SystemCoreClock variable.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
    /* Configure the Vector Table location address */
    SCB->VTOR = (uint32_t) __VECTOR_TABLE_ADDRESS;

    /* Reset the RST clock configuration to the default reset state */

    /* Reset all clock but RST_CLK bit */
    MDR_RST_CLK->PER_CLOCK     = (uint32_t)0x00000010;

    /* Reset CPU_CLOCK bits */
    MDR_RST_CLK->CPU_CLOCK     = (uint32_t)0x00000000;

    /* Reset PLL_CONTROL bits */
    MDR_RST_CLK->PLL_CONTROL   = (uint32_t)0x00000000;

    /* Reset HSEON and HSEBYP bits */
    MDR_RST_CLK->HS_CONTROL    = (uint32_t)0x00000000;

    /* Reset USB_CLOCK bits */
    MDR_RST_CLK->USB_CLOCK     = (uint32_t)0x00000000;

    /* Reset ADC_MCO_CLOCK bits */
    MDR_RST_CLK->ADC_MCO_CLOCK = (uint32_t)0x00000000;

    /* Reset RTC_CLOCK bits */
    MDR_RST_CLK->RTC_CLOCK     = (uint32_t)0x00000000;

    /* Reset TIM_CLOCK bits */
    MDR_RST_CLK->TIM_CLOCK     = (uint32_t)0x00000000;

    /* Reset UART_CLOCK bits */
    MDR_RST_CLK->UART_CLOCK    = (uint32_t)0x00000000;

    /* Reset SSP_CLOCK bits */
    MDR_RST_CLK->SSP_CLOCK     = (uint32_t)0x00000000;

    /* Reset DSP_CLOCK bits */
    MDR_RST_CLK->DSP_CLOCK     = (uint32_t)0x00000000;

    /* Reset SPP2_CLOCK bits */
    MDR_RST_CLK->SPP2_CLOCK    = (uint32_t)0x00000000;

    SystemCoreClockUpdate();
}

/** @} */ /* End of group __MDR32FG16S1QI_System_Private_Functions */

/** @} */ /* End of group MDR32FG16S1QI_System */

/** @} */ /* End of group MDR32FG16S1QI */

/** @} */ /* End of group __CMSIS */

/*
*
* END OF FILE system_MDR32FG16S1QI.c */

