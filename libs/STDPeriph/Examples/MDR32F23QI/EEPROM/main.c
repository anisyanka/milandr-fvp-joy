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
#include "MDR32F2xQI_eeprom.h"
#include "MDR32F2xQI_utils.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK214_EVAL K1986VK214 Evaluation Board
 *  @{
 */

/** @addtogroup EEPROM_214 EEPROM example for K1986VK214 evaluation board
 *  @{
 */

/* Private define ------------------------------------------------------------*/
#define EEPROM_MAIN_BANK_SIZE (32 * 1024)
#define EEPROM_INFO_BANK_SIZE (2 * 1024)

#define LED_MASK              (PORT_PIN_14 | PORT_PIN_15)

/* Private macro -------------------------------------------------------------*/
#define LED_TST_NUM(num) (((num) << 14) & LED_MASK)

/* Private variables ---------------------------------------------------------*/
static PORT_InitTypeDef PORT_InitStructure;

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
 * @brief  Check main block is empty.
 * @param  None.
 * @retval Result - @ref ErrorStatus.
 */
ErrorStatus BlankCheckMainMemory(void)
{
    uint32_t                  Address = 0;
    EEPROM_MemoryBank_TypeDef BankSelector;
    uint32_t                  Data = 0;
    uint32_t                  i    = 0;
    ErrorStatus               Errs = SUCCESS;

    /* Check main memory bank */
    Address      = 0x00008000;
    BankSelector = EEPROM_BANK_MAIN;
    Data         = 0xFFFFFFFF;
    for (i = 0; i < EEPROM_MAIN_BANK_SIZE; i += 4) {
        if (EEPROM_ReadWord(Address + i, BankSelector) != Data) {
            Errs = ERROR;
            break;
        }
    }
    return Errs;
}

/**
 * @brief  Check info block is empty.
 * @param  None.
 * @retval Result - @ref ErrorStatus.
 */
ErrorStatus BlankCheckInfoMemory(void)
{
    uint32_t                  Address = 0;
    EEPROM_MemoryBank_TypeDef BankSelector;
    uint32_t                  Data = 0;
    uint32_t                  i    = 0;
    ErrorStatus               Errs = SUCCESS;

    /* Check information memory bank */
    Address      = 0x00008000;
    BankSelector = EEPROM_BANK_INFO;
    Data         = 0xFFFFFFFF;
    for (i = 0; i < EEPROM_INFO_BANK_SIZE; i += 4) {
        if (EEPROM_ReadWord(Address + i, BankSelector) != Data) {
            Errs = ERROR;
            break;
        }
    }
    return Errs;
}

/**
 * @brief  Fill main memory bank with pseudo-random values.
 * @param  None.
 * @retval None.
 */
void FillMainMemory(void)
{
    uint32_t                  Address = 0;
    EEPROM_MemoryBank_TypeDef BankSelector;
    uint32_t                  Data = 0;
    uint32_t                  i    = 0;

    /* Fill main memory bank */
    Address      = 0x00008000;
    BankSelector = EEPROM_BANK_MAIN;
    for (i = 0; i < EEPROM_MAIN_BANK_SIZE; i += 11 * 4) {
        Data = GeneratePseudoRandomNumber(Address + i);
        EEPROM_ProgramWord(Address + i, BankSelector, Data);
    }
}

/**
 * @brief  Verify main memory bank.
 * @param  None.
 * @retval Result - @ref ErrorStatus.
 */
ErrorStatus VerifyMainMemory(void)
{
    uint32_t                  Address = 0;
    EEPROM_MemoryBank_TypeDef BankSelector;
    uint32_t                  Data = 0;
    uint32_t                  i    = 0;
    ErrorStatus               Errs = SUCCESS;

    /* Check main memory bank */
    Address      = 0x00008000;
    BankSelector = EEPROM_BANK_MAIN;
    for (i = 0; i < EEPROM_MAIN_BANK_SIZE; i += 11 * 4) {
        Data = GeneratePseudoRandomNumber(Address + i);
        if (EEPROM_ReadWord(Address + i, BankSelector) != Data) {
            Errs = ERROR;
            break;
        }
    }
    return Errs;
}

/**
 * @brief  Fill info memory bank with pseudo-random values.
 * @param  None.
 * @retval None.
 */
void FillInfoMemory(void)
{
    uint32_t                  Address = 0;
    EEPROM_MemoryBank_TypeDef BankSelector;
    uint32_t                  Data = 0;
    uint32_t                  i    = 0;

    /* Fill information memory bank */
    Address      = 0x00008000;
    BankSelector = EEPROM_BANK_INFO;
    for (i = 0; i < EEPROM_INFO_BANK_SIZE; i += 4) {
        Data = GeneratePseudoRandomNumber(Address + i + 1);
        EEPROM_ProgramWord(Address + i, BankSelector, Data);
    }
}

/**
 * @brief  Verify info memory bank.
 * @param  None.
 * @retval Result - @ref ErrorStatus.
 */
ErrorStatus VerifyInfoMemory(void)
{
    uint32_t                  Address = 0;
    EEPROM_MemoryBank_TypeDef BankSelector;
    uint32_t                  Data = 0;
    uint32_t                  i    = 0;
    ErrorStatus               Errs = SUCCESS;

    /* Check information memory bank */
    Address      = 0x00008000;
    BankSelector = EEPROM_BANK_INFO;
    for (i = 0; i < EEPROM_INFO_BANK_SIZE; i += 4) {
        Data = GeneratePseudoRandomNumber(Address + i + 1);
        if (EEPROM_ReadWord(Address + i, BankSelector) != Data) {
            Errs = ERROR;
            break;
        }
    }
    return Errs;
}

/**
 * @brief  Indicates status of selected test.
 * @param  TestStatus - @ref ErrorStatus - test result.
 * @param  TestNum - test number.
 * @retval None.
 */
void ShowTestStatus(ErrorStatus TestStatus, uint32_t TestNum)
{
    uint32_t Leds = LED_TST_NUM(TestNum);
    if (TestStatus == SUCCESS) {
        PORT_Write(MDR_PORTC, Leds);
        DELAY_WaitMs(500);
    } else {
        while (1) {
            Leds ^= LED_TST_NUM(TestNum);
            PORT_Write(MDR_PORTC, Leds & LED_MASK);
            DELAY_WaitMs(500);
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
    RST_CLK_DeInit();

    SystemCoreClockUpdate();

    DELAY_Init(DELAY_MODE_SYSTICK);

    /* Enables peripheral clocks */
    RST_CLK_PCLKCmd((RST_CLK_PCLK_PORTC | RST_CLK_PCLK_EEPROM), ENABLE);

    /* Configure PORTC pins 0..1 for output to switch LEDs on/off */
    PORT_StructInit(&PORT_InitStructure);
    PORT_InitStructure.PORT_Pin       = LED_MASK;
    PORT_InitStructure.PORT_Direction = PORT_DIRECTION_OUT;
    PORT_InitStructure.PORT_Function  = PORT_FUNCTION_PORT;
    PORT_InitStructure.PORT_Mode      = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_Power     = PORT_POWER_SPEED_SLOW;

    PORT_Init(MDR_PORTC, &PORT_InitStructure);

    /* Erase main and information memory bank */
    EEPROM_EraseBlock(0x00008000, EEPROM_BANK_ALL);

    /* Indicate status of erasing main memory bank */
    ShowTestStatus(BlankCheckMainMemory(), 0x1);

    /* Indicate status of erasing information memory bank */
    ShowTestStatus(BlankCheckInfoMemory(), 0x2);

    /* Fill main memory bank */
    FillMainMemory();

    /* Verify main memory bank */
    ShowTestStatus(VerifyMainMemory(), 0x3);

    /* Fill information memory bank */
    FillInfoMemory();

    /* Verify information memory bank */
    ShowTestStatus(VerifyInfoMemory(), 0x1);

    /* Erase main memory bank only */
    EEPROM_EraseBlock(0x00008000, EEPROM_BANK_MAIN);

    /* Indicate status of erasing main memory bank */
    ShowTestStatus(BlankCheckMainMemory(), 0x2);

    /* Verify information memory bank */
    ShowTestStatus(VerifyInfoMemory(), 0x3);

    /* Fill main memory bank */
    FillMainMemory();

    /* Verify main memory bank */
    ShowTestStatus(VerifyMainMemory(), 0x1);

    PORT_Write(MDR_PORTC, LED_MASK);
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

/** @} */ /* End of group EEPROM_214 */

/** @} */ /* End of group __K1986VK214_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr *******************************/

/* END OF FILE main.c */


