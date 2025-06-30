/**
 *******************************************************************************
 * @file    MDR32VF0xI_rst_clk.c
 * @author  Milandr Application Team
 * @version V0.1.0
 * @date    18/04/2024
 * @brief   This file contains all the RST_CLK firmware functions.
 *******************************************************************************
 * <br><br>
 *
 * THE PRESENT FIRMWARE IS FOR GUIDANCE ONLY. IT AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING MILANDR'S PRODUCTS IN ORDER TO FACILITATE
 * THE USE AND SAVE TIME. MILANDR SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES RESULTING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR A USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2024 Milandr</center></h2>
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "MDR32VF0xI_rst_clk.h"

/** @addtogroup MDR32VF0xI_StdPeriph_Driver MDR32VF0xI Standard Peripheral Driver
 * @{
 */

/** @defgroup RST_CLK RST_CLK
 * @{
 */

/** @defgroup RST_CLK_Exported_Functions RST_CLK Exported Functions
 * @{
 */

/**
 * @brief  Set the RST_CLK clock configuration to the default reset state.
 * @param  None.
 * @return None.
 */
void RST_CLK_DeInit(void)
{
    uint32_t BKP_OldKey;

    /* Reset all peripheral clocks except RST_CLK and BKP_CLK. */
    MDR_RST_CLK->PER2_CLOCK = (RST_CLK_PER2_CLOCK_PCLK_EN_RST_CLK | RST_CLK_PER2_CLOCK_PCLK_EN_BKP);

    /* Read BKP key to variable. */
    BKP_OldKey = MDR_BKP->WPR;
    /* Write key to BKP_WPR to unlock access if key is not written. */
    if (BKP_OldKey != BKP_WPR_BKP_WPR_UNLOCK) {
        MDR_BKP->WPR = BKP_WPR_BKP_WPR_UNLOCK;
    }

    /* Enable HSI in BKP because RST_CLK deinitialization changes HCLK source to HSI. */
    MDR_BKP->CLK |= BKP_CLK_HSI_ON;

    /* Wait until HSI is ready. */
    while ((MDR_BKP->CLK & BKP_CLK_HSI_RDY) == RESET) { }

    /* Lock BKP access if it was locked before function call. */
    if (BKP_OldKey != BKP_WPR_BKP_WPR_UNLOCK) {
        MDR_BKP->WPR = BKP_OldKey;
    }

    /* Reset all peripheral clocks except RST_CLK. */
    MDR_RST_CLK->PER2_CLOCK = RST_CLK_PER2_CLOCK_PCLK_EN_RST_CLK;

    MDR_RST_CLK->ADC_CLOCK = (uint32_t)0x00000000;
    MDR_RST_CLK->RTC_CLOCK = (uint32_t)0x00000000;

    /* Reset everything except DMA_EN bit. */
    MDR_RST_CLK->PER1_CLOCK = RST_CLK_PER1_CLOCK_DMA_EN;

    MDR_RST_CLK->TIM_CLOCK  = (uint32_t)0x00000000;
    MDR_RST_CLK->UART_CLOCK = (uint32_t)0x00000000;
    MDR_RST_CLK->SSP_CLOCK  = (uint32_t)0x00000000;

#if defined(USE_MDR32F02_REV_2) || defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
    MDR_RST_CLK->DMA_DONE_STICK = (uint32_t)0x00000000;
    MDR_RST_CLK->DIV_SYS_TIM    = (uint32_t)0x00000000;
#endif

    /* Reset CPU_CLOCK bits. */
#if defined(USE_MDR32F02)
    /* HSI is selected as HCLK. */
    MDR_RST_CLK->CPU_CLOCK = (uint32_t)0x00000000;
#elif defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
    /* CPU_C3 (from HSI) divided by 16 is selected as HCLK. */
    MDR_RST_CLK->CPU_CLOCK = ((uint32_t)RST_CLK_CPU_C3_PRESCALER_DIV_16 | RST_CLK_CPU_CLOCK_HCLK_SEL_CPU_C3);
#endif
    MDR_RST_CLK->PLL_CONTROL = (uint32_t)0x00000000;
    MDR_RST_CLK->HS_CONTROL  = (uint32_t)0x00000000;
}

/**
 * @brief  HSE (High Speed External) clock mode and source selection based on @ref HSE_EXT_GEN_MODE definition.
 * @note   @ref HSE_EXT_GEN_MODE definition (MDR32VF0xI_config.h) should be set in valid state.
 * @param  NewState: @ref FunctionalState - a new state of the HSE clock.
 * @return None.
 */
void RST_CLK_HSE_Cmd(FunctionalState NewState)
{
    /* Check the parameters. */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
#if (HSE_EXT_GEN_MODE == 0)
        MDR_RST_CLK->HS_CONTROL = (MDR_RST_CLK->HS_CONTROL & ~RST_CLK_HS_CONTROL_HSE_BYP) | RST_CLK_HS_CONTROL_HSE_ON;
#else
        MDR_RST_CLK->HS_CONTROL |= (RST_CLK_HS_CONTROL_HSE_BYP | RST_CLK_HS_CONTROL_HSE_ON);
#endif
    } else {
#if (HSE_EXT_GEN_MODE == 0)
        MDR_RST_CLK->HS_CONTROL &= ~(RST_CLK_HS_CONTROL_HSE_BYP | RST_CLK_HS_CONTROL_HSE_ON);
#else
        MDR_RST_CLK->HS_CONTROL = (MDR_RST_CLK->HS_CONTROL | RST_CLK_HS_CONTROL_HSE_BYP) & ~RST_CLK_HS_CONTROL_HSE_ON;
#endif
    }
}

/**
 * @brief  HSE (High Speed External) clock mode and source selection.
 * @param  HSE_Mode: @ref RST_CLK_HSE_Mode_TypeDef - mode selector.
 * @return None.
 */
void RST_CLK_HSE_Config(RST_CLK_HSE_Mode_TypeDef HSE_Mode)
{
    /* Check the parameters. */
    assert_param(IS_RST_CLK_HSE(HSE_Mode));

    /* Reset HSEON and HSEBYP bits before configuring the HSE */
    MDR_RST_CLK->HS_CONTROL &= ~(RST_CLK_HS_CONTROL_HSE_ON | RST_CLK_HS_CONTROL_HSE_BYP);

    MDR_RST_CLK->HS_CONTROL |= (uint32_t)HSE_Mode;
}

/**
 * @brief  Get the HSE clock mode and source selection.
 * @param  None.
 * @return @ref RST_CLK_HSE_Mode_TypeDef - current HSE mode mode and source selection.
 */
RST_CLK_HSE_Mode_TypeDef RST_CLK_HSE_GetModeConfig(void)
{
    return (RST_CLK_HSE_Mode_TypeDef)(MDR_RST_CLK->HS_CONTROL &
                                      (RST_CLK_HS_CONTROL_HSE_ON | RST_CLK_HS_CONTROL_HSE_BYP));
}

/**
 * @brief  Get HSE clock status.
 * @param  None.
 * @return @ref ErrorStatus - SUCCESS if HSE clock is ready, else ERROR.
 */
ErrorStatus RST_CLK_HSE_GetStatus(void)
{
    __IO uint32_t StartCounter = 0;
    ErrorStatus   State;
    FlagStatus    Flag;

    /* Wait until HSE is ready or time out occurs. */
    do {
        Flag = RST_CLK_GetFlagStatus(RST_CLK_FLAG_HSERDY);
        StartCounter++;
    } while ((StartCounter < HSERDY_TIMEOUT_TICK) && (Flag == RESET));

    if (RST_CLK_GetFlagStatus(RST_CLK_FLAG_HSERDY) != RESET) {
        State = SUCCESS;
    } else {
        State = ERROR;
    }

    return State;
}

/**
 * @brief  Configure the CPU_PLL clock source and multiplication factor.
 * @param  PLLCPU_Source: @ref RST_CLK_PLLCPU_ClockSource_TypeDef - specify the PLL entry clock source.
 * @param  PLLCPU_Multiplier: @ref RST_CLK_PLLCPU_Multiplier_TypeDef - specify the PLL multiplication factor.
 * @return None.
 */
void RST_CLK_CPU_PLL_Config(RST_CLK_PLLCPU_ClockSource_TypeDef PLLCPU_Source, uint32_t PLLCPU_Multiplier)
{
    uint32_t Temp;

    /* Check the parameters. */
    assert_param(IS_RST_CLK_PLLCPU_CLOCK_SOURCE(PLLCPU_Source));
    assert_param(IS_RST_CLK_PLLCPU_MUL(PLLCPU_Multiplier));

    /* Select CPUPLL source. */
    if (PLLCPU_Source != RST_CLK_PLLCPU_CLK_SRC_HSE) {
        Temp = MDR_RST_CLK->CPU_CLOCK;
        /* Clear CPU_C1_SEL bits. */
        Temp &= ~RST_CLK_CPU_CLOCK_CPU_C1_SEL_Msk;
        /* Set the CPU_C1_SEL bits. */
        Temp |= PLLCPU_Source;
        /* Store the new value. */
        MDR_RST_CLK->CPU_CLOCK = Temp;
    }

    /* Set CPUPLL multiplier. */
    Temp = MDR_RST_CLK->PLL_CONTROL;
    if (PLLCPU_Source != RST_CLK_PLLCPU_CLK_SRC_HSE) {
        Temp |= RST_CLK_PLL_CONTROL_PLL_CPU_SEL;
    } else {
        Temp &= ~RST_CLK_PLL_CONTROL_PLL_CPU_SEL;
    }
    /* Clear PLLMUL[3:0] bits. */
    Temp &= ~RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Msk;
    /* Set PLLMUL[3:0] bits. */
    Temp |= (PLLCPU_Multiplier << RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Pos);
    /* Store the new value. */
    MDR_RST_CLK->PLL_CONTROL = Temp;
}

/**
 * @brief  Enable or disable the CPU_PLL clock.
 * @param  NewState: @ref FunctionalState - a new state of the CPU_PLL clock.
 * @return None.
 */
void RST_CLK_CPU_PLL_Cmd(FunctionalState NewState)
{
    uint32_t Temp;

    /* Check the parameters. */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    Temp = MDR_RST_CLK->PLL_CONTROL;

    if (NewState != DISABLE) {
        Temp |= RST_CLK_PLL_CONTROL_PLL_CPU_ON;
    } else {
        Temp &= ~RST_CLK_PLL_CONTROL_PLL_CPU_ON;
    }

    MDR_RST_CLK->PLL_CONTROL = Temp;
}

/**
 * @brief  Get the CPU_PLL clock work state.
 * @param  None.
 * @return @ref FunctionalState - the current work state of the CPU_PLL clock.
 */
FunctionalState RST_CLK_CPU_PLL_GetCmdState(void)
{
    FunctionalState State;

    if ((MDR_RST_CLK->PLL_CONTROL & RST_CLK_PLL_CONTROL_PLL_CPU_ON) == 0) {
        State = DISABLE;
    } else {
        State = ENABLE;
    }

    return State;
}

/**
 * @brief  Get CPU_PLL clock status.
 * @param  None.
 * @return @ref ErrorStatus - SUCCESS if CPU_PLL output clock is ready, else ERROR.
 */
ErrorStatus RST_CLK_CPU_PLL_GetStatus(void)
{
    __IO uint32_t StartCounter = 0;
    ErrorStatus   State;
    FlagStatus    Flag;

    /* Wait until CPUPLL is ready or time out is occurred. */
    do {
        Flag = RST_CLK_GetFlagStatus(RST_CLK_FLAG_PLLCPURDY);
        StartCounter++;
    } while ((StartCounter < PLLCPURDY_TIMEOUT_TICK) && (Flag == RESET));

    if (RST_CLK_GetFlagStatus(RST_CLK_FLAG_PLLCPURDY) != RESET) {
        State = SUCCESS;
    } else {
        State = ERROR;
    }

    return State;
}

/**
 * @brief  Select the CPU_C1 clock source.
 * @param  CPU_C1_Source: @ref RST_CLK_CPU_C1_ClockSource_TypeDef - specify the CPU_C1 clock source.
 * @return None.
 */
void RST_CLK_CPU_C1_ClkSelection(RST_CLK_CPU_C1_ClockSource_TypeDef CPU_C1_Source)
{
    uint32_t Temp = 0;

    /* Check the parameters. */
    assert_param(IS_RST_CLK_CPU_C1_CLOCK_SOURCE(CPU_C1_Source));

    Temp = MDR_RST_CLK->CPU_CLOCK;

    /* Clear CPU_C1_SEL bits. */
    Temp &= ~RST_CLK_CPU_CLOCK_CPU_C1_SEL_Msk;

    /* Set the CPU_C1_SEL bits. */
    Temp |= (uint32_t)CPU_C1_Source;

    /* Store the new value. */
    MDR_RST_CLK->CPU_CLOCK = Temp;
}

/**
 * @brief  Select the CPU_C2 clock source.
 * @param  CPU_C2_Source: @ref RST_CLK_CPU_C2_ClockSource_TypeDef - specify the CPU_C2 clock source.
 * @return None.
 */
void RST_CLK_CPU_C2_ClkSelection(RST_CLK_CPU_C2_ClockSource_TypeDef CPU_C2_Source)
{
    uint32_t Temp = 0;

    /* Check the parameters. */
    assert_param(IS_RST_CLK_CPU_C2_CLOCK_SOURCE(CPU_C2_Source));

    Temp = MDR_RST_CLK->CPU_CLOCK;
    Temp &= ~RST_CLK_CPU_CLOCK_CPU_C2_SEL;
    Temp |= (uint32_t)CPU_C2_Source;

    /* Configure CPU_CLOCK register with new value. */
    MDR_RST_CLK->CPU_CLOCK = Temp;
}

/**
 * @brief  Сonfigure the CPU_C3_SEL division factor.
 * @param  CPU_CLK_DivValue: @ref RST_CLK_CPU_C3_Prescaler_TypeDef - specify the CPU_C3_SEL division factor.
 * @return None.
 */
void RST_CLK_CPU_C3_SetPrescaler(RST_CLK_CPU_C3_Prescaler_TypeDef CPU_CLK_DivValue)
{
    uint32_t Temp;

    /* Check the parameters. */
    assert_param(IS_RST_CLK_CPU_C3_DIV(CPU_CLK_DivValue));

    Temp = MDR_RST_CLK->CPU_CLOCK;

    /* Clear CPU_C3_SEL bits. */
    Temp &= ~RST_CLK_CPU_CLOCK_CPU_C3_SEL_Msk;

    /* Set CPU_C3_SEL bits. */
    Temp |= (uint32_t)CPU_CLK_DivValue;

    /* Store the new value. */
    MDR_RST_CLK->CPU_CLOCK = Temp;
}

/**
 * @brief  Select the HCLK clock source.
 * @param  CPU_HCLK_Source: @ref RST_CLK_HCLK_ClockSource_TypeDef - specify the HCLK clock source.
 * @return None.
 */
void RST_CLK_HCLK_ClkSelection(RST_CLK_HCLK_ClockSource_TypeDef CPU_HCLK_Source)
{
    uint32_t Temp;

    /* Check the parameters. */
    assert_param(IS_RST_CLK_HCLK_CLOCK_SOURCE(CPU_HCLK_Source));

    Temp = MDR_RST_CLK->CPU_CLOCK;

    /* Clear CPU_C3_SEL bits. */
    Temp &= ~RST_CLK_CPU_CLOCK_HCLK_SEL_Msk;

    /* Set the CPU_C3_SEL bits. */
    Temp |= (uint32_t)CPU_HCLK_Source;

    /* Store the new value. */
    MDR_RST_CLK->CPU_CLOCK = Temp;
}

/**
 * @brief  Select the ADC_C1 clock source.
 * @param  ADC_CLK_Source: @ref RST_CLK_ADC_ClockSource_TypeDef - specify the ADC_C1 clock source.
 * @return None.
 */
void RST_CLK_ADC_ClkSelection(RST_CLK_ADC_ClockSource_TypeDef ADC_CLK_Source)
{
    uint32_t Temp;

    /* Check the parameters. */
    assert_param(IS_RST_CLK_ADC_CLOCK_SOURCE(ADC_CLK_Source));

    /* Select ADC_CLK source. */
    Temp = MDR_RST_CLK->ADC_CLOCK;

    /* Clear ADC_C1 SEL bits. */
    Temp &= ~(RST_CLK_ADC_CLOCK_ADC_C1_SEL_Msk);

    /* Set ADC_C1 SEL bits. */
    Temp |= (uint32_t)ADC_CLK_Source;

    /* Store the new value. */
    MDR_RST_CLK->ADC_CLOCK = Temp;
}

/**
 * @brief  Сonfigure the ADC_C3_SEL division factor.
 * @param  ADC_CLK_DivValue: @ref RST_CLK_ADC_C3_Prescaler_TypeDef - specify the ADC_C3_SEL division factor.
 * @return None.
 */
void RST_CLK_ADC_SetPrescaler(RST_CLK_ADC_C3_Prescaler_TypeDef ADC_CLK_DivValue)
{
    uint32_t Temp;

    /* Check the parameters. */
    assert_param(IS_RST_CLK_ADC_C3_DIV(ADC_CLK_DivValue));

    Temp = MDR_RST_CLK->ADC_CLOCK;

    /* Clear ADC_C3_SEL bits */
    Temp &= ~RST_CLK_ADC_CLOCK_ADC_C3_SEL_Msk;

    /* Set ADC_C3_SEL bits */
    Temp |= (uint32_t)ADC_CLK_DivValue;

    /* Store the new value */
    MDR_RST_CLK->ADC_CLOCK = Temp;
}

/**
 * @brief  Enable or disable the ADC_CLK clock.
 * @param  NewState: @ref FunctionalState - a new state of the ADC_CLK clock.
 * @return None.
 */
void RST_CLK_ADC_ClkCmd(FunctionalState NewState)
{
    /* Check the parameters. */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Form new value. */
    if (NewState != DISABLE) {
        /* Enable the ADC CLK by setting the ADC_CLK_EN bit in the ADC_CLOCK register. */
        MDR_RST_CLK->ADC_CLOCK |= RST_CLK_ADC_CLOCK_ADC_CLK_EN;
    } else {
        /* Disable the ADC CLK by resetting the ADC_CLK_EN bit in the ADC_CLOCK register. */
        MDR_RST_CLK->ADC_CLOCK &= ~RST_CLK_ADC_CLOCK_ADC_CLK_EN;
    }
}

/**
 * @brief  Get the ADC_CLK clock work state.
 * @param  None.
 * @return @ref FunctionalState - the current work state of the ADC_CLK clock.
 */
FunctionalState RST_CLK_ADC_GetClkCmdState(void)
{
    FunctionalState State;

    if ((MDR_RST_CLK->ADC_CLOCK & RST_CLK_ADC_CLOCK_ADC_CLK_EN) == 0) {
        State = DISABLE;
    } else {
        State = ENABLE;
    }

    return State;
}

/**
 * @brief  Set the RST_CLK_ADCUI clock configuration to the default reset state.
 * @param  None.
 * @return None.
 */
void RST_CLK_ADCUI_ClkDeInit(void)
{
    MDR_RST_CLK->ADC_CLOCK &= ~(RST_CLK_ADC_CLOCK_ADCUI_C1_SEL_Msk |
                                RST_CLK_ADC_CLOCK_ADCUI_C3_SEL_Msk |
                                RST_CLK_ADC_CLOCK_ADCUI_CLK_EN);
}

/**
 * @brief  Select the ADCUI_C1 clock source.
 * @param  ADCUI_CLK_C1_Source: @ref RST_CLK_ADCUI_ClockSource_TypeDef - specify the ADCUI_C1 clock source.
 * @return None.
 */
void RST_CLK_ADCUI_ClkSelection(RST_CLK_ADCUI_ClockSource_TypeDef ADCUI_CLK_C1_Source)
{
    uint32_t Temp = 0;

    /* Check the parameters. */
    assert_param(IS_RST_CLK_ADCUI_CLOCK_SOURCE(ADCUI_CLK_C1_Source));

    /* Select ADC_CLK source. */
    Temp = MDR_RST_CLK->ADC_CLOCK;

    /* Clear ADCUI_C1 SEL bits. */
    Temp &= ~RST_CLK_ADC_CLOCK_ADCUI_C1_SEL_Msk;

    /* Set ADCUI_C1 SEL bits. */
    Temp |= (uint32_t)ADCUI_CLK_C1_Source;

    /* Store the new value. */
    MDR_RST_CLK->ADC_CLOCK = Temp;
}

/**
 * @brief  Configure the ADCUI_C3_SEL division factor.
 * @param  ADCUI_CLK_DivValue: @ref RST_CLK_ADCUI_C3_Prescaler_TypeDef - specify the ADCUI_C3_SEL division factor.
 * @return None.
 */
void RST_CLK_ADCUI_SetPrescaler(RST_CLK_ADCUI_C3_Prescaler_TypeDef ADCUI_CLK_DivValue)
{
    uint32_t Temp = 0;

    /* Check the parameters. */
    assert_param(IS_RST_CLK_ADCUI_C3_DIV(ADCUI_CLK_DivValue));

    Temp = MDR_RST_CLK->ADC_CLOCK;

    /* Clear ADCUI_C3_SEL bits. */
    Temp &= ~RST_CLK_ADC_CLOCK_ADCUI_C3_SEL_Msk;

    /* Set ADCUI_C3_SEL bits. */
    Temp |= (uint32_t)ADCUI_CLK_DivValue;

    /* Store the new value. */
    MDR_RST_CLK->ADC_CLOCK = Temp;
}

/**
 * @brief  Enable or disable the ADCUI_CLK clock.
 * @param  NewState: @ref FunctionalState - a new state of the ADCUI_CLK clock.
 * @return None.
 */
void RST_CLK_ADCUI_ClkCmd(FunctionalState NewState)
{
    /* Check the parameters. */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Form a new value. */
    if (NewState != DISABLE) {
        /* Enable ADCUI_CLK clock by setting the ADCUI_CLK_EN bit in the ADC_CLOCK register. */
        MDR_RST_CLK->ADC_CLOCK |= RST_CLK_ADC_CLOCK_ADCUI_CLK_EN;
    } else {
        /* Disable ADCUI_CLK clock by resetting the ADCUI_CLK_EN bit in the ADC_CLOCK register. */
        MDR_RST_CLK->ADC_CLOCK &= ~RST_CLK_ADC_CLOCK_ADCUI_CLK_EN;
    }
}

/**
 * @brief  Get the ADCUI_CLK clock work state.
 * @param  None.
 * @return @ref FunctionalState - the current work state of the ADCUI_CLK clock.
 */
FunctionalState RST_CLK_ADCUI_GetClkCmdState(void)
{
    FunctionalState State;

    if ((MDR_RST_CLK->ADC_CLOCK & RST_CLK_ADC_CLOCK_ADCUI_CLK_EN) == 0) {
        State = DISABLE;
    } else {
        State = ENABLE;
    }

    return State;
}

/**
 * @brief  Configure the HSI_C1_SEL division factor.
 * @param  HSI_CLK_DivValue: @ref RST_CLK_HSI_C1_Prescaler_TypeDef - specify the HSI_C1_SEL division factor.
 * @return None.
 */
void RST_CLK_HSI_C1_SetPrescaler(RST_CLK_HSI_C1_Prescaler_TypeDef HSI_CLK_DivValue)
{
    uint32_t Temp;

    /* Check the parameters. */
    assert_param(IS_RST_CLK_HSI_CLK_DIV(HSI_CLK_DivValue));

    Temp = MDR_RST_CLK->RTC_CLOCK;

    /* Clear HSI_SEL bits. */
    Temp &= ~RST_CLK_RTC_CLOCK_HSI_SEL_Msk;

    /* Set HSI_SEL bits. */
    Temp |= (uint32_t)HSI_CLK_DivValue;

    /* Store the new value. */
    MDR_RST_CLK->RTC_CLOCK = Temp;
}

/**
 * @brief  Enable or disable the RTCHSI clock.
 * @param  NewState: @ref FunctionalState - a new state of the RTCHSI clock.
 * @return None.
 */
void RST_CLK_RTC_HSI_ClkCmd(FunctionalState NewState)
{
    /* Check the parameters. */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        /* Enable HSI RTC clock by setting the HSE_RTC_EN bit in the RTC_CLOCK register. */
        MDR_RST_CLK->RTC_CLOCK |= RST_CLK_RTC_CLOCK_HSI_RTC_EN;
    } else {
        /* Disable HSI RTC clock by resetting the HSE_RTC_EN bit in the RTC_CLOCK register. */
        MDR_RST_CLK->RTC_CLOCK &= ~RST_CLK_RTC_CLOCK_HSI_RTC_EN;
    }
}

/**
 * @brief  Get the RTCHSI clock work state.
 * @param  None.
 * @return @ref FunctionalState - the current work state of the RTCHSI clock.
 */
FunctionalState RST_CLK_RTC_HSI_GetClkCmdState(void)
{
    FunctionalState State;

    if ((MDR_RST_CLK->RTC_CLOCK & RST_CLK_RTC_CLOCK_HSI_RTC_EN) == 0) {
        State = DISABLE;
    } else {
        State = ENABLE;
    }

    return State;
}

/**
 * @brief  Configure the HSE_C1_SEL division factor.
 * @param  HSE_CLK_DivValue: @ref RST_CLK_HSE_C1_Prescaler_TypeDef - specify the HSE_C1_SEL division factor.
 * @return None.
 */
void RST_CLK_HSE_C1_SetPrescaler(RST_CLK_HSE_C1_Prescaler_TypeDef HSE_CLK_DivValue)
{
    uint32_t Temp;

    /* Check the parameters. */
    assert_param(IS_RST_CLK_HSE_CLK_DIV(HSE_CLK_DivValue));

    Temp = MDR_RST_CLK->RTC_CLOCK;

    /* Clear HSE_SEL bits. */
    Temp &= ~RST_CLK_RTC_CLOCK_HSE_SEL_Msk;

    /* Set HSE_SEL bits. */
    Temp |= (uint32_t)HSE_CLK_DivValue;

    /* Store the new value. */
    MDR_RST_CLK->RTC_CLOCK = Temp;
}

/**
 * @brief  Enable or disable the RTCHSE clock.
 * @param  NewState: @ref FunctionalState - a new state of the HSE_RTC_EN bit.
 * @return None.
 */
void RST_CLK_RTC_HSE_ClkCmd(FunctionalState NewState)
{
    /* Check the parameters. */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        /* Enable HSI RTC clock by setting the HSE_RTC_EN bit in the RTC_CLOCK register. */
        MDR_RST_CLK->RTC_CLOCK |= RST_CLK_RTC_CLOCK_HSE_RTC_EN;
    } else {
        /* Disable HSI RTC clock by resetting the HSE_RTC_EN bit in the RTC_CLOCK register. */
        MDR_RST_CLK->RTC_CLOCK &= ~RST_CLK_RTC_CLOCK_HSE_RTC_EN;
    }
}

/**
 * @brief  Get the RTCHSE clock work state.
 * @param  None.
 * @return @ref FunctionalState - the current work state of the RTCHSE clock.
 */
FunctionalState RST_CLK_RTC_HSE_GetClkCmdState(void)
{
    FunctionalState State;

    if ((MDR_RST_CLK->RTC_CLOCK & RST_CLK_RTC_CLOCK_HSE_RTC_EN) == 0) {
        State = DISABLE;
    } else {
        State = ENABLE;
    }

    return State;
}

/**
 * @brief  Select a clock source for PER1_C1.
 * @param  PER1_C1_Source: @ref RST_CLK_PER1_C1_ClockSource_TypeDef - specify PER1_C1 clock source.
 * @return None.
 */
void RST_CLK_PER1_C1_ClkSelection(RST_CLK_PER1_C1_ClockSource_TypeDef PER1_C1_Source)
{
    /* Check the parameters. */
    assert_param(IS_RST_CLK_PER1_C1_CLOCK_SOURCE(PER1_C1_Source));

    /* Clear an old clock source and set a new one. */
    MDR_RST_CLK->PER1_CLOCK = (MDR_RST_CLK->PER1_CLOCK & (~RST_CLK_PER1_CLOCK_PER1_C1_SEL_Msk)) |
                              (uint32_t)PER1_C1_Source;
}

/**
 * @brief  Select a clock source for PER1_C2.
 * @param  PER1_C2_Source: @ref RST_CLK_PER1_C2_ClockSource_TypeDef - specify PER1_C2 clock source.
 * @return None.
 */
void RST_CLK_PER1_C2_ClkSelection(RST_CLK_PER1_C2_ClockSource_TypeDef PER1_C2_Source)
{
    /* Check the parameters. */
    assert_param(IS_RST_CLK_PER1_C2_CLOCK_SOURCE(PER1_C2_Source));

    MDR_RST_CLK->PER1_CLOCK = (MDR_RST_CLK->PER1_CLOCK & (~RST_CLK_PER1_CLOCK_PER1_C2_SEL_Msk)) |
                              (uint32_t)PER1_C2_Source;
}

/**
 * @brief  Configure division factor for @ref RST_CLK_PER1_C2_X_TypeDef peripheral clock.
 * @param  PER1_CLK: @ref RST_CLK_PER1_C2_X_TypeDef - specify peripheral clock.
 * @param  PER1_C2_DivValue: @ref RST_CLK_PER1_C2_Prescaler_TypeDef - specify a prescaler.
 * @return None.
 */
void RST_CLK_PER1_C2_SetPrescaler(RST_CLK_PER1_C2_X_TypeDef PER1_CLK, RST_CLK_PER1_C2_Prescaler_TypeDef PER1_C2_DivValue)
{
    /* Check the parameters. */
    assert_param(IS_RST_CLK_PER1_CLK(PER1_CLK));
    assert_param(IS_RST_CLK_PER1_C2_DIV(PER1_C2_DivValue));

    switch (PER1_CLK) {
        case RST_CLK_PER1_C2_UART1:
            MDR_RST_CLK->UART_CLOCK = (MDR_RST_CLK->UART_CLOCK & (~RST_CLK_UART_CLOCK_UART1_BRG_Msk)) |
                                      ((uint32_t)PER1_C2_DivValue << RST_CLK_UART_CLOCK_UART1_BRG_Pos);
            break;
        case RST_CLK_PER1_C2_UART2:
            MDR_RST_CLK->UART_CLOCK = (MDR_RST_CLK->UART_CLOCK & (~RST_CLK_UART_CLOCK_UART2_BRG_Msk)) |
                                      ((uint32_t)PER1_C2_DivValue << RST_CLK_UART_CLOCK_UART2_BRG_Pos);
            break;
        case RST_CLK_PER1_C2_UART3:
            MDR_RST_CLK->UART_CLOCK = (MDR_RST_CLK->UART_CLOCK & (~RST_CLK_UART_CLOCK_UART3_BRG_Msk)) |
                                      ((uint32_t)PER1_C2_DivValue << RST_CLK_UART_CLOCK_UART3_BRG_Pos);
            break;
        case RST_CLK_PER1_C2_UART4:
            MDR_RST_CLK->UART_CLOCK = (MDR_RST_CLK->UART_CLOCK & (~RST_CLK_UART_CLOCK_UART4_BRG_Msk)) |
                                      ((uint32_t)PER1_C2_DivValue << RST_CLK_UART_CLOCK_UART4_BRG_Pos);
            break;
        case RST_CLK_PER1_C2_TIM1:
            MDR_RST_CLK->TIM_CLOCK = (MDR_RST_CLK->TIM_CLOCK & (~RST_CLK_TIM_CLOCK_TIM1_BRG_Msk)) |
                                     ((uint32_t)PER1_C2_DivValue << RST_CLK_TIM_CLOCK_TIM1_BRG_Pos);
            break;
        case RST_CLK_PER1_C2_TIM2:
            MDR_RST_CLK->TIM_CLOCK = (MDR_RST_CLK->TIM_CLOCK & (~RST_CLK_TIM_CLOCK_TIM2_BRG_Msk)) |
                                     ((uint32_t)PER1_C2_DivValue << RST_CLK_TIM_CLOCK_TIM2_BRG_Pos);
            break;
        case RST_CLK_PER1_C2_TIM3:
            MDR_RST_CLK->TIM_CLOCK = (MDR_RST_CLK->TIM_CLOCK & (~RST_CLK_TIM_CLOCK_TIM3_BRG_Msk)) |
                                     ((uint32_t)PER1_C2_DivValue << RST_CLK_TIM_CLOCK_TIM3_BRG_Pos);
            break;
        case RST_CLK_PER1_C2_TIM4:
            MDR_RST_CLK->TIM_CLOCK = (MDR_RST_CLK->TIM_CLOCK & (~RST_CLK_TIM_CLOCK_TIM4_BRG_Msk)) |
                                     ((uint32_t)PER1_C2_DivValue << RST_CLK_TIM_CLOCK_TIM4_BRG_Pos);
            break;
        case RST_CLK_PER1_C2_SSP1:
            MDR_RST_CLK->SSP_CLOCK = (MDR_RST_CLK->SSP_CLOCK & (~RST_CLK_SSP_CLOCK_SSP1_BRG_Msk)) |
                                     ((uint32_t)PER1_C2_DivValue << RST_CLK_SSP_CLOCK_SSP1_BRG_Pos);
            break;
        case RST_CLK_PER1_C2_SSP2:
            MDR_RST_CLK->SSP_CLOCK = (MDR_RST_CLK->SSP_CLOCK & (~RST_CLK_SSP_CLOCK_SSP2_BRG_Msk)) |
                                     ((uint32_t)PER1_C2_DivValue << RST_CLK_SSP_CLOCK_SSP2_BRG_Pos);
            break;
        case RST_CLK_PER1_C2_SSP3:
            MDR_RST_CLK->SSP_CLOCK = (MDR_RST_CLK->SSP_CLOCK & (~RST_CLK_SSP_CLOCK_SSP3_BRG_Msk)) |
                                     ((uint32_t)PER1_C2_DivValue << RST_CLK_SSP_CLOCK_SSP3_BRG_Pos);
            break;
    }
}

/**
 * @brief  Enable or disable @ref RST_CLK_PER1_C2_X_TypeDef peripheral clock.
 * @param  PER1_CLK: Specify peripheral clock.
 *         This parameter can be any combination of @ref RST_CLK_PER1_C2_X_TypeDef values.
 * @param  NewState: @ref FunctionalState - a new state of the peripheral clock.
 * @return None.
 */
void RST_CLK_PER1_C2_Cmd(uint32_t PER1_CLK, FunctionalState NewState)
{
    uint32_t Temp;

    /* Check the parameters. */
    assert_param(IS_RST_CLK_PER1_CLKS(PER1_CLK));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Handle UARTx. */
    Temp = PER1_CLK & RST_CLK_PER1_CLK_UART_MASK;
    if (Temp != 0) {
        Temp >>= 0;
        if (NewState != DISABLE) {
            MDR_RST_CLK->UART_CLOCK |= (Temp << RST_CLK_UART_CLOCK_UART1_CLK_EN_Pos);
        } else {
            MDR_RST_CLK->UART_CLOCK &= ~(Temp << RST_CLK_UART_CLOCK_UART1_CLK_EN_Pos);
        }
    }

    /* Handle TIMERx. */
    Temp = PER1_CLK & RST_CLK_PER1_CLK_TIM_MASK;
    if (Temp != 0) {
        Temp >>= 7;
        if (NewState != DISABLE) {
            MDR_RST_CLK->TIM_CLOCK |= (Temp << RST_CLK_TIM_CLOCK_TIM1_CLK_EN_Pos);
        } else {
            MDR_RST_CLK->TIM_CLOCK &= ~(Temp << RST_CLK_TIM_CLOCK_TIM1_CLK_EN_Pos);
        }
    }

    /* Handle SSPx. */
    Temp = PER1_CLK & RST_CLK_PER1_CLK_SSP_MASK;
    if (Temp != 0) {
        Temp >>= 11;
        if (NewState != DISABLE) {
            MDR_RST_CLK->SSP_CLOCK |= (Temp << RST_CLK_SSP_CLOCK_SSP1_CLK_EN_Pos);
        } else {
            MDR_RST_CLK->SSP_CLOCK &= ~(Temp << RST_CLK_SSP_CLOCK_SSP1_CLK_EN_Pos);
        }
    }
}

/**
 * @brief  Select a clock source for WWDG_C1.
 * @param  WWDG_C1_Source: @ref RST_CLK_WWDG_C1_ClockSource_TypeDef - specify WWDG_C1 clock source.
 * @return None.
 */
void RST_CLK_WWDG_C1_ClkSelection(RST_CLK_WWDG_C1_ClockSource_TypeDef WWDG_C1_Source)
{
    /* Check the parameters. */
    assert_param(IS_RST_CLK_WWDG_C1_CLOCK_SOURCE(WWDG_C1_Source));

    /* Clear an old clock source and set a new one. */
    MDR_RST_CLK->PER1_CLOCK = (MDR_RST_CLK->PER1_CLOCK & (~RST_CLK_PER1_CLOCK_WDG_C1_SEL_Msk)) |
                              (uint32_t)WWDG_C1_Source;
}

/**
 * @brief  Select a clock source for WWDG_C2.
 * @param  WWDG_C2_Source: @ref RST_CLK_WWDG_C2_ClockSource_TypeDef - specify WWDG_C2 clock source.
 * @return None.
 */
void RST_CLK_WWDG_C2_ClkSelection(RST_CLK_WWDG_C2_ClockSource_TypeDef WWDG_C2_Source)
{
    /* Check the parameters. */
    assert_param(IS_RST_CLK_WWDG_C2_CLOCK_SOURCE(WWDG_C2_Source));

    MDR_RST_CLK->PER1_CLOCK = (MDR_RST_CLK->PER1_CLOCK & (~RST_CLK_PER1_CLOCK_WDG_C2_SEL_Msk)) |
                              (uint32_t)WWDG_C2_Source;
}

/**
 * @brief  Configure division factor for the WWDG peripheral.
 * @param  WWDG_C2_DivValue: @ref RST_CLK_WWDG_C2_Prescaler_TypeDef - specify the prescaler.
 * @return None.
 */
void RST_CLK_WWDG_C2_SetPrescaler(RST_CLK_WWDG_C2_Prescaler_TypeDef WWDG_C2_DivValue)
{
    /* Check the parameters. */
    assert_param(IS_RST_CLK_WWDG_C2_DIV(WWDG_C2_DivValue));

    MDR_RST_CLK->PER1_CLOCK = (MDR_RST_CLK->PER1_CLOCK & (~RST_CLK_PER1_CLOCK_WDG_BRG_Msk)) |
                              (uint32_t)WWDG_C2_DivValue;
}

/**
 * @brief  Enable or disable the WWDG peripheral clock.
 * @param  NewState: @ref FunctionalState - a new state of the WWDG peripheral clock.
 * @return None.
 */
void RST_CLK_WWDG_C2_Cmd(FunctionalState NewState)
{
    /* Check the parameters. */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        MDR_RST_CLK->PER1_CLOCK |= RST_CLK_PER1_CLOCK_WDG_CLK_EN;
    } else {
        MDR_RST_CLK->PER1_CLOCK &= ~RST_CLK_PER1_CLOCK_WDG_CLK_EN;
    }
}

/**
 * @brief  Set the DMA clock state.
 * @param  NewState: @ref FunctionalState - a new state of the DMA clock.
 * @return None.
 */
void RST_CLK_DMACmd(FunctionalState NewState)
{
    /* Check the parameters. */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        MDR_RST_CLK->PER1_CLOCK |= RST_CLK_PER1_CLOCK_DMA_EN;
    } else {
        MDR_RST_CLK->PER1_CLOCK &= ~RST_CLK_PER1_CLOCK_DMA_EN;
    }
}

/**
 * @brief  Get the DMA clock state.
 * @param  None.
 * @return @ref FunctionalState - the state of the DMA clock.
 */
FunctionalState RST_CLK_GetDMACmdState(void)
{
    FunctionalState DMA_State;

    if (MDR_RST_CLK->PER1_CLOCK & RST_CLK_PER1_CLOCK_DMA_EN) {
        DMA_State = ENABLE;
    } else {
        DMA_State = DISABLE;
    }

    return (FunctionalState)DMA_State;
}

/**
 * @brief  Enable or disable BKP keys RAM reset.
 * @param  NewState: @ref FunctionalState - a new state of BKP keys RAM reset.
 * @return None.
 */
void RST_CLK_ResetKeyRAM_Cmd(FunctionalState NewState)
{
    /* Check the parameters. */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        MDR_RST_CLK->PER1_CLOCK |= RST_CLK_PER1_CLOCK_KEY_RESET_PROG;
    } else {
        MDR_RST_CLK->PER1_CLOCK &= ~RST_CLK_PER1_CLOCK_KEY_RESET_PROG;
    }
}

/**
 * @brief  Get BKP keys RAM reset state.
 * @param  None.
 * @return @ref FunctionalState - BKP keys RAM reset state.
 */
FunctionalState RST_CLK_GetResetKeyRAMCmdState(void)
{
    FunctionalState ResetState;

    if (MDR_RST_CLK->PER1_CLOCK & RST_CLK_PER1_CLOCK_KEY_RESET_PROG) {
        ResetState = ENABLE;
    } else {
        ResetState = DISABLE;
    }

    return (FunctionalState)ResetState;
}

#if defined(USE_MDR32F02_REV_2) || defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)

/**
 * @brief  Check whether the operation of the specified DMA channel is complete or not.
 * @param  Channel: @ref RST_CLK_DMADoneStickChannels_TypeDef - specify the flag to check.
 * @return @ref FlagStatus - Current state of dma_done flag for specified channel (SET or RESET).
 */
FlagStatus RST_CLK_DMADone_GetFlagStatus(RST_CLK_DMADoneStickChannels_TypeDef Channel)
{
    FlagStatus Status;

    /* Check the parameters. */
    assert_param(IS_RST_CLK_DMA_DONE_STICK_CHANNEL(Channel));

    if ((MDR_RST_CLK->DMA_DONE_STICK & (uint32_t)Channel) == 0) {
        Status = RESET;
    } else {
        Status = SET;
    }

    return Status;
}

/**
 * @brief  Return the dma_done combined value of all the channels.
 * @param  None.
 * @return dma_done status flags combined value.
 */
uint32_t RST_CLK_DMADone_GetStatus(void)
{
    return MDR_RST_CLK->DMA_DONE_STICK;
}

/**
 * @brief  Clear dma_done flags for the specified channels.
 * @param  Channels: Specify channels to clear.
 *         This parameter can be any combination of @ref RST_CLK_DMADoneStickChannels_TypeDef values.
 * @return None.
 */
void RST_CLK_DMADone_ClearFlags(uint32_t Channels)
{
    MDR_RST_CLK->DMA_DONE_STICK = ~Channels;
}

/**
 * @brief  Configure a division factor for machine timer.
 * @param  SysTimDivValue: Core machine timer division factor.
 *         This parameter can be a value from 2 to 257.
 * @return None.
 */
void RST_CLK_SystemTimer_SetPrescaler(uint16_t SysTimDivValue)
{
    /* Check the parameters. */
    assert_param(IS_RST_CLK_DIV_SYS_TIM(SysTimDivValue));

    MDR_RST_CLK->DIV_SYS_TIM = SysTimDivValue - 2;
}

/**
 * @brief  Get a division factor for core machine timer.
 * @return SysTimDivValue: Core machine timer division factor (from 2 to 257).
 */
uint16_t RST_CLK_SystemTimer_GetPrescaler(void)
{
    return (uint16_t)(MDR_RST_CLK->DIV_SYS_TIM + 2);
}

#endif

/**
 * @brief  Enable or disable clock of peripherals.
 * @param  RST_CLK_PCLK: Specify the peripheral to gate its clock.
 *         This parameter can be any combination of the @ref RST_CLK_PCLK_TypeDef values.
 * @param  NewState: @ref FunctionalState - a new state of the specified peripheral clock.
 * @return None.
 */
void RST_CLK_PCLKCmd(uint32_t RST_CLK_PCLK, FunctionalState NewState)
{
    /* Check the parameters. */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_RST_CLK_PCLKS(RST_CLK_PCLK));

    if (NewState != DISABLE) {
        MDR_RST_CLK->PER2_CLOCK |= RST_CLK_PCLK;
    } else {
        MDR_RST_CLK->PER2_CLOCK &= ~RST_CLK_PCLK;
    }
}

/**
 * @brief  Get the work state of the specified peripheral PLCK clock.
 * @param  RST_CLK_PCLK: @ref RST_CLK_PCLK_TypeDef - specify the peripheral PCLK clock.
 * @return @ref FunctionalState - the current work state of the specified peripheral PLCK clock.
 */
FunctionalState RST_CLK_GetPCLKCmdState(RST_CLK_PCLK_TypeDef RST_CLK_PCLK)
{
    FunctionalState State;

    /* Check the parameters. */
    assert_param(IS_RST_CLK_PCLK(RST_CLK_PCLK));

    if ((MDR_RST_CLK->PER2_CLOCK & RST_CLK_PCLK) == 0) {
        State = DISABLE;
    } else {
        State = ENABLE;
    }

    return State;
}

/**
 * @brief  Put the values of on-chip clock frequencies into RST_CLK_Clocks structure.
 * @param  RST_CLK_Clocks: The pointer to the @ref RST_CLK_Freq_TypeDef structure which will hold
 *         the clock frequencies values.
 * @return None.
 */
void RST_CLK_GetClocksFreq(RST_CLK_Freq_TypeDef* RST_CLK_Clocks)
{
    uint32_t CPU_C1_Freq = 0, CPU_C2_Freq = 0, CPU_C3_Freq = 0;
    uint32_t ADC_C1_Freq = 0, ADC_C3_Freq = 0;
    uint32_t ADCUI_C1_Freq = 0, ADCUI_C3_Freq = 0;
    uint32_t HSI_C1_Freq = 0, HSE_C1_Freq = 0;
    uint32_t PER_C1_Freq = 0, PER_C2_Freq = 0;
    uint32_t PLL_Mult = 0;
    uint32_t Temp1 = 0, Temp2 = 0;

    /* Check the parameters. */
    assert_param(RST_CLK_Clocks != 0);

    /* Compute the CPU_CLK frequency. */

    Temp1 = MDR_RST_CLK->CPU_CLOCK;

    /* Determine the CPU_C1 frequency. */
    switch (Temp1 & RST_CLK_CPU_CLOCK_CPU_C1_SEL_Msk) {
        case RST_CLK_CPU_C1_CLK_SRC_HSI:
            CPU_C1_Freq = HSI_FREQUENCY_Hz;
            break;
        case RST_CLK_CPU_C1_CLK_SRC_HSI_DIV_2:
            CPU_C1_Freq = HSI_FREQUENCY_Hz / 2;
            break;
        case RST_CLK_CPU_C1_CLK_SRC_HSE:
            CPU_C1_Freq = HSE_FREQUENCY_Hz;
            break;
        case RST_CLK_CPU_C1_CLK_SRC_HSE_DIV_2:
            CPU_C1_Freq = HSE_FREQUENCY_Hz / 2;
            break;
    }

    /* Determine the CPU_C2 frequency. */

    /* Determine the CPU PLL output frequency. */
    Temp2 = MDR_RST_CLK->PLL_CONTROL;
    /* PLL multiplier value. */
    PLL_Mult = ((Temp2 & RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Msk) >> RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Pos) + 1;

    /* Determine the CPU_C2 source. */
    if (Temp1 & RST_CLK_CPU_CLOCK_CPU_C2_SEL) {
        /* Determine the CPU_PLL source. */
        if (Temp2 & RST_CLK_PLL_CONTROL_PLL_CPU_SEL) {
            CPU_C2_Freq = CPU_C1_Freq * PLL_Mult;
        } else {
            CPU_C2_Freq = HSE_FREQUENCY_Hz * PLL_Mult;
        }
    } else {
        CPU_C2_Freq = CPU_C1_Freq;
    }

    /* Select the CPU_CLK from HSI, CPU_C3, LSE, LSI cases. */
    switch (Temp1 & RST_CLK_CPU_CLOCK_HCLK_SEL_Msk) {
        case RST_CLK_CPU_CLOCK_HCLK_SEL_HSI:
            RST_CLK_Clocks->CPU_CLK_Frequency = HSI_FREQUENCY_Hz;
            break;
        case RST_CLK_CPU_CLOCK_HCLK_SEL_CPU_C3:
            /* Determine the CPU_C3 frequency. */
            Temp1 = ((Temp1 & RST_CLK_CPU_CLOCK_CPU_C3_SEL_Msk) >> RST_CLK_CPU_CLOCK_CPU_C3_SEL_Pos);
            if ((Temp1 & ((uint32_t)0x8)) == 0x00) {
                CPU_C3_Freq = CPU_C2_Freq;
            } else {
                CPU_C3_Freq = CPU_C2_Freq >> ((Temp1 & ((uint32_t)0x7)) + 1);
            }
            RST_CLK_Clocks->CPU_CLK_Frequency = CPU_C3_Freq;
            break;
        case RST_CLK_CPU_CLOCK_HCLK_SEL_LSE:
            RST_CLK_Clocks->CPU_CLK_Frequency = LSE_FREQUENCY_Hz;
            break;
        case RST_CLK_CPU_CLOCK_HCLK_SEL_LSI:
            RST_CLK_Clocks->CPU_CLK_Frequency = LSI_FREQUENCY_Hz;
            break;
    }

        /* Compute the machine timer frequency. */
#if defined(USE_MDR32F02_REV_1X)
    RST_CLK_Clocks->MachineTimer_Frequency = LSE_FREQUENCY_Hz;
#else
    Temp1                                  = (MDR_RST_CLK->DIV_SYS_TIM & RST_CLK_DIV_SYS_TIM_DIV_SYS_TIM_Msk) + 2;
    RST_CLK_Clocks->MachineTimer_Frequency = RST_CLK_Clocks->CPU_CLK_Frequency / Temp1;
#endif

    /* Compute the RTCHSE_CLK frequency. */
    Temp1 = MDR_RST_CLK->RTC_CLOCK;
    if ((Temp1 & ((uint32_t)0x8)) == 0) {
        HSE_C1_Freq = HSE_FREQUENCY_Hz;
    } else {
        HSE_C1_Freq = HSE_FREQUENCY_Hz >> ((Temp1 & ((uint32_t)0x7)) + 1);
    }
    RST_CLK_Clocks->RTCHSE_Frequency = HSE_C1_Freq;

    /* Compute the RTCHSI_CLK frequency. */
    Temp1 >>= RST_CLK_RTC_CLOCK_HSI_SEL_Pos;
    if ((Temp1 & ((uint32_t)0x8)) == 0) {
        HSI_C1_Freq = HSI_FREQUENCY_Hz;
    } else {
        HSI_C1_Freq = HSI_FREQUENCY_Hz >> ((Temp1 & ((uint32_t)0x7)) + 1);
    }
    RST_CLK_Clocks->RTCHSI_Frequency = HSI_C1_Freq;

    Temp1 = MDR_RST_CLK->PER1_CLOCK;

    /* Compute WWDG_CLK frequency. */
    switch (Temp1 & RST_CLK_PER1_CLOCK_WDG_C2_SEL_Msk) {
        case RST_CLK_WWDG_C2_CLK_SRC_CPU_C1:
            RST_CLK_Clocks->WWDG_Frequency = CPU_C1_Freq;
            break;
        case RST_CLK_WWDG_C2_CLK_SRC_WWDG_C1:
            switch (Temp1 & RST_CLK_PER1_CLOCK_WDG_C1_SEL_Msk) {
                case RST_CLK_WWDG_C1_CLK_SRC_LSI:
                    RST_CLK_Clocks->WWDG_Frequency = LSI_FREQUENCY_Hz;
                    break;
                case RST_CLK_WWDG_C1_CLK_SRC_LSI_DIV_2:
                    RST_CLK_Clocks->WWDG_Frequency = LSI_FREQUENCY_Hz / 2;
                    break;
                case RST_CLK_WWDG_C1_CLK_SRC_LSE:
                    RST_CLK_Clocks->WWDG_Frequency = LSE_FREQUENCY_Hz;
                    break;
                case RST_CLK_WWDG_C1_CLK_SRC_LSE_DIV_2:
                    RST_CLK_Clocks->WWDG_Frequency = LSE_FREQUENCY_Hz / 2;
                    break;
            }
            break;
        case RST_CLK_WWDG_C2_CLK_SRC_PLLCPU:
            PER_C1_Freq = CPU_C1_Freq * PLL_Mult;
            break;
        case RST_CLK_WWDG_C2_CLK_SRC_HSI_C1_CLK:
            PER_C1_Freq = RST_CLK_Clocks->RTCHSI_Frequency;
            break;
    }
    RST_CLK_Clocks->WWDG_Frequency >>= ((Temp1 & RST_CLK_PER1_CLOCK_WDG_BRG_Msk) >> RST_CLK_PER1_CLOCK_WDG_BRG_Pos);

    /* Determine the clock frequency PER_C1. */
    switch (Temp1 & RST_CLK_PER1_CLOCK_PER1_C1_SEL_Msk) {
        case RST_CLK_PER1_C1_CLK_SRC_LSI:
            PER_C1_Freq = LSI_FREQUENCY_Hz;
            break;
        case RST_CLK_PER1_C1_CLK_SRC_LSI_DIV_2:
            PER_C1_Freq = LSI_FREQUENCY_Hz / 2;
            break;
        case RST_CLK_PER1_C1_CLK_SRC_LSE:
            PER_C1_Freq = LSE_FREQUENCY_Hz;
            break;
        case RST_CLK_PER1_C1_CLK_SRC_LSE_DIV_2:
            PER_C1_Freq = LSE_FREQUENCY_Hz / 2;
            break;
    }

    /* Determine clock frequency PER_C2. */
    switch (Temp1 & RST_CLK_PER1_CLOCK_PER1_C2_SEL_Msk) {
        case RST_CLK_PER1_C2_CLK_SRC_CPU_C1:
            PER_C2_Freq = CPU_C1_Freq;
            break;
        case RST_CLK_PER1_C2_CLK_SRC_PER1_C1:
            PER_C2_Freq = PER_C1_Freq;
            break;
        case RST_CLK_PER1_C2_CLK_SRC_PLLCPU:
            PER_C2_Freq = CPU_C1_Freq * PLL_Mult;
            break;
        case RST_CLK_PER1_C2_CLK_SRC_HSI_C1_CLK:
            PER_C2_Freq = HSI_FREQUENCY_Hz;
            break;
    }

    Temp1                              = MDR_RST_CLK->TIM_CLOCK;
    RST_CLK_Clocks->TIMER_Frequency[0] = PER_C2_Freq >> ((Temp1 & RST_CLK_TIM_CLOCK_TIM1_BRG_Msk) >> RST_CLK_TIM_CLOCK_TIM1_BRG_Pos);
    RST_CLK_Clocks->TIMER_Frequency[1] = PER_C2_Freq >> ((Temp1 & RST_CLK_TIM_CLOCK_TIM2_BRG_Msk) >> RST_CLK_TIM_CLOCK_TIM2_BRG_Pos);
    RST_CLK_Clocks->TIMER_Frequency[2] = PER_C2_Freq >> ((Temp1 & RST_CLK_TIM_CLOCK_TIM3_BRG_Msk) >> RST_CLK_TIM_CLOCK_TIM3_BRG_Pos);
    RST_CLK_Clocks->TIMER_Frequency[3] = PER_C2_Freq >> ((Temp1 & RST_CLK_TIM_CLOCK_TIM4_BRG_Msk) >> RST_CLK_TIM_CLOCK_TIM4_BRG_Pos);
    Temp1                              = MDR_RST_CLK->UART_CLOCK;
    RST_CLK_Clocks->UART_Frequency[0]  = PER_C2_Freq >> ((Temp1 & RST_CLK_UART_CLOCK_UART1_BRG_Msk) >> RST_CLK_UART_CLOCK_UART1_BRG_Pos);
    RST_CLK_Clocks->UART_Frequency[1]  = PER_C2_Freq >> ((Temp1 & RST_CLK_UART_CLOCK_UART2_BRG_Msk) >> RST_CLK_UART_CLOCK_UART2_BRG_Pos);
    RST_CLK_Clocks->UART_Frequency[2]  = PER_C2_Freq >> ((Temp1 & RST_CLK_UART_CLOCK_UART3_BRG_Msk) >> RST_CLK_UART_CLOCK_UART3_BRG_Pos);
    RST_CLK_Clocks->UART_Frequency[3]  = PER_C2_Freq >> ((Temp1 & RST_CLK_UART_CLOCK_UART4_BRG_Msk) >> RST_CLK_UART_CLOCK_UART4_BRG_Pos);
    Temp1                              = MDR_RST_CLK->SSP_CLOCK;
    RST_CLK_Clocks->SSP_Frequency[0]   = PER_C2_Freq >> ((Temp1 & RST_CLK_SSP_CLOCK_SSP1_BRG_Msk) >> RST_CLK_SSP_CLOCK_SSP1_BRG_Pos);
    RST_CLK_Clocks->SSP_Frequency[1]   = PER_C2_Freq >> ((Temp1 & RST_CLK_SSP_CLOCK_SSP2_BRG_Msk) >> RST_CLK_SSP_CLOCK_SSP2_BRG_Pos);
    RST_CLK_Clocks->SSP_Frequency[2]   = PER_C2_Freq >> ((Temp1 & RST_CLK_SSP_CLOCK_SSP3_BRG_Msk) >> RST_CLK_SSP_CLOCK_SSP3_BRG_Pos);

    /* Compute the ADC_CLK frequency. */
    Temp1 = MDR_RST_CLK->ADC_CLOCK;
    /* Select the ADC_C1 from the CPU_C1, PER_C1_Freq, CPU_C2_Freq, HSE_C1 cases. */
    switch (Temp1 & RST_CLK_ADC_CLOCK_ADC_C1_SEL_Msk) {
        case RST_CLK_ADC_CLK_SRC_CPU_C1:
            ADC_C1_Freq = CPU_C1_Freq;
            break;
        case RST_CLK_ADC_CLK_SRC_PER1_C1:
            ADC_C1_Freq = PER_C1_Freq;
            break;
        case RST_CLK_ADC_CLK_SRC_PLLCPU:
            ADC_C1_Freq = CPU_C2_Freq;
            break;
        case RST_CLK_ADC_CLK_SRC_HSE_C1_CLK:
            ADC_C1_Freq = RST_CLK_Clocks->RTCHSE_Frequency;
            break;
    }
    /* Compute the ADC_CLK divisor. */
    if (((Temp1 >> RST_CLK_ADC_CLOCK_ADC_C3_SEL_Pos) & ((uint32_t)0x8)) == 0) {
        ADC_C3_Freq = ADC_C1_Freq;
    } else {
        ADC_C3_Freq = ADC_C1_Freq >> (((Temp1 >> RST_CLK_ADC_CLOCK_ADC_C3_SEL_Pos) & ((uint32_t)0x7)) + 1);
    }

    RST_CLK_Clocks->ADC_Frequency = ADC_C3_Freq;

    /* Compute the ADCUI_CLK frequency. */

    /* Select the ADCUI_C1 from the CPU_C1, PER_C1_Freq, CPU_C2_Freq, HSE_C1 cases. */
    switch (Temp1 & RST_CLK_ADC_CLOCK_ADCUI_C1_SEL_Msk) {
        case RST_CLK_ADCUI_CLK_SRC_CPU_C1:
            ADCUI_C1_Freq = CPU_C1_Freq;
            break;
        case RST_CLK_ADCUI_CLK_SRC_PER1_C1:
            ADCUI_C1_Freq = PER_C1_Freq;
            break;
        case RST_CLK_ADCUI_CLK_SRC_PLLCPU:
            ADCUI_C1_Freq = CPU_C2_Freq;
            break;
        case RST_CLK_ADCUI_CLK_SRC_HSE_C1_CLK:
            ADCUI_C1_Freq = RST_CLK_Clocks->RTCHSE_Frequency;
            break;
    }

    if (((Temp1 >> RST_CLK_ADC_CLOCK_ADCUI_C3_SEL_Pos) & ((uint32_t)0x8)) == 0) {
        ADCUI_C3_Freq = ADCUI_C1_Freq;
    } else {
        ADCUI_C3_Freq = ADCUI_C1_Freq >> (((Temp1 >> RST_CLK_ADC_CLOCK_ADCUI_C3_SEL_Pos) & ((uint32_t)0x7)) + 1);
    }

    RST_CLK_Clocks->ADCUI_Frequency = ADCUI_C3_Freq;
}

/**
 * @brief  Check whether a specified RST flag is set or not.
 * @param  RST_CLK_Flag: @ref RST_CLK_Flags_TypeDef - specify a flag to check.
 * @return @ref FlagStatus - the state of clock ready flag (SET or RESET).
 */
FlagStatus RST_CLK_GetFlagStatus(RST_CLK_Flags_TypeDef RST_CLK_Flag)
{
    FlagStatus Status;

    /* Check the parameters. */
    assert_param(IS_RST_CLK_FLAG(RST_CLK_Flag));

    /* Get the flag status on a proper position. */
    if ((MDR_RST_CLK->CLOCK_STATUS & (uint32_t)RST_CLK_Flag) != RESET) {
        Status = SET;
    } else {
        Status = RESET;
    }

    /* Return the flag status. */
    return Status;
}

/** @} */ /* End of the group RST_CLK_Exported_Functions */

/** @} */ /* End of the group RST_CLK */

/** @} */ /* End of the group MDR32VF0xI_StdPeriph_Driver */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE MDR32VF0xI_rst_clk.c */

