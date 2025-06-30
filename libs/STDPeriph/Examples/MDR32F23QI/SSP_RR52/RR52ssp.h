/**
 ******************************************************************************
 * @file    RR52ssp.h
 * @author  Milandr Application Team
 * @version V1.0.0
 * @date    25/05/2023
 * @brief   This file contains all prototypes to work with 1636RR52 flash memory.
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
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RR52SSP_H
#define __RR52SSP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "MDR32F2xQI_config.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK214_EVAL K1986VK214 Evaluation Board
 *  @{
 */

/** @addtogroup SSP_RR52_214 SSP RR52 example for K1986VK214 evaluation board
 *  @{
 */

/** @addtogroup SSP_RR52_214_DRIVER SSP RR52 driver for K1986VK214 evaluation board
 *  @{
 */

/* Bit field masks: */
#define SPRL                                     ((uint8_t)0x80)
#define RSTE                                     ((uint8_t)0x40)

/** @defgroup SSP_RR52_214_Exported_Functions SSP RR52 Exported Function
 * @{
 */
void     RR52SSP_PortConfig(void);
void     RR52SSP_SSPConfig(void);
void     RR52SSP_Unlock(uint32_t Address);
void     RR52SSP_WriteEnable(void);
uint32_t RR52SSP_ReadUnlock(uint32_t Address);
uint8_t  RR52SSP_ReadStat(void);
void     RR52SSP_WriteStat(BitStatus SPRL_Value, BitStatus RSTE_Value);
void     RR52SSP_MassErase(void);
void     RR52SSP_SectorErase(uint32_t Address);
uint16_t RR52SSP_ReadID(void);
void     RR52SSP_WriteByte(uint32_t Address, uint8_t Data);
uint8_t  RR52SSP_ReadByte(uint32_t Address);
uint8_t  RR52SSP_CommandRXTX_8bit(uint8_t Data);

/** @} */ /* End of group SSP_RR52_214_Exported_Functions */

/** @} */ /* End of group SSP_RR52_214_DRIVER */

/** @} */ /* End of group SSP_RR52_214 */

/** @} */ /* End of group __K1986VK214_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* __RR52SSP_H */

/******************* (C) COPYRIGHT 2024 Milandr *******************************/

/* END OF FILE RR52ssp.h */


