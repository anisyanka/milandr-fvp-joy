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
 * <h2><center>&copy; COPYRIGHT 2024 Milandr</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "MDR32F2xQI_rst_clk.h"
#include "MDR32F2xQI_port.h"
#include "MDR32F2xQI_timer.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK234_EVAL K1986VK234 Evaluation Board
 *  @{
 */

/** @addtogroup TIMER_PWM_234 TIMER_PWM example for K1986VK234 evaluation board
 * @{
 */

/* Private variables ---------------------------------------------------------*/
#define TIMER_CNT_PERIOD 0x31F

/* Private variables ---------------------------------------------------------*/
static uint16_t CH1_CCR_Val = (uint16_t)((double)(TIMER_CNT_PERIOD + 1) * 12.5) / 100;
static uint16_t CH2_CCR_Val = (uint16_t)((double)(TIMER_CNT_PERIOD + 1) * 25.0) / 100;
static uint16_t CH3_CCR_Val = (uint16_t)((double)(TIMER_CNT_PERIOD + 1) * 50.0) / 100;

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program.
 * @param  None.
 * @retval None.
 */
int main(void)
{
    PORT_InitTypeDef         PORT_InitStructure;
    TIMER_CNT_InitTypeDef    TIMER_CNT_InitStructure;
    TIMER_CH_InitTypeDef     TIMER_CH_InitStructure;
    TIMER_CH_OUT_InitTypeDef TIMER_CH_OUT_InitStructure;

    /* Reset the clock system settings */
    RST_CLK_DeInit();

    /* Configure PORTA[5:0] (TMR0_CH3(N)-TMR0_CH1(N)) for TIMER0 */
    RST_CLK_PCLKCmd((RST_CLK_PCLK_PORTA), ENABLE);
    PORT_DeInit(MDR_PORTA);

    PORT_StructInit(&PORT_InitStructure);
    PORT_InitStructure.PORT_Pin      = PORT_PIN_0 | PORT_PIN_1 | PORT_PIN_2 | PORT_PIN_3 | PORT_PIN_4 | PORT_PIN_5;
    PORT_InitStructure.PORT_Mode     = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_Function = PORT_FUNCTION_MAIN;
    PORT_InitStructure.PORT_Power    = PORT_POWER_SPEED_MAXFAST;
    PORT_Init(MDR_PORTA, &PORT_InitStructure);

    /* Configure TIMER0 ---------------------------------------------------
    Generates 6 PWM signals with 3 different duty cycles
    TIM0_CLK = HSI_CLK = 8 MHz, CounterPrescaler = 0
    TIM0_CNT_CLK = TIM0_CLK / (CounterPrescaler + 1) = 8 MHz
    TIM0 frequency = TIM0_CNT_CLK / (CounterPeriod + 1) = 10 KHz
    - PWM duty cycle = CCR1 / (CounterPeriod + 1): CH1 = 12.5% and CH1N = 87.5%
    - PWM duty cycle = CCR2 / (CounterPeriod + 1): CH2 = 25% and CH2N = 75%
    - PWM duty cycle = CCR3 / (CounterPeriod + 1): CH3 = 50% and CH3N = 50%
    -----------------------------------------------------------------------*/
    RST_CLK_PCLKCmd(RST_CLK_PCLK_TIMER0, ENABLE);

    RST_CLK_HSI_C1_SetPrescaler(RST_CLK_HSI_PRESCALER_DIV_1);
    RST_CLK_PER1_C2_ClkSelection(RST_CLK_PER1_C2_CLK_SRC_HSI_CLK);

    RST_CLK_PER1_C2_SetPrescaler(RST_CLK_PER1_C2_TIM0, RST_CLK_PER1_PRESCALER_DIV_1);
    RST_CLK_PER1_C2_Cmd(RST_CLK_PER1_C2_TIM0, ENABLE);

    TIMER_DeInit(MDR_TIMER0);

    /* Initializes the TIMER0 Counter -------------------------------------*/
    TIMER_CNT_StructInit(&TIMER_CNT_InitStructure);
    TIMER_CNT_InitStructure.TIMER_CounterPrescaler   = 0x0;
    TIMER_CNT_InitStructure.TIMER_CounterPeriod      = TIMER_CNT_PERIOD;
    TIMER_CNT_InitStructure.TIMER_CounterEventSource = TIMER_CNT_EVNT_SRC_TIM_CLK;
    TIMER_CNT_InitStructure.TIMER_CounterMode        = TIMER_CNT_MODE_CLK_FIXED_DIR;
    TIMER_CNT_InitStructure.TIMER_CounterDirection   = TIMER_CNT_DIR_UP;
    TIMER_CNT_Init(MDR_TIMER0, &TIMER_CNT_InitStructure);

    /* Initializes the TIMER0 Channels 1, 2 and 3 -------------------------*/
    TIMER_CH_StructInit(&TIMER_CH_InitStructure);
    TIMER_CH_InitStructure.TIMER_CH_Mode           = TIMER_CH_MODE_PWM;
    TIMER_CH_InitStructure.TIMER_CH_PWM_REF_Format = TIMER_CH_PWM_REF_FORMAT_6;

    TIMER_CH_InitStructure.TIMER_CH_Number = TIMER_CH1;
    TIMER_CH_Init(MDR_TIMER0, &TIMER_CH_InitStructure);

    TIMER_CH_InitStructure.TIMER_CH_Number = TIMER_CH2;
    TIMER_CH_Init(MDR_TIMER0, &TIMER_CH_InitStructure);

    TIMER_CH_InitStructure.TIMER_CH_Number = TIMER_CH3;
    TIMER_CH_Init(MDR_TIMER0, &TIMER_CH_InitStructure);

    TIMER_CH_PWM_SetCompare(MDR_TIMER0, TIMER_CH1, CH1_CCR_Val);
    TIMER_CH_PWM_SetCompare(MDR_TIMER0, TIMER_CH2, CH2_CCR_Val);
    TIMER_CH_PWM_SetCompare(MDR_TIMER0, TIMER_CH3, CH3_CCR_Val);

    /* Initializes the TIMER0 Channel 1,1N,2,2N,3,3N Outputs --------------*/
    TIMER_CH_OUT_StructInit(&TIMER_CH_OUT_InitStructure);
    TIMER_CH_OUT_InitStructure.TIMER_CH_OUTP_Source = TIMER_CH_OUTP_SRC_REF;
    TIMER_CH_OUT_InitStructure.TIMER_CH_OUTP_Mode   = TIMER_CH_OUTP_MODE_OUT;
    TIMER_CH_OUT_InitStructure.TIMER_CH_OUTN_Source = TIMER_CH_OUTN_SRC_REF;
    TIMER_CH_OUT_InitStructure.TIMER_CH_OUTN_Mode   = TIMER_CH_OUTN_MODE_OUT;

    TIMER_CH_OUT_InitStructure.TIMER_CH_Number = TIMER_CH1;
    TIMER_CH_OUT_Init(MDR_TIMER0, &TIMER_CH_OUT_InitStructure);

    TIMER_CH_OUT_InitStructure.TIMER_CH_Number = TIMER_CH2;
    TIMER_CH_OUT_Init(MDR_TIMER0, &TIMER_CH_OUT_InitStructure);

    TIMER_CH_OUT_InitStructure.TIMER_CH_Number = TIMER_CH3;
    TIMER_CH_OUT_Init(MDR_TIMER0, &TIMER_CH_OUT_InitStructure);

    /* Enable TIMER0 */
    TIMER_CNT_Cmd(MDR_TIMER0, ENABLE);

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

/** @} */ /* End of group TIMER_PWM_234 */

/** @} */ /* End of group __K1986VK234_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE main.c */


