/**
 ******************************************************************************
 * @file    main.c
 * @author  Milandr Application Team
 * @version V1.0.0
 * @date    25/05/2023
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
#include "MDR32F23QI_lcd.h"
#include "MDR32F2xQI_bkp.h"
#include "MDR32F2xQI_config.h"
#include "MDR32F2xQI_port.h"
#include "MDR32F2xQI_rst_clk.h"

#include "XT13468PSPA.h"
#include "string.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK214_EVAL K1986VK214 Evaluation Board
 *  @{
 */

/** @addtogroup LDC_STRING_214 LCD Display example for K1986VK214 evaluation board
 *  @{
 */

/* Private variables ---------------------------------------------------------*/
LCD_InitTypeDef  LCD_InitStruct;
PORT_InitTypeDef PORT_InitStructure;
LCD_SreenData    data;

int main(void)
{
    RST_CLK_DeInit();

    RST_CLK_PCLKCmd((RST_CLK_PCLK_LCD | RST_CLK_PCLK_BKP | RST_CLK_PCLK_PORTA |
                     RST_CLK_PCLK_PORTB | RST_CLK_PCLK_PORTC),
                    ENABLE);

    BKP_LSE_Config(BKP_LSE_ON);
    while (BKP_LSE_GetStatus() == !SUCCESS) { } /* Good LSE clock */

    LCD_DeInit();

    PORT_DeInit(MDR_PORTA);
    PORT_DeInit(MDR_PORTB);
    PORT_DeInit(MDR_PORTC);

    PORT_StructInit(&PORT_InitStructure);
    LCD_StructInit(&LCD_InitStruct);

    PORT_InitStructure.PORT_Pin       = (PORT_PIN_8 | PORT_PIN_9);
    PORT_InitStructure.PORT_Direction = PORT_DIRECTION_IN;
    PORT_InitStructure.PORT_Function  = PORT_FUNCTION_PORT;
    PORT_InitStructure.PORT_Mode      = PORT_MODE_DIGITAL;
    PORT_Init(MDR_PORTC, &PORT_InitStructure);

    LCD_InitStruct.LCD_Multiplex   = LCD_MUX_DIV_4;
    LCD_InitStruct.LCD_Divider     = 0x100;
    LCD_InitStruct.LCD_SourceClock = LCD_CLK_LSE;
    LCD_Init(&LCD_InitStruct);

    LCD_Cmd(ENABLE);

    data.SEGMCOM0 = UINT32_MAX;
    data.SEGMCOM1 = UINT32_MAX;
    data.SEGMCOM2 = UINT32_MAX;
    data.SEGMCOM3 = UINT32_MAX;

    while (1) {
        if (!(PORT_ReadInputDataBit(MDR_PORTC, PORT_PIN_8))) {
            for (int i = 0; i < 100000; i++) { }
            LCD_SetScreen(&data);
        }

        if (!(PORT_ReadInputDataBit(MDR_PORTC, PORT_PIN_9))) {
            for (int i = 0; i < 100000; i++) { }
            XTSPA_LCD_Print("hell0");
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
#endif /* USE_ASSERT_INFO */

/** @} */ /* End of group LDC_STRING_214 */

/** @} */ /* End of group __K1986VK214_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr *******************************/

/* END OF FILE main.c */


