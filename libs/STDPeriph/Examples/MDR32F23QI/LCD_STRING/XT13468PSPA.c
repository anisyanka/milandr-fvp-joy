/**
 ******************************************************************************
 * @file    XT13468PSPA.c
 * @author  Milandr Application Team
 * @version V1.0.0
 * @date    11/11/2022
 * @brief   This file contains all the LCD firmware functions.
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
#include "XT13468PSPA.h"
#include "MDR32F23QI_lcd.h"
#include "string.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK214_EVAL K1986VK214 Evaluation Board
 *  @{
 */

/** @addtogroup LDC_STRING_214 LCD Display example for K1986VK214 evaluation board
 *  @{
 */

/** @addtogroup LDC_BRD_214_DRIVER LCD Display driver for K1986VK214 evaluation board
 *  @{
 */

/** @defgroup XT13468PSPA_Private_Variables XT13468PSPA Private Variables
 * @{
 */

LCD_XT_DATA MDR_LCD_Data;

/** @} */ /* End of group XT13468PSPA_Private_Variables */

/** @defgroup XT13468PSPA_Exported_Functions XT13468PSPA Exported Functions
 * @{
 */

/**
 * @brief  XTSPA apply data.
 * @param  lcdData: pointer to @ref LCD_XT_DATA data.
 * @param  toAlterMem - @ref LCD_Mem - LCD memory.
 * @retval None.
 */
void XTSPA_DataApply(LCD_XT_DATA* lcdData, LCD_Mem toAlterMem)
{
    if (toAlterMem == LCD_Main) {
        MDR_LCD->ROW1 = lcdData->LCD_ROW1;
        MDR_LCD->ROW2 = lcdData->LCD_ROW2;
        MDR_LCD->ROW3 = lcdData->LCD_ROW3;
        MDR_LCD->ROW4 = lcdData->LCD_ROW4;
    } else {
        MDR_LCD->AROW1 = lcdData->LCD_ROW1;
        MDR_LCD->AROW2 = lcdData->LCD_ROW2;
        MDR_LCD->AROW3 = lcdData->LCD_ROW3;
        MDR_LCD->AROW4 = lcdData->LCD_ROW4;
    }
}

/**
 * @brief  XTSPA clear data.
 * @param  lcdData: pointer to @ref LCD_XT_DATA data.
 * @retval None.
 */
void XTSPA_DataClear(LCD_XT_DATA* lcdData)
{
    lcdData->LCD_ROW1 = 0;
    lcdData->LCD_ROW2 = 0;
    lcdData->LCD_ROW3 = 0;
    lcdData->LCD_ROW4 = 0;
}

/**
 * @brief  XTSPA clear data number.
 * @param  lcdData: pointer to @ref LCD_XT_DATA data.
 * @param  numItem - @ref XTSPA_NUM_Item - number.
 * @retval None.
 */
void XTSPA_DataClearNUM(LCD_XT_DATA* lcdData, XTSPA_NUM_Item numItem)
{
    lcdData->LCD_ROW1 &= ~(LCD_NUM_CLEAR[0] << ((uint32_t)numItem << 1));
    lcdData->LCD_ROW2 &= ~(LCD_NUM_CLEAR[1] << ((uint32_t)numItem << 1));
    lcdData->LCD_ROW3 &= ~(LCD_NUM_CLEAR[2] << ((uint32_t)numItem << 1));
    lcdData->LCD_ROW4 &= ~(LCD_NUM_CLEAR[3] << ((uint32_t)numItem << 1));
}

/**
 * @brief  XTSPA clear data string.
 * @param  lcdData: pointer to @ref LCD_XT_DATA data.
 * @retval None.
 */
void XTSPA_DataClearStr(LCD_XT_DATA* lcdData)
{
    lcdData->LCD_ROW1 &= LCD_NUMS_CLEAR_MASK_R123;
    lcdData->LCD_ROW2 &= LCD_NUMS_CLEAR_MASK_R123;
    lcdData->LCD_ROW3 &= LCD_NUMS_CLEAR_MASK_R123;
    lcdData->LCD_ROW4 &= LCD_NUMS_CLEAR_MASK_R4;
}

/**
 * @brief  Convert char to font index.
 * @param  symbol: data char.
 * @retval Font index.
 */
static int CharToFontIndex(uint8_t symbol)
{
    if ((symbol >= CHAR_0) && (symbol <= CHAR_9)) {
        return symbol - CHAR_0;
    } else if ((symbol >= CHAR_A) && (symbol <= CHAR_Z)) {
        return symbol - CHAR_A + 10;
    } else if ((symbol >= CHAR_a) && (symbol <= CHAR_z)) {
        return symbol - CHAR_a + 10;
    } else {
        return -1;
    }
}

/**
 * @brief  XTSPA add char.
 * @param  lcdData: pointer to @ref LCD_XT_DATA data.
 * @param  numItem - @ref XTSPA_NUM_Item - number.
 * @param  symbol: data char.
 * @retval None.
 */

void XTSPA_AddChar(LCD_XT_DATA* lcdData, XTSPA_NUM_Item numItem, uint8_t symbol)
{
    int32_t fntIndex = CharToFontIndex(symbol);
    if (fntIndex < 0) {
        return;
    }

    lcdData->LCD_ROW1 |= (LCD_XT_Font[4 * fntIndex] << NUM_OFFSET(numItem));
    lcdData->LCD_ROW2 |= (LCD_XT_Font[4 * fntIndex + 1] << NUM_OFFSET(numItem));
    lcdData->LCD_ROW3 |= (LCD_XT_Font[4 * fntIndex + 2] << NUM_OFFSET(numItem));
    lcdData->LCD_ROW4 |= (LCD_XT_Font[4 * fntIndex + 3] << NUM_OFFSET(numItem));
}

/**
 * @brief  XTSPA data write string.
 * @param  lcdData: pointer to @ref LCD_XT_DATA data.
 * @param  string: char pointer.
 * @retval None.
 */
void XTSPA_DataWriteStr(LCD_XT_DATA* lcdData, const char* string)
{
    uint32_t i;
    XTSPA_DataClearStr(lcdData);

    for (i = 0; i < LCD_XT_NUM_COUNT; ++i) {
        if (string[i] == 0) {
            break;
        }
        XTSPA_AddChar(lcdData, (XTSPA_NUM_Item)i, string[i]);
    }
}

/**
 * @brief  XTSPA data write string and apply to memory.
 * @param  lcdData: pointer to @ref LCD_XT_DATA data.
 * @param  string: char pointer.
 * @param  toAlterMem - @ref LCD_Mem - LCD memory.
 * @retval None.
 */
void XTSPA_DataPring(LCD_XT_DATA* lcdData, const char* string, LCD_Mem toAlterMem)
{
    XTSPA_DataWriteStr(lcdData, string);
    XTSPA_DataApply(lcdData, toAlterMem);
}

/**
 * @brief  XTSPA LCD print.
 * @param  string: char pointer.
 * @retval None.
 */
void XTSPA_LCD_Print(const char* string)
{
    XTSPA_DataWriteStr(&MDR_LCD_Data, string);
    XTSPA_DataApply(&MDR_LCD_Data, LCD_Main);
}

/**
 * @brief XTSPA LCD clear.
 * @retval None
 */
void XTSPA_LCD_Clear(void)
{
    XTSPA_DataClear(&MDR_LCD_Data);
    XTSPA_DataApply(&MDR_LCD_Data, LCD_Main);
}

/**
 * @brief  XTSPA LCD shift string.
 * @param  inpString: input string pointer.
 * @param  shift: string shift amount.
 * @param  screenWidth: LCD screen width.
 * @param  outString: output char pointer.
 * @retval None.
 */
void XTSPA_LCD_ShiftString(const char* inpString, uint8_t shift, uint8_t screenWidth, char* outString)
{
    uint8_t i, j;

    if (!strlen(inpString)) {
        return;
    }

    j = shift;
    for (i = 0; i < screenWidth; i++) {
        if (inpString[j] == 0) {
            j = 0;
        }

        outString[i] = inpString[j];
        ++j;
    }
    outString[screenWidth] = 0;
}

/**
 * @brief  XTSPA LCD scroll string.
 * @param  string: string pointer.
 * @param  shift: scroll amount.
 * @retval None.
 */
void XTSPA_LCD_ScrollString(const char* string, uint8_t shift)
{
    char scroll[LCD_SCREEN_WIDTH + 1];

    XTSPA_LCD_ShiftString(string, shift, LCD_SCREEN_WIDTH, scroll);
    XTSPA_LCD_Print(scroll);
}

/**
 * @brief  XTSPA LCD scroll string to left.
 * @param  inpString: input string pointer.
 * @param  strLength: string length.
 * @retval None.
 */
void XTSPA_LCD_ScrollStringLeft(const char* inpString, uint8_t strLength)
{
    static uint8_t shift = 0;

    XTSPA_LCD_ScrollString(inpString, shift);

    ++shift;
    if (shift >= strLength) {
        shift = 0;
    }
}

/** @} */ /* End of group XT13468PSPA_Exported_Functions */

/** @} */ /* End of group LDC_BRD_214_DRIVER */

/** @} */ /* End of group LDC_STRING_214 */

/** @} */ /* End of group __K1986VK214_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE XT1346PSPA.c */


