/**
 ******************************************************************************
 * @file    main.c
 * @author  Milandr Application Team
 * @version V0.1.0
 * @date    08/04/2024
 * @brief   Main program body.
 ******************************************************************************
 * THE PRESENT FIRMWARE IS FOR GUIDANCE ONLY. IT AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING MILANDR'S PRODUCTS IN ORDER TO FACILITATE
 * THE USE AND SAVE TIME. MILANDR SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES RESULTING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR A USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN THEIR PRODUCTS.
 */

/* Includes ------------------------------------------------------------------*/
#include "MDR32VF0xI_config.h"
#include "MDR32VF0xI_rst_clk.h"
#include "MDR32VF0xI_port.h"

/* Private define ------------------------------------------------------------*/
#define LED_PORT        MDR_PORTD
#define LED_PORT_CLK    RST_CLK_PCLK_PORTD
#define LED_PIN_1       PORT_PIN_0
#define LED_PIN_2       PORT_PIN_1
#define LED_PIN_3       PORT_PIN_2
#define LED_PIN_4       PORT_PIN_3

/* Private functions ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;

uint32_t DelayCnt = 0;

/* Main function */
int main()
{
    RST_CLK_DeInit();
    RST_CLK_HCLK_ClkSelection(RST_CLK_CPU_HCLK_CLK_SRC_HSI);

    RST_CLK_PCLKCmd(LED_PORT_CLK, ENABLE);

    PORT_StructInit(&PORT_InitStructure);
    PORT_InitStructure.PORT_Pin       = (LED_PIN_1 | LED_PIN_2 | LED_PIN_3 | LED_PIN_4);
    PORT_InitStructure.PORT_Direction = PORT_DIRECTION_OUT;
    PORT_InitStructure.PORT_Function  = PORT_FUNCTION_PORT;
    PORT_InitStructure.PORT_Mode      = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_Power     = PORT_POWER_NOMINAL_UPTO_2mA;
    PORT_Init(LED_PORT, &PORT_InitStructure);

    PORT_SetBits(LED_PORT, (LED_PIN_1 | LED_PIN_2 | LED_PIN_3 | LED_PIN_4));

    while (1) {
        if (DelayCnt++ >= 0x00010000) {
            DelayCnt = 0;
            PORT_ToggleBit(LED_PORT, (LED_PIN_1 | LED_PIN_2 | LED_PIN_3 | LED_PIN_4));
        }
    }
}

