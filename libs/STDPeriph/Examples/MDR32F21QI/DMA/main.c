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
#include "MDR32F21QI_dma.h"
#include "MDR32F2xQI_port.h"
#include "MDR32F2xQI_rst_clk.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK234_EVAL K1986VK234 Evaluation Board
 *  @{
 */

/** @addtogroup DMA_234 DMA example for K1986VK234 evaluation board
 *  @{
 */

/* Private define ------------------------------------------------------------*/
#define SIZE 512

/* Private variables ---------------------------------------------------------*/

volatile ErrorStatus TransferStatus1 = ERROR, TransferStatus2 = ERROR;

uint32_t DestBuf[SIZE];
uint32_t PriBuf[SIZE];
uint32_t AltBuf[SIZE];

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Generation of random values.
 * @param  MemAddr: memory address.
 * @retval Random 32-bit value.
 */
uint32_t GeneratePseudoRandomNumber(uint32_t MemAddr)
{
    uint32_t Hash = 0;
    uint32_t Cnt;
    char*    Key = (char*)&MemAddr;

    for (Cnt = 0; Cnt < 4; Cnt++) {
        Hash += Key[Cnt];
        Hash += (Hash << 10);
        Hash ^= (Hash >> 6);
    }

    for (Cnt = 0; Cnt < 256; Cnt++) {
        Hash += (Hash << 10);
        Hash ^= (Hash >> 6);
    }

    Hash += (Hash << 3);
    Hash ^= (Hash >> 11);
    Hash += (Hash << 15);

    return Hash;
}

/**
 * @brief  Initialisation of memory with pseudo random values.
 * @param  MemAddr: memory address.
 * @param  Size: memory Size.
 * @retval None.
 */
void InitRAM(uint32_t* MemAddr, uint32_t Size)
{
    uint32_t Cnt;

    for (Cnt = 0; Cnt < Size; Cnt++) {
        *MemAddr = GeneratePseudoRandomNumber((uint32_t)MemAddr + Cnt * 4);
        MemAddr++;
    }
}

/**
 * @brief  Compares two buffers.
 * @param  BufSize: buffer's size.
 * @param  PtrBuffer1: first comparison buffer.
 * @param  PtrBuffer2: second comparison buffers.
 * @retval Comparison result.
 *         SUCCESS: PtrBuffer1 identical to PtrBuffer2;
 *         ERROR: PtrBuffer1 differs from PtrBuffer2.
 */
ErrorStatus VerifyMemory(uint32_t BufSize, uint32_t* PtrBuffer1, uint32_t* PtrBuffer2)
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
 * @param  None.
 * @retval None.
 */
int main(void)
{
    DMA_Channel_InitTypeDef  DMA_InitStr;
    DMA_CtrlData_InitTypeDef DMA_PriCtrlStr;
    DMA_CtrlData_InitTypeDef DMA_AltCtrlStr;

    DMA_ValidChannels_TypeDef DMA_Channel = DMA_CHANNEL_SW1;

    RST_CLK_DeInit();
    RST_CLK_PCLKCmd(RST_CLK_PCLK_DMA, ENABLE);

    /* Init RAM */
    InitRAM(DestBuf, SIZE);
    InitRAM(PriBuf, SIZE);
    InitRAM(AltBuf, SIZE);

    /* Reset all settings */
    DMA_DeInit();
    DMA_StructInit(&DMA_InitStr);

    /* Set Primary Control Data */
    DMA_PriCtrlStr.DMA_SourceBaseAddr    = (uint32_t)PriBuf;
    DMA_PriCtrlStr.DMA_DestBaseAddr      = (uint32_t)DestBuf;
    DMA_PriCtrlStr.DMA_SourceIncSize     = DMA_DATA_INC_32BIT;
    DMA_PriCtrlStr.DMA_DestIncSize       = DMA_DATA_INC_32BIT;
    DMA_PriCtrlStr.DMA_MemoryDataSize    = DMA_MEMORY_DATA_SIZE_32BIT;
    DMA_PriCtrlStr.DMA_Mode              = DMA_MODE_BASIC;
    DMA_PriCtrlStr.DMA_CycleSize         = SIZE;
    DMA_PriCtrlStr.DMA_ArbitrationPeriod = DMA_ARB_AFTER_16;

    /* Set Alternate Control Data */
    DMA_AltCtrlStr.DMA_SourceBaseAddr    = (uint32_t)AltBuf;
    DMA_AltCtrlStr.DMA_DestBaseAddr      = (uint32_t)DestBuf;
    DMA_AltCtrlStr.DMA_SourceIncSize     = DMA_DATA_INC_32BIT;
    DMA_AltCtrlStr.DMA_DestIncSize       = DMA_DATA_INC_32BIT;
    DMA_AltCtrlStr.DMA_MemoryDataSize    = DMA_MEMORY_DATA_SIZE_32BIT;
    DMA_AltCtrlStr.DMA_Mode              = DMA_MODE_AUTO_REQUEST;
    DMA_AltCtrlStr.DMA_CycleSize         = SIZE;
    DMA_AltCtrlStr.DMA_ArbitrationPeriod = DMA_ARB_AFTER_16;

    /* Set Channel Structure */
    DMA_InitStr.DMA_PriCtrlData         = &DMA_PriCtrlStr;
    DMA_InitStr.DMA_AltCtrlData         = &DMA_AltCtrlStr;
    DMA_InitStr.DMA_UseHighPriority     = DISABLE;
    DMA_InitStr.DMA_UseBurst            = DISABLE;
    DMA_InitStr.DMA_SelectDataStructure = DMA_CTRL_DATA_PRIMARY;

    /* Init DMA channel */
    DMA_Init(DMA_Channel, &DMA_InitStr);

    /* Run channel */
    DMA_Cmd(DMA_Channel, ENABLE);
    DMA_Request(DMA_Channel);

    /* Transfer complete */
    while (DMA_GetFlagStatus(DMA_Channel, DMA_FLAG_CHANNEL_ENABLED)) {
        if (DMA_GetState() == DMA_STATE_IDLE) {
            DMA_Request(DMA_Channel);
        }
    }
    /* Check the correctness of written data */
    TransferStatus1 = VerifyMemory(SIZE, PriBuf, DestBuf);
    /* TransferStatus1 = SUCCESS, if the data transmitted and received are correct */
    /* TransferStatus1 = ERROR, if the data transmitted and received are different */

    /* Set Channel Structure */
    DMA_InitStr.DMA_SelectDataStructure = DMA_CTRL_DATA_ALTERNATE;

    /* Init DMA channel */
    DMA_Init(DMA_Channel, &DMA_InitStr);

    /* Run channel */
    DMA_Cmd(DMA_Channel, ENABLE);
    DMA_Request(DMA_Channel);

    /* Transfer complete */
    while (DMA_GetFlagStatus(DMA_Channel, DMA_FLAG_CHANNEL_ENABLED)) { }

    /* Check the correctness of written data */
    TransferStatus2 = VerifyMemory(SIZE, AltBuf, DestBuf);

    /* TransferStatus2 = SUCCESS, if the data transmitted and received are correct */
    /* TransferStatus2 = ERROR, if the data transmitted and received are different */

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

/** @} */ /* End of group DMA_234 */

/** @} */ /* End of group __K1986VK234_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/*********************** (C) COPYRIGHT 2024 Milandr ***************************/

/* END OF FILE main.c */


