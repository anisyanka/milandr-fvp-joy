  /**
  @addtogroup SSP_RR52_234 SSP RR52 example for K1986VK234 evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2024 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    25/05/2023
  * @brief   Description of the SSP RR52 Example.
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

A demonstration of interaction with 1636PP52 memory using the SSP module.
The memory chip ID is read, all sectors are unlocked, the entire chip is erased and data is written.
The recorded data is checked.



@par Directory contains:

    - main.c        Main program
    - RR52ssp.c     Functions for interacting with 1636RR52.
    - RR52ssp.h     Functions prototypes for interacting with 1636RR52.

@par Hardware and Software environment:

    - This example is intended to run on K1986VK234 eval board with MDR32F21QI microcontroller.


@par How to use

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - Add main.c file.
    - Add RR52ssp.c (RR52ssp.h).
    - Add the required files from "Libraries" folder:
        MDR32F2xQI_config.h
        MDR32F2xQI_rst_clk.c
        MDR32F2xQI_port.c
        MDR32F2xQI_ssp.c
    - Edit the MDR32F2xQI_config.h to set appropriate run-time parameter checking level.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.

 * <h3><center>&copy; COPYRIGHT 2024 Milandr</center></h3>
 */


