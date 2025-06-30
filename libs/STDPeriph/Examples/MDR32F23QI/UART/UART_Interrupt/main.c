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
#include "MDR32F2xQI_uart.h"
#include "MDR32F2xQI_port.h"
#include <string.h>
#include <stdlib.h>

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK214_EVAL K1986VK214 Evaluation Board
 *  @{
 */

/** @addtogroup UART_Interrupt_214 UART_Interrupt example for K1986VK214 evaluation board
 * @{
 */

/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE 512

/* Private variables ---------------------------------------------------------*/
volatile static FlagStatus UART0_ITFlagTxComplete = RESET;           /* IT Transfer Complete Flag */
volatile static FlagStatus UART1_ITFlagRxComplete = RESET;           /* IT Receive Complete Flag */
static uint8_t             TxData[BUFFER_SIZE], RxData[BUFFER_SIZE]; /* Transmit and receive buffers */
static uint32_t            NumOfByteTxRx = BUFFER_SIZE / 2;          /* Specified number of bytes to transmit and receive */
static uint32_t            CurrentByteTx, CurrentByteRx;             /* Number of the currently transmitted and received byte */

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  UART0 interrupt handler.
 * @param  None.
 * @retval None.
 */
void UART0_IRQHandler(void)
{
    if (UART_GetITFlagStatus(MDR_UART0, UART_IT_TX) == SET) {
        UART_ClearITFlags(MDR_UART0, UART_IT_TX);

        if (CurrentByteTx < NumOfByteTxRx) {
            UART_SendData(MDR_UART0, TxData[CurrentByteTx++]);
        } else {
            UART0_ITFlagTxComplete = SET;
        }
    }
}

/**
 * @brief  UART1 interrupt handler.
 * @param  None.
 * @retval None.
 */
void UART1_IRQHandler(void)
{
    if (UART_GetITFlagStatus(MDR_UART1, UART_IT_RX) == SET) {
        UART_ClearITFlags(MDR_UART1, UART_IT_RX);

        RxData[CurrentByteRx++] = (uint8_t)UART_ReceiveData(MDR_UART1);
        if (CurrentByteRx == NumOfByteTxRx) {
            UART1_ITFlagRxComplete = SET;
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
    PORT_InitTypeDef PORT_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    int32_t          Error;
    uint32_t         ByteNum;

    /* Reset the clock system settings */
    RST_CLK_DeInit();

    /* Enable HSE clock */
    RST_CLK_HSE_Config(RST_CLK_HSE_ON);
    if (RST_CLK_HSE_GetStatus() == ERROR) {
        while (1) { }
    }

    /* Select the source for HCLK (HCLK = CPU_CLK = CPU_C3 = CPU_C2 = CPU_C1 = HSE) */
    RST_CLK_CPU_C1_ClkSelection(RST_CLK_CPU_C1_SRC_HSE);
    RST_CLK_CPU_C2_ClkSelection(RST_CLK_CPU_C2_SRC_CPU_C1);
    RST_CLK_CPU_C3_SetPrescaler(RST_CLK_CPU_PRESCALER_DIV_1);
    RST_CLK_HCLK_ClkSelection(RST_CLK_CPU_CLK_CPU_C3);

    /* Configure and set PORTC[2] to disable pin RO (connected to PORTC[1]) of D5 (RS-485) */
    RST_CLK_PCLKCmd(RST_CLK_PCLK_PORTC, ENABLE);
    PORT_DeInit(MDR_PORTC);

    PORT_StructInit(&PORT_InitStructure);
    PORT_InitStructure.PORT_Pin       = PORT_PIN_2;
    PORT_InitStructure.PORT_Mode      = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_Direction = PORT_DIRECTION_OUT;
    PORT_InitStructure.PORT_Power     = PORT_POWER_SPEED_SLOW;
    PORT_Init(MDR_PORTC, &PORT_InitStructure);

    PORT_SetBits(MDR_PORTC, PORT_PIN_2);

    /* Configure PORTC[1] (UART1_RXD) for UART1 */
    PORT_InitStructure.PORT_Pin       = PORT_PIN_1;
    PORT_InitStructure.PORT_Function  = PORT_FUNCTION_MAIN;
    PORT_InitStructure.PORT_Direction = PORT_DIRECTION_IN;
    PORT_InitStructure.PORT_Power     = PORT_POWER_OUTPUT_OFF;
    PORT_Init(MDR_PORTC, &PORT_InitStructure);

    /* Configure PORTB[0] (UART0_TXD) for UART0 */
    RST_CLK_PCLKCmd(RST_CLK_PCLK_PORTB, ENABLE);
    PORT_DeInit(MDR_PORTB);

    PORT_InitStructure.PORT_Pin   = PORT_PIN_0;
    PORT_InitStructure.PORT_Power = PORT_POWER_SPEED_MAXFAST;
    PORT_Init(MDR_PORTB, &PORT_InitStructure);

    /* Configure UART0 and UART1 */
    /* Clock source configuration for UART0 and UART1 */
    RST_CLK_PCLKCmd(RST_CLK_PCLK_UART0 | RST_CLK_PCLK_UART1, ENABLE);

    RST_CLK_PER1_C2_ClkSelection(RST_CLK_PER1_C2_UART0, RST_CLK_PER1_C2_CLK_SRC_CPU_C1);
    RST_CLK_PER1_C2_SetPrescaler(RST_CLK_PER1_C2_UART0, RST_CLK_PER1_PRESCALER_DIV_1);
    RST_CLK_PER1_C2_Cmd(RST_CLK_PER1_C2_UART0, ENABLE);

    RST_CLK_PER1_C2_ClkSelection(RST_CLK_PER1_C2_UART1, RST_CLK_PER1_C2_CLK_SRC_CPU_C1);
    RST_CLK_PER1_C2_SetPrescaler(RST_CLK_PER1_C2_UART1, RST_CLK_PER1_PRESCALER_DIV_1);
    RST_CLK_PER1_C2_Cmd(RST_CLK_PER1_C2_UART1, ENABLE);

    /* Initialization of parameters for UART0 and UART1 */
    UART_StructInit(&UART_InitStructure);
    UART_InitStructure.UART_BaudRate = 115200;

    /* Configure UART0 */
    UART_DeInit(MDR_UART0);
    UART_Init(MDR_UART0, &UART_InitStructure);
    UART_ITConfig(MDR_UART0, UART_IT_TX, ENABLE);
    NVIC_ClearPendingIRQ(UART0_IRQn);
    NVIC_EnableIRQ(UART0_IRQn);

    /* Configure UART1 */
    UART_DeInit(MDR_UART1);
    UART_Init(MDR_UART1, &UART_InitStructure);
    UART_ITConfig(MDR_UART1, UART_IT_RX, ENABLE);
    NVIC_ClearPendingIRQ(UART1_IRQn);
    NVIC_EnableIRQ(UART1_IRQn);

    /* Enable UART0 and UART1 */
    UART_Cmd(MDR_UART0, ENABLE);
    UART_Cmd(MDR_UART1, ENABLE);

    while (1) {
        /* Data initialization */
        for (ByteNum = 0; ByteNum < NumOfByteTxRx; ByteNum++) {
            RxData[ByteNum] = 0x00;
            TxData[ByteNum] = (uint8_t)rand();
        }
        CurrentByteRx = 0;
        CurrentByteTx = 0;

        /* UART0: initial filling of the TxFIFO buffer, then the data will be transmitted in the interrupt handler */
        while (UART_GetFlagStatus(MDR_UART0, UART_FLAG_TXFF) == SET) { }
        UART_SendData(MDR_UART0, TxData[CurrentByteTx++]);

        /* UART0: Waiting for the transfer of all data */
        while (UART0_ITFlagTxComplete != SET) { }
        UART0_ITFlagTxComplete = RESET;

        /* UART1: waiting for all data to be received. Data received in interrupt handler */
        while (UART1_ITFlagRxComplete != SET) { }
        UART1_ITFlagRxComplete = RESET;

        /* Verification of received data */
        Error = memcmp(RxData, TxData, NumOfByteTxRx);
        if (Error) {
            while (1) { }
        }

        /* Changing the amount of data transferred */
        NumOfByteTxRx++;
        if (NumOfByteTxRx == BUFFER_SIZE)
            NumOfByteTxRx = BUFFER_SIZE / 2;
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

/** @} */ /* End of group UART_Interrupt_234 */

/** @} */ /* End of group __K1986VK234_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE main.c */


