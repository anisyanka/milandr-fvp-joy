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
#include "MDR32F2xQI_adcui.h"
#include "MDR32F2xQI_rst_clk.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK214_EVAL K1986VK214 Evaluation Board
 *  @{
 */

/** @addtogroup ADCUI_214 ADCUI example for K1986VK214 evaluation board
 *  @{
 */

/* Private macro -------------------------------------------------------------*/
#define SAMPLE 1024

/* Private variables ---------------------------------------------------------*/
uint32_t ADCUI_Data[SAMPLE];
uint32_t Cnt = 0;

ADCUI_InitTypeDef        ADCUI_InitStructure;
ADCUI_FxUnit_InitTypeDef ADCUI_Fx_Unit_InitStructure;

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program.
 * @param  None.
 * @retval None.
 */
int main(void)
{
    RST_CLK_DeInit();

    RST_CLK_PCLKCmd(RST_CLK_PCLK_ADCUI, ENABLE);

    ADCUI_DeInit();

    ADCUI_Fx_StructInit(&ADCUI_Fx_Unit_InitStructure);
    ADCUI_Fx_Unit_InitStructure.ADCUI_Fx_IntegratorEnable = ADCUI_INT_DISABLE;

    ADCUI_Fx_Unit_InitStructure.ADCUI_F0_I3IntegratorEnable = ADCUI_INT_DISABLE;
    ADCUI_Fx_Unit_InitStructure.ADCUI_F0_IChannelSelect     = ADCUI_F0_I_CH_I0;
    ADCUI_Fx_Init(ADCUI_F0_UNIT, &ADCUI_Fx_Unit_InitStructure);

    ADCUI_StructInit(&ADCUI_InitStructure);
    ADCUI_InitStructure.ADCUI_ZeroCrossLowPassFilter = ADCUI_ZXLPF_FILTER_DISABLE;
    ADCUI_InitStructure.ADCUI_DataResol              = ADCUI_DATA_RESOL_24BIT;
    ADCUI_InitStructure.ADCUI_VRefConfig             = ADCUI_REFERENCE_VOLTAGE_INTERNAL;

    ADCUI_Init(&ADCUI_InitStructure);

    ADCUI_ChannelConfig(ADCUI_CH_F0_V0, ENABLE);

    RST_CLK_ADCUI_ClkCmd(ENABLE);

    while (Cnt < SAMPLE) {
        if (!(ADCUI_GetFlagStatus(ADCUI_F0_UNIT, ADCUI_Fx_VF_EMP))) {
            ADCUI_Data[Cnt] = ADCUI_GetResult(ADCUI_CH_F0_V0);
            Cnt++;
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

/** @} */ /* End of group ADCUI_214 */

/** @} */ /* End of group __K1986VK214_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr ********************************
 *
 * END OF FILE main.c */


