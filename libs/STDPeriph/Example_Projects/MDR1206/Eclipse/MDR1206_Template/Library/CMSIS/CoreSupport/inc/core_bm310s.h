/**
 *******************************************************************************
 * @file    core_bm310s.h
 * @author  Milandr Application Team
 * @version V0.1.0
 * @date    26/03/2024
 * @brief   CMSIS BM-310S Core Peripheral Access Layer Header File.
 *******************************************************************************
 * <br><br>
 *
 * THE PRESENT FIRMWARE IS FOR GUIDANCE ONLY. IT AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING MILANDR'S PRODUCTS IN ORDER TO FACILITATE
 * THE USE AND SAVE TIME. MILANDR SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES RESULTING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR A USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2024 Milandr</center></h2>
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CORE_BM310S
#define CORE_BM310S

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @addtogroup CORE_SUPPORT Core Support
 * @{
 */

/** @addtogroup CORE_BM310S BM-310S Core
 * @{
 */

/** @defgroup CORE_BM310S_Peripheral_Units BM310S Peripheral Units
 * @{
 */

#if defined(CLINT_PRESENT) && (CLINT_PRESENT == 1)
#include "core_clint.h"
#endif

#if defined(PLIC_PRESENT) && (PLIC_PRESENT == 1)
#include "core_plic.h"
#endif

#if defined(CLIC_PRESENT) && (CLIC_PRESENT == 1)
#include "core_clic.h"
#endif

#include "core_csr.h"

#if defined(CORE_EXTENSION_XGOST) && (CORE_EXTENSION_XGOST == 1)
#include "core_extension_xgost.h"
#endif

/** @} */ /* End of group CORE_BM310S_Peripheral_Units */

/** @} */ /* End of the group CORE_BM310S */

/** @} */ /* End of the group CORE_SUPPORT */

#ifdef __cplusplus
} // extern "C" block end
#endif /* __cplusplus */

#endif /* CORE_BM310S */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE core_bm310s.h */

