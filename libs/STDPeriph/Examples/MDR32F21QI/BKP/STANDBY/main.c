/**
 ******************************************************************************
 * @file    main.c
 * @author  Milandr Application Team
 * @version V1.0.0
 * @date    29/05/2023
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
#include "MDR32F2xQI_bkp.h"
#include "MDR32F2xQI_utils.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK234_EVAL K1986VK234 Evaluation Board
 *  @{
 */

/** @addtogroup BKP_STANDBY_234 BKP STANDBY example for K1986VK234 evaluation board
 *  @{
 */

/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Main program.
 * @param  None.
 * @retval None.
 */
int main(void)
{
    uint32_t Cnt;

    RST_CLK_DeInit();

    SystemCoreClockUpdate();

    DELAY_Init(DELAY_MODE_SYSTICK);

    /* Enables the HSI clock on PORTC and BKP */
    RST_CLK_PCLKCmd((RST_CLK_PCLK_PORTC | RST_CLK_PCLK_BKP), ENABLE);

    PORT_DeInit(MDR_PORTC);

    /* Configure PORTC pin 4 for output to switch LED on/off */
    PORT_StructInit(&PORT_InitStructure);
    PORT_InitStructure.PORT_Pin       = PORT_PIN_4;
    PORT_InitStructure.PORT_Direction = PORT_DIRECTION_OUT;
    PORT_InitStructure.PORT_Function  = PORT_FUNCTION_PORT;
    PORT_InitStructure.PORT_Mode      = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_Power     = PORT_POWER_SPEED_SLOW;

    PORT_Init(MDR_PORTC, &PORT_InitStructure);

    for (Cnt = 0; Cnt < 5; Cnt++) {
        if (PORT_ReadInputDataBit(MDR_PORTC, PORT_PIN_4) == 0) {
            PORT_SetBits(MDR_PORTC, PORT_PIN_4);
        } else {
            PORT_ResetBits(MDR_PORTC, PORT_PIN_4);
        }
        DELAY_WaitMs(500);
    }

    /* Enter STANDBY mode */
    BKP_EnterSTANDBYMode();

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

/** @} */ /* End of group BKP_STANDBY_234 */

/** @} */ /* End of group __K1986VK234_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr *******************************/

/* END OF FILE main.c */


