/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V2.0.3
  * @date    26/06/2023
  * @brief   This file contains an example of using the interface Ethernet.
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
  * FILE main.c
  */

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "MDR32FxQI_rst_clk.h"
#include "MDR32FxQI_eeprom.h"
#include "MDR32FxQI_bkp.h"
#include "MDR32FxQI_port.h"
#include "MDR32FxQI_utils.h"
#include "MDR32FxQI_eth.h"
#include "MDR32FxQI_timer.h"
#include "tcpip.h"

/** @addtogroup __MDR32FxQI_StdPeriph_Examples MDR32FxQI StdPeriph Examples
  * @{
  */

/** @addtogroup __MDR32F1QI_EVAL MDR32F1QI Evaluation Board
  * @{
  */

/** @addtogroup Ethernet_IT Ethernet
  * @{
  */

/* Private define ------------------------------------------------------------*/
#define ONE_WORD_TIME_RECEIVING_mkS     20

/* Private macro -------------------------------------------------------------*/
#if defined (_USE_DEBUG_UART_)
    #define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
    #define DEBUG_PRINTF(...)
#endif /* #if defined _USE_DEBUG_UART_ */

/* Private variables ---------------------------------------------------------*/
static PORT_InitTypeDef PORT_InitStructure;

/* Private function prototypes -----------------------------------------------*/
void ClockConfigure(void);

extern uint32_t InputFrame[1514/4];
void ETH_InputPacketHandler(MDR_ETHERNET_TypeDef * ETHERNETx);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
#if defined (_USE_DEBUG_UART_)
    ErrorStatus Status;
#endif
    static ETH_InitTypeDef  ETH_InitStruct;

    /* Configure CPU clock */
    ClockConfigure();

    SystemCoreClockUpdate();

#if defined (_USE_DEBUG_UART_)
    Status = STDIO_Init();
    if (Status == ERROR)
    {
        while (1) { }
    }
#endif /* #if defined (_USE_DEBUG_UART_) */

    /* Enable peripheral clocks */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);
    /* Reset PORTB settings */
    PORT_DeInit(MDR_PORTB);

    /* LEDs Port Init */
    /* Configure PORTB pins 2,3,4,5,6,7 */
    PORT_StructInit(&PORT_InitStructure);
    PORT_InitStructure.PORT_Pin   = (PORT_Pin_2 | PORT_Pin_3 | PORT_Pin_4 |
                                     PORT_Pin_5 | PORT_Pin_6 | PORT_Pin_7);
    PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_OE    = PORT_OE_OUT;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;
    PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
    PORT_InitStructure.PORT_PD    = PORT_PD_DRIVER;
    PORT_Init(MDR_PORTB, &PORT_InitStructure);

    DEBUG_PRINTF("Init LEDs PORT ... Ok\r\n");

    /* Reset ehernet clock settings */
    ETH_ClockDeInit();

    RST_CLK_PCLKcmd(RST_CLK_PCLK_DMA, ENABLE);

    /* Enable HSE2 oscillator */
    RST_CLK_HSE2config(RST_CLK_HSE2_ON);
    while(RST_CLK_HSE2status() == ERROR) {}

    /* Config PHY clock */
    ETH_PHY_ClockConfig(ETH_PHY_CLOCK_SOURCE_HSE2, ETH_PHY_HCLKdiv1);

    /* Init the BRG ETHERNET */
    ETH_BRGInit(ETH_HCLKdiv1);

    /* Enable the ETHERNET clock */
    ETH_ClockCMD(ETH_CLK1, ENABLE);

    /* Reset to default ethernet settings */
    ETH_DeInit(MDR_ETHERNET1);

    /* Init ETH_InitStruct members with its default value */
    ETH_StructInit((ETH_InitTypeDef * ) &ETH_InitStruct);
    /* Set the speed of the chennel */
    ETH_InitStruct.ETH_PHY_Mode        = ETH_PHY_MODE_AutoNegotiation;
    ETH_InitStruct.ETH_Transmitter_RST = SET;
    ETH_InitStruct.ETH_Receiver_RST    = SET;
    /* Set the buffer mode */
    ETH_InitStruct.ETH_Buffer_Mode     = ETH_BUFFER_MODE_LINEAR;

    ETH_InitStruct.ETH_Source_Addr_HASH_Filter = DISABLE;

    /* Set the MAC address. */
    ETH_InitStruct.ETH_MAC_Address[2] = (MAC_0 << 8) | MAC_1;
    ETH_InitStruct.ETH_MAC_Address[1] = (MAC_2 << 8) | MAC_3;
    ETH_InitStruct.ETH_MAC_Address[0] = (MAC_4 << 8) | MAC_5;

    /* Set the buffer size of transmitter and receiver */
    ETH_InitStruct.ETH_Dilimiter = 0x1000;

    /* Init the ETHERNET 1 */
    ETH_Init(MDR_ETHERNET1, (ETH_InitTypeDef *) &ETH_InitStruct);

    /* Enable PHY module */
    ETH_PHYCmd(MDR_ETHERNET1, ENABLE);

    TCPLowLevelInit();

    /* Start the ETHERNET1 */
    ETH_Start(MDR_ETHERNET1);

    if(ETH_InitStruct.ETH_PHY_Mode == ETH_PHY_MODE_AutoNegotiation)
    {
        /* Wait for autonegotiation to complete */
        while(ETH_GetPHYAutonegStatus(MDR_ETHERNET1) != SET) {}
    }
    /* Wait for link up */
    while(ETH_GetPHYStatus(MDR_ETHERNET1, ETH_PHY_FLAG_LINK) != RESET) {};

    /* Infinite loop */
    while(1)
    {
        ETH_InputPacketHandler(MDR_ETHERNET1);
        DoNetworkStuff(MDR_ETHERNET1);
    }
}

void ETH_InputPacketHandler(MDR_ETHERNET_TypeDef * ETHERNETx)
{
    volatile uint16_t status_reg;
    ETH_StatusPacketReceptionTypeDef ETH_StatusPacketReceptionStruct;

    /* Check that the packet is received */
    status_reg = ETH_GetMACITStatusRegister(ETHERNETx);

    if (ETH_GetRxFrameCount(ETHERNETx) != 0)
    {
        ETH_DecrementRxFrameCount(ETHERNETx);
        ETH_StatusPacketReceptionStruct.Status = ETH_ReceivedFrame(ETHERNETx, InputFrame);

        if (ETH_StatusPacketReceptionStruct.Fields.UCA)
        {
            ProcessEthIAFrame(InputFrame, ETH_StatusPacketReceptionStruct.Fields.Length);
        }
        if (ETH_StatusPacketReceptionStruct.Fields.BCA)
        {
            ProcessEthBroadcastFrame(InputFrame, ETH_StatusPacketReceptionStruct.Fields.Length);
        }
    }
}


/**
  * @brief  Configure CPU clock.
  * @param  None
  * @retval None
  */
void ClockConfigure(void)
{
    RST_CLK_DeInit();

    /* Enable HSE (High Speed External) clock */
    RST_CLK_HSEconfig(RST_CLK_HSE_ON);
    while(RST_CLK_HSEstatus() == ERROR) {}

    /* Configures the CPU_PLL clock source */
    RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul16);

    /* Enables the CPU_PLL */
    RST_CLK_CPU_PLLcmd(ENABLE);
    while(RST_CLK_CPU_PLLstatus() == ERROR) {}

    /* Select the CPU_PLL output as input for CPU_C3_SEL */
    RST_CLK_CPU_PLLuse(ENABLE);
    /* Set CPUClk Prescaler */
    RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);

    /* Enables the RST_CLK_PCLK_EEPROM */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE);
    /* Sets the code latency value */
    EEPROM_SetLatency(EEPROM_Latency_5);

    /* Enables the RST_CLK_PCLK_BKP */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_BKP, ENABLE);
    /* Setup internal DUcc voltage regulator work mode based on clock frequency */
    BKP_DUccMode(BKP_DUcc_over_80MHz);

    /* Select the CPU clock source */
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
}

/**
  * @brief  Reports the source file name, the source line number
  *         and expression text (if USE_ASSERT_INFO == 2) where
  *         the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @param  expr:
  * @retval None
  */
#if (USE_ASSERT_INFO == 1)
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the source file name and line number.
       Ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while(1) {}
}
#elif (USE_ASSERT_INFO == 2)
void assert_failed(uint8_t* file, uint32_t line, const uint8_t* expr)
{
    /* User can add his own implementation to report the source file name, line number and
       expression text.
       Ex: printf("Wrong parameters value (%s): file %s on line %d\r\n", expr, file, line) */
    DEBUG_PRINTF("Wrong parameters value (%s): file %s on line %d\r\n", expr, file, line);

    /* Infinite loop */
    while(1) {}
}
#endif /* USE_ASSERT_INFO */

/** @} */ /* End of group Ethernet_MDR32F1QI */

/** @} */ /* End of group __MDR32F1QI_EVAL MDR32F1QI Evaluation Board */

/** @} */ /* End of group __MDR32FxQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr ********************************
*
* END OF FILE main.c */


