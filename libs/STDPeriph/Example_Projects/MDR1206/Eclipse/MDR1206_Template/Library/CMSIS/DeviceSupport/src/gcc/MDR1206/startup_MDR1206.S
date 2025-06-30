/**
 *******************************************************************************
 * @file    startup_MDR1206.S
 * @author  Milandr Application Team
 * @version V0.1.0
 * @date    29/03/2024
 * @brief   Core device startup file for MDR1206.
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

#include "system_MDR32VF0xI_config.h"

#if defined ( __GNUC__ ) /* GCC compiler */
#if defined ( USE_MDR1206FI ) || defined ( USE_MDR1206AFI ) || defined ( USE_MDR1206 ) /* MCU definition */

#if !defined(CSR_MTVT)
#define CSR_MTVT       0x307
#endif
#if !defined(CSR_MCLICBASE)
#define CSR_MCLICBASE  0x308
#endif

#if __riscv_xlen == 64
    #define LREG           ld
    #define SREG           sd
    #define REGBYTES       8
    #define LOG2_REGBYTES  3
    #define XWORD          .dword
#else
    #define LREG           lw
    #define SREG           sw
    #define REGBYTES       4
    #define LOG2_REGBYTES  2
    #define XWORD          .word
#endif

.section ".text.init"
.globl _start

_start:
//    // Only first hart runs
//    csrr s10, mhartid
//    beqz s10, 1f
//    j .
//1:

    // Zeroing registers
    li  x1, 0
    li  x2, 0
    li  x3, 0
    li  x4, 0
    li  x5, 0
    li  x6, 0
    li  x7, 0
    li  x8, 0
    li  x9, 0
    li  x10,0
    li  x11,0
    li  x12,0
    li  x13,0
    li  x14,0
    li  x15,0
    li  x16,0
    li  x17,0
    li  x18,0
    li  x19,0
    li  x20,0
    li  x21,0
    li  x22,0
    li  x23,0
    li  x24,0
    li  x25,0
    li  x26,0
    li  x27,0
    li  x28,0
    li  x29,0
    li  x30,0
    li  x31,0

    // Global pointer (gp) allows using more effective gp-relative addressing instead of
    // abosulte/pc-relative for arbitrary 4KiB memory range. For the optimisation to be effective,
    // it should cover the most intensely used RAM area. For standard newlib apps, this is the
    // area where the .sdata section is located. The linker uses the (!) __global_pointer$ symbol
    // definition to compare the memory addresses and, if within range, it replaces
    // absolute/pc-relative addressing with gp-relative addressing, which makes the code more
    // efficient. This process is also called relaxing, and can be disabled by -Wl,--no-relax.
    // The gp register should be loaded during startup and should not be changed later.
    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop

    // Set stack pointer to the end of available memory
    la   sp, __stack_top

    /*-------------------------*/
    /*  Copy functions to RAM  */
    /*-------------------------*/
    la a0, __text_load_start
    la a1, __text_start
    la a2, __text_end
    beq  a0, a1, 2f          // do i need to copy?
    bgeu a1, a2, 2f
1:
    lw t0, (a0)
    sw t0, (a1)
    addi a0, a0, 4
    addi a1, a1, 4
    bltu a1, a2, 1b
2:

    /*-------------------------*/
    /*  Copy functions to RAM  */
    /*-------------------------*/
    la a0, __ramfunc_load_start
    la a1, __ramfunc_start
    la a2, __ramfunc_end
    beq  a0, a1, 2f          // do i need to copy?
    bgeu a1, a2, 2f
1:
    lw t0, (a0)
    sw t0, (a1)
    addi a0, a0, 4
    addi a1, a1, 4
    bltu a1, a2, 1b
2:

    /*-------------*/
    /*  Init data  */
    /*-------------*/
    la a0, __data_load_start
    la a1, __data_start
    la a2, __data_end
    beq  a0, a1, 2f          // do i need to copy?
    bgeu a1, a2, 2f
1:
    lw t0, (a0)
    sw t0, (a1)
    addi a0, a0, 4
    addi a1, a1, 4
    bltu a1, a2, 1b
2:

    /*-------------*/
    /* Zeroing bss */
    /*-------------*/
    la    t0, __bss_start
    la    t1, __bss_end
1:
    bge   t0, t1, 2f
    sw    zero, 0(t0)
    addi  t0, t0, 4
    j     1b
2:

    /*-----------------*/
    /* AHB_RAM Section */
    /*-----------------*/
    /*  Init AHB_RAM data  */
    la a0, __ahbram_data_load_start
    la a1, __ahbram_data_start
    la a2, __ahbram_data_end
    bgeu a1, a2, 2f
1:
    lw t0, (a0)
    sw t0, (a1)
    addi a0, a0, 4
    addi a1, a1, 4
    bltu a1, a2, 1b
2:
    /* Zeroing AHB_RAM bss */
    la    t0, __ahbram_bss_start
    la    t1, __ahbram_bss_end
1:
    bge   t0, t1, 2f
    sw    zero, 0(t0)
    addi  t0, t0, 4
    j     1b
2:

    /* Call static constructors */
    call __libc_init_array

    /* Call system initialization */
    call SystemInit

    /*-----------*/
    /* Call main */
    /*-----------*/
    call main
    // get main return code
    mv t0, a0

    j .

#endif /* USE_MDR1206 */
#endif /* __GNUC__ */


