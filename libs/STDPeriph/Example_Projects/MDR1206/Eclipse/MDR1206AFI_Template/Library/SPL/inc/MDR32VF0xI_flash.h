/**
 *******************************************************************************
 * @file    MDR32VF0xI_flash.h
 * @author  Milandr Application Team
 * @version V0.1.0
 * @date    16/04/2024
 * @brief   This file contains all the functions prototypes for the FLASH
 *          firmware library.
 *******************************************************************************
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
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MDR32VF0xI_FLASH_H
#define MDR32VF0xI_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32VF0xI_config.h"

/** @addtogroup MDR32VF0xI_StdPeriph_Driver MDR32VF0xI Standard Peripheral Driver
 * @{
 */

/** @addtogroup FLASH FLASH
 * @{
 */

/** @defgroup FLASH_Exported_Types FLASH Exported Types
 * @{
 */
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI) || defined(USE_MDR1206)
/**
 * @brief FLASH latency cycles.
 */
typedef enum {
    FLASH_LATENCY_CYCLE_0 = FLASH_CMD_DELAY_CYCLE_0, /*!< Select 0 FLASH latency cycle (MCU clock up to: MDR32F02 - 30 MHz, MDR1206FI - 25 MHz). */
    FLASH_LATENCY_CYCLE_1 = FLASH_CMD_DELAY_CYCLE_1  /*!< Select 1 FLASH latency cycle (MCU clock up to: MDR32F02 - 60 MHz, MDR1206FI - 48 MHz). */
} FLASH_Latency_TypeDef;

#define IS_FLASH_LATENCY(LATENCY) (((LATENCY) == FLASH_LATENCY_CYCLE_0) || \
                                   ((LATENCY) == FLASH_LATENCY_CYCLE_1))
#endif

/**
 * @brief FLASH memory banks.
 */
typedef enum {
    FLASH_BANK_MAIN = ((uint32_t)0x00), /*!< Select the FLASH main bank. */
    FLASH_BANK_INFO = ((uint32_t)0x01)  /*!< Select the FLASH information bank. */
} FLASH_MemoryBank_TypeDef;

#define IS_FLASH_MEMORY_BANK(BANK) (((BANK) == FLASH_BANK_MAIN) || \
                                    ((BANK) == FLASH_BANK_INFO))

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
/**
 * @brief FLASH memory protection regions.
 */
typedef enum {
    FLASH_PROTECTION_REGION_0 = ((uint32_t)0x01), /*!< FLASH memory protection for region 0 (0x10000000-0x1000FFFF). */
    FLASH_PROTECTION_REGION_1 = ((uint32_t)0x02), /*!< FLASH memory protection for region 1 (0x10010000-0x1001FFFF). */
    FLASH_PROTECTION_REGION_2 = ((uint32_t)0x04), /*!< FLASH memory protection for region 2 (0x10020000-0x1002FFFF). */
    FLASH_PROTECTION_REGION_3 = ((uint32_t)0x08), /*!< FLASH memory protection for region 3 (0x10030000-0x1003FFFF). */
    FLASH_PROTECTION_REGION_4 = ((uint32_t)0x10), /*!< FLASH memory protection for region 4 (0x10040000-0x1004FFFF). */
    FLASH_PROTECTION_REGION_5 = ((uint32_t)0x20), /*!< FLASH memory protection for region 5 (0x10050000-0x1005FFFF). */
    FLASH_PROTECTION_REGION_6 = ((uint32_t)0x40), /*!< FLASH memory protection for region 6 (0x10060000-0x1006FFFF). */
    FLASH_PROTECTION_REGION_7 = ((uint32_t)0x80)  /*!< FLASH memory protection for region 7 (0x10070000-0x1007FFFF). */
} FLASH_ProtectionRegions_TypeDef;

#define IS_FLASH_PROTECTION_REGIONS(PRGS) (((PRGS) & ~(uint32_t)(FLASH_PROTECTION_REGION_0 | \
                                                                 FLASH_PROTECTION_REGION_1 | \
                                                                 FLASH_PROTECTION_REGION_2 | \
                                                                 FLASH_PROTECTION_REGION_3 | \
                                                                 FLASH_PROTECTION_REGION_4 | \
                                                                 FLASH_PROTECTION_REGION_5 | \
                                                                 FLASH_PROTECTION_REGION_6 | \
                                                                 FLASH_PROTECTION_REGION_7)) == 0)

/**
 * @brief FLASH factory data structure definition.
 */
typedef struct
{
    uint32_t UniqueID[5];
    uint8_t  TrimHSI;
    uint8_t  TrimBG;
    uint8_t  TrimLDO;
    uint8_t  TrimTS;
} FLASH_FactoryData_TypeDef;
#endif

/** @} */ /* End of the group FLASH_Exported_Types */

/** @defgroup FLASH_Exported_Defines FLASH Exported Defines
 * @{
 */

/**
 * @brief FLASH parameters with a safe margin of ~7% (HSI spread).
 */
#if defined(USE_MDR32F02) || defined(USE_MDR1206FI) || defined(USE_MDR1206)
#define FLASH_TNVS_US   (6)
#define FLASH_TNVH_US   (6)
#define FLASH_TNVH1_US  (107)
#define FLASH_TPGS_US   (11)
#define FLASH_TRCV_US   (11)
#define FLASH_THV_US    (16000)

#define FLASH_TPROG_US  (30)
#define FLASH_TERASE_US (30000)
#define FLASH_TME_US    (30000)
#endif

#if defined(USE_MDR1206AFI) || defined(USE_MDR1206)
#define FLASH_TNVS_PROGRAM_US        (22)
#define FLASH_TRCV_PROGRAM_US        (54)
#define FLASH_TRW_PROGRAM_US         (1)
#define FLASH_TADH_PROGRAM_US        (1)
#define FLASH_TADS_PROGRAM_US        (1)
#define FLASH_TPGH_PROGRAM_US        (1)
#define FLASH_TPGS_PROGRAM_US        (50)
#define FLASH_TPROG_PROGRAM_US       (5)

#define FLASH_TNVS_SECTOR_ERASE_US   (22)
#define FLASH_TRCV_SECTOR_ERASE_US   (54)
#define FLASH_TRW_SECTOR_ERASE_US    (1)
#define FLASH_TERASE_SECTOR_ERASE_US (2500)

#define FLASH_TNVS_CHIP_ERASE_US     (86)
#define FLASH_TRCV_CHIP_ERASE_US     (214)
#define FLASH_TRW_CHIP_ERASE_US      (11)
#define FLASH_TERASE_CHIP_ERASE_US   (35000)
#endif

/**
 * @brief FLASH address alignment.
 */
#define FLASH_ADDR_TWO_BYTE_ALIGN_MASK        ((uint32_t)0x1)
#define FLASH_ADDR_FOUR_BYTE_ALIGN_MASK       ((uint32_t)0x3)

#define IS_FLASH_ADDR_TWO_BYTE_ALIGNED(ADDR)  (((ADDR) & FLASH_ADDR_TWO_BYTE_ALIGN_MASK) == 0)
#define IS_FLASH_ADDR_FOUR_BYTE_ALIGNED(ADDR) (((ADDR) & FLASH_ADDR_FOUR_BYTE_ALIGN_MASK) == 0)

/** @} */ /* End of the group FLASH_Exported_Defines */

/** @addtogroup FLASH_Exported_Functions FLASH Exported Functions
 * @{
 */

/** @addtogroup FLASH_Functions_Allowed_to_Be_Located_in_FLASH Functions Allowed to Be Located in the FLASH
 * @{
 */

/** @addtogroup FLASH_Control_Functions FLASH Control Functions
 * @{
 */

void       FLASH_SetRegistersLock(FlagStatus LockState);
FlagStatus FLASH_GetRegistersLock(void);

/** @} */ /* End of the group FLASH_Control_Functions */

/** @addtogroup FLASH_Special_Fields_Control_Functions FLASH Special Fields Control Functions
 * @{
 */

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
void     FLASH_ProgramProtection(uint32_t ProtectionRegions);
uint32_t FLASH_ReadProtection(void);

void        FLASH_ProgramFactoryData(const FLASH_FactoryData_TypeDef* FactoryData);
ErrorStatus FLASH_ReadFactoryData(FLASH_FactoryData_TypeDef* FactoryData);
#endif

/** @} */ /* End of the group FLASH_Special_Fields_Control_Functions */

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
/** @addtogroup FLASH_JTAG_Locking_Functions FLASH JTAG Locking Functions
 * @{
 */

void       FLASH_JTAG_SetRegistersLock(FlagStatus LockState);
FlagStatus FLASH_JTAG_GetRegistersLock(void);

void FLASH_JTAG_SetLockForever(void);

/** @} */ /* End of the group FLASH_JTAG_Locking_Functions */
#endif

/** @} */ /* End of the group FLASH_Functions_Allowed_to_Be_Located_in_FLASH */

/** @addtogroup FLASH_Functions_Not_Allowed_to_Be_Located_in_FLASH Functions Not Allowed to Be Located in the FLASH
 * @{
 */

#if defined(USE_MDR32F02) || defined(USE_MDR1206FI) || defined(USE_MDR1206)
/** @addtogroup FLASH_Control_Functions FLASH Control Functions
 * @{
 */

__RAMFUNC void                  FLASH_SetLatency(FLASH_Latency_TypeDef Latency);
__RAMFUNC FLASH_Latency_TypeDef FLASH_GetLatency(void);

/** @} */ /* End of the group FLASH_Control_Functions */
#endif

/** @addtogroup FLASH_Register_Access_Functions FLASH Register Access Functions
 * @{
 */

__RAMFUNC uint32_t FLASH_ReadWord(uint32_t Address, FLASH_MemoryBank_TypeDef Bank);
__RAMFUNC uint16_t FLASH_ReadHalfWord(uint32_t Address, FLASH_MemoryBank_TypeDef Bank);
__RAMFUNC uint8_t  FLASH_ReadByte(uint32_t Address, FLASH_MemoryBank_TypeDef Bank);
__RAMFUNC void     FLASH_ReadWordArrayBurst(const uint32_t* PtrAddressArray, uint32_t* PtrDataArray, uint32_t ArraySize, FLASH_MemoryBank_TypeDef Bank);

__RAMFUNC void     FLASH_ErasePage(uint32_t Address, FLASH_MemoryBank_TypeDef Bank);
__RAMFUNC void     FLASH_EraseAllPages(FLASH_MemoryBank_TypeDef Bank);
__RAMFUNC void     FLASH_EraseAllBanks(void);
__RAMFUNC uint32_t FLASH_GetPageAddress(uint32_t PageNumber, FLASH_MemoryBank_TypeDef Bank);

__RAMFUNC void FLASH_ProgramWord(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint32_t Data);
__RAMFUNC void FLASH_ProgramHalfWord(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint16_t Data);
__RAMFUNC void FLASH_ProgramByte(uint32_t Address, FLASH_MemoryBank_TypeDef Bank, uint8_t Data);
__RAMFUNC void FLASH_ProgramWordArrayBurst(const uint32_t* PtrAddressArray, const uint32_t* PtrDataArray, uint32_t ArraySize, FLASH_MemoryBank_TypeDef Bank);

/** @} */ /* End of the group FLASH_Register_Access_Functions */

/** @} */ /* End of the group FLASH_Functions_not_Allowed_to_Be_Located_in_FLASH */

/** @} */ /* End of the group FLASH_Exported_Functions */

/** @} */ /* End of the group FLASH */

/** @} */ /* End of the group MDR32VF0xI_StdPeriph_Driver */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* MDR32VF0xI_FLASH_H */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE MDR32VF0xI_flash.h */

