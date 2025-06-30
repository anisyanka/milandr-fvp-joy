/**
 *******************************************************************************
 * @file    link_RAM_TCM_release.ld
 * @author  Milandr Application Team
 * @version V1.0.0
 * @date    10/04/2024
 * @brief   Linker script file for MDR1206.
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

/* It defines following symbols, which code can use without definition:
 *   __flash_start
 *   __flash_end
 *   __boot_ram_start
 *   __boot_ram_size
 *   __boot_ram_end
 *   __text_init_start
 *   __text_init_end
 *   __text_load_start
 *   __text_start
 *   __text_end
 *   __data_load_start
 *   __data_start
 *   __sdata_start
 *   __data_end
 *   __bss_start
 *   __bss_end
 *   __global_pointer$
 *   __end
 *   __ahbram_data_load_start
 *   __ahbram_data_start
 *   __ahbram_data_end
 *   __ahbram_bss_start
 *   __ahbram_bss_end
 *   __ramfunc_load_start
 *   __ramfunc_start
 *   __ramfunc_end
 *   __firmware_end
 *   __stack_top
 *   __stack_size
 *   __stack_limit
 *   __freertos_irq_stack_top
 *   __heap_top
 *   __heap_min_size
 *   __heap_size
 *   __heap_limit
 *   end
 */

OUTPUT_ARCH( "riscv" )
ENTRY(_start)

/* __stack_size - total stack size. */
__stack_size    = 0x1000;
/* __heap_min_size - minimum heap size, used to check if space can be allocated. The total heap size (__heap_size) takes up all space from end of the .bss to beginning of .stack. */
__heap_min_size = 0x1000;
/* __boot_ram_size - size of virtual area, used in common bootloader projects. */
__boot_ram_size = 0x2000;

/* Specify the memory areas. */
MEMORY
{
    BOOT      (rx)      : ORIGIN = 0x00020000, LENGTH = 15K
    FLASH     (rx)      : ORIGIN = 0x10000000, LENGTH = 512K
    RAM_TCMA  (xrw)     : ORIGIN = 0x40000000, LENGTH = 64K
    RAM_TCMB  (xrw)     : ORIGIN = 0x40010000, LENGTH = 32K
    RAM_AHB   (xrw)     : ORIGIN = 0x20000000, LENGTH = 16K
    /* Virtual area, used in common bootloader project. */
    BOOT_RAM  (xrw)     : ORIGIN = ORIGIN(RAM_TCMB) + LENGTH(RAM_TCMB) - __boot_ram_size, LENGTH = __boot_ram_size
}

/* Description of regions:
- REGION_LOAD       - region where the program is stored,
- REGION_TEXT       - region where the code is located,
- REGION_DATA       - region where initialized and uninitialized data, stack and heap are located,
- REGION_RAMFUNC    - the region of RAM TCM where the code is stored. */


/* ---------------------- */
/* LOAD AND RUN FROM BOOT */
/* ---------------------- */
/*
REGION_ALIAS("REGION_LOAD",       BOOT);
REGION_ALIAS("REGION_TEXT",       BOOT);
REGION_ALIAS("REGION_DATA",       BOOT_RAM);
REGION_ALIAS("REGION_RAMFUNC",    BOOT_RAM);
*/

/* ----------------------- */
/* LOAD AND RUN FROM FLASH */
/* ----------------------- */
/*
REGION_ALIAS("REGION_LOAD",       FLASH);
REGION_ALIAS("REGION_TEXT",       FLASH);
REGION_ALIAS("REGION_DATA",       RAM_TCMA);
REGION_ALIAS("REGION_RAMFUNC",    RAM_TCMB);
*/

/* ------------------------- */
/* LOAD AND RUN FROM RAM TCM */
/* ------------------------- */
/*
REGION_ALIAS("REGION_LOAD",       RAM_TCMA);
REGION_ALIAS("REGION_TEXT",       RAM_TCMA);
REGION_ALIAS("REGION_DATA",       RAM_TCMA);
REGION_ALIAS("REGION_RAMFUNC",    RAM_TCMB);
*/

/* ------------------------- */
/* LOAD AND RUN FROM RAM AHB */
/* ------------------------- */
/*
REGION_ALIAS("REGION_LOAD",       RAM_AHB);
REGION_ALIAS("REGION_TEXT",       RAM_AHB);
REGION_ALIAS("REGION_DATA",       RAM_TCMA);
REGION_ALIAS("REGION_RAMFUNC",    RAM_TCMB);
*/

/* ------------------------------------------ */
/* LOAD FROM FLASH TO RAM TCM, RUN IN RAM TCM */
/* ------------------------------------------ */

REGION_ALIAS("REGION_LOAD",       FLASH);
REGION_ALIAS("REGION_TEXT",       RAM_TCMA);
REGION_ALIAS("REGION_DATA",       RAM_TCMA);
REGION_ALIAS("REGION_RAMFUNC",    RAM_TCMB);


/* ------------------------------------------ */
/* LOAD FROM FLASH TO RAM AHB, RUN IN RAM AHB */
/* ------------------------------------------ */
/*
REGION_ALIAS("REGION_LOAD",       FLASH);
REGION_ALIAS("REGION_TEXT",       RAM_AHB);
REGION_ALIAS("REGION_DATA",       RAM_TCMA);
REGION_ALIAS("REGION_RAMFUNC",    RAM_TCMB);
*/

/* Define output sections: */
/* ">vma_region" - VMA (virtual memory address),
   "AT>lma_region" - LMA (load memory address). */

/* Define symbols for flash memory start and size. */
__flash_start = ORIGIN(FLASH);
__flash_end   = ORIGIN(FLASH) + LENGTH(FLASH);

/* Define symbols for boot RAM memory start and size. */
__boot_ram_start = ORIGIN(BOOT_RAM);
__boot_ram_end   = ORIGIN(BOOT_RAM) + LENGTH(BOOT_RAM);

SECTIONS
{
    /* The startup code goes first into REGION_LOAD. */
    .text.init :
    {
        __text_init_start = .;
        *(.text.header)
        *(.text.init)
        . = ALIGN(4);
        __text_init_end = .;
    } >REGION_LOAD

    /* The program code and other data goes into REGION_TEXT, load from REGION_LOAD. */
    __text_load_start = LOADADDR(.text);
    .text :
    {
        . = ALIGN(4);
        __text_start = .;
        *(.text*)
        . = ALIGN(4);
        __text_end = .;
    } >REGION_TEXT AT>REGION_LOAD

    /* Read-only data goes into REGION_LOAD. */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata*)
        *(.srodata*)
        . = ALIGN(4);
    } >REGION_LOAD

    /* Initialized data sections goes into REGION_DATA, load from REGION_LOAD. */
    __data_load_start = LOADADDR(.data);
    .data :
    {
        . = ALIGN(4);
        __data_start = .;
        *(.data*)
        . = ALIGN(8);
        __sdata_start = .;
        *(.sdata*)
    } >REGION_DATA AT>REGION_LOAD

    . = ALIGN(4);
    __data_end = .;

    /* Uninitialized data section goes into REGION_DATA. */
    .bss :
    {
        . = ALIGN(8);
        __bss_start = .;
        *(.sbss*)
        *(.scommon)
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end = .;
        /* The linker uses the __global_pointer$ symbol definition to compare the memory addresses and,
           if within range, it replaces absolute/pc-relative addressing with gp-relative addressing,
           which makes the code more efficient. */
        __global_pointer$ = MIN(__sdata_start + 0x800, MAX(__data_start + 0x800, __bss_end - 0x800));
    } >REGION_DATA

    . = ALIGN(8);
    __end = .;

    /* Alignment of the following section .ahbram. */
    .aalign :
    {
        . = ALIGN(4);
    } >REGION_LOAD

    /* RAM_AHB section. */
    /* Initialized ahbram_data section goes into RAM_AHB, load from REGION_LOAD. */
    __ahbram_data_load_start = LOADADDR(.ahbram_data);
    .ahbram_data :
    {
        . = ALIGN(4);
        __ahbram_data_start = .;
        *(.ahbram_data)
        . = ALIGN(4);
        __ahbram_data_end = .;
    } >RAM_AHB AT>REGION_LOAD

    .ahbram_bss :
    {
        . = ALIGN(4);
        __ahbram_bss_start = .;
        *(.ahbram_bss)
        . = ALIGN(4);
        __ahbram_bss_end = .;
    } >RAM_AHB

    /* Alignment of the following section .ramfunc. */
    .ralign :
    {
        . = ALIGN(4);
    } >REGION_LOAD

    /* RAM functions goes into REGION_RAMFUNC, loaded from REGION_LOAD. */
    __ramfunc_load_start = LOADADDR(.ramfunc);
    .ramfunc :
    {
        . = ALIGN(4);
        __ramfunc_start = .;
        *(.ramfunc*)
        . = ALIGN(4);
        __ramfunc_end = .;
    } >REGION_RAMFUNC AT>REGION_LOAD

    /* Dummy section to get last LMA. */
    __firmware_end = LOADADDR(.trailer);
    .trailer :
    {
        KEEP(.trailer*)
    } >REGION_LOAD

    /* Stack and Heap symbol definitions. */
    __stack_top = ORIGIN(REGION_DATA) + LENGTH(REGION_DATA);
    __stack_limit = __stack_top - __stack_size;
    __freertos_irq_stack_top = __stack_top;
    __heap_top = __end;
    __heap_size = __stack_limit - __heap_top;
    __heap_limit = __heap_top + __heap_size;
    PROVIDE(end = __heap_top);

    /* Check allocation for heap of size __heap_min_size. */
    ASSERT(__heap_size >= __heap_min_size, "Unable to allocate __heap_min_size for heap!")

    /* .heap section doesn't contains any symbols.
       It is only used for linker to calculate size of heap sections, and assign values to heap symbols. */
    .heap __heap_top :
    {
        . += __heap_size;
    } >REGION_DATA

    /* .stack section doesn't contains any symbols.
       It is only used for linker to calculate size of stack sections, and assign values to stack symbols. */
    .stack __stack_limit :
    {
        . += __stack_size;
    } >REGION_DATA

    /* Relocation data sections. */
    .rela.init         0 : { *(.rela.init) }
    .rela.text         0 : { *(.rela.text .rela.text.* .rela.gnu.linkonce.t.*) }
    .rela.rodata       0 : { *(.rela.rodata .rela.rodata.* .rela.gnu.linkonce.r.*) }
    .rela.data         0 : { *(.rela.data .rela.data.* .rela.gnu.linkonce.d.*) }
    .rela.sdata        0 : { *(.rela.sdata .rela.sdata.* .rela.gnu.linkonce.s.*) }
    .rela.sbss         0 : { *(.rela.sbss .rela.sbss.* .rela.gnu.linkonce.sb.*) }
    .rela.bss          0 : { *(.rela.bss .rela.bss.* .rela.gnu.linkonce.b.*) }
    /* Stabs debugging sections. */
    .stab              0 : { *(.stab) }
    .stabstr           0 : { *(.stabstr) }
    .stab.excl         0 : { *(.stab.excl) }
    .stab.exclstr      0 : { *(.stab.exclstr) }
    .stab.index        0 : { *(.stab.index) }
    .stab.indexstr     0 : { *(.stab.indexstr) }
    .comment           0 (INFO) : { *(.comment); LINKER_VERSION; }
    /* DWARF debug sections.
       Symbols in the DWARF debugging sections are relative to the beginning
       of the section so we begin them at 0. */
    /* DWARF 1. */
    .debug             0 : { *(.debug) }
    .line              0 : { *(.line) }
    /* GNU DWARF 1 extensions. */
    .debug_srcinfo     0 : { *(.debug_srcinfo) }
    .debug_sfnames     0 : { *(.debug_sfnames) }
    /* DWARF 1.1 and DWARF 2. */
    .debug_aranges     0 : { *(.debug_aranges) }
    .debug_pubnames    0 : { *(.debug_pubnames) }
    /* DWARF 2. */
    .debug_info        0 : { *(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev      0 : { *(.debug_abbrev) }
    .debug_line        0 : { *(.debug_line .debug_line.* .debug_line_end) }
    .debug_frame       0 : { *(.debug_frame) }
    .debug_str         0 : { *(.debug_str) }
    .debug_loc         0 : { *(.debug_loc) }
    .debug_macinfo     0 : { *(.debug_macinfo) }
    /* SGI/MIPS DWARF 2 extensions. */
    .debug_weaknames   0 : { *(.debug_weaknames) }
    .debug_funcnames   0 : { *(.debug_funcnames) }
    .debug_typenames   0 : { *(.debug_typenames) }
    .debug_varnames    0 : { *(.debug_varnames) }
    /* DWARF 3. */
    .debug_pubtypes    0 : { *(.debug_pubtypes) }
    .debug_ranges      0 : { *(.debug_ranges) }
    /* DWARF 5. */
    .debug_addr        0 : { *(.debug_addr) }
    .debug_line_str    0 : { *(.debug_line_str) }
    .debug_loclists    0 : { *(.debug_loclists) }
    .debug_macro       0 : { *(.debug_macro) }
    .debug_names       0 : { *(.debug_names) }
    .debug_rnglists    0 : { *(.debug_rnglists) }
    .debug_str_offsets 0 : { *(.debug_str_offsets) }
    .debug_sup         0 : { *(.debug_sup) }
    .gnu.attributes    0 : { KEEP (*(.gnu.attributes)) }
    .riscv.attributes  0 : { KEEP (*(.riscv.attributes)) }
    /DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) *(.gnu.lto_*) }
}

