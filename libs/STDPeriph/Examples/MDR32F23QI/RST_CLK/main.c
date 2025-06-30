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
 * <h2><center>&copy; COPYRIGHT 2024 Milandr </center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "MDR32F2xQI_port.h"
#include "MDR32F2xQI_rst_clk.h"
#include "MDR32F2xQI_bkp.h"
#include "MDR32F2xQI_eeprom.h"
#include "MDR32F2xQI_utils.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK214_EVAL K1986VK214 Evaluation Board
 *  @{
 */

/** @addtogroup RST_CLK_214 RST_CLK example for K1986VK214 evaluation board
 *  @{
 */

/* Private define ------------------------------------------------------------*/

#define LED1       PORT_PIN_14
#define LED2       PORT_PIN_15

#define LED_PERIOD 100000
#define BLINK_NUM  5

/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef GPIOInitStruct;

/* Private function prototypes -----------------------------------------------*/
void BlinkLED(uint32_t num, uint32_t del, uint32_t led);

/**
 * @brief  Main program.
 * @param  None.
 * @retval None.
 */
int main(void)
{
    RST_CLK_DeInit();
    RST_CLK_PCLKCmd((RST_CLK_PCLK_PORTC | RST_CLK_PCLK_EEPROM | RST_CLK_PCLK_BKP), ENABLE);

    /* Setup internal DUcc voltage regulator work mode based on clock frequency */
    BKP_DUcc_SetMode(BKP_DUCC_CLK_UPTO_10MHz);

    /* Setup EEPROM access delay to 0 because HSI = 8MHz < 18MHz.
       EEPROM access delay sould be set before the clock frequency increases or
       after the clock frequency decreases. */
    EEPROM_SetLatency(EEPROM_LATENCY_CYCLE_0);

    PORT_DeInit(MDR_PORTC);

    /* Configure PORTC pins 14..15 for output to switch LEDs on/off */
    PORT_StructInit(&GPIOInitStruct);
    GPIOInitStruct.PORT_Pin       = (LED1 | LED2);
    GPIOInitStruct.PORT_Direction = PORT_DIRECTION_OUT;
    GPIOInitStruct.PORT_Power     = PORT_POWER_SPEED_FAST;
    GPIOInitStruct.PORT_Mode      = PORT_MODE_DIGITAL;
    PORT_Init(MDR_PORTC, &GPIOInitStruct);

    /* Consequently turn all used LEDs on and off */
    PORT_SetBits(MDR_PORTC, LED1);
    DELAY_PROGRAM_WaitLoopsAsm(4 * LED_PERIOD);
    PORT_ResetBits(MDR_PORTC, LED1);
    DELAY_PROGRAM_WaitLoopsAsm(4 * LED_PERIOD);
    PORT_SetBits(MDR_PORTC, LED2);
    DELAY_PROGRAM_WaitLoopsAsm(4 * LED_PERIOD);
    PORT_ResetBits(MDR_PORTC, LED2);

    /* 1. CPU_CLK = 4*HSE/2 clock */

    /* Enable HSE clock oscillator */
    RST_CLK_HSE_Config(RST_CLK_HSE_ON);
    /* Switch LED1 on and wait for HSE ready status */
    PORT_SetBits(MDR_PORTC, LED1);
    DELAY_PROGRAM_WaitLoopsAsm(LED_PERIOD);
    if (RST_CLK_HSE_GetStatus() == SUCCESS) /* Good HSE clock */
    {
        /* Select HSE clock as CPU_PLL input clock source */
        /* Set PLL multiplier to 4 */
        RST_CLK_CPU_PLL_Config(RST_CLK_PLLCPU_SRC_HSE, RST_CLK_PLLCPU_MUL_4);
        /* Enable CPU_PLL */
        RST_CLK_CPU_PLL_Cmd(ENABLE);
        /* Wait for PLL ready status */
        if (RST_CLK_CPU_PLL_GetStatus() == SUCCESS) /* Good CPU PLL */
        {
            /* Switch LED1 off */
            PORT_ResetBits(MDR_PORTC, LED1);
            /* Set CPU_C3_prescaler to 2 */
            RST_CLK_CPU_C3_SetPrescaler(RST_CLK_CPU_PRESCALER_DIV_2);
            /* Set CPU_C2_SEL to CPU_PLL output instead of CPU_C1 clock */
            RST_CLK_CPU_C2_ClkSelection(RST_CLK_CPU_C2_SRC_PLLCPU);

            /* Setup internal DUcc voltage regulator work mode based on clock frequency */
            BKP_DUcc_SetMode(BKP_DUCC_CLK_UPTO_40MHz);
            /* Setup EEPROM access delay to 0: 4*HSE/2 = 16MHz < 18MHz */
            EEPROM_SetLatency(EEPROM_LATENCY_CYCLE_0);

            /* Select CPU_C3 clock on the CPU clock MUX */
            RST_CLK_HCLK_ClkSelection(RST_CLK_CPU_CLK_CPU_C3);
            /* LED1 blinking with 4*HSE/2 clock as input clock source */
            BlinkLED(BLINK_NUM, LED_PERIOD, LED1);
        } else /* CPU_PLL timeout */
        {
            while (1) { }
        }
    } else /* HSE timeout */
    {
        while (1) { }
    }

    /* 2. CPU_CLK = LSI clock */

    /* Enable LSI clock source */
    BKP_LSI_Cmd(ENABLE);
    /* Switch LED2 on and wait for LSI ready status */
    PORT_SetBits(MDR_PORTC, LED2);
    DELAY_PROGRAM_WaitLoopsAsm(LED_PERIOD);
    if (BKP_LSI_GetStatus() == SUCCESS) /* Good LSI clock */
    {
        /* Switch LED2 off */
        PORT_ResetBits(MDR_PORTC, LED2);
        /* Select LSI clock on the CPU clock MUX */
        RST_CLK_HCLK_ClkSelection(RST_CLK_CPU_CLK_LSI);

        /* Disable CPU_PLL which was used on previous step */
        RST_CLK_CPU_PLL_Cmd(DISABLE);

        /* Setup internal DUcc voltage regulator work mode based on clock frequency */
        BKP_DUcc_SetMode(BKP_DUCC_CLK_UPTO_200kHz);
        /* Setup EEPROM access delay to 0: LSI = 40kHz < 18MHz */
        EEPROM_SetLatency(EEPROM_LATENCY_CYCLE_0);

        /* LED2 blinking with LSI clock as input clock source */
        BlinkLED(BLINK_NUM, LED_PERIOD / 100, LED2);
    } else /* LSI timeout */
    {
        while (1) { }
    }

    /* 2. CPU_CLK = LSE clock */

    /* Enable LSE clock source */
    BKP_LSE_Config(BKP_LSE_ON);
    /* Switch LED1 and LED2 on and wait for LSE ready status */
    PORT_SetBits(MDR_PORTC, (LED1 | LED2));
    DELAY_PROGRAM_WaitLoopsAsm(LED_PERIOD / 10);
    if (BKP_LSE_GetStatus() == SUCCESS) /* Good LSE clock */
    {
        /* Switch LED4 off */
        PORT_ResetBits(MDR_PORTC, (LED1 | LED2));

        /* Select LSE clock on the CPU clock MUX */
        RST_CLK_HCLK_ClkSelection(RST_CLK_CPU_CLK_LSE);

        /* LED1 and LED2 blinking with LSI clock as input clock source */
        BlinkLED(BLINK_NUM, LED_PERIOD / 100, (LED1 | LED2));
    } else /* LSE timeout */
    {
        while (1) { }
    }

    while (1) { }
}


/**
 * @brief  Blink with LED.
 * @param  num - blinks number.
 * @param  del - delay.
 * @param  led - led number.
 * @retval None.
 */
void BlinkLED(uint32_t num, uint32_t del, uint32_t led)
{
    uint32_t cnt;

    for (cnt = 0; cnt < num; cnt++) {
        PORT_SetBits(MDR_PORTC, led);
        DELAY_PROGRAM_WaitLoopsAsm(del);
        PORT_ResetBits(MDR_PORTC, led);
        DELAY_PROGRAM_WaitLoopsAsm(del);
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
#endif    /* USE_ASSERT_INFO */

/** @} */ /* End of group RST_CLK_214 */

/** @} */ /* End of group __K1986VK214_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr *******************************/

/* END OF FILE main.c */


