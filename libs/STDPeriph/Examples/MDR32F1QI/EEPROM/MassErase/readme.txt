  /**
  @addtogroup EEPROM_Mass_Erase EEPROM_Mass_Erase for MDR32F1QI evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2024 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V2.0.1
  * @date    31/10/2024
  * @brief   Description of the EEPROM MassErase Example.
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

This example shows how to use EEPROM.
    - Erase main and information memory banks
    - Blank check main and information memory banks
    - Word program and verification main memory bank
    - Word program and verification information memory bank
    - Erase main memory bank only
    - Blank check main memory bank
    - Verification information memory bank
    - Word program and verification main memory bank
    - Erase main and information memory banks
    - Blank check main and information memory banks


@par Directory contains:

    - main.c              Main program


@par Hardware and Software environment:

    - This example is intended to run on MDR32F1QI eval board with MDR32F1QIT1 microcontroller.
    - VD7 (LED0), VD8 (LED1), VD9 (LED2), VD10 (LED3) are connected to PD.7, PD.8, PD.9, PD.10 pins, respectively.


@par How to use.

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - Add main.c file.
    - Add the required files from "Library" folder:
        MDR32FxQI_config.h
        MDR32FxQI_port.c
        MDR32FxQI_rst_clk.c
        MDR32FxQI_eeprom.c
    - Edit the MDR32FxQI_config.h to set appropriate run-time parameter checking level.
    - To allocate all code in RAM edit appropriate linker options.
      Place EEPROM functions in RAM by defining section "EXECUTABLE_MEMORY_SECTION" as RAM section.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.


 * <h3><center>&copy; COPYRIGHT 2024 Milandr</center></h3>
 */


