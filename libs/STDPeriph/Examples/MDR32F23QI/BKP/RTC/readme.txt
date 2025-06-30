/**
  @addtogroup BKP_RTC_214 BKP RTC example for K1986VK214 evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2024 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    29/05/2023
  * @brief   Description of the BKP_RTC Example.
  ******************************************************************************
  * THE PRESENT FIRMWARE IS FOR GUIDANCE ONLY. IT AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING MILANDR'S PRODUCTS IN ORDER TO FACILITATE
  * THE USE AND SAVE TIME. MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES RESULTING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR A USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN THEIR PRODUCTS.
  ******************************************************************************
  @endverbatim
  @endlatexonly

@par Example Description

This example shows how to use BKP module functions.
That action includes the following steps:
    - Configure PORTC lines 14-15 as output LEDs 1-2;
    - Configure RTC clock source;
    - Enable RTC interrupts;
    - Set RTC counter, prescaler and alarm values to count seconds and minutes;
    - Enable RTC;
    - Switch on/off LED1 when SECF Flag is set;
    - When ALRF is set, switch on LED2.


@par Directory contains:

    - main.c                          Main program


@par Hardware and Software environment:

    - This example is intended to run on K1986VK214 eval board with MDR32F23QI microcontroller.
    - LED1-LED2 are connected to PC14-PC15 pins, respectively.


@par How to use.

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - Add main.c file.
    - Add the required files from "Libraries" folder:
        MDR32F2xQI_config.h
        MDR32F2xQI_port.c
        MDR32F2xQI_rst_clk.c
        MDR32F2xQI_bkp.c
    - Edit the MDR32F2xQI_config.h to set appropriate run-time parameter checking level.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.

 * <h3><center>&copy; COPYRIGHT 2024 Milandr</center></h3>
 */

