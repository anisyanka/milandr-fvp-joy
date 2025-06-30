/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __XT13468PSPA_H
#define __XT13468PSPA_H

/* Includes ------------------------------------------------------------------*/
#include "MDR32F23QI.h"
#include "font_XT13468PSPA.h"
#include <stdint.h>

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK214_EVAL K1986VK214 Evaluation Board
 *  @{
 */

/** @addtogroup LCD_STRING_214 XT13468PSPA Display example for K1986VK214 evaluation board
 *  @{
 */

/** @addtogroup XT13468PSPA_BRD_214_DRIVER XT13468PSPA Display driver for K1986VK214 evaluation board
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup XT13468PSPA_Private_Defines XT13468PSPA Private Defines
 * @{
 */

#define LCD_XT_NUM_COUNT         8

#define LCD_NUMS_CLEAR_MASK_R123 0x00FC0003UL
#define LCD_NUMS_CLEAR_MASK_R4   0x00FEAAABUL

#define LCD_SCREEN_WIDTH         8

#define NUM_OFFSET(n)            ((uint32_t)n << 1)

/** @} */ /* End of group XT13468PSPA_Private_Defines */

/** @defgroup XT13468PSPA_Exported_Types XT13468PSPA Exported Types
 * @{
 */

typedef enum {
    LCD_Main  = 0,
    LCD_Alter = 1
} LCD_Mem;

typedef enum {
    LCD_XT_NUM1,
    LCD_XT_NUM2,
    LCD_XT_NUM3,
    LCD_XT_NUM4,
    LCD_XT_NUM5,
    LCD_XT_NUM6,
    LCD_XT_NUM7,
    LCD_XT_NUM8
} XTSPA_NUM_Item;

typedef struct {
    uint32_t s0_Reverse   : 1;
    uint32_t s1_Tar_1     : 1;
    uint32_t s2_Num1_F    : 1;
    uint32_t s3_Num1_A    : 1;
    uint32_t s4_Num2_F    : 1;
    uint32_t s5_Num2_A    : 1;
    uint32_t s6_Num3_F    : 1;
    uint32_t s7_Num3_A    : 1;
    uint32_t s8_Num4_F    : 1;
    uint32_t s9_Num4_A    : 1;
    uint32_t s10_Num5_F   : 1;
    uint32_t s11_Num5_A   : 1;
    uint32_t s12_Num6_F   : 1;
    uint32_t s13_Num6_A   : 1;
    uint32_t s14_Num7_F   : 1;
    uint32_t s15_Num7_A   : 1;
    uint32_t s16_Num8_F   : 1;
    uint32_t s17_Num8_A   : 1;
    uint32_t s18_var_k    : 1;
    uint32_t s19_A_plus   : 1;
    uint32_t s20_W_k      : 1;
    uint32_t s21_VA_A     : 1;
    uint32_t s22_Circ_1   : 1;
    uint32_t s23_Num7_dot : 1;
    uint32_t unused       : 8;
} LCD_XT_ROW1_Bits;

typedef struct {
    uint32_t s0_Tarif    : 1;
    uint32_t s1_Tar_2    : 1;
    uint32_t s2_Num1_G   : 1;
    uint32_t s3_Num1_B   : 1;
    uint32_t s4_Num2_G   : 1;
    uint32_t s5_Num2_B   : 1;
    uint32_t s6_Num3_G   : 1;
    uint32_t s7_Num3_B   : 1;
    uint32_t s8_Num4_G   : 1;
    uint32_t s9_Num4_B   : 1;
    uint32_t s10_Num5_G  : 1;
    uint32_t s11_Num5_B  : 1;
    uint32_t s12_Num6_G  : 1;
    uint32_t s13_Num6_B  : 1;
    uint32_t s14_Num7_G  : 1;
    uint32_t s15_Num7_B  : 1;
    uint32_t s16_Num8_G  : 1;
    uint32_t s17_Num8_B  : 1;
    uint32_t s18_var_h   : 1;
    uint32_t s19_A_minus : 1;
    uint32_t s20_VA_V    : 1;
    uint32_t s21_var     : 1;
    uint32_t s22_Circ_2  : 1;
    uint32_t s23_Lock    : 1;
    uint32_t unused      : 8;
} LCD_XT_ROW2_Bits;

typedef struct {
    uint32_t s0_In       : 1;
    uint32_t s1_Tar_3    : 1;
    uint32_t s2_Num1_E   : 1;
    uint32_t s3_Num1_C   : 1;
    uint32_t s4_Num2_E   : 1;
    uint32_t s5_Num2_C   : 1;
    uint32_t s6_Num3_E   : 1;
    uint32_t s7_Num3_C   : 1;
    uint32_t s8_Num4_E   : 1;
    uint32_t s9_Num4_C   : 1;
    uint32_t s10_Num5_E  : 1;
    uint32_t s11_Num5_C  : 1;
    uint32_t s12_Num6_E  : 1;
    uint32_t s13_Num6_C  : 1;
    uint32_t s14_Num7_E  : 1;
    uint32_t s15_Num7_C  : 1;
    uint32_t s16_Num8_E  : 1;
    uint32_t s17_Num8_C  : 1;
    uint32_t s18_Data    : 1;
    uint32_t s19_R_Plus  : 1;
    uint32_t s20_W       : 1;
    uint32_t s21_W_h     : 1;
    uint32_t s22_Circ_3  : 1;
    uint32_t s23_Battery : 1;
    uint32_t unused      : 8;
} LCD_XT_ROW3_Bits;

typedef struct {
    uint32_t s0_Lambda     : 1;
    uint32_t s1_Tar_4      : 1;
    uint32_t s2_Num1_D     : 1;
    uint32_t s3_Plus_H     : 1;
    uint32_t s4_Num2_D     : 1;
    uint32_t s5_Num2_dot   : 1;
    uint32_t s6_Num3_D     : 1;
    uint32_t s7_Num3_dotc  : 1;
    uint32_t s8_Num4_D     : 1;
    uint32_t s9_Num3_dot   : 1;
    uint32_t s10_Num5_D    : 1;
    uint32_t s11_Num4_dot  : 1;
    uint32_t s12_Num6_D    : 1;
    uint32_t s13_Num5_dot  : 1;
    uint32_t s14_Num7_D    : 1;
    uint32_t s15_Num6_dot  : 1;
    uint32_t s16_Num8_D    : 1;
    uint32_t s17_Num5_dotc : 1;
    uint32_t s18_Time      : 1;
    uint32_t s19_R_Minus   : 1;
    uint32_t s20_VA_k      : 1;
    uint32_t s21_Hz        : 1;
    uint32_t s22_Circ_4    : 1;
    uint32_t s23_Plus_V    : 1;
    uint32_t unused        : 8;
} LCD_XT_ROW4_Bits;

typedef struct {
    union {
        volatile uint32_t LCD_ROW1;
        LCD_XT_ROW1_Bits  LCD_ROW1_b;
    };
    union {
        volatile uint32_t LCD_ROW2;
        LCD_XT_ROW2_Bits  LCD_ROW2_b;
    };
    union {
        volatile uint32_t LCD_ROW3;
        LCD_XT_ROW3_Bits  LCD_ROW3_b;
    };
    union {
        volatile uint32_t LCD_ROW4;
        LCD_XT_ROW4_Bits  LCD_ROW4_b;
    };
} LCD_XT_DATA;

/** @} */ /* End of group XT13468PSPA_Exported_Types */

/** @defgroup XT13468PSPA_Exported_Functions XT13468PSPA Exported Functions
 * @{
 */
void       XTSPA_DataApply(LCD_XT_DATA* lcdData, LCD_Mem toAlterMem);
void       XTSPA_DataClear(LCD_XT_DATA* lcdData);
void       XTSPA_DataClearNUM(LCD_XT_DATA* lcdData, XTSPA_NUM_Item numItem);
void       XTSPA_DataClearStr(LCD_XT_DATA* lcdData);
static int CharToFontIndex(uint8_t symbol);
void       XTSPA_AddChar(LCD_XT_DATA* lcdData, XTSPA_NUM_Item numItem, uint8_t symbol);
void       XTSPA_DataWriteStr(LCD_XT_DATA* lcdData, const char* string);
void       XTSPA_DataPring(LCD_XT_DATA* lcdData, const char* string, LCD_Mem toAlterMem);
void       XTSPA_LCD_Print(const char* string);
void       XTSPA_LCD_Clear(void);
void       XTSPA_LCD_ShiftString(const char* inpString, uint8_t shift, uint8_t screenWidth, char* outString);
void       XTSPA_LCD_ScrollString(const char* string, uint8_t shift);
void       XTSPA_LCD_ScrollStringLeft(const char* inpString, uint8_t strLength);

/** @} */ /* End of group XT13468PSPA_Exported_Functions */

#ifdef __cplusplus
} // extern "C" block end
#endif

/** @} */ /* End of group XT13468PSPA_BRD_214_DRIVER */

/** @} */ /* End of group LDC_STRING_214 */

/** @} */ /* End of group __K1986VK214_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

#endif    //  XT13468PSPA


