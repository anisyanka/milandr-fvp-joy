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
#include "MDR32F21QI_crc.h"
#include "MDR32F2xQI_port.h"
#include "MDR32F2xQI_rst_clk.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK234_EVAL K1986VK234 Evaluation Board
 *  @{
 */

/** @addtogroup CRC_234 CRC example for K1986VK234 evaluation board
 *  @{
 */

/* Private define ------------------------------------------------------------*/
// #define DATA_LOAD_8BIT
// #define DATA_LOAD_16BIT
#define DATA_LOAD_32BIT

#define CRC_ARRAY_SIZE 24

#define LED1           PORT_PIN_4

#if defined DATA_LOAD_8BIT
#define CRC_DATA_LOAD_SIZE CRC_DATA_LOAD_SIZE_8BIT
#elif defined DATA_LOAD_16BIT
#define CRC_DATA_LOAD_SIZE CRC_DATA_LOAD_SIZE_16BIT
#elif defined DATA_LOAD_32BIT
#define CRC_DATA_LOAD_SIZE CRC_DATA_LOAD_SIZE_32BIT
#endif

/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;
CRC_InitTypeDef  CRC_InitStruct;
uint8_t          TestArray[CRC_ARRAY_SIZE];

#if defined   DATA_LOAD_8BIT
uint8_t*      PtrData = TestArray;
#elif defined DATA_LOAD_16BIT
uint16_t* PtrData = (uint16_t*)TestArray;
#elif defined DATA_LOAD_32BIT
uint32_t* PtrData = (uint32_t*)TestArray;
#endif
/* The lookup table that is calculated for the crc16 polynomial 0x8005 */
const unsigned short Crc16Table[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};
uint16_t CRC16Soft, CRC16Hard, Cnt;

/* Private function prototypes -----------------------------------------------*/
void     ClockConfigure(void);
uint16_t Crc16(uint8_t* PtrBlock, uint16_t BlockLength);
uint16_t ReverseBits(uint16_t crc);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program.
 * @param  None.
 * @retval None.
 */
int main(void)
{
    RST_CLK_DeInit();

    /* Enable CRC clock and PORTC clock */
    RST_CLK_PCLKCmd((RST_CLK_PCLK_CRC | RST_CLK_PCLK_PORTC), ENABLE);

    PORT_DeInit(MDR_PORTC);

    /* Init the PORTC */
    PORT_StructInit(&PORT_InitStructure);
    PORT_InitStructure.PORT_Pin       = LED1;
    PORT_InitStructure.PORT_Function  = PORT_FUNCTION_PORT;
    PORT_InitStructure.PORT_Mode      = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_Direction = PORT_DIRECTION_OUT;
    PORT_InitStructure.PORT_Power     = PORT_POWER_SPEED_MAXFAST;

    PORT_Init(MDR_PORTC, &PORT_InitStructure);

    /* Reset the crc block */
    CRC_DeInit();

    /* Init the CRC_InitStruct */
    CRC_StructInit(&CRC_InitStruct);

    /* Init crc block */
    CRC_InitStruct.CRC_DataLoadSize = CRC_DATA_LOAD_SIZE;
    CRC_InitStruct.CRC_EndianMode   = CRC_MODE_LITTLE_ENDIAN;
    CRC_InitStruct.CRC_Polynom      = 0x8005;
    CRC_InitStruct.CRC_StartValue   = 0xFFFF;
    CRC_Init(&CRC_InitStruct);

    /* Fill the test array */
    for (Cnt = 0; Cnt < CRC_ARRAY_SIZE; Cnt++) {
        TestArray[Cnt] = Cnt + 0x10;
    }

    /* Software crc calculation */
    CRC16Soft = Crc16(TestArray, CRC_ARRAY_SIZE);

    /* Hardware crc calculation */
    CRC_Cmd(ENABLE);

    /* Fill the FIFO crc block */
    for (Cnt = 0; Cnt < (CRC_ARRAY_SIZE / sizeof(*PtrData)); Cnt++) {
        while (CRC_GetFlagStatus(CRC_FIFO_FULL) == SET) { }
        CRC_WriteData(PtrData[Cnt]);
    }

    /* Wait until crc not computed */
    while (CRC_GetFlagStatus(CRC_CONV_COMP) != 1) { }

    /* Disable crc  */
    CRC_Cmd(DISABLE);

    /* Read the crc value */
    CRC16Hard = CRC_GetCRC();

    /* reverse CRC result */
    CRC16Hard = ReverseBits(CRC16Hard);

    /* Check the crc */
    if (CRC16Soft == CRC16Hard) {
        PORT_SetBits(MDR_PORTC, LED1);
    }

    while (1) { }
}

/**
 * @brief  Software crc calculation.
 * @param  PtrBlock: pointer to input data.
 * @param  BlockLength: number of data byte.
 * @return crc: crc value.
 */
uint16_t Crc16(uint8_t* PtrBlock, uint16_t BlockLength)
{
    uint16_t CRCResult = 0xFFFF;

    while (BlockLength--) {
        CRCResult = (CRCResult >> 8) ^ Crc16Table[(CRCResult & 0xFF) ^ *PtrBlock++];
    }

    return CRCResult;
}

/**
 * @brief  Revert bits in CRC value.
 * @param  CRCValue: input CRC value.
 * @return Inverse CRC value.
 */
uint16_t ReverseBits(uint16_t CRCValue)
{
    uint16_t ReversedCRC = 0;
    int32_t  Cnt;

    for (Cnt = 15; Cnt >= 0; Cnt--) {
        ReversedCRC |= ((CRCValue >> Cnt) & 1) << (15 - Cnt);
    }
    return (ReversedCRC);
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

/** @} */ /* End of group CRC_234 */

/** @} */ /* End of group __K1986VK234_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr *******************************/

/* END OF FILE main.c */


