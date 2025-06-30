  /**
  @addtogroup TIMER_PWM_214 TIMER_PWM example for K1986VK214 evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2024 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    29/05/2023
  * @brief   Description of the TIMER_PWM Example.
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

A demonstration of the timer operation in PWM mode. PWM pulses are generated at the PA0 and PA1 pins with duty cycle:
- CH1 (PA0) = 25% and CH1N (PA1) = 75%.


@par Directory contains:

    - main.c        Main program


@par Hardware and Software environment:

    - This example is intended to run on K1986VK214 eval board with MDR32F23QI microcontroller.


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


