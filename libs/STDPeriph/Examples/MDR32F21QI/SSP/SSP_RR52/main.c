/**
 ******************************************************************************
 * @file    main.c
 * @author  Milandr Application Team
 * @version V1.0.0
 * @date    24/05/2023
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
#include "MDR32F2xQI_port.h"
#include "MDR32F2xQI_ssp.h"
#include "MDR32F2xQI_utils.h"
#include "RR52ssp.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK234_EVAL K1986VK234 Evaluation Board
 *  @{
 */

/** @addtogroup SSP_RR52_234 SSP RR52 example for K1986VK234 evaluation board
 *  @{
 */

/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE 32

/* Private variables ---------------------------------------------------------*/
volatile uint8_t  Data[BUFFER_SIZE];
volatile uint32_t ID;
volatile uint32_t Stat;
volatile uint32_t ProtectionRegion0, ProtectionRegion1;

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
    uint32_t Cnt;

    RST_CLK_DeInit();

    RST_CLK_HSE_Config(RST_CLK_HSE_ON);
    while (RST_CLK_HSE_GetStatus() == ERROR) { }

    RST_CLK_CPU_PLL_Config(RST_CLK_PLLCPU_SRC_HSE, RST_CLK_PLLCPU_MUL_4);
    RST_CLK_CPU_PLL_Cmd(ENABLE);

    while (RST_CLK_CPU_PLL_GetStatus() == ERROR) { }

    RST_CLK_CPU_C3_SetPrescaler(RST_CLK_CPU_PRESCALER_DIV_1);
    RST_CLK_CPU_C2_ClkSelection(RST_CLK_CPU_C2_SRC_PLLCPU);
    RST_CLK_HCLK_ClkSelection(RST_CLK_CPU_CLK_CPU_C3);

    SystemCoreClockUpdate();

    DELAY_Init(DELAY_MODE_SYSTICK);

    for (Cnt = 0; Cnt < BUFFER_SIZE; Cnt++) {
        Data[Cnt] = Cnt;
    }

    RR52SSP_PortConfig();
    RR52SSP_SSPConfig();

    ID = RR52SSP_ReadID();

    /* If can not read ID */
    while (ID != 0x01C8) { }

    /* Unprotect all sectors */
    RR52SSP_Unlock(0x00000000); // Sector 0
    RR52SSP_Unlock(0x00010000);

    Stat = RR52SSP_ReadStat();

    ProtectionRegion0 = RR52SSP_ReadUnlock(0x00000);
    ProtectionRegion1 = RR52SSP_ReadUnlock(0x10000);

    /* If protection is not removed */
    while (ProtectionRegion0 || ProtectionRegion1) { }

    RR52SSP_MassErase();

    DELAY_WaitUs(100000);

    /* Write array to memory */
    for (Cnt = 0; Cnt < BUFFER_SIZE; Cnt++) {
        RR52SSP_WriteByte(Cnt, Cnt & 0xFF);
        DELAY_WaitUs(200);
    }

    /* Read array from memory */
    for (Cnt = 0; Cnt < BUFFER_SIZE; Cnt++) {
        if (Data[Cnt] != RR52SSP_ReadByte(Cnt)) {
            while (1) { }
        }
    }

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

/** @} */ /* End of group SSP_RR52_234 */

/** @} */ /* End of group __K1986VK234_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr *******************************/

/* END OF FILE main.c */


