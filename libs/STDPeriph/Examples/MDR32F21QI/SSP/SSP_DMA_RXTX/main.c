/**
 ******************************************************************************
 * @file    main.c
 * @author  Milandr Application Team
 * @version V1.0.0
 * @date    26/05/2023
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
#include "MDR32F2xQI_rst_clk.h"
#include "MDR32F2xQI_port.h"
#include "MDR32F2xQI_ssp.h"
#include "MDR32F21QI_dma.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK234_EVAL K1986VK234 Evaluation Board
 *  @{
 */

/** @addtogroup SSP_DMA_RXTX_234 SSP DMA RXTX example for K1986VK234 evaluation board
 *  @{
 */

/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE 32

/* Private variables ---------------------------------------------------------*/

PORT_InitTypeDef         PORT_InitStructure;
DMA_Channel_InitTypeDef  DMA_InitStructure;
DMA_CtrlData_InitTypeDef DMA_PriCtrlInitCtructure;
SSP_InitTypeDef          SSP_InitStructure;

uint16_t SrcBuf[BUFFER_SIZE] __attribute__((aligned(4)));

uint16_t DstBuf[BUFFER_SIZE] __attribute__((aligned(4)));

volatile ErrorStatus TransferStatus1 = ERROR;

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Compares two buffers.
 * @param  BufSize: buffer's size.
 * @param  PtrBuffer1: first comparison buffer.
 * @param  PtrBuffer2: second comparison buffers.
 * @retval Comparison result.
 *         SUCCESS: PtrBuffer1 identical to PtrBuffer2;
 *         ERROR: PtrBuffer1 differs from PtrBuffer2.
 */
ErrorStatus VerifyMemory(uint32_t BufSize, uint16_t* PtrBuffer1, uint16_t* PtrBuffer2)
{
    uint32_t Cnt;

    for (Cnt = 0; Cnt < BufSize; Cnt++) {
        if (*PtrBuffer1++ != *PtrBuffer2++) {
            return (ERROR);
        }
    }

    return (SUCCESS);
}

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
    uint32_t Cnt;

    RST_CLK_DeInit();
    RST_CLK_PCLKCmd((RST_CLK_PCLK_RST_CLK | RST_CLK_PCLK_SSP |
                     RST_CLK_PCLK_PORTA | RST_CLK_PCLK_DMA),
                    ENABLE);

    PORT_DeInit(MDR_PORTA);

    PORT_StructInit(&PORT_InitStructure);
    /* Configure SSP pins: FSS, CLK, RXD, TXD */
    PORT_InitStructure.PORT_Pin       = (PORT_PIN_12 | PORT_PIN_13 | PORT_PIN_15);
    PORT_InitStructure.PORT_Function  = PORT_FUNCTION_MAIN;
    PORT_InitStructure.PORT_Mode      = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_Direction = PORT_DIRECTION_OUT;
    PORT_Init(MDR_PORTA, &PORT_InitStructure);

    PORT_InitStructure.PORT_Pin       = (PORT_PIN_14);
    PORT_InitStructure.PORT_Direction = PORT_DIRECTION_IN;
    PORT_Init(MDR_PORTA, &PORT_InitStructure);

    for (Cnt = 0; Cnt < BUFFER_SIZE; Cnt++) {
        SrcBuf[Cnt] = Cnt;
    }

    SSP_DeInit();

    RST_CLK_PER1_C2_SetPrescaler(RST_CLK_PER1_C2_SSP, RST_CLK_PER1_PRESCALER_DIV_16);
    RST_CLK_PER1_C2_Cmd(RST_CLK_PER1_C2_SSP, ENABLE);

    /* SSP MASTER configuration */
    SSP_StructInit(&SSP_InitStructure);

    SSP_InitStructure.SSP_SCR           = 0x10;
    SSP_InitStructure.SSP_CPSDVSR       = 2;
    SSP_InitStructure.SSP_Mode          = SSP_MODE_MASTER;
    SSP_InitStructure.SSP_WordLength    = SSP_WORD_LENGTH_16BIT;
    SSP_InitStructure.SSP_ClockPhase    = SSP_CLOCK_PHASE_1EDGE;
    SSP_InitStructure.SSP_ClockPolarity = SSP_CLOCK_POLARITY_LOW;
    SSP_InitStructure.SSP_FrameFormat   = SSP_FRAME_FORMAT_SPI;
    SSP_InitStructure.SSP_LoopbackMode  = SSP_LOOPBACK_MODE_ENABLE;
    SSP_Init(&SSP_InitStructure);

    /* Reset all DMA settings */
    DMA_DeInit();

    DMA_StructInit(&DMA_InitStructure);
    /* DMA_Channel_SSP1_RX configuration */
    /* Set Primary Control Data */
    DMA_PriCtrlInitCtructure.DMA_SourceBaseAddr = (uint32_t)(&(MDR_SSP->DR));
    DMA_PriCtrlInitCtructure.DMA_DestBaseAddr   = (uint32_t)DstBuf;
    DMA_PriCtrlInitCtructure.DMA_SourceIncSize  = DMA_DATA_INC_NO;
    DMA_PriCtrlInitCtructure.DMA_DestIncSize    = DMA_DATA_INC_16BIT;
    DMA_PriCtrlInitCtructure.DMA_MemoryDataSize = DMA_MEMORY_DATA_SIZE_16BIT;
    DMA_PriCtrlInitCtructure.DMA_Mode           = DMA_MODE_BASIC;
    DMA_PriCtrlInitCtructure.DMA_CycleSize      = BUFFER_SIZE;
    /* Set Channel Structure */
    DMA_InitStructure.DMA_PriCtrlData         = &DMA_PriCtrlInitCtructure;
    DMA_InitStructure.DMA_UseHighPriority     = ENABLE;
    DMA_InitStructure.DMA_SelectDataStructure = DMA_CTRL_DATA_PRIMARY;
    /* Init DMA channel */
    DMA_Init(DMA_CHANNEL_SSP_RX, &DMA_InitStructure);

    /* DMA_Channel_SSP2_TX configuration */
    /* Set Primary Control Data */
    DMA_PriCtrlInitCtructure.DMA_SourceBaseAddr = (uint32_t)SrcBuf;
    DMA_PriCtrlInitCtructure.DMA_DestBaseAddr   = (uint32_t)(&(MDR_SSP->DR));
    DMA_PriCtrlInitCtructure.DMA_SourceIncSize  = DMA_DATA_INC_16BIT;
    DMA_PriCtrlInitCtructure.DMA_DestIncSize    = DMA_DATA_INC_NO;
    /* Init DMA channel */
    DMA_Init(DMA_CHANNEL_SSP_TX, &DMA_InitStructure);

    /* Enable SSP */
    SSP_Cmd(ENABLE);

    /* Enable SSP DMA Rx and Tx request */
    SSP_DMACmd(SSP_DMA_REQ_RX | SSP_DMA_REQ_TX, ENABLE);

    while ((DMA_GetFlagStatus(DMA_CHANNEL_SSP_TX, DMA_FLAG_CHANNEL_ENABLED))) { }
    while ((DMA_GetFlagStatus(DMA_CHANNEL_SSP_RX, DMA_FLAG_CHANNEL_ENABLED))) { }

    /* Check the corectness of written data */
    TransferStatus1 = VerifyMemory((BUFFER_SIZE), SrcBuf, DstBuf);

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

/** @} */ /* End of group SSP_DMA_RXTX_234 */

/** @} */ /* End of group __K1986VK234_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr *******************************/

/* END OF FILE main.c */


