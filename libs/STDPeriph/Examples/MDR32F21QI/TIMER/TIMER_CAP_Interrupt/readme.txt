  /**
  @addtogroup TIMER_CAP_Interrupt_234 TIMER_CAP_Interrupt example for K1986VK234 evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2024 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    30/05/2023
  * @brief   Description of the TIMER_CAP_Interrupt Example.
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

A demonstration of the timer operation in Capture mode.
Pin PA0 (TIMER0 CH1) is configured to capture external events. The channel 1 input is configured with 
a filter that passes only pulses longer than 1 ms. The rising edge is selected as a capture event, 
interrupt is also enabled.

On the evaluation board button B1 is connected to PA0 pin. When the button is released, the edge will be fixed, 
along which LED1 will be switched.


@par Directory contains:

    - main.c        Main program


@par Hardware and Software environment:

    - This example is intended to run on K1986VK234 eval board with MDR32F21QI microcontroller.
    - B1 is connected to PA0 pin.
    - LED1 is connected to PC4 pin.


@par How to use

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - Add main.c file.
    - Add the required files from "Libraries" folder:
        MDR32F2xQI_config.h
        MDR32F2xQI_rst_clk.c
        MDR32F2xQI_port.c
        MDR32F2xQI_timer.c
    - Edit the MDR32F2xQI_config.h to set appropriate run-time parameter checking level.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.

 * <h3><center>&copy; COPYRIGHT 2024 Milandr</center></h3>
 */


