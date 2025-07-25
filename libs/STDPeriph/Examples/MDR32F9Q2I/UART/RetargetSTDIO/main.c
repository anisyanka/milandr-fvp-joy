/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V2.0.3
  * @date    21/06/2023
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
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR32FxQI_rst_clk.h"
#include "MDR32FxQI_eeprom.h"
#include "MDR32FxQI_bkp.h"
#include "MDR32FxQI_uart.h"
#include "MDR32FxQI_utils.h"
#include <stdio.h>

/** @addtogroup __MDR32FxQI_StdPeriph_Examples MDR32FxQI StdPeriph Examples
  * @{
  */

/** @addtogroup __MDR32F9Q2I_EVAL MDR32F9Q2I Evaluation Board
  * @{
  */

/** @addtogroup UART_Retarget_STDIO UART_Retarget_STDIO
  * @{
  */

/* Private variables ---------------------------------------------------------*/
static char symbol;

/* Private function prototypes -----------------------------------------------*/
void ClockConfigure(void);

/* Private functions ---------------------------------------------------------*/  

#if defined (_USE_DEBUG_UART_)
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    ErrorStatus Status;

    ClockConfigure();
    SystemCoreClockUpdate();

    Status = STDIO_Init();
    if (Status == ERROR) {
        while (1) {}
    }

    /* Print message to UART using printf function */
    printf("\r\n====== MDR32F9Q2I Microcontroller Evaluation Board ======\r\n");
    printf("============= Example UART_Retarget_STDIO =============\r\n");
    printf("Start echo:\r\n");

    /* Print of received symbols to UART */
    while(1)
    {
        symbol = getchar();
        putchar(symbol);
    }
}
#else
    #error "_USE_DEBUG_UART_ not selected in MDR32FxQI_config.h file"
#endif /* _USE_DEBUG_UART_ */

/**
  * @brief  Configure CPU clock
  * @param  None
  * @retval None
  */
void ClockConfigure(void)
{
    /* Reset the clock system settings */
    RST_CLK_DeInit();

    /* Enable HSE (High Speed External) clock */
    RST_CLK_HSEconfig(RST_CLK_HSE_ON);
    while(RST_CLK_HSEstatus() == ERROR) {}

    /* Configures the CPU_PLL clock source (CPU_CLK = HSE_CLK * 10 = 8 MHz * 10 = 80 MHz) */
    RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10);
    /* Enables the CPU_PLL */
    RST_CLK_CPU_PLLcmd(ENABLE);
    while(RST_CLK_CPU_PLLstatus() == ERROR) {}

    /* Select the CPU_PLL output as input for CPU_C2_SEL */
    RST_CLK_CPU_PLLuse(ENABLE);
    /* Set CPUClk Prescaler (CPU_C3_SEL = CPU_C2) */
    RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);

    /* Enables the RST_CLK_PCLK_EEPROM */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE);
    /* Sets the code latency value (CPU_CLK up to 100 MHz) */
    EEPROM_SetLatency(EEPROM_Latency_3);
    /* Disables the RST_CLK_PCLK_EEPROM */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, DISABLE);
    
    /* Enables the RST_CLK_PCLK_BKP */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_BKP, ENABLE);
    /* Setting the parameters of the voltage regulator SelectRI and LOW in the BKP controller (CPU_CLK = 80 MHz) */
    BKP_DUccMode(BKP_DUcc_upto_80MHz);

    /* Select the CPU clock source */
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
}

/**
  * @brief  Reports the source file name, the source line number
  *         and expression text (if USE_ASSERT_INFO == 2) where
  *         the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @param  expr:
  * @retval None
  */
#if (USE_ASSERT_INFO == 1)
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the source file name and line number.
       Ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while(1) {}
}
#elif (USE_ASSERT_INFO == 2)
void assert_failed(uint8_t* file, uint32_t line, const uint8_t* expr)
{
    /* User can add his own implementation to report the source file name, line number and
       expression text.
       Ex: printf("Wrong parameters value (%s): file %s on line %d\r\n", expr, file, line) */

    /* Infinite loop */
    while(1) {}
}
#endif /* USE_ASSERT_INFO */

/** @} */ /* End of group UART_Retarget_STDIO */

/** @} */ /* End of group __MDR32F9Q2I_EVAL */

/** @} */ /* End of group __MDR32FxQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr *******************************/

/* END OF FILE main.c */


