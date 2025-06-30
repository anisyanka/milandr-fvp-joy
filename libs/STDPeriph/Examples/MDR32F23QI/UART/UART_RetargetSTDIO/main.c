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
 * <h2><center>&copy; COPYRIGHT 2024 Milandr</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "MDR32F2xQI_rst_clk.h"
#include "MDR32F2xQI_eeprom.h"
#include "MDR32F2xQI_bkp.h"
#include "MDR32F2xQI_utils.h"
#include <stdio.h>

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK214_EVAL K1986VK214 Evaluation Board
 *  @{
 */

/** @addtogroup UART_Retarget_STDIO_214 UART_Retarget_STDIO example for K1986VK214 evaluation board
 *  @{
 */

/* Private variables ---------------------------------------------------------*/
static char symbol;

/* Private function prototypes -----------------------------------------------*/
void ClockConfigure(void);

/* Private functions ---------------------------------------------------------*/

#if (USE_STDIO_RETARGET == 1)
/**
 * @brief  Main program.
 * @param  None.
 * @retval None.
 */
int main(void)
{
    ErrorStatus Status;

    ClockConfigure();
    Status = STDIO_Init();
    if (Status == ERROR) {
        while (1) { }
    }

    /* Print message to UART using printf function */
    printf("\r\n==== K1986VK214 Microcontroller Evaluation Board ====\r\n");
    printf("============ Example UART_Retarget_STDIO ============\r\n");
    printf("Start echo:\r\n");

    /* Print of received symbols to UART */
    while (1) {
        symbol = (char)getchar();
        putchar(symbol);
    }
}
#else
#error "Selected USE_STDIO_RETARGET != 1 in MDR32F2xQI config.h file"
#endif /* USE_STDIO_RETARGET */

/**
 * @brief  Configures CPU clock.
 * @param  None.
 * @retval None.
 */
void ClockConfigure(void)
{
    /* Reset the clock system settings */
    RST_CLK_DeInit();

    /* Enable HSE clock */
    RST_CLK_HSE_Config(RST_CLK_HSE_ON);
    if (RST_CLK_HSE_GetStatus() == ERROR) {
        while (1) { }
    }

    /* Configure the PLLCPU clock source (PLLCPU_CLK = HSE_CLK * 4 = 8 MHz * 4 = 32 MHz) */
    RST_CLK_CPU_PLL_Config(RST_CLK_PLLCPU_SRC_HSE, RST_CLK_PLLCPU_MUL_4);
    RST_CLK_CPU_PLL_Cmd(ENABLE);
    if (RST_CLK_CPU_PLL_GetStatus() == ERROR) {
        while (1) { }
    }

    /* Select the source for CPU_C2 (CPU_C2 = PLLCPU) */
    RST_CLK_CPU_C2_ClkSelection(RST_CLK_CPU_C2_SRC_PLLCPU);
    /* Set CPU_C3 Prescaler (CPU_C3 = CPU_C2) */
    RST_CLK_CPU_C3_SetPrescaler(RST_CLK_CPU_PRESCALER_DIV_1);

    /* Set the EEPROM latency value (CPU_CLK up to 36 MHz) */
    RST_CLK_PCLKCmd(RST_CLK_PCLK_EEPROM, ENABLE);
    EEPROM_SetLatency(EEPROM_LATENCY_CYCLE_1);
    RST_CLK_PCLKCmd(RST_CLK_PCLK_EEPROM, DISABLE);

    /* Setting the parameters of the voltage regulator SelectRI and LOW in the BKP controller (CPU_CLK up to 40 MHz) */
    RST_CLK_PCLKCmd(RST_CLK_PCLK_BKP, ENABLE);
    BKP_DUcc_SetMode(BKP_DUCC_CLK_UPTO_40MHz);
    RST_CLK_PCLKCmd(RST_CLK_PCLK_BKP, DISABLE);

    /* Select the source for HCLK (HCLK = CPU_CLK = CPU_C3) */
    RST_CLK_HCLK_ClkSelection(RST_CLK_CPU_CLK_CPU_C3);

    /* Update SystemCoreClock */
    SystemCoreClockUpdate();
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
    while (1) {
        __NOP();
    }
}
#endif    /* USE_ASSERT_INFO */

/** @} */ /* End of group UART_Retarget_STDIO_214 */

/** @} */ /* End of group __K1986VK214_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE main.c */


