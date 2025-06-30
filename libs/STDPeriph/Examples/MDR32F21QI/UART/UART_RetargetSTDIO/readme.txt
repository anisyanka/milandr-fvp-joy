  /**
  @addtogroup UART_Retarget_STDIO_234 UART_Retarget_STDIO example for K1986VK234 evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2024 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    29/05/2023
  * @brief   Description of the UART_Retarget_STDIO Example.
  ********************************************************************************
  * THE PRESENT FIRMWARE IS FOR GUIDANCE ONLY. IT AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING MILANDR'S PRODUCTS IN ORDER TO FACILITATE
  * THE USE AND SAVE TIME. MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES RESULTING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR A USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN THEIR PRODUCTS.
  ********************************************************************************
  @endverbatim
  @endlatexonly

@par Example Description

This example shows how to redirect standard I/O (STDIO) to UART using the MDR32F2xQI_utils module.
UART parameters are set in MDR32F2xQI_config.h, USE_STDIO_RETARGET block. The default parameters are:
- UART peripheral: UART0 (PB0 as UART0_TXD, PB1 as UART0_RXD);
- baud rate: 115200 baud;
- data size: 8 bits;
- stop bits: 1 bit;
- parity: no;
- UART clock source: PLLCPU.


@par Directory contains:

    - main.c                 Main program


@par Hardware and Software environment:

    - This example is intended to run on K1986VK234 eval board with MDR32F21QI microcontroller.


@par How to use

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - For the IAR EW in the project settings "General options -> Library Configuration -> Library" you must set the "Full" parameter.
    - Add main.c file.
    - Add the required files from "Libraries" folder:
        MDR32F2xQI_config.h
        MDR32F2xQI_rst_clk.c
        MDR32F2xQI_port.c
        MDR32F2xQI_uart.c
        MDR32F2xQI_utils.c
        MDR32F2xQI_bkp.c
        MDR32F2xQI_eeprom.c
    - Be sure to set the macro USE_STDIO_RETARGET = 1, which is defined in MDR32F2xQI_config.h.
    - Edit the MDR32F2xQI_config.h to set appropriate run-time parameter checking level.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.

 * <h3><center>&copy; COPYRIGHT 2024 Milandr</center></h3>
 */


