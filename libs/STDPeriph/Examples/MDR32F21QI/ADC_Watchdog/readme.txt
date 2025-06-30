  /**
  @addtogroup ADC_Watchdog_234 ADC_Watchdog example for K1986VK234 evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2024 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    10/04/2023
  * @brief   Description of the ADC_Watchdog Example.
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

This example demonstrates how to use ADC to control continuously
an ADC channel 4 in the given range (0x0800-0x0900).
The ADC1 is configured to convert continuously ADC channel 4.

The analog watchdog is configured and enabled to guard a single regular channel.
Each time the channel 4 converted value exceed programmed analog watchdog high
threshold (value 0x0900) or goes down analog watchdog low threshold (value 0x0800)
an AWOIF interrupt is generated and the output pin connected to LED1 is turn on.
The LED1 will bright as long as the AWOIF interrupt is generated which means
that the converted value of regular ADC channel 4 is outside the range limited by high
and low analog watchdog thresholds.
The LED2 will bright as long as the converted value of regular ADC channel 4 is above the upper limit.
Regular ADC channel 4 connected to the trim resistor.


@par Directory contains:

    - main.c        Main program


@par Hardware and Software environment:

    - This example is intended to run on K1986VK234 eval board with MDR32F21QI microcontroller.


@par How to use

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - Add main.c file.
    - Add the required files from "Libraries" folder:
        MDR32F2xQI_config.h
        MDR32F2xQI_rst_clk.c
        MDR32F2xQI_port.c
        MDR32F2xQI_adc.c
    - Edit the MDR32F2xQI_config.h to set appropriate run-time parameter checking level.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.

 * <h3><center>&copy; COPYRIGHT 2024 Milandr</center></h3>
 */


