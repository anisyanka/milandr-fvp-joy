/**
 ******************************************************************************
 * @file    main.c
 * @author  Milandr Application Team
 * @version V1.0.0
 * @date    30/05/2023
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

/** @addtogroup TIMER_CAP_Interrupt_234 TIMER_CAP_Interrupt example for K1986VK234 evaluation board
 * @{
 */

/* Private define ------------------------------------------------------------*/
#define LED_PIN PORT_PIN_4

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  TIMER0 interrupt handler.
 * @param  None.
 * @retval None.
 */
void TIMER0_IRQHandler(void)
{
    if (TIMER_GetITFlagStatus(MDR_TIMER0, TIMER_FLAG_CH1_CAP_CCR) == SET) {
        TIMER_ClearITFlags(MDR_TIMER0, TIMER_FLAG_CH1_CAP_CCR);
        PORT_ToggleBit(MDR_PORTC, LED_PIN);
    }
}

/**
 * @brief  Main program.
 * @param  None.
 * @retval None.
 */
int main(void)
{
    PORT_InitTypeDef      PORT_InitStructure;
    TIMER_CNT_InitTypeDef TIMER_CNT_InitStructure;
    TIMER_CH_InitTypeDef  TIMER_CH_InitStructure;

    /* Reset the clock system settings */
    RST_CLK_DeInit();

    /* Configure PA0 (TMR0_CH1) for TIMER0 */
    RST_CLK_PCLKCmd((RST_CLK_PCLK_PORTA), ENABLE);
    PORT_DeInit(MDR_PORTA);

    PORT_StructInit(&PORT_InitStructure);
    PORT_InitStructure.PORT_Pin      = PORT_PIN_0;
    PORT_InitStructure.PORT_Mode     = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_Function = PORT_FUNCTION_MAIN;
    PORT_Init(MDR_PORTA, &PORT_InitStructure);

    /* Configure LED_PIN for output to switch LED on/off */
    RST_CLK_PCLKCmd((RST_CLK_PCLK_PORTC), ENABLE);
    PORT_DeInit(MDR_PORTC);

    PORT_InitStructure.PORT_Pin       = LED_PIN;
    PORT_InitStructure.PORT_Mode      = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_Function  = PORT_FUNCTION_PORT;
    PORT_InitStructure.PORT_Direction = PORT_DIRECTION_OUT;
    PORT_InitStructure.PORT_Power     = PORT_POWER_SPEED_SLOW;
    PORT_Init(MDR_PORTC, &PORT_InitStructure);

    /* Configure TIMER0 ---------------------------------------------------
    TIM0_CLK = HSI_CLK/8 = 1 MHz, CounterPrescaler = 0
    TIM0_CNT_CLK = TIM0_CLK / (CounterPrescaler + 1) = 1 MHz
    FDTS = TIM0_CLK / 4 = 250 kHz, CH1i filter = 256xT_FDTS = 1,024 ms
    -----------------------------------------------------------------------*/
    RST_CLK_PCLKCmd(RST_CLK_PCLK_TIMER0, ENABLE);

    RST_CLK_HSI_C1_SetPrescaler(RST_CLK_HSI_PRESCALER_DIV_1);
    RST_CLK_PER1_C2_ClkSelection(RST_CLK_PER1_C2_CLK_SRC_HSI_CLK);

    RST_CLK_PER1_C2_SetPrescaler(RST_CLK_PER1_C2_TIM0, RST_CLK_PER1_PRESCALER_DIV_8);
    RST_CLK_PER1_C2_Cmd(RST_CLK_PER1_C2_TIM0, ENABLE);

    TIMER_DeInit(MDR_TIMER0);

    /* Initializes the TIMER0 Counter -------------------------------------*/
    TIMER_CNT_StructInit(&TIMER_CNT_InitStructure);
    TIMER_CNT_InitStructure.TIMER_CounterPrescaler   = 0x0;
    TIMER_CNT_InitStructure.TIMER_CounterPeriod      = 0xFFFF;
    TIMER_CNT_InitStructure.TIMER_CounterEventSource = TIMER_CNT_EVNT_SRC_TIM_CLK;
    TIMER_CNT_InitStructure.TIMER_CounterMode        = TIMER_CNT_MODE_CLK_FIXED_DIR;
    TIMER_CNT_InitStructure.TIMER_CounterDirection   = TIMER_CNT_DIR_UP;
    TIMER_CNT_InitStructure.TIMER_FDTS_Prescaler     = TIMER_FDTS_PRESCALER_DIV_4;
    TIMER_CNT_Init(MDR_TIMER0, &TIMER_CNT_InitStructure);

    /* Initializes the TIMER0 Channel 1 -----------------------------------*/
    TIMER_CH_StructInit(&TIMER_CH_InitStructure);
    TIMER_CH_InitStructure.TIMER_CH_Number              = TIMER_CH1;
    TIMER_CH_InitStructure.TIMER_CH_Mode                = TIMER_CH_MODE_CAPTURE;
    TIMER_CH_InitStructure.TIMER_CH_CAP_CCR_EventSource = TIMER_CH_CAP_CCR_EVNT_SRC_RE;
    TIMER_CH_InitStructure.TIMER_CH_CAP_EventPrescaler  = TIMER_CH_CAP_EVNT_PRESCALER_DIV_1;
    TIMER_CH_InitStructure.TIMER_CH_Filter              = TIMER_CH_FILTER_8FF_AT_FDTS_DIV_32;
    TIMER_CH_Init(MDR_TIMER0, &TIMER_CH_InitStructure);

    /* Configure TIMER0 interrupt */
    TIMER_ITConfig(MDR_TIMER0, TIMER_FLAG_CH1_CAP_CCR, ENABLE);
    NVIC_ClearPendingIRQ(TIMER0_IRQn);
    NVIC_EnableIRQ(TIMER0_IRQn);

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

/** @} */ /* End of group TIMER_CAP_Interrupt_234 */

/** @} */ /* End of group __K1986VK234_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE main.c */


