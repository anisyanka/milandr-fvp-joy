/**
  ******************************************************************************
  * @file    MIL_STD_1553_BusController.h
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
  ******************************************************************************
  * FILE MIL_STD_1553_BusController.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MIL_STD_1553_BUSCONTROLLER_H_
#define MIL_STD_1553_BUSCONTROLLER_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
    READ_DATA,
    WRITE_DATA,
    REQUEST_LAST_COMMAND,
    REQUEST_STATUS_WORD
} MIL_STD_1553_Request;

#ifdef __cplusplus
}
#endif

#endif /* MIL_STD_1553_BUSCONTROLLER_H_ */

/******************* (C) COPYRIGHT 2024 Milandr ********************************
*
* END OF FILE MIL_STD_1553_BusController.h */


