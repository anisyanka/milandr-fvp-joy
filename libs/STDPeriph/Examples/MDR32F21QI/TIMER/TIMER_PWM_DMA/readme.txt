  /**
  @addtogroup TIMER_PWM_DMA_234 TIMER_PWM_DMA example for K1986VK234 evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2024 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    29/05/2023
  * @brief   Description of the TIMER_PWM_DMA Example.
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

A demonstration of the timer operation in PWM mode using DMA.
The example sets up timer 0, channel 1. The PWM signal is output to the PA0 (CH1) and PA1 (CH1N) pins. 
After timer setting configures the DMA to cycle the value in channel 1's CCR register to based on the values in the CCR_Val array. 
The PWM signal is generated based on the REF signal, which, when operating in the OCCM = 6 and DIR = 0 (direct count) mode, has the format:
- REF = 1 if DIR = 0 (count up) and CNT < CCR, otherwise REF = 0.

Thus, in this case, the value in the CCR register is the duration of the positive PWM pulse during the ARR counting period of the main CNT counter.


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
        MDR32F2xQI_timer.c
        MDR32F21QI_dma.c
    - Edit the MDR32F2xQI_config.h to set appropriate run-time parameter checking level.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.

 * <h3><center>&copy; COPYRIGHT 2024 Milandr</center></h3>
 */


