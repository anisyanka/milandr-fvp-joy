/**
 ******************************************************************************
 * @file    main.c
 * @author  Milandr Application Team
 * @version V1.0.0
 * @date    22/05/2023
 * @brief   Main program body.
 ******************************************************************************
 * <br><br>
 *
 * THE PRESENT FIRMWARE IS FOR GUIDANCE ONLY. IT AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING MILANDR'S PRODUCTS IN ORDER TO FACILITATE
 * THE USE AND SAVE TIME. MILANDR SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES RESULTING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR A USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2024 Milandr </center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "MDR32F2xQI_adc.h"
#include "MDR32F2xQI_port.h"
#include "MDR32F2xQI_rst_clk.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK234_EVAL K1986VK234 Evaluation Board
 *  @{
 */

/** @addtogroup ADC_Watchdog_234 ADC_Watchdog example for K1986VK234 evaluation board
 *  @{
 */

/* Private define ------------------------------------------------------------*/
#define LED1 PORT_PIN_4
#define LED2 PORT_PIN_5

/* Private variables ---------------------------------------------------------*/
volatile uint32_t UpperLevel = 0x900;
volatile uint32_t LowerLevel = 0x800;

volatile uint32_t Result;

PORT_InitTypeDef GPIOInitStruct;
ADC_InitTypeDef  ADCInitStruct;
ADCx_InitTypeDef ADCxInitStruct;

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  ADC interrupt handler.
 * @param  None.
 * @retval None.
 */
void ADC_IRQHandler(void)
{
    if (ADC1_GetITFlagStatus(ADC1_IT_OUT_OF_RANGE) != RESET) {
        /* Turns LED1 On */
        PORT_SetBits(MDR_PORTC, LED1);
    } else {
        /* Turns LED1 Off */
        PORT_ResetBits(MDR_PORTC, LED1);
    }

    Result = ADC1_GetResult() & ADC1_RESULT_RESULT_Msk;

    if (Result > UpperLevel) {
        /* Turns LED2 On */
        PORT_SetBits(MDR_PORTC, LED2);
    } else {
        /* Turns LED2 Off */
        PORT_ResetBits(MDR_PORTC, LED2);
    }

    /* Clear ADC1 OUT_OF_RANGE interrupt bit */
    ADC1_ClearITFlags(ADC1_IT_OUT_OF_RANGE);
}

/**
 * @brief  Main program.
 * @param  None.
 * @retval None.
 */
int main(void)
{
    RST_CLK_DeInit();

    /* Enable peripheral clocks */
    RST_CLK_PCLKCmd((RST_CLK_PCLK_ADC | RST_CLK_PCLK_PORTC), ENABLE);

    /* Reset PORTD settings */
    PORT_DeInit(MDR_PORTC);

    PORT_StructInit(&GPIOInitStruct);

    GPIOInitStruct.PORT_Pin       = (LED1 | LED2);
    GPIOInitStruct.PORT_Direction = PORT_DIRECTION_OUT;
    GPIOInitStruct.PORT_Power     = PORT_POWER_SPEED_FAST;
    GPIOInitStruct.PORT_Mode      = PORT_MODE_DIGITAL;
    PORT_Init(MDR_PORTC, &GPIOInitStruct);

    /* ADC Configuration */
    /* Reset all ADC settings */
    ADC_DeInit();
    ADC_StructInit(&ADCInitStruct);
    ADC_Init(&ADCInitStruct);

    ADCx_StructInit(&ADCxInitStruct);
    ADCxInitStruct.ADC_ClockSource      = ADC_CLOCK_SOURCE_CPU;
    ADCxInitStruct.ADC_SamplingMode     = ADC_SAMPLING_MODE_CYCLIC_CONV;
    ADCxInitStruct.ADC_ChannelSwitching = ADC_CH_SWITCHING_DISABLE;
    ADCxInitStruct.ADC_ChannelNumber    = ADC_CH_ADC4;
    ADCxInitStruct.ADC_LevelControl     = ADC_LEVEL_CONTROL_ENABLE;
    ADCxInitStruct.ADC_LowLevel         = (uint16_t)LowerLevel;
    ADCxInitStruct.ADC_HighLevel        = (uint16_t)UpperLevel;
    ADCxInitStruct.ADC_VRefSource       = ADC_VREF_SOURCE_INTERNAL;
    ADCxInitStruct.ADC_IntVRefSource    = ADC_INT_VREF_SOURCE_INEXACT;
    ADCxInitStruct.ADC_Prescaler        = ADC_PRESCALER_DIV_2048;
    ADCxInitStruct.ADC_DelayGo          = 0x7;
    ADC1_Init(&ADCxInitStruct);

    /* Enable ADC1 EOCIF and AWOIFEN interrupts */
    ADC1_ITConfig((ADC1_IT_END_OF_CONVERSION | ADC1_IT_OUT_OF_RANGE), ENABLE);

    /* Enable ADC interrupt */
    NVIC_EnableIRQ(ADC_IRQn);

    /* ADC1 enable */
    ADC1_Cmd(ENABLE);

    while (1) { }
}

#if (USE_ASSERT_INFO == 1)
/**
 * @brief  Reports the source file name, the source line number
 *         where the assert_param error has occurred (USE_ASSERT_INFO == 1).
 * @param  file: pointer to the source file name.
 * @param  line: assert_param error line source number.
 * @retval None.
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the source file name and line number.
       Ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1) { }
}
#elif (USE_ASSERT_INFO == 2)
/**
 * @brief  Reports the source file name, the source line number and
 *         expression text where the assert_param error has occurred (if USE_ASSERT_INFO == 2).
 * @param  file: pointer to the source file name.
 * @param  line: assert_param error line source number.
 * @param  expr: expression test where assert_param error occured.
 * @retval None.
 */
void assert_failed(uint8_t* file, uint32_t line, const uint8_t* expr)
{
    /* User can add his own implementation to report the source file name, line number and
       expression text.
       Ex: printf("Wrong parameters value (%s): file %s on line %d\r\n", expr, file, line) */

    /* Infinite loop */
    while (1) { }
}
#endif    /* USE_ASSERT_INFO */

/** @} */ /* End of group ADC_Watchdog_234 */

/** @} */ /* End of group __K1986VK234_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr *******************************/

/* END OF FILE main.c */


