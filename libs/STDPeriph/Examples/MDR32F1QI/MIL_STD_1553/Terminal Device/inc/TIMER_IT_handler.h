/**
  ******************************************************************************
  * @file    TIMER_IT_handler.h
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    22/09/2021
  * @brief   Header file for user interrupt handler for the TIMER1.
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
#ifndef TIMER_IT_HANDLER_H_
#define TIMER_IT_HANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32FxQI_timer.h"

/* Function prototypes -------------------------------------------------------*/
void My_TIMER1_IRQ_Handler(MDR_TIMER_TypeDef * TIMERx, MDR_MIL_STD_1553_TypeDef * MIL_STD_1553x);

#ifdef __cplusplus
}
#endif

#endif /* TIMER_IT_HANDLER_H_ */

/******************* (C) COPYRIGHT 2024 Milandr ********************************
*
* END OF FILE TIMER_IT_handler.h */


