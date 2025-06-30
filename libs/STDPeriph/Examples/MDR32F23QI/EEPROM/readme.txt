/**
  @addtogroup EEPROM_214 EEPROM example for K1986VK214 evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2024 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    29/05/2023
  * @brief   Description of the EEPROM Example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  @endverbatim
  @endlatexonly

@par Example Description

This example shows how to use EEPROM.
    - Erase main and information memory banks.
    - Blank check main and information memory banks.
    - Programm and verify main memory bank.
    - Programm and verify information memory bank.
    - Erase main memory bank only.
    - Blank check main memory bank.
    - Verify information memory bank.
    - Programm and verify main memory bank.


@par Directory contains:

    - main.c              Main program


@par Hardware and Software environment:

    - This example is intended to run on K1986VK214 eval board with MDR32F23QI microcontroller.
    - LED0-LED2 are connected to PC14-PC15 pins, respectively.


@par How to use.

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - Add main.c file.
    - Add the required files from "Library" folder:
        MDR32FxQI_config.h
        MDR32FxQI_port.c
        MDR32FxQI_rst_clk.c
        MDR32FxQI_eeprom.c
        MDR32FxQI_utils.c
    - Edit the MDR32F2xQI_config.h to set appropriate run-time parameter checking level.
    - Edit appropriate linker options to allocate code in RAM.
      Place EEPROM functions in RAM by defining section "EXECUTABLE_MEMORY_SECTION" as RAM section.
      For Keil uVision pack-provided scatter file could be used.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.

 * <h3><center>&copy; COPYRIGHT 2024 Milandr</center></h3>
 */


