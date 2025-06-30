/**
 ******************************************************************************
 * @file    main.c
 * @author  Milandr Application Team
 * @version V0.1.0
 * @date    25/04/2024
 * @brief   Main program body.
 ******************************************************************************
 * THE PRESENT FIRMWARE IS FOR GUIDANCE ONLY. IT AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING MILANDR'S PRODUCTS IN ORDER TO FACILITATE
 * THE USE AND SAVE TIME. MILANDR SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES RESULTING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR A USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN THEIR PRODUCTS.
 */

/* Includes ------------------------------------------------------------------*/
#include "MDR32VF0xI_port.h"
#include "MDR32VF0xI_rst_clk.h"
#include "MDR32VF0xI_otp.h"

/* Private defines -----------------------------------------------------------*/

/* Demoboard LEDs definitions. */
#define LED_PORT        MDR_PORTD
#define LED_PORT_CLK    RST_CLK_PCLK_PORTD
#define LED_PIN_1       PORT_PIN_0
#define LED_PIN_2       PORT_PIN_1
#define LED_PIN_3       PORT_PIN_2
#define LED_PIN_4       PORT_PIN_3


/* Private variables ---------------------------------------------------------*/

uint32_t DelayCnt = 0;

uint32_t WriteWord = 0xF1402D73;
volatile uint32_t ReadWord = 0;
uint32_t RW_OTP_Address = 0x000207D0;

OTP_RegDelays_InitTypeDef OTP_Delays;
PORT_InitTypeDef PORT_InitStruct = {
    .PORT_Pin = (LED_PIN_1 | LED_PIN_2 | LED_PIN_3 | LED_PIN_4),
    .PORT_Mode = PORT_MODE_DIGITAL,
    .PORT_Direction = PORT_DIRECTION_OUT,
    .PORT_Function = PORT_FUNCTION_PORT,
    .PORT_Power = PORT_POWER_INCREASED_UPTO_4mA,
#if defined(USE_MDR32F02_REV_2)
    .PORT_PullUp = PORT_PULL_UP_OFF,
#endif
    .PORT_PullDown = PORT_PULL_DOWN_OFF
};


/* Private functions ---------------------------------------------------------*/

/* Main function */
int main(void)
{
    /* Setup MCU clock as HSE 8MHz. */
    RST_CLK_DeInit();

    RST_CLK_HSE_Config(RST_CLK_HSE_ON);
    while (RST_CLK_HSE_GetStatus() != SUCCESS) {}

    RST_CLK_CPU_C1_ClkSelection(RST_CLK_CPU_C1_CLK_SRC_HSE);
    RST_CLK_CPU_C2_ClkSelection(RST_CLK_CPU_C2_CLK_SRC_CPU_C1);
    RST_CLK_CPU_C3_SetPrescaler(RST_CLK_CPU_C3_PRESCALER_DIV_1);
    RST_CLK_HCLK_ClkSelection(RST_CLK_CPU_HCLK_CLK_SRC_CPU_C3);

    /* Setup OTP register access delays. */
    OTP_RegAccessDelaysStructConfig(&OTP_Delays, 8000000);
    OTP_RegAccessDelaysConfig(&OTP_Delays);

    /* Setup OTP AHB delay - not necessary on 8 MHz, used for demonstration purposes. */
    OTP_SetAHBDelay(OTP_AHB_DELAY_CYCLE_1);

    /* Setup demoboard LEDs for indication. */
    RST_CLK_PCLKCmd(LED_PORT_CLK, ENABLE);
    PORT_Init(LED_PORT, &PORT_InitStruct);
    PORT_WriteBit(LED_PORT, (LED_PIN_1 | LED_PIN_2 | LED_PIN_3 | LED_PIN_4), SET);

    /* Read word from OTP. */
    ReadWord = OTP_ReadWord(RW_OTP_Address);

    /* If read word is not equal to target WriteWord - program OTP word. */
    if (ReadWord != WriteWord) {
        OTP_ProgramWord(RW_OTP_Address, WriteWord);
    }

    /* Read word from OTP again for validation. */
    ReadWord = OTP_ReadWord(RW_OTP_Address);

    /* Indicate failed validation. */
    if(ReadWord != WriteWord) {
        while (1) {
            if (DelayCnt++ >= 0x00010000) {
                DelayCnt = 0;
                PORT_ToggleBit(LED_PORT, (LED_PIN_3 | LED_PIN_4));
            }
        }
    }

    /* Successful validation. */
    while (1) {
        if (DelayCnt++ >= 0x0010000) {
            DelayCnt = 0;
            PORT_ToggleBit(LED_PORT, LED_PIN_1);
        }
    }
}


#if (USE_ASSERT_INFO == 1)
/**
 * @brief  Reports the source file name, the source line number
 *         where the assert_param error has occurred (USE_ASSERT_INFO == 1).
 * @param  file: Pointer to the source file name.
 * @param  line: assert_param error line source number.
 * @return None.
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the source file name and line number.
       Ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop. */
    while (1) { }
}
#elif (USE_ASSERT_INFO == 2)
/**
 * @brief  Reports the source file name, the source line number and
 *         expression text where the assert_param error has occurred (if USE_ASSERT_INFO == 2).
 * @param  file: Pointer to the source file name.
 * @param  line: assert_param error line source number.
 * @param  expr: Expression test where assert_param error occured.
 * @return None.
 */
void assert_failed(uint8_t* file, uint32_t line, const uint8_t* expr)
{
    /* User can add his own implementation to report the source file name, line number and
       expression text.
       Ex: printf("Wrong parameters value (%s): file %s on line %d\r\n", expr, file, line) */

    /* Infinite loop. */
    while (1) { }
}
#endif    /* USE_ASSERT_INFO */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE main.c */

