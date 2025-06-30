/**
  ******************************************************************************
  * @file    MDR1206_Interrupts/src/main.c
  * @author  Milandr Application Team
  * @version V0.1.0
  * @date    28/03/2024
  * @brief   Main program body.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR32VF0xI_rst_clk.h"
#include "MDR32VF0xI_port.h"
#include "MDR32VF0xI_timer.h"
#include "system_MDR32VF0xI_it.h"

/* Private define ------------------------------------------------------------*/
/***** Parameters *****/
#define TIMER1_PERIOD           (0x0000FFFF + 1)
#define TIMER2_PERIOD           (0x01000000 + 1)
#define MTIMER_PERIOD           4000000
#define CLINT_SW_PERIOD         0x00012000

/***** LED *****/
#define LED_PORT        MDR_PORTD
#define LED_PORT_CLK    RST_CLK_PCLK_PORTD
#define LED_PIN_0       PORT_PIN_0
#define LED_PIN_1       PORT_PIN_1
#define LED_PIN_2       PORT_PIN_2
#define LED_PIN_3       PORT_PIN_3

/* Private variables ---------------------------------------------------------*/
uint32_t SW_DelayCnt = 0;

/* Private function prototypes -----------------------------------------------*/
void Setup_LED_Port(void);
void Setup_Timer1(void);
void Setup_Timer2(void);
void Setup_MachineTimer(void);
void Setup_SoftwareMachineIRQ(void);

/* Private functions ---------------------------------------------------------*/

/* --- */
#if (CLIC_SYSTEM_INIT_VECTOR_MODE_IRQ == 1)
__interrupt
#endif
__RAMFUNC void MTIP_IRQHandler(void)
{
    CLINT_MTIMER_SetCompareTime(CLINT_MTIMER_GetTime() + MTIMER_PERIOD);
    PORT_ToggleBit(LED_PORT, LED_PIN_3);
}

/* --- */
#if (CLIC_SYSTEM_INIT_VECTOR_MODE_IRQ == 1)
__interrupt
#endif
__RAMFUNC void MSIP_IRQHandler(void)
{
    CLINT_SetSoftwareMachineIRQ(RESET);
    PORT_ToggleBit(LED_PORT, LED_PIN_2);
}

/* --- */
#if (CLIC_SYSTEM_INIT_VECTOR_MODE_IRQ == 1)
__interrupt
#endif
__RAMFUNC void TIMER1_IRQHandler(void)
{
    TIMER_ClearFlags(MDR_TIMER1, TIMER_FLAG_CNT_ZERO);
    PORT_ToggleBit(LED_PORT, LED_PIN_1);
}

/* --- */
#if (CLIC_SYSTEM_INIT_VECTOR_MODE_IRQ == 1)
__interrupt
#endif
__RAMFUNC void TIMER2_IRQHandler(void)
{
    TIMER_ClearFlags(MDR_TIMER2, TIMER_FLAG_CNT_ZERO);
    PORT_ToggleBit(LED_PORT, LED_PIN_0);
}

/* Main Function */
int main()
{
    Setup_LED_Port();
    Setup_Timer1();
    Setup_Timer2();
    Setup_MachineTimer();
    Setup_SoftwareMachineIRQ();

    while (1)
    {
        if (SW_DelayCnt++ >= CLINT_SW_PERIOD)
        {
            SW_DelayCnt = 0;
            SW_DelayCnt++;
            CLINT_SetSoftwareMachineIRQ(SET);
        }
    }
}

/* --- */
void Setup_LED_Port(void)
{
    PORT_InitTypeDef PORT_InitStructure;

    RST_CLK_PCLKCmd(LED_PORT_CLK, ENABLE);

    PORT_StructInit(&PORT_InitStructure);
    PORT_InitStructure.PORT_Pin       = LED_PIN_0 | LED_PIN_1 | LED_PIN_2 | LED_PIN_3;
    PORT_InitStructure.PORT_Mode      = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_Direction = PORT_DIRECTION_OUT;
    PORT_Init(LED_PORT, &PORT_InitStructure);

    PORT_WriteBit(LED_PORT, LED_PIN_0, SET);
    PORT_WriteBit(LED_PORT, LED_PIN_1, SET);
    PORT_WriteBit(LED_PORT, LED_PIN_2, SET);
    PORT_WriteBit(LED_PORT, LED_PIN_3, SET);
}

/* --- */
void Setup_SoftwareMachineIRQ(void)
{
    CLIC_EnableIRQ(MSIP_IRQn);
}

/* --- */
void Setup_MachineTimer(void)
{
    RST_CLK_SystemTimer_SetPrescaler(2);
    
    CLINT_MTIMER_SetCompareTime(CLINT_MTIMER_GetTime() + MTIMER_PERIOD);
    CLIC_EnableIRQ(MTIP_IRQn);
}

/* --- */
void Setup_Timer1(void)
{
    TIMER_CNT_InitTypeDef TIMER_CNT_InitStructure;

    RST_CLK_PCLKCmd(RST_CLK_PCLK_TIMER1, ENABLE);

    RST_CLK_PER1_C2_ClkSelection(RST_CLK_PER1_C2_CLK_SRC_CPU_C1);
    RST_CLK_PER1_C2_SetPrescaler(RST_CLK_PER1_C2_TIM1, RST_CLK_PER1_PRESCALER_DIV_1);
    RST_CLK_PER1_C2_Cmd(RST_CLK_PER1_C2_TIM1, ENABLE);

    /* Reset all TIMER1 settings */
    TIMER_DeInit(MDR_TIMER1);

    /* Initializes the TIMER1 Counter -------------------------------------*/
    TIMER_CNT_StructInit(&TIMER_CNT_InitStructure);
    TIMER_CNT_InitStructure.TIMER_CNT_Prescaler   = 0x000F;
    TIMER_CNT_InitStructure.TIMER_CNT_Period      = TIMER1_PERIOD;
    TIMER_CNT_InitStructure.TIMER_CNT_EventSource = TIMER_CNT_EVNT_SRC_TIM_CLK;
    TIMER_CNT_InitStructure.TIMER_CNT_Mode        = TIMER_CNT_MODE_CLK_FIXED_DIR;
    TIMER_CNT_InitStructure.TIMER_CNT_Direction   = TIMER_CNT_DIR_UP;
    TIMER_CNT_Init(MDR_TIMER1, &TIMER_CNT_InitStructure);

    /* Configure TIMER1 interrupt */
    TIMER_ITConfig(MDR_TIMER1, TIMER_FLAG_CNT_ZERO, ENABLE);
    CLIC_EnableIRQ(TIMER1_IRQn);

    /* Enable TIMER1 */
    TIMER_CNT_Cmd(MDR_TIMER1, ENABLE);
}

/* --- */
void Setup_Timer2(void)
{
    TIMER_CNT_InitTypeDef TIMER_CNT_InitStructure;

    RST_CLK_PCLKCmd(RST_CLK_PCLK_TIMER2, ENABLE);

    RST_CLK_PER1_C2_ClkSelection(RST_CLK_PER1_C2_CLK_SRC_CPU_C1);
    RST_CLK_PER1_C2_SetPrescaler(RST_CLK_PER1_C2_TIM2, RST_CLK_PER1_PRESCALER_DIV_1);
    RST_CLK_PER1_C2_Cmd(RST_CLK_PER1_C2_TIM2, ENABLE);

    /* Reset all TIMER2 settings */
    TIMER_DeInit(MDR_TIMER2);

    /* Initializes the TIMER2 Counter -------------------------------------*/
    TIMER_CNT_StructInit(&TIMER_CNT_InitStructure);
    TIMER_CNT_InitStructure.TIMER_CNT_Prescaler   = 0;
    TIMER_CNT_InitStructure.TIMER_CNT_Period      = TIMER2_PERIOD;
    TIMER_CNT_InitStructure.TIMER_CNT_EventSource = TIMER_CNT_EVNT_SRC_TIM_CLK;
    TIMER_CNT_InitStructure.TIMER_CNT_Mode        = TIMER_CNT_MODE_CLK_FIXED_DIR;
    TIMER_CNT_InitStructure.TIMER_CNT_Direction   = TIMER_CNT_DIR_UP;
    TIMER_CNT_Init(MDR_TIMER2, &TIMER_CNT_InitStructure);

    /* Configure TIMER2 interrupt */
    TIMER_ITConfig(MDR_TIMER2, TIMER_FLAG_CNT_ZERO, ENABLE);
    CLIC_EnableIRQ(TIMER2_IRQn);

    /* Enable TIMER2 */
    TIMER_CNT_Cmd(MDR_TIMER2, ENABLE);
}





/**
 ******************************************************************************
 * @file    main.c
 * @author  Milandr Application Team
 * @version V0.1.0
 * @date    12/02/2024
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
//#include "MDR32VF0xI_config.h"

/* Private functions ---------------------------------------------------------*/

/* Main function */
//int main()
//{
//    while (1) { }
//}

