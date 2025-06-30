  /**
  @addtogroup WWDG_Use_Interrupt_MDR32F1QI WWDG_Use_Interrupt for MDR32F1QI evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2024 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V2.0.1
  * @date    20/06/2023
  * @brief   Description of the WWDG_Use_Interrupt Example.
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

This example shows how to use WWDG module functions.
That action includes the following steps:
    - WWDG module initialization;
    - Enable interrupt for WWDG;
    - Get interrupts status;
    - Clear interrupt pending bit;
    - EWI flag clearing;
    - Counter value setting.

LED1 is used to indicate MCU reboot and the EWI flag state.


@par Directory contains:

    - main.c             Main program


@par Hardware and Software environment:

    - This example is intended to run on MDR32F1QI eval board with MDR32F1QI microcontroller.
    - VD7 (LED1) are connected to PD.7 pin.


@par How to use.

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - Add main.c file.
    - Add the required files from "Libraries" folder:
        MDR32FxQI_config.h
        MDR32FxQI_port.c
        MDR32FxQI_rst_clk.c
        MDR32FxQI_wwdg.c
    - Edit the MDR32FxQI_config.h to set appropriate run-time parameter checking level.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.


 * <h3><center>&copy; COPYRIGHT 2024 Milandr</center></h3>
 */


