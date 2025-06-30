/**
 ******************************************************************************
 * @file    MDR32VF0xI_utils.c
 * @author  Milandr Application Team
 * @version V0.1.0
 * @date    16/04/2024
 * @brief   This file contains all utility functions of the firmware library.
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
#include "MDR32VF0xI_utils.h"
#include "core_clint.h"
#if defined(USE_MDR32F02_REV_1X)
#include "MDR32VF0xI_bkp.h"
#endif

/** @addtogroup MDR32VF0xI_StdPeriph_Driver MDR32VF0xI Standard Peripheral Driver
 * @{
 */

/** @addtogroup UTILS UTILS
 * @{
 */

/** @defgroup UTILS_Private_Variables UTILS Private Variables
 * @{
 */

static uint32_t DelayMode = DELAY_MODE_PROGRAM;

static uint32_t DelayProgramConst      = DELAY_PROGRAM_GET_CONST(HSI_FREQUENCY_Hz);
static uint32_t DelayMachineTimerConst = DELAY_MACHINE_TIMER_GET_CONST(HSI_FREQUENCY_Hz);

/** @} */ /* End of the group UTILS_Private_Variables */

/** @defgroup UTILS_Private_Function_Prototypes UTILS Private Functions Prototypes
 * @{
 */

ErrorStatus DELAY_MachineTimer_Init(void);
void        DELAY_MachineTimer_WaitTicks(uint32_t Ticks);

/** @} */ /* End of the group UTILS_Private_Function_Prototypes */

/** @addtogroup UTILS_Exported_Functions UTILS Exported Functions
 * @{
 */

/**
 * @brief  Initialize the DELAY module according to a specified mode.
 * @note   The core clock from SystemCoreClock is used to calculate the delay time.
 *         You must configure the required core clock and call SystemCoreClockUpdate() before calling DELAY_Init().
 * @param  Mode: @ref DELAY_Mode_TypeDef - specify the delay mode.
 * @return @ref ErrorStatus - SUCCESS if initialized, else ERROR.
 */
ErrorStatus DELAY_Init(DELAY_Mode_TypeDef Mode)
{
    assert_param(IS_DELAY_MODE(Mode));

    DelayMode = Mode;

    switch (DelayMode) {
        case DELAY_MODE_PROGRAM:
            DelayProgramConst = DELAY_PROGRAM_GET_CONST(SystemCoreClock);
            break;
        case DELAY_MODE_MACHINE_TIMER:
#if defined(USE_MDR32F02_REV_1X)
            DelayMachineTimerConst = DELAY_MACHINE_TIMER_GET_CONST(LSE_FREQUENCY_Hz);
#else
            DelayMachineTimerConst = DELAY_MACHINE_TIMER_GET_CONST(SystemCoreClock / 2);
#endif
            return DELAY_MachineTimer_Init();
            break;
    }
    return SUCCESS;
}

/**
 * @brief  Perform a blocking delay in microseconds.
 * @param  Us: Specify the delay time in microseconds.
 *         This parameter can be a value:
 *         - DELAY_MODE_PROGRAM:       Us <= 16785412 / CPU_CLK (MHz);
 *         - DELAY_MODE_MACHINE_TIMER: Us <= 4294967295 / CPU_CLK (MHz).
 * @return None.
 */
void DELAY_WaitUs(uint32_t Us)
{
    switch (DelayMode) {
        case DELAY_MODE_PROGRAM:
            DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_US(Us, DelayProgramConst));
            break;
        case DELAY_MODE_MACHINE_TIMER:
            DELAY_MachineTimer_WaitTicks(DELAY_MACHINE_TIMER_GET_TICKS_US(Us, DelayMachineTimerConst));
            break;
    }
}

/**
 * @brief  Perform a blocking delay in milliseconds.
 * @param  Ms: Specify the delay time in milliseconds.
 *         This parameter can be a value:
 *         - DELAY_MODE_PROGRAM:       Ms <= 17179869 / CPU_CLK (MHz);
 *         - DELAY_MODE_MACHINE_TIMER: Ms <= 4294967 / CPU_CLK (MHz).
 * @return None.
 */
void DELAY_WaitMs(uint32_t Ms)
{
    switch (DelayMode) {
        case DELAY_MODE_PROGRAM:
            DELAY_PROGRAM_WaitLoopsAsm(DELAY_PROGRAM_GET_LOOPS_MS(Ms, DelayProgramConst));
            break;
        case DELAY_MODE_MACHINE_TIMER:
            DELAY_MachineTimer_WaitTicks(DELAY_MACHINE_TIMER_GET_TICKS_MS(Ms, DelayMachineTimerConst));
            break;
    }
}

#if defined(USE_MDR1206FI) || defined(USE_MDR1206AFI) || defined(USE_MDR1206)
/**
 * @brief  Get microcontroller ID (215 or 217) for MDR1206.
 * @note   This function can be used only for MDR1206 microcontroller.
 * @return Microcontroller ID value.
 */
uint8_t GetChipID(void)
{
    uint32_t Per2Clock, ChipID;

    Per2Clock = MDR_RST_CLK->PER2_CLOCK;

    MDR_RST_CLK->PER2_CLOCK |= RST_CLK_PER2_CLOCK_PCLK_EN_FLASH;
    ChipID = MDR_FLASH->CHIP_ID_CTRL;

    MDR_RST_CLK->PER2_CLOCK = Per2Clock;

    ChipID = (ChipID & FLASH_CHIP_ID_CTRL_CHIP_ID_Msk) >> FLASH_CHIP_ID_CTRL_CHIP_ID_Pos;

    return (uint8_t)ChipID;
}
#endif

/** @} */ /* End of the group UTILS_Exported_Functions */

/** @defgroup UTILS_Private_Functions UTILS Private Functions
 * @{
 */

/**
 * @brief  Initialize the machine timer.
 * @param  None.
 * @return @ref ErrorStatus - SUCCESS if initialized, else ERROR.
 */
ErrorStatus DELAY_MachineTimer_Init(void)
{
#if defined(USE_MDR32F02_REV_1X)
    uint32_t TempReg1, TempReg2, TempReg3;

    /* Enable the BKP clock if it is disabled. */
    TempReg1 = MDR_RST_CLK->PER2_CLOCK;
    if ((TempReg1 & RST_CLK_PER2_CLOCK_PCLK_EN_BKP_Msk) == 0) {
        MDR_RST_CLK->PER2_CLOCK = TempReg1 | RST_CLK_PER2_CLOCK_PCLK_EN_BKP;
    }

    /* Check the LSE_RDY flag. */
    TempReg2 = MDR_BKP->CLK;
    if ((TempReg2 & BKP_CLK_LSE_RDY_Msk) == 0) {
        /* Return error if LSE has been enabled but still is not ready. */
        if ((TempReg2 & BKP_CLK_LSE_ON_Msk) != 0) {
            return ERROR;
        }

        /* Disable BKP write protection. */
        TempReg3 = MDR_BKP->WPR;
        if (TempReg3 != BKP_WPR_BKP_WPR_UNLOCK) {
            MDR_BKP->WPR = BKP_WPR_BKP_WPR_UNLOCK;
        }
        /* Enable the LSE clock. If it is rev.1 or rev.1.1, assume errata0001 has been taken
         into account and on-board LSE generator is used. */
        BKP_LSE_Config(BKP_LSE_BYPASS);

        if (BKP_LSE_GetStatus() == ERROR) {
            return ERROR;
        }

        /* Restore the BKP_WPR register value. */
        if (TempReg3 != BKP_WPR_BKP_WPR_UNLOCK) {
            MDR_BKP->WPR = TempReg3;
        }
    }

    /* Restore the PER2_CLOCK register value. */
    if ((TempReg1 & RST_CLK_PER2_CLOCK_PCLK_EN_BKP_Msk) == 0) {
        MDR_RST_CLK->PER2_CLOCK = TempReg1;
    }

#else
    MDR_RST_CLK->DIV_SYS_TIM = 0x00000000;
#endif
    return SUCCESS;
}

/**
 * @brief  Perform a blocking delay with a system tick timer, in ticks.
 * @param  Ticks: Specify the delay time in ticks.
 * @return None.
 */
void DELAY_MachineTimer_WaitTicks(uint32_t Ticks)
{
    uint64_t CompareValue = (uint64_t)Ticks + CLINT_MTIMER_GetTime();
    while (CLINT_MTIMER_GetTime() < CompareValue) { }
}

/** @} */ /* End of the group UTILS_Private_Functions */

/** @} */ /* End of the group UTILS */

/** @} */ /* End of the group MDR32VF0xI_StdPeriph_Driver */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE MDR32VF0xI_utils.c */

