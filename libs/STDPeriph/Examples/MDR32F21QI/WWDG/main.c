/**
 ******************************************************************************
 * @file    main.c
 * @author  Milandr Application Team
 * @version V1.0.0
 * @date    30/05/2023
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
#include "MDR32F2xQI_port.h"
#include "MDR32F2xQI_rst_clk.h"
#include "MDR32F2xQI_wwdg.h"
#include "MDR32F2xQI_utils.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK234_EVAL K1986VK234 Evaluation Board
 *  @{
 */

/** @addtogroup WWDG_234 WWDG example for K1986VK234 evaluation board
 *  @{
 */

/* Private define ------------------------------------------------------------*/
#define LED1 PORT_PIN_4
#define LED2 PORT_PIN_5

#define LED_PERIOD 300

/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef GPIOInitStruct;

/**
 * @brief  LEDOn and LEDOff.
 * @param  LED_Num: specifies LED.
 * @retval None
 */

void LEDOn(uint32_t LED_Num)
{
    PORT_SetBits(MDR_PORTC, LED_Num);
}

void LEDOff(uint32_t LED_Num)
{
    PORT_ResetBits(MDR_PORTC, LED_Num);
}

/**
 * @brief  Blink with LED1
 * @param  num - blinks number
 * @param  del - delay
 * @retval None
 */
void BlinkLED1(uint32_t num, uint32_t del)
{
    uint32_t cnt;

    for (cnt = 0; cnt < num; cnt++) {
        LEDOn(LED1);
        DELAY_WaitMs(del);
        LEDOff(LED1);
        DELAY_WaitMs(del);
    }
}

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
    RST_CLK_DeInit();

    RST_CLK_HSE_Config(RST_CLK_HSE_ON);
    if (RST_CLK_HSE_GetStatus() == ERROR) {
        while (1) { }
    }

    RST_CLK_CPU_PLL_Config(RST_CLK_PLLCPU_SRC_HSE, RST_CLK_PLLCPU_MUL_4);
    RST_CLK_CPU_PLL_Cmd(ENABLE);

    if (RST_CLK_CPU_PLL_GetStatus() == ERROR) {
        while (1) { }
    }

    RST_CLK_CPU_C3_SetPrescaler(RST_CLK_CPU_PRESCALER_DIV_1);
    RST_CLK_CPU_C2_ClkSelection(RST_CLK_CPU_C2_SRC_PLLCPU);
    RST_CLK_HCLK_ClkSelection(RST_CLK_CPU_CLK_CPU_C3);

    /* Enable peripheral clocks --------------------------------------------------*/
    RST_CLK_PCLKCmd((RST_CLK_PCLK_WWDG | RST_CLK_PCLK_PORTC), ENABLE);

    /* Reset PORTD settings */
    PORT_DeInit(MDR_PORTC);

    PORT_StructInit(&GPIOInitStruct);

    GPIOInitStruct.PORT_Pin       = LED1 | LED2;
    GPIOInitStruct.PORT_Direction = PORT_DIRECTION_OUT;
    GPIOInitStruct.PORT_Power     = PORT_POWER_SPEED_FAST;
    GPIOInitStruct.PORT_Mode      = PORT_MODE_DIGITAL;
    PORT_Init(MDR_PORTC, &GPIOInitStruct);

    /* Set WWDG Prescaler value */
    WWDG_SetPrescaler(WWDG_PRESCALER_DIV_8);

    /* Enable WWDG and load start counter value */
    WWDG_Enable(0x7F);

    while (1) {
        if (WWDG_GetFlagStatus() == SET) {
            /* Load counter value */
            WWDG_SetCounter(0x7F);

            BlinkLED1(1, LED_PERIOD);

            /* Clears Early Wakeup interrupt flag */
            WWDG_ClearFlagStatus();
        }
    }
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

/** @} */ /* End of group WWDG_234 */

/** @} */ /* End of group __K1986VK234_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr *******************************/

/* END OF FILE main.c */


