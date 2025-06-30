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
#include "MDR32F2xQI_port.h"
#include "MDR32F2xQI_rst_clk.h"
#include "MDR32F2xQI_utils.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK234_EVAL K1986VK234 Evaluation Board
 *  @{
 */

/** @addtogroup LED_BUTTONS_234 LED Buttons example for K1986VK234 evaluation board
 *  @{
 */

/* Private define -------------------------------------------------------------*/
#define LED1       PORT_PIN_4
#define LED2       PORT_PIN_5
#define LED3       PORT_PIN_6
#define LED4       PORT_PIN_7

#define B1         PORT_PIN_0
#define B2         PORT_PIN_1
#define B3         PORT_PIN_2
#define B4         PORT_PIN_3

#define LED_PERIOD 300

/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef GPIOInitStruct;
uint32_t         BlinkingLeds;

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
    RST_CLK_DeInit();

    RST_CLK_PCLKCmd((RST_CLK_PCLK_PORTA | RST_CLK_PCLK_PORTC), ENABLE);

    PORT_DeInit(MDR_PORTC);

    PORT_StructInit(&GPIOInitStruct);
    GPIOInitStruct.PORT_Pin       = (LED1 | LED2 | LED3 | LED4);
    GPIOInitStruct.PORT_Direction = PORT_DIRECTION_OUT;
    GPIOInitStruct.PORT_Power     = PORT_POWER_SPEED_FAST;
    GPIOInitStruct.PORT_Mode      = PORT_MODE_DIGITAL;
    PORT_Init(MDR_PORTC, &GPIOInitStruct);

    PORT_DeInit(MDR_PORTA);

    GPIOInitStruct.PORT_Pin       = (B1 | B2 | B3 | B4);
    GPIOInitStruct.PORT_Direction = PORT_DIRECTION_IN;
    PORT_Init(MDR_PORTA, &GPIOInitStruct);

    DELAY_Init(DELAY_MODE_SYSTICK);

    while (1) {

        BlinkingLeds = 0;

        if (!PORT_ReadInputDataBit(MDR_PORTA, B1)) {
            BlinkingLeds |= LED1;
        }

        if (!PORT_ReadInputDataBit(MDR_PORTA, B2)) {
            BlinkingLeds |= LED2;
        }

        if (!PORT_ReadInputDataBit(MDR_PORTA, B3)) {
            BlinkingLeds |= LED3;
        }

        if (!PORT_ReadInputDataBit(MDR_PORTA, B4)) {
            BlinkingLeds |= LED4;
        }

        if (BlinkingLeds == 0) {
            BlinkingLeds = (LED1 | LED2 | LED3 | LED4);
        }

        PORT_SetBits(MDR_PORTC, BlinkingLeds);
        DELAY_WaitMs(LED_PERIOD);

        PORT_ResetBits(MDR_PORTC, BlinkingLeds);
        DELAY_WaitMs(LED_PERIOD);
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

/** @} */ /* End of group LED_BUTTONS_234 */

/** @} */ /* End of group __K1986VK234_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr *******************************/

/* END OF FILE main.c */


