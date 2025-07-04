/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V2.1.0
  * @date    27/06/2023
  * @brief   Main program body.
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

/* Includes ------------------------------------------------------------------*/
#include "MDR32FxQI_can.h"
#include "MDR32FxQI_rst_clk.h"
#include "MDR32FxQI_port.h"

/** @addtogroup __MDR32FxQI_StdPeriph_Examples MDR32FxQI StdPeriph Examples
  * @{
  */

/** @addtogroup __MDR32F1QI_EVAL MDR32F1QI Evaluation Board
  * @{
  */

/** @addtogroup CAN_LoopBack_Polling_MDR32F1QI CAN_LoopBack_Polling
  * @{
  */

/* Private define ------------------------------------------------------------*/
// #define CONFIG_CAN_PINS

#define LED1            PORT_Pin_10
#define LED2            PORT_Pin_11

/* Private variables ---------------------------------------------------------*/
__IO uint32_t rx_buf = 0;
__IO uint32_t tx_buf = 1;
volatile ErrorStatus TestRx;
PORT_InitTypeDef PORT_InitStructure;

/* Private function prototypes -----------------------------------------------*/
ErrorStatus CAN_Polling(void);
void LEDOn(uint32_t LED_Num);
void LEDOff(uint32_t LED_Num);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    RST_CLK_DeInit();

    /* Select HSE as CPU_CLK source */
    RST_CLK_HSEconfig(RST_CLK_HSE_ON);
    while(RST_CLK_HSEstatus() == ERROR);
    RST_CLK_CPUclkSelectionC1(RST_CLK_CPU_C1srcHSEdiv1);
    RST_CLK_CPU_PLLuse(DISABLE);
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

    SystemCoreClockUpdate();

    /* Periph clocks enable */
    RST_CLK_PCLKcmd((RST_CLK_PCLK_RST_CLK | RST_CLK_PCLK_PORTD
                    | RST_CLK_PCLK_CAN1), ENABLE);

    /* Reset PORTD settings */
    PORT_DeInit(MDR_PORTD);

    /* Configure PORTD pins 10,11 for output to switch LED on/off */
    PORT_StructInit(&PORT_InitStructure);
    PORT_InitStructure.PORT_Pin   = (PORT_Pin_10 | PORT_Pin_11);
    PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
    PORT_InitStructure.PORT_OE    = PORT_OE_OUT;
    PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;
    PORT_Init(MDR_PORTD, &PORT_InitStructure);

#ifdef CONFIG_CAN_PINS
    /* Enable PORTC clock */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);

    /* Reset PORTC settings */
    PORT_DeInit(MDR_PORTC);

    /* Configure PORTC pin 9 as CAN1_RX */
    PORT_InitStructure.PORT_Pin   = PORT_Pin_9;
    PORT_InitStructure.PORT_FUNC  = PORT_FUNC_OVERRID;
    PORT_InitStructure.PORT_OE    = PORT_OE_IN;
    PORT_Init(MDR_PORTC, &PORT_InitStructure);

    /* Configure PORTC pin 10 as CAN1_TX */
    PORT_InitStructure.PORT_Pin   = PORT_Pin_10;
    PORT_InitStructure.PORT_FUNC  = PORT_FUNC_OVERRID;
    PORT_InitStructure.PORT_OE    = PORT_OE_OUT;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;
    PORT_Init(MDR_PORTC, &PORT_InitStructure);
#endif

    /* CAN transmit at 125Kb/s and receive by polling in loopback mode */
    TestRx = CAN_Polling();
    if (TestRx == ERROR)
    {
        /* Turn on led LED2 */
        LEDOn(LED2);
    }
    else
    {
        /* Turn on led LED1 */
        LEDOn(LED1);
    }

    while(1) {}
}

/**
  * @brief  Configures the CAN, transmit and receive by polling
  * @param  None
  * @retval SUCCESS if the reception is well done, ERROR in other case
  */
ErrorStatus CAN_Polling(void)
{
    CAN_InitTypeDef  sCAN;
    CAN_TxMsgTypeDef TxMsg;
    CAN_RxMsgTypeDef RxMsg;
    uint32_t i = 0;

    /* Set the HCLK division factor = 1 for CAN1*/
    CAN_BRGInit(MDR_CAN1, CAN_HCLKdiv1);

    /* CAN register init */
    CAN_DeInit(MDR_CAN1);

    /* CAN cell init */
    CAN_StructInit(&sCAN);

    sCAN.CAN_ROP  = ENABLE;
    sCAN.CAN_SAP  = ENABLE;
    sCAN.CAN_STM  = ENABLE;
    sCAN.CAN_ROM  = DISABLE;
    sCAN.CAN_PSEG = CAN_PSEG_Mul_2TQ;
    sCAN.CAN_SEG1 = CAN_SEG1_Mul_5TQ;
    sCAN.CAN_SEG2 = CAN_SEG2_Mul_5TQ;
    sCAN.CAN_SJW  = CAN_SJW_Mul_4TQ;
    sCAN.CAN_SB   = CAN_SB_3_SAMPLE;
    sCAN.CAN_BRP  = 4;

    CAN_Init(MDR_CAN1,&sCAN);

    CAN_Cmd(MDR_CAN1, ENABLE);

    /* Disable all CAN1 interrupt */
    CAN_ITConfig(MDR_CAN1, (CAN_IT_GLBINTEN | CAN_IT_RXINTEN | CAN_IT_TXINTEN |
                           CAN_IT_ERRINTEN | CAN_IT_ERROVERINTEN), DISABLE);

    /* Enable CAN1 interrupt from receive buffer */
    CAN_RxITConfig(MDR_CAN1, (1 << rx_buf), ENABLE);
    /* Enable CAN1 interrupt from transmit buffer */
    CAN_TxITConfig(MDR_CAN1, (1 << tx_buf), ENABLE);

    /* Receive buffer enable */
    CAN_Receive(MDR_CAN1, rx_buf, DISABLE);

    /* Transmit */
    TxMsg.IDE     = CAN_ID_EXT;
    TxMsg.DLC     = 0x08;
    TxMsg.PRIOR_0 = DISABLE;
    TxMsg.ID      = 0x12345678;
    TxMsg.Data[1] = 0x01234567;
    TxMsg.Data[0] = 0x89ABCDEF;

    CAN_Transmit(MDR_CAN1, tx_buf, &TxMsg);

    i = 0;
    while(((CAN_GetStatus(MDR_CAN1) & CAN_STATUS_TX_READY) != RESET) && (i != 0xFFF))
    {
        i++;
    }
    CAN_ITClearRxTxPendingBit(MDR_CAN1, tx_buf, CAN_STATUS_TX_READY);

    i = 0;
    while(((CAN_GetStatus(MDR_CAN1) & CAN_STATUS_RX_READY) == RESET) && (i != 0xFFF))
    {
        i++;
    }

    /* Receive */
    CAN_GetRawReceivedData(MDR_CAN1, rx_buf, &RxMsg);

    CAN_ITClearRxTxPendingBit(MDR_CAN1, rx_buf, CAN_STATUS_RX_READY);

    CAN_Cmd(MDR_CAN1, DISABLE);

    if (RxMsg.Rx_Header.IDE != TxMsg.IDE)
    {
        return ERROR;
    }

    if (RxMsg.Rx_Header.DLC != TxMsg.DLC)
    {
        return ERROR;
    }

    if (RxMsg.Rx_Header.ID != TxMsg.ID)
    {
        return ERROR;
    }

    if (RxMsg.Data[1] != TxMsg.Data[1])
    {
        return ERROR;
    }

    if (RxMsg.Data[0] != TxMsg.Data[0])
    {
        return ERROR;
    }
    else
    {
        return SUCCESS;  /* Test Passed */
    }
}

/**
  * @brief  Turns selected LED On.
  * @param  LED_Num: Specifies the Led to be set on.
  *         This parameter can be one of following parameters:
  *             @arg LED1
  *             @arg LED2
 * @retval None
  */
void LEDOn(uint32_t LED_Num)
{
    PORT_SetBits(MDR_PORTD, LED_Num);
}

/**
  * @brief  Turns selected LED Off.
  * @param  LED_Num: Specifies the Led to be set off.
  *         This parameter can be one of following parameters:
  *             @arg LED1
  *             @arg LED2
  * @retval None
  */
void LEDOff(uint32_t LED_Num)
{
    PORT_ResetBits(MDR_PORTD, LED_Num);
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

    /* Infinite loop */
    while(1) {}
}
#endif /* USE_ASSERT_INFO */

/** @} */ /* End of group CAN_LoopBack_Polling_MDR32F1QI */

/** @} */ /* End of group __MDR32F1QI_EVAL */

/** @} */ /* End of group __MDR32FxQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr *******************************/

/* END OF FILE main.c */


