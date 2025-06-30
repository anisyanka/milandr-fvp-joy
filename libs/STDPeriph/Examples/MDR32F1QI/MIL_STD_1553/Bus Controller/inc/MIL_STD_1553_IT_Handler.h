/**
  ******************************************************************************
  * @file    MIL_STD_1553_IT_Handler.h
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    22/09/2021
  * @brief   This file contains the user defines and typedef and functions
  *          prototypes.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE IS FOR GUIDANCE ONLY. IT AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING MILANDR'S PRODUCTS IN ORDER TO FACILITATE
  * THE USE AND SAVE TIME. MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES RESULTING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR A USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2024 Milandr </center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MIL_STD_1553_IT_HANDLER_H_
#define MIL_STD_1553_IT_HANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup __MDR32FxQI_StdPeriph_Examples MDR32FxQI StdPeriph Examples
  * @{
  */

/** @addtogroup __MDR32F1QI_EVAL MDR32F1QI Evaluation Board
  * @{
  */

/** @addtogroup MIL_STD_1553_Bus_Controller_MDR32F1QI MIL_STD_1553 Bus Controller
 *  @{
 */

/* Includes ------------------------------------------------------------------*/
#include "MDR32FxQI_mil_std_1553.h"


/* Function prototypes -------------------------------------------------------*/
void My_MIL_STD_1553_Handler( MDR_MIL_STD_1553_TypeDef * MIL_STD_1553x );

/** @} */ /* End of MIL_STD_1553_Bus_Controller */

/** @} */ /* End of group ____MDR32F1QI_EVAL MDR32F1QI Evaluation Board */

/** @} */ /* End of group __MDR32FxQI_StdPeriph_Examples */

#ifdef __cplusplus
}
#endif

#endif /* MIL_STD_1553_IT_HANDLER_H_ */

/******************* (C) COPYRIGHT 2024 Milandr ********************************
*
* END OF FILE MIL_STD_1553_IT_Handler.h */


