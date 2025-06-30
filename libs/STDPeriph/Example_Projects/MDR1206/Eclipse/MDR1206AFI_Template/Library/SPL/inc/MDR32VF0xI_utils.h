/**
 ******************************************************************************
 * @file    MDR32VF0xI_utils.h
 * @author  Milandr Application Team
 * @version V0.1.0
 * @date    16/04/2024
 * @brief   This file contains all prototypes of the firmware library
 *          utility functions.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MDR32VF0xI_UTILS_H
#define MDR32VF0xI_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32VF0xI_config.h"

/** @addtogroup MDR32VF0xI_StdPeriph_Driver MDR32VF0xI Standard Peripheral Driver
 * @{
 */

/** @addtogroup UTILS UTILS
 * @{
 */

/** @defgroup UTILS_Exported_Types UTILS Exported Types
 * @{
 */

/**
 * @brief Delay mode
 */
typedef enum {
    DELAY_MODE_PROGRAM       = ((uint32_t)0x0), /*!< Select the delay module mode based on the program cycle. */
    DELAY_MODE_MACHINE_TIMER = ((uint32_t)0x1)  /*!< Select the delay module mode based on the RISC-V machine timer. */
} DELAY_Mode_TypeDef;

#define IS_DELAY_MODE(MODE) (((MODE) == DELAY_MODE_PROGRAM) || \
                             ((MODE) == DELAY_MODE_MACHINE_TIMER))

/** @} */ /* End of the group UTILS_Exported_Types */

/** @defgroup UTILS_Exported_Defines UTILS Exported Defines
 * @{
 */

#define DELAY_PROGRAM_LOOP_CYCLES                  ((uint32_t)2)

#define DELAY_PROGRAM_GET_CONST(CLK)               (((((CLK) + DELAY_PROGRAM_LOOP_CYCLES - 1) / DELAY_PROGRAM_LOOP_CYCLES) + (uint32_t)999) / (uint32_t)1000)

#define DELAY_PROGRAM_GET_LOOPS_US(N, CONST)       (((N) * (CONST)) / (uint32_t)1000)
#define DELAY_PROGRAM_GET_LOOPS_MS(N, CONST)       ((N) * (CONST))

#define DELAY_MACHINE_TIMER_GET_CONST(CLK)         (((CLK) + (uint32_t)999) / (uint32_t)1000)

#define DELAY_MACHINE_TIMER_GET_TICKS_US(N, CONST) (((N) * (CONST)) / (uint32_t)1000)
#define DELAY_MACHINE_TIMER_GET_TICKS_MS(N, CONST) (((N) * (CONST)))

/** @} */ /* End of the group UTILS_Exported_Defines */

/** @addtogroup UTILS_Exported_Functions UTILS Exported Functions
 * @{
 */

ErrorStatus DELAY_Init(DELAY_Mode_TypeDef Mode);
void        DELAY_WaitUs(uint32_t Us);
void        DELAY_WaitMs(uint32_t Ms);

__RAMFUNC void DELAY_PROGRAM_WaitLoopsAsm(uint32_t Loops);

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
uint8_t GetChipID(void);
#endif

/** @} */ /* End of the group UTILS_Exported_Functions */

/** @} */ /* End of the group UTILS */

/** @} */ /* End of the group MDR32VF0xI_StdPeriph_Driver */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* MDR32VF0xI_UTILS_H */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE MDR32VF0xI_utils.h */

