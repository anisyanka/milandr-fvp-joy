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
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2024 Milandr</center></h2>
 */

/* Includes ------------------------------------------------------------------*/
#include "MDR32F2xQI_port.h"
#include "MDR32F2xQI_rst_clk.h"
#include "MDR32F2xQI_bkp.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK214_EVAL K1986VK214 Evaluation Board
 *  @{
 */

/** @addtogroup BKP_RTC_214 BKP RTC example for K1986VK214 evaluation board
 *  @{
 */

/* Private define ------------------------------------------------------------*/
#define RTC_LSE_CLK

#define COUNT_VALUE     0
#define ALARM_VALUE     50
#define PRESC_VALUE_HS  1000000
#define PRESC_VALUE_LSI 32000
#define PRESC_VALUE_LSE 32768
#define RTCHSI_PRESC    RST_CLK_HSI_PRESCALER_DIV_8
#define RTCHSE_PRESC    RST_CLK_HSE_PRESCALER_DIV_8

/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  BKP RTC interrupt hanlder.
 * @param  None.
 * @retval None.
 */
void BKP_RTC_IRQHandler(void)
{
    if (BKP_RTC_GetITFlagStatus(BKP_RTC_IT_SECF) != RESET) {
        if (PORT_ReadInputDataBit(MDR_PORTC, PORT_PIN_14) == 0) {
            PORT_SetBits(MDR_PORTC, PORT_PIN_14);
        } else {
            PORT_ResetBits(MDR_PORTC, PORT_PIN_14);
        }
    }
    if (BKP_RTC_GetITFlagStatus(BKP_RTC_IT_ALRF) != RESET) {
        PORT_SetBits(MDR_PORTC, PORT_PIN_15);
    }
    BKP_RTC_ClearITFlags(BKP_RTC_IT_SECF | BKP_RTC_IT_ALRF);
}

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
    /* Deinit RST_CLK */
    RST_CLK_DeInit();

    /* Enables the HSI clock on PORTC */
    RST_CLK_PCLKCmd(RST_CLK_PCLK_PORTC, ENABLE);

    PORT_DeInit(MDR_PORTC);

    PORT_StructInit(&PORT_InitStructure);
    PORT_InitStructure.PORT_Pin       = (PORT_PIN_14 | PORT_PIN_15);
    PORT_InitStructure.PORT_Direction = PORT_DIRECTION_OUT;
    PORT_InitStructure.PORT_Power     = PORT_POWER_SPEED_FAST;
    PORT_InitStructure.PORT_Mode      = PORT_MODE_DIGITAL;
    PORT_Init(MDR_PORTC, &PORT_InitStructure);

    /* Enables the HSI clock for BKP control */
    RST_CLK_PCLKCmd(RST_CLK_PCLK_BKP, ENABLE);

    /* Deinit BKP */
    BKP_DeInit();

    /* RTC reset */
    BKP_RTC_Reset(ENABLE);
    BKP_RTC_Reset(DISABLE);

#if defined(RTC_HSI_CLK)
    /* Configure RTCHSI as RTC clock source */
    BKP_HSI_Adjust(25);
    RST_CLK_HSI_C1_SetPrescaler(RTCHSI_PRESC);
    RST_CLK_RTC_HSI_ClkCmd(ENABLE);
    BKP_RTC_SetClkSource(BKP_RTC_HSI_CLK);
#elif defined(RTC_HSE_CLK)
    /* Configure RTCHSE as RTC clock source */
    RST_CLK_HSE_Config(RST_CLK_HSE_ON);
    while (RST_CLK_HSE_GetStatus() != SUCCESS) { }
    RST_CLK_HSE_C1_SetPrescaler(RTCHSE_PRESC);
    RST_CLK_RTC_HSE_ClkCmd(ENABLE);
    BKP_RTC_SetClkSource(BKP_RTC_HSE_CLK);
#elif defined(RTC_LSI_CLK)
    /* Configure LSI as RTC clock source */
    BKP_LSI_Adjust(12);
    BKP_RTC_SetClkSource(BKP_RTC_LSI_CLK);
#endif
#ifdef RTC_LSE_CLK
    /* Configure LSE as RTC clock source */
    BKP_LSE_Config(BKP_LSE_ON);
    while (BKP_LSE_GetStatus() != SUCCESS) { }
    BKP_RTC_SetClkSource(BKP_RTC_LSE_CLK);
#endif

    /* Set the RTC prescaler value */
    BKP_RTC_WaitForUpdate();
#ifdef RTC_HSI_CLK
    BKP_RTC_SetPrescaler(PRESC_VALUE_HS - 1);
#endif
#ifdef RTC_HSE_CLK
    BKP_RTC_SetPrescaler(PRESC_VALUE_HS - 1);
#endif
#ifdef RTC_LSI_CLK
    BKP_RTC_SetPrescaler(PRESC_VALUE_LSI - 1);
#endif
#ifdef RTC_LSE_CLK
    BKP_RTC_SetPrescaler(PRESC_VALUE_LSE - 1);
#endif

    /* Set the RTC alarm value */
    BKP_RTC_WaitForUpdate();
    BKP_RTC_SetAlarm(ALARM_VALUE - 1);

    /* Set the RTC counter value */
    BKP_RTC_WaitForUpdate();
    BKP_RTC_SetCounter(COUNT_VALUE);

    /* Enable all RTC interrupts */
    BKP_RTC_WaitForUpdate();
    BKP_RTC_ITConfig((BKP_RTC_IT_ALRF | BKP_RTC_IT_SECF | BKP_RTC_IT_OWF), ENABLE);
    NVIC_EnableIRQ(BKP_RTC_IRQn);

    /* RTC enable */
    BKP_RTC_WaitForUpdate();
    BKP_RTC_Cmd(ENABLE);

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

/** @} */ /* End of group BKP_RTC_214 */

/** @} */ /* End of group __K1986VK214_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr *******************************/

/* END OF FILE main.c */


