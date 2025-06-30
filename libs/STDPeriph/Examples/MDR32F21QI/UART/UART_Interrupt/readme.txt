  /**
  @addtogroup UART_Interrupt_234 UART_Interrupt example for K1986VK234 evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2024 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    29/05/2023
  * @brief   Description of the UART_Interrupt Example.
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

This example shows how to configure the UART to send and receive data in an interrupt handler.
Configure and set PC2 to disable pin RO (connected to PC1) of D5 (RS-485).
Configure PB0 as UART0_TX. Configure PC1 as UART1_RX.
Configure UARTs on an exchange with 115200 baud rate, 8 bit data size, 1 stop-bit and no parity check.
Enable transmitter interrupt for UART0. Enable receiver interrupt for UART1.
UART0 transmits the specified number of bytes in the interrupt handler, and then sets the transfer complete flag.
UART1 receives the specified number of bytes in the interrupt handler, and then sets the receive complete flag.


@par Directory contains:

    - main.c                       Main program


@par Hardware and Software environment:

    - This example is intended to run on K1986VK234 eval board with MDR32F21QI microcontroller.
    - PB0 (UART0_TX) and PC1 (UART1_RX) must be connected together.


@par How to use.

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - Add main.c file.
    - Add the required files from "Libraries" folder:
        MDR32F2xQI_config.h
        MDR32F2xQI_rst_clk.c
        MDR32F2xQI_port.c
        MDR32F2xQI_uart.c
    - Edit the MDR32F2xQI_config.h to set appropriate run-time parameter checking level.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.

 * <h3><center>&copy; COPYRIGHT 2024 Milandr</center></h3>
 */


