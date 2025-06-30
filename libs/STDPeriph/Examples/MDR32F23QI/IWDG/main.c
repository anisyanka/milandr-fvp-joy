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
#include "MDR32F2xQI_iwdg.h"
#include "MDR32F2xQI_port.h"
#include "MDR32F2xQI_rst_clk.h"
#include "MDR32F2xQI_utils.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK214_EVAL K1986VK214 Evaluation Board
 *  @{
 */

/** @addtogroup IWDG_214 IWDG example for K1986VK214 evaluation board
 *  @{
 */

/* Private define ------------------------------------------------------------*/
#define LED1 PORT_PIN_14

#define LED_PERIOD 300

/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef GPIOInitStruct;

/* Private functions ---------------------------------------------------------*/

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

    RST_CLK_CPU_PLL_Config(RST_CLK_PLLCPU_SRC_HSE_DIV_2, RST_CLK_PLLCPU_MUL_4);
    RST_CLK_CPU_PLL_Cmd(ENABLE);

    if (RST_CLK_CPU_PLL_GetStatus() == ERROR) {
        while (1) { }
    }

    RST_CLK_CPU_C3_SetPrescaler(RST_CLK_CPU_PRESCALER_DIV_1);
    RST_CLK_CPU_C2_ClkSelection(RST_CLK_CPU_C2_SRC_PLLCPU);
    RST_CLK_HCLK_ClkSelection(RST_CLK_CPU_CLK_CPU_C3);

    /* Enable peripheral clocks --------------------------------------------------*/
    RST_CLK_PCLKCmd((RST_CLK_PCLK_IWDG | RST_CLK_PCLK_PORTC), ENABLE);

    /* Reset PORTD settings */
    PORT_DeInit(MDR_PORTC);

    PORT_StructInit(&GPIOInitStruct);

    GPIOInitStruct.PORT_Pin       = LED1;
    GPIOInitStruct.PORT_Direction = PORT_DIRECTION_OUT;
    GPIOInitStruct.PORT_Power     = PORT_POWER_SPEED_MAXFAST;
    GPIOInitStruct.PORT_Mode      = PORT_MODE_DIGITAL;
    PORT_Init(MDR_PORTC, &GPIOInitStruct);

    /* Enable IWDG */
    IWDG_Enable();

    /* Enables write access to IWDG_PR,IWDG_RLR registers */
    IWDG_WriteAccessEnable();

    /* Set IWDG Prescaler value */
    IWDG_SetPrescaler(IWDG_PRESCALER_DIV_64);

    /* Wait when Prescaler Value was updated */
    while (IWDG_GetFlagStatus(IWDG_FLAG_PVU) != 1) { }

    /* Set IWDG Reload value */
    IWDG_SetReload(0xFFF);
    DELAY_WaitMs(LED_PERIOD);
    LEDOn(LED1);

    while (1) {
        /* Load counter value and blink LED after delay */
        DELAY_WaitMs(LED_PERIOD);
        IWDG_ReloadCounter();
        BlinkLED1(1, LED_PERIOD);
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

/** @} */ /* End of group IWDG_214 */

/** @} */ /* End of group __K1986VK214_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr *******************************/

/* END OF FILE main.c */


