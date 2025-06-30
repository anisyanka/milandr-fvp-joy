/**
 *******************************************************************************
 * @file    MDR32VF0xI_config.h
 * @author  Milandr Application Team
 * @version V0.1.0
 * @date    11/04/2024
 * @brief   Standard Peripherals Library configuration file.
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

/**
 * @mainpage MDR32VF0xI Standard Peripherals Library.
 * MDR32VF0xI Standard Peripherals Library is a package consisting of
 * all standard peripheral device drivers for MDR32F02 and MDR1206 microcontrollers.
 * This library is a firmware package which contains a collection of routines,
 * data structures and macros covering the features of Milandr MDR32VF0xI
 * peripherals. It includes a description of the device drivers plus a set of
 * examples for each peripheral. The firmware library allows any device to be
 * used in the user application without the need for in-depth study of each
 * peripherals specifications. Using the Standard Peripherals Library has two
 * advantages: it saves significant time that would otherwise be spent in
 * coding, while simultaneously reducing application development and
 * integration costs.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MDR32VF0xI_CONFIG_H
#define MDR32VF0xI_CONFIG_H

#ifndef __ASSEMBLER__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/** MDR32VF0xI SPL version numbers - major, minor, patch. */
#define _MDR32VF0xI_SPL_VERSION_MAJOR ((uint32_t)0) /*!< [23:16] - major version. */
#define _MDR32VF0xI_SPL_VERSION_MINOR ((uint32_t)1) /*!< [15:8]  - minor version. */
#define _MDR32VF0xI_SPL_VERSION_PATCH ((uint32_t)0) /*!< [7:0]   - patch version. */

/** MDR32VF0xI SPL version number (combined value). */
#define MDR32VF0xI_SPL_VERSION ((_MDR32VF0xI_SPL_VERSION_MAJOR << 16) | \
                                (_MDR32VF0xI_SPL_VERSION_MINOR << 8) |  \
                                (_MDR32VF0xI_SPL_VERSION_PATCH))

#endif /* __ASSEMBLER__ */

/** MCU selection.
    USE_MDR32F02_REV_1X: MDR32F02 rev.1.0 and rev.1.1,
    USE_MDR32F02_REV_2:  MDR32F02 rev.2.0,
    USE_MDR1206FI:       MDR1206FI,
    USE_MDR1206AFI:      MDR1206AFI,
    USE_MDR1206:         MDR1206FI or MDR1206AFI: proper functions which differ in functionality will be selected by ID. */
//#define USE_MDR32F02_REV_1X
//#define USE_MDR32F02_REV_2
#define USE_MDR1206FI
//#define USE_MDR1206AFI
//#define USE_MDR1206

#if defined(USE_MDR32F02_REV_1X) || defined(USE_MDR32F02_REV_2)
#undef USE_MDR32F02
#define USE_MDR32F02
#endif

#if !defined(USE_MDR32F02_REV_1X) && !defined(USE_MDR32F02_REV_2) && !defined(USE_MDR1206FI) && !defined(USE_MDR1206AFI) && !defined(USE_MDR1206)
#error "Microcontroller is not selected in MDR32VF0xI_config.h file."
#endif

#ifndef __ASSEMBLER__

/** JTAG pins protection.
    Prevents from setting PORT register bits for pins combined with JTAG function when doing PORT RXTX register read-modify-write.
    Uncomment the definition below to define protection of PORT pins combined with JTAG. Uncommented by default.
    Comment the definition below if there is no need to protect PORT pins combined with JTAG. */
#define USE_JTAG

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
/** Protection of factory data located in FLASH.
    Prevents erasing and programming of factory data located in the FLASH memory bank. The entire page is protected from erasure.
    Uncomment the definition below to define protection of factory data. Uncommented by default.
    Comment the definition below if there is no need to protect factory data. */
#define USE_FLASH_FACTORY_DATA_PROTECTION
#endif

/* Target system parameters. */

/* Clock generators frequencies. */
/** HSI clock value [Hz].
    Default: 8000000 (8MHz). */
#define HSI_FREQUENCY_Hz ((uint32_t)8000000)
/** HSE clock value [Hz].
    Default: 8000000 (8MHz). */
#define HSE_FREQUENCY_Hz ((uint32_t)8000000)
/** LSI clock value [Hz].
    Default: 31250 (31.25kHz). */
#define LSI_FREQUENCY_Hz ((uint32_t)HSI_FREQUENCY_Hz / 256)
/** LSE clock value [Hz].
    Default: 32768 (32.768kHz). */
#define LSE_FREQUENCY_Hz ((uint32_t)32768)

/* Clock generators mode. */
/** Specify if HSE external generator mode is used:
    0: HSE is in oscillator mode (external generator mode is not used);
    1: HSE is in external generator mode.
    Default: 0 (oscillator mode). */
#define HSE_EXT_GEN_MODE 0
/** Specify if LSE external generator mode is used:
    0: LSE is in oscillator mode (external generator mode is not used);
    1: LSE is in external generator mode.
    Default: 0 (oscillator mode). */
#define LSE_EXT_GEN_MODE 0

#if (HSE_EXT_GEN_MODE != 0) && (HSE_EXT_GEN_MODE != 1)
#error "HSE_EXT_GEN_MODE should be 0 (oscillator mode) or 1 (external generator mode)."
#endif

#if (LSE_EXT_GEN_MODE != 0) && (LSE_EXT_GEN_MODE != 1)
#error "LSE_EXT_GEN_MODE should be 0 (oscillator mode) or 1 (external generator mode)."
#endif

/* Clock frequencies startup timeouts settings. */
/** HSI timeout startup value.
    Default: 0x0600. */
#define HSIRDY_TIMEOUT_TICK ((uint16_t)0x0600)
/** HSE timeout startup value.
    Default: 0x0600. */
#define HSERDY_TIMEOUT_TICK ((uint16_t)0x0600)
/** LSI timeout startup value.
    Default: 0x0600. */
#define LSIRDY_TIMEOUT_TICK ((uint16_t)0x0600)
/** LSE timeout startup value.
    Default: 0x0600. */
#define LSERDY_TIMEOUT_TICK ((uint16_t)0x0600)
/** PLLCPU timeout startup value.
    Default: 0x0600. */
#define PLLCPURDY_TIMEOUT_TICK ((uint16_t)0x0600)

/* DMA configuration parameters. */
/** Alternate control data structure usage:
    0: DMA_ALTERNATE_DATA_DISABLED;
    1: DMA_ALTERNATE_DATA_ENABLED.
    Default: 1 (DMA_ALTERNATE_DATA_ENABLED). */
#define DMA_ALTERNATE_DATA 1

#if (DMA_ALTERNATE_DATA != 0) && (DMA_ALTERNATE_DATA != 1)
#error "DMA_ALTERNATE_DATA should be 0 (DMA_ALTERNATE_DATA_DISABLED) or 1 (DMA_ALTERNATE_DATA_ENABLED)."
#endif

#if (DMA_ALTERNATE_DATA == 0)
/** Number of DMA channels to use.
    Specifies size of structure table for channels in range [0; DMA_CHANNELS_NUMBER-1] where 0 is channel 0.
    Could be changed only if DMA_ALTERNATE_DATA = 0 to reduce RAM usage.
    If DMA_ALTERNATE_DATA = 1, 32 channels are always used.
    This parameter is in range [1; 32].
    Default: 32 */
#define DMA_CHANNELS_NUMBER 32

#if (DMA_CHANNELS_NUMBER < 1) || (DMA_CHANNELS_NUMBER > 32)
#error "DMA_CHANNELS_NUMBER should be in range [1; 32]."
#endif

#endif

#if defined(USE_MDR32F02_REV_1X) || defined(USE_MDR32F02_REV_2)
/* PLIC configuration parameters. */
/** Initialize PLIC for regarding the removal of a pending request from the DMA in SystemInit() (called before entering main()):
    0: PLIC is not initialized;
    1: PLIC is initialized for regarding the removal of a pending request from the DMA.
    Default: 1 (PLIC is initialized). */
#define PLIC_SYSTEM_INIT 1

#if (PLIC_SYSTEM_INIT != 0) && (PLIC_SYSTEM_INIT != 1)
#error "PLIC_SYSTEM_INIT should be 0 (PLIC is not initialized) or 1 (PLIC is initialized)."
#endif

#endif

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
/* CLIC configuration parameters. */
/** Initialize CLIC for all interrupts in SystemInit() (called before entering main()):
    0: CLIC is not initialized;
    1: CLIC is initialized according to the parameters below.
    Default: 1 (CLIC is initialized). */
#define CLIC_SYSTEM_INIT 1

#if (CLIC_SYSTEM_INIT != 0) && (CLIC_SYSTEM_INIT != 1)
#error "CLIC_SYSTEM_INIT should be 0 (CLIC is not initialized) or 1 (CLIC is initialized)."
#endif

#if (CLIC_SYSTEM_INIT == 1)
/** Enable or disable vector mode for interrupts:
    1: Enable vector mode for interrupts;
    0: Disable vector mode for interrupts.
    Default: 1 (enable vector mode for interrupts). */
#define CLIC_SYSTEM_INIT_VECTOR_MODE_IRQ 1
/** Specify the maximum number of interrupt levels:
    CLIC_MAX_LEVELS_1:  1 interrupt level and 16 interrupt priorities;
    CLIC_MAX_LEVELS_2:  2 interrupt levels and 8 interrupt priorities;
    CLIC_MAX_LEVELS_4:  4 interrupt levels and 4 interrupt priorities;
    CLIC_MAX_LEVELS_8:  8 interrupt levels and 2 interrupt priorities;
    CLIC_MAX_LEVELS_16: 16 interrupt levels and 1 interrupt priorities.
    Default: CLIC_MAX_LEVELS_4. */
#define CLIC_SYSTEM_INIT_MAX_LEVELS CLIC_MAX_LEVELS_4
/** Specify the trigger type and polarity for interrupts:
    CLIC_TRIGGER_IRQ_LEVEL_HIGH:   High trigger level;
    CLIC_TRIGGER_IRQ_EDGE_RISING:  Rising edge triggering;
    CLIC_TRIGGER_IRQ_LEVEL_LOW:    Low trigger level;
    CLIC_TRIGGER_IRQ_EDGE_FALLING: Falling edge triggering.
    Default: CLIC_TRIGGER_IRQ_LEVEL_HIGH.
    All peripheral controllers, except DMA, generate high-level interrupts, so it is recommended to use CLIC_TRIGGER_IRQ_LEVEL_HIGH
    or CLIC_TRIGGER_IRQ_EDGE_RISING. For DMA this parameter is forced to CLIC_TRIGGER_IRQ_EDGE_RISING. */
#define CLIC_SYSTEM_INIT_TRIGGER_IRQ CLIC_TRIGGER_IRQ_LEVEL_HIGH
/** Specify available privilege levels for interrupts:
    CLIC_PRIVILEGE_LEVELS_M:  M-mode is available for interrupts;
    CLIC_PRIVILEGE_LEVELS_MU: M-mode and U-mode are available for interrupts.
    Default: CLIC_PRIVILEGE_LEVELS_M. */
#define CLIC_SYSTEM_INIT_PRIVILEGE_LEVELS CLIC_PRIVILEGE_LEVELS_M
/** Specify the privilege mode for interrupts:
    CLIC_PRIVILEGE_MODE_IRQ_M: Interrupts operate in M-mode;
    CLIC_PRIVILEGE_MODE_IRQ_U: Interrupts operate in U-mode.
    Default: CLIC_PRIVILEGE_MODE_IRQ_M. */
#define CLIC_SYSTEM_INIT_PRIVILEGE_MODE_IRQ CLIC_PRIVILEGE_MODE_IRQ_M
#endif
#endif

/* Parameter run-time check support. */
/** Parameter checking level:
    0: No parameter checks ("assert_param" macro is disabled)
    1: Check enabled, source file ID and line number are available
    2: Check enabled, source file ID, line number and checking expression (as string) are available (increased code size)
    Default: 0 ("assert_param" macro is disabled) */
#define USE_ASSERT_INFO 0

/**
 * @brief  The assert_param macro is used for function's parameters check.
 * @note   That function should not return. If expr is true, nothing is done.
 * @param  expr: If expr is false, it calls assert_failed user's function which
 *         reports the name of the source file, source line number
 *         and expression text (if USE_ASSERT_INFO == 2) of the call that failed.
 * @return None.
 */
#if (USE_ASSERT_INFO == 0)
#define assert_param(expr) ((void)0)
#elif (USE_ASSERT_INFO == 1)
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t*)__FILE__, __LINE__))
void assert_failed(uint8_t* file, uint32_t line);
#elif (USE_ASSERT_INFO == 2)
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t*)__FILE__, __LINE__, (const uint8_t*)#expr))
void assert_failed(uint8_t* file, uint32_t line, const uint8_t* expr);
#else
#error "Unsupported USE_ASSERT_INFO level."
#endif /* USE_ASSERT_INFO */

// </h>

#if defined(__GNUC__) /* GCC compiler. */
#define __RAMFUNC     __attribute__((section(".ramfunc")))
#define __ahbram_data __attribute__((section(".ahbram_data")))
#define __ahbram_bss  __attribute__((section(".ahbram_bss")))
#elif defined(__ICCRISCV__) /* IAR RISC-V compiler. */
#define __RAMFUNC     __attribute__((section("ramfunc")))
#define __ahbram_data __attribute__((section("ahbram_data")))
#define __ahbram_bss  __attribute__((section("ahbram_bss")))
#endif

#if defined(USE_MDR32F02) || defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
#include "system_MDR32VF0xI_config.h"
#endif

#ifdef __cplusplus
}
#endif

#endif /* __ASSEMBLER__ */

#endif /* MDR32VF0xI_CONFIG_H */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE MDR32VF0xI_config.h */

