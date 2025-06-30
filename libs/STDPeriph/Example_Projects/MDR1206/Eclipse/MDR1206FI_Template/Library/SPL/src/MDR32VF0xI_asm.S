/**
 *******************************************************************************
 * @file    MDR32VF0xI_asm.S
 * @author  Milandr Application Team
 * @version V0.1.0
 * @date    18/04/2024
 * @brief   This file contains assembler functions.
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

#if defined ( __GNUC__ ) /* GCC compiler. */
    .section .ramfunc, "ax"
#elif defined ( __IASMRISCV__ )  /* IAR RISCV compiler. */
    SECTION ramfunc:CODE:NOROOT(2)
#endif

;/**
;  * @brief   Perform a blocking program delay in loops.
;  * @note    Register r0 is not stored on the stack because it is non preserved register.
;  * @param   Loops: Specifies the delay time in loops.
;  * @return  None.
;  */
#if defined ( __GNUC__ ) /* GCC compiler. */
    .type DELAY_PROGRAM_WaitLoopsAsm, @function
    .globl DELAY_PROGRAM_WaitLoopsAsm
#elif defined ( __IASMRISCV__ )  /* IAR RISCV compiler. */
    PUBLIC DELAY_PROGRAM_WaitLoopsAsm
#endif
DELAY_PROGRAM_WaitLoopsAsm:
    beqz a0, DELAY_PROGRAM_Exit
DELAY_PROGRAM_Loop:
    addi a0, a0, -1
    bnez a0, DELAY_PROGRAM_Loop
DELAY_PROGRAM_Exit:
    ret

#if defined ( __IASMRISCV__ )  /* IAR RISCV compiler. */
    END
#endif


