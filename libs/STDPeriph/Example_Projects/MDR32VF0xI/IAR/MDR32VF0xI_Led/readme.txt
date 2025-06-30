  /**
  @addtogroup MDR32VF0xI_Led_Example MDR32VF0xI_Led example for MDR32F02, MDR1206(A)FI evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2024 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    01/04/2024
  * @brief   Description of the MDR32VF0xI_Led Example.
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

This example shows how to use output ports.
PORTD pins [3:0] are configured for output and connected to on-board LED [4:1] indicators.
LED [4:1] blink simultaneously and periodically.


@par Directory contains:

    - src/main.c    Main program.
    - Library       SPL and CMSIS libraries directory for MDR32VF0xI.


@par Hardware and Software environment:

    - This example is intended to run on MDR32F02, MDR1206(A)FI eval boards with MDR32F02 or MDR1206(A)FI microcontroller.
    - LED [4:1] are connected to PD[3:0] pins, respectively.


@par How to use.

To launch the example, you must do the following:
    - Open current project in IAR EW for RISC-V and setup necessary project configurations.
    - Edit the MDR32VF0xI_config.h (in Library/SPL) to set appropriate run-time parameter checking level.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.

 * <h3><center>&copy; COPYRIGHT 2024 Milandr</center></h3>
 */

