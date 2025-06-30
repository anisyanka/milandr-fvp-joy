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
#include "MDR32F21QI_dma.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK234_EVAL K1986VK234 Evaluation Board
 *  @{
 */

/** @addtogroup TIMER_PWM_DMA_234 TIMER_PWM_DMA example for K1986VK234 evaluation board
 * @{
 */

/* Private define ------------------------------------------------------------*/
#define CCR_DATA_LENGTH 6

/* Private variables ---------------------------------------------------------*/
static uint16_t CCR_Val[CCR_DATA_LENGTH] = { 10, 50, 100, 200, 300, 400 };

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  DMA interrupt handler.
 * @param  None.
 * @retval None.
 */
void DMA_IRQHandler(void)
{
    /* Processing of the DMA_Channel_TIM0 */
    if ((DMA_GetChannelState(DMA_CHANNEL_TIM0, DMA_CTRL_DATA_PRIMARY) == DMA_MODE_STOP) ||
        (DMA_GetChannelState(DMA_CHANNEL_TIM0, DMA_CTRL_DATA_ALTERNATE) == DMA_MODE_STOP)) {
        /* Reconfigure the inactive DMA data structure */
        if (DMA_GetFlagStatus(DMA_CHANNEL_TIM0, DMA_FLAG_CHANNEL_ALTERNATE) == SET) {
            DMA_ChannelReloadCycle(DMA_CHANNEL_TIM0, DMA_CTRL_DATA_PRIMARY, CCR_DATA_LENGTH, DMA_MODE_PING_PONG);
        } else {
            DMA_ChannelReloadCycle(DMA_CHANNEL_TIM0, DMA_CTRL_DATA_ALTERNATE, CCR_DATA_LENGTH, DMA_MODE_PING_PONG);
        }
    }
}

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
    DMA_Channel_InitTypeDef  DMA_InitStr;
    DMA_CtrlData_InitTypeDef DMA_PriAltCtrlStr;

    /* Reset the clock system settings */
    RST_CLK_DeInit();

    /* Configure PORTA[1:0] (TMR0_CH1(N)) for TIMER0 */
    RST_CLK_PCLKCmd((RST_CLK_PCLK_PORTA), ENABLE);
    PORT_DeInit(MDR_PORTA);

    PORT_StructInit(&PORT_InitStructure);
    PORT_InitStructure.PORT_Pin      = PORT_PIN_0 | PORT_PIN_1;
    PORT_InitStructure.PORT_Mode     = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_Function = PORT_FUNCTION_MAIN;
    PORT_InitStructure.PORT_Power    = PORT_POWER_SPEED_MAXFAST;
    PORT_Init(MDR_PORTA, &PORT_InitStructure);

    /* Configure TIMER0 to PWM mode */
    RST_CLK_PCLKCmd(RST_CLK_PCLK_TIMER0, ENABLE);

    RST_CLK_HSI_C1_SetPrescaler(RST_CLK_HSI_PRESCALER_DIV_1);
    RST_CLK_PER1_C2_ClkSelection(RST_CLK_PER1_C2_CLK_SRC_HSI_CLK);

    RST_CLK_PER1_C2_SetPrescaler(RST_CLK_PER1_C2_TIM0, RST_CLK_PER1_PRESCALER_DIV_1);
    RST_CLK_PER1_C2_Cmd(RST_CLK_PER1_C2_TIM0, ENABLE);

    TIMER_DeInit(MDR_TIMER0);

    /* Initializes the TIMER0 Counter -------------------------------------*/
    TIMER_CNT_StructInit(&TIMER_CNT_InitStructure);
    TIMER_CNT_InitStructure.TIMER_CounterPrescaler   = 7;
    TIMER_CNT_InitStructure.TIMER_CounterPeriod      = 499;
    TIMER_CNT_InitStructure.TIMER_CounterEventSource = TIMER_CNT_EVNT_SRC_TIM_CLK;
    TIMER_CNT_InitStructure.TIMER_CounterMode        = TIMER_CNT_MODE_CLK_FIXED_DIR;
    TIMER_CNT_InitStructure.TIMER_CounterDirection   = TIMER_CNT_DIR_UP;
    TIMER_CNT_Init(MDR_TIMER0, &TIMER_CNT_InitStructure);

    /* Initializes the TIMER0 Channel 1 -----------------------------------*/
    TIMER_CH_StructInit(&TIMER_CH_InitStructure);
    TIMER_CH_InitStructure.TIMER_CH_Number             = TIMER_CH1;
    TIMER_CH_InitStructure.TIMER_CH_Mode               = TIMER_CH_MODE_PWM;
    TIMER_CH_InitStructure.TIMER_CH_PWM_REF_Format     = TIMER_CH_PWM_REF_FORMAT_6;
    TIMER_CH_InitStructure.TIMER_CH_PWM_CCR_UpdateMode = TIMER_CH_PWM_CCR_UPD_ON_CNT_EQ_0;
    TIMER_CH_Init(MDR_TIMER0, &TIMER_CH_InitStructure);

    /* Initializes the TIMER0 Channel 1,1N Outputs ------------------------*/
    TIMER_CH_OUT_StructInit(&TIMER_CH_OUT_InitStructure);
    TIMER_CH_OUT_InitStructure.TIMER_CH_Number      = TIMER_CH1;
    TIMER_CH_OUT_InitStructure.TIMER_CH_OUTP_Source = TIMER_CH_OUTP_SRC_REF;
    TIMER_CH_OUT_InitStructure.TIMER_CH_OUTP_Mode   = TIMER_CH_OUTP_MODE_OUT;
    TIMER_CH_OUT_InitStructure.TIMER_CH_OUTN_Source = TIMER_CH_OUTN_SRC_REF;
    TIMER_CH_OUT_InitStructure.TIMER_CH_OUTN_Mode   = TIMER_CH_OUTN_MODE_OUT;
    TIMER_CH_OUT_Init(MDR_TIMER0, &TIMER_CH_OUT_InitStructure);

    /* Enable TIMER0 DMA request (CNT == ARR) */
    TIMER_DMACmd(MDR_TIMER0, TIMER_FLAG_CNT_ARR, ENABLE);

    /* Configure DMA */
    RST_CLK_PCLKCmd(RST_CLK_PCLK_DMA, ENABLE);
    DMA_DeInit();

    /* Set Primary/Alternate Control Data ---------------------------------*/
    DMA_PriAltCtrlStr.DMA_SourceBaseAddr    = (uint32_t)CCR_Val;
    DMA_PriAltCtrlStr.DMA_DestBaseAddr      = (uint32_t)(&(MDR_TIMER0->CCR1));
    DMA_PriAltCtrlStr.DMA_SourceIncSize     = DMA_DATA_INC_16BIT;
    DMA_PriAltCtrlStr.DMA_DestIncSize       = DMA_DATA_INC_NO;
    DMA_PriAltCtrlStr.DMA_MemoryDataSize    = DMA_MEMORY_DATA_SIZE_16BIT;
    DMA_PriAltCtrlStr.DMA_Mode              = DMA_MODE_PING_PONG;
    DMA_PriAltCtrlStr.DMA_CycleSize         = CCR_DATA_LENGTH;
    DMA_PriAltCtrlStr.DMA_ArbitrationPeriod = DMA_ARB_AFTER_1;

    /* Set Channel Structure ----------------------------------------------*/
    DMA_StructInit(&DMA_InitStr);
    DMA_InitStr.DMA_PriCtrlData         = &DMA_PriAltCtrlStr;
    DMA_InitStr.DMA_AltCtrlData         = &DMA_PriAltCtrlStr;
    DMA_InitStr.DMA_UseHighPriority     = DISABLE;
    DMA_InitStr.DMA_UseBurst            = DISABLE;
    DMA_InitStr.DMA_SelectDataStructure = DMA_CTRL_DATA_PRIMARY;

    /* Init DMA channel TIMER0 */
    DMA_Init(DMA_CHANNEL_TIM0, &DMA_InitStr);
    NVIC_ClearPendingIRQ(DMA_IRQn);
    NVIC_EnableIRQ(DMA_IRQn);

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

/** @} */ /* End of group TIMER_PWM_DMA_234 */

/** @} */ /* End of group __K1986VK234_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE main.c */


