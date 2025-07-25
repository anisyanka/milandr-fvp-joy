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
  * <h2><center>&copy; COPYRIGHT 2024 Milandr</center></h2>
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

/** @addtogroup CAN_LoopBack_RTR_MDR32F1QI CAN_LoopBack_RTR
  * @{
  */

/* Private define ------------------------------------------------------------*/
// #define CONFIG_CAN_PINS

#define LED1            PORT_Pin_10
#define LED2            PORT_Pin_11

/* Private variables ---------------------------------------------------------*/
__IO uint32_t ret = 0; /* for return of the interrupt handling */
__IO uint32_t rx_buf = 0;
__IO uint32_t rtr_request_buf = 1;
__IO uint32_t rtr_reply_buf = 2;
volatile ErrorStatus TestRx;
PORT_InitTypeDef PORT_InitStructure;

/* Private function prototypes -----------------------------------------------*/
ErrorStatus CAN_Interrupt(void);
void LEDOn(uint32_t LED_Num);
void LEDOff(uint32_t LED_Num);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  CAN1 interrupt handler.
  * @param  None
  * @retval None
  */
void CAN1_IRQHandler(void)
{
    CAN_RxMsgTypeDef RxMessage;

    CAN_GetRawReceivedData(MDR_CAN1, rx_buf, &RxMessage);

    if ((RxMessage.Rx_Header.ID == 0x00005678) && (RxMessage.Rx_Header.IDE == CAN_ID_EXT)
        && (RxMessage.Rx_Header.DLC == 2) && (RxMessage.Data[0] == 0x7777))
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }
    CAN_ITClearRxTxPendingBit(MDR_CAN1, rx_buf, CAN_STATUS_RX_READY);
}

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

    /* CAN transmit at 500Kb/s and receive by interrupt in loopback mode */
    TestRx = CAN_Interrupt();
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
  * @brief  Configures the CAN, transmit and receive using interrupt.
  * @param  None
  * @retval SUCCESS if the reception is well done, ERROR in other case
  */
ErrorStatus CAN_Interrupt(void)
{
    CAN_InitTypeDef  sCAN;
    CAN_RTRReplyTypeDef RTRReply;
    CAN_RTRMessageTypeDef RTRMes;
    CAN_FilterInitTypeDef Filter;
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
    sCAN.CAN_SEG1 = CAN_SEG1_Mul_3TQ;
    sCAN.CAN_SEG2 = CAN_SEG2_Mul_2TQ;
    sCAN.CAN_SJW  = CAN_SJW_Mul_2TQ;
    sCAN.CAN_SB   = CAN_SB_3_SAMPLE;
    sCAN.CAN_BRP  = 1;
    CAN_Init(MDR_CAN1, &sCAN);

    CAN_Cmd(MDR_CAN1, ENABLE);

    /* Enable CAN1 interrupt */
    NVIC_EnableIRQ(CAN1_IRQn);
    /* Enable CAN1 GLB_INT and RX_INT interrupts */
    CAN_ITConfig(MDR_CAN1, (CAN_IT_GLBINTEN | CAN_IT_RXINTEN), ENABLE);

    /* Enable CAN1 interrupt from receive buffer */
    CAN_RxITConfig(MDR_CAN1, (1 << rx_buf), ENABLE);

    /* config buffer for receive RTR reply*/
    CAN_Receive(MDR_CAN1, rx_buf, DISABLE);
    Filter.Filter_ID = 0x00005678;
    Filter.Mask_ID   = 0x0000FFFF;
    CAN_FilterInit(MDR_CAN1, rx_buf, &Filter);

    /* config buffer for transmit RTR reply*/
    RTRReply.ID      = 0x00005678;
    RTRReply.IDE     = CAN_ID_EXT;
    RTRReply.DLC     = 2;
    RTRReply.Data[0] = 0x7777;
    RTRReply.Data[1] = 0x0;

    CAN_RTRReply(MDR_CAN1, rtr_reply_buf, &RTRReply);

    Filter.Filter_ID = 0x00001234;
    Filter.Mask_ID   = 0x0000FFFF;
    CAN_FilterInit(MDR_CAN1, rtr_reply_buf, &Filter);

    /* config buffer for transmit RTR request*/
    RTRMes.ID      = 0x00001234;
    RTRMes.IDE     = CAN_ID_EXT;
    RTRMes.PRIOR_0 = DISABLE;

    /* transmit RTR*/
    CAN_SendRTR(MDR_CAN1, rtr_request_buf, &RTRMes);

    /* initialize the value that will be returned */
    ret = 0xFF;

    /* receive message with interrupt handling */
    i = 0;
    while((ret == 0xFF) && (i < 0xFFF))
    {
        i++;
    }

    if (i == 0xFFF)
    {
        ret = 0;
    }

    /* disable interrupt handling */
    CAN_ITConfig(MDR_CAN1, (CAN_IT_GLBINTEN | CAN_IT_RXINTEN), DISABLE);

    return (ErrorStatus)ret;
}

/**
  * @brief  Turns selected LED On.
  * @param  LED_Num: Specifies the Led to be set on.
  *         This parameter can be one of following parameters:
  *            @arg LED1
  *            @arg LED2
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

/** @} */ /* End of group CAN_LoopBack_MDR32F1QI */

/** @} */ /* End of group __MDR32F1QI_EVAL */

/** @} */ /* End of group __MDR32FxQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2024 Milandr *******************************/

/* END OF FILE main.c */


