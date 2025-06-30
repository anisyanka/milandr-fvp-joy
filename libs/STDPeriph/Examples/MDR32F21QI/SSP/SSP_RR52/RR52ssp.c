/**
 ******************************************************************************
 * @file    RR52ssp.c
 * @author  Milandr Application Team
 * @version V1.0.0
 * @date    26/05/2023
 * @brief   This file contains all the functions prototypes for the SSP 1636RR52
 *          firmware library.
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
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/

#include "RR52ssp.h"
#include "MDR32F2xQI_rst_clk.h"
#include "MDR32F2xQI_port.h"
#include "MDR32F2xQI_ssp.h"

/** @addtogroup __MDR32F2xQI_StdPeriph_Examples MDR32F2xQI StdPeriph Examples
 *  @{
 */

/** @addtogroup __K1986VK234_EVAL K1986VK234 Evaluation Board
 *  @{
 */

/** @addtogroup SSP_RR52_234 SSP RR52 example for K1986VK234 evaluation board
 *  @{
 */

/** @addtogroup SSP_RR52_234_DRIVER SSP RR52 driver for K1986VK234 evaluation board
 *  @{
 */

#define nCS_ACTIVE     PORT_ResetBits(MDR_PORTB, PORT_PIN_12)
#define nCS_NOT_ACTIVE PORT_SetBits(MDR_PORTB, PORT_PIN_12)

/** @addtogroup SSP_RR52_234_Exported_Functions SSP RR52 Exported Function
 * @{
 */

/**
 * @brief  Configures SSP pins to work with on-board Flash memory 1636RR52.
 * @param  None.
 * @retval None.
 */
void RR52SSP_PortConfig(void)
{
    PORT_InitTypeDef PORT_InitStructure;

    RST_CLK_PCLKCmd((RST_CLK_PCLK_PORTA | RST_CLK_PCLK_PORTB), ENABLE);

    PORT_DeInit(MDR_PORTA);
    PORT_DeInit(MDR_PORTB);

    /*Init structure*/
    PORT_StructInit(&PORT_InitStructure);

    /* Configure SSP pins: FSS, CLK, RXD, TXD */
    PORT_InitStructure.PORT_Pin      = (PORT_PIN_15 | PORT_PIN_14 | PORT_PIN_13);
    PORT_InitStructure.PORT_Function = PORT_FUNCTION_MAIN;
    PORT_InitStructure.PORT_Mode     = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_Power    = PORT_POWER_SPEED_FAST;
    PORT_Init(MDR_PORTA, &PORT_InitStructure);

    PORT_InitStructure.PORT_Pin       = (PORT_PIN_12);
    PORT_InitStructure.PORT_Function  = PORT_FUNCTION_PORT;
    PORT_InitStructure.PORT_Direction = PORT_DIRECTION_OUT;
    PORT_InitStructure.PORT_Power     = PORT_POWER_SPEED_FAST;
    PORT_Init(MDR_PORTB, &PORT_InitStructure);

    PORT_SetBits(MDR_PORTB, PORT_PIN_12);
}

/**
 * @brief  Configures SSP to work with on-board Flash memory 1636RR52.
 * @param  None.
 * @retval None.
 */
void RR52SSP_SSPConfig(void)
{
    SSP_InitTypeDef SSP_InitStructure;

    RST_CLK_PCLKCmd(RST_CLK_PCLK_SSP, ENABLE);

    /* Reset all SSP settings */
    SSP_DeInit();

    RST_CLK_PER1_C2_SetPrescaler(RST_CLK_PER1_C2_SSP, RST_CLK_PER1_PRESCALER_DIV_4);
    RST_CLK_PER1_C2_Cmd(RST_CLK_PER1_C2_SSP, ENABLE);

    /* SSP MASTER configuration */
    SSP_StructInit(&SSP_InitStructure);

    SSP_InitStructure.SSP_SCR           = 0x1;
    SSP_InitStructure.SSP_CPSDVSR       = 4;
    SSP_InitStructure.SSP_Mode          = SSP_MODE_MASTER;
    SSP_InitStructure.SSP_WordLength    = SSP_WORD_LENGTH_8BIT;
    SSP_InitStructure.SSP_ClockPhase    = SSP_CLOCK_PHASE_1EDGE;
    SSP_InitStructure.SSP_ClockPolarity = SSP_CLOCK_POLARITY_LOW;
    SSP_InitStructure.SSP_FrameFormat   = SSP_FRAME_FORMAT_SPI;
    SSP_Init(&SSP_InitStructure);

    /* Enable SSP */
    SSP_Cmd(ENABLE);
}

/**
 * @brief  Memory sector unlock command.
 * @param  Address: sector address.
 * @retval None.
 */
void RR52SSP_Unlock(uint32_t Address)
{
    uint32_t Cnt;

    RR52SSP_WriteEnable();

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }

    RR52SSP_CommandRXTX_8bit(0x39);
    RR52SSP_CommandRXTX_8bit((uint8_t)(Address >> 16) & 0xFF);
    RR52SSP_CommandRXTX_8bit((uint8_t)(Address >> 8) & 0xFF);
    RR52SSP_CommandRXTX_8bit((uint8_t)(Address)&0xFF);

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_NOT_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }
}

/**
 * @brief  Write enable command.
 * @param  None.
 * @retval None.
 */
void RR52SSP_WriteEnable(void)
{
    uint32_t Cnt;

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }

    RR52SSP_CommandRXTX_8bit(0x06);

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_NOT_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }
}

/**
 * @brief  Read sector protection register.
 * @param  Address: sector address.
 * @retval Protection state.
 */
uint32_t RR52SSP_ReadUnlock(uint32_t Address)
{
    uint32_t Cnt;
    uint8_t  Response;

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }

    RR52SSP_CommandRXTX_8bit(0x3C);
    RR52SSP_CommandRXTX_8bit((uint8_t)(Address >> 16) & 0xFF);
    RR52SSP_CommandRXTX_8bit((uint8_t)(Address >> 8) & 0xFF);
    RR52SSP_CommandRXTX_8bit((uint8_t)(Address)&0xFF);
    Response = RR52SSP_CommandRXTX_8bit(0x00);

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_NOT_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }

    return Response;
}

/**
 * @brief  Read status register
 * @param  None.
 * @retval Status register.
 */
uint8_t RR52SSP_ReadStat(void)
{
    uint32_t Cnt;
    uint8_t  Response;

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }

    RR52SSP_CommandRXTX_8bit(0x05);
    RR52SSP_CommandRXTX_8bit(0x00);
    RR52SSP_CommandRXTX_8bit(0x00);
    Response = RR52SSP_CommandRXTX_8bit(0x00);

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_NOT_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }

    return Response;
}

/**
 * @brief  Write status register.
 * @param  SPRL_Value - @ref BitStatus - SPRL value.
 * @param  RSTE_Value - @ref BitStatus - RSTE value.
 * @retval None.
 */
void RR52SSP_WriteStat(BitStatus SPRL_Value, BitStatus RSTE_Value)
{
    uint32_t Cnt;

    uint8_t Status = 0;

    if (SPRL_Value == SET)
        Status |= SPRL;
    else
        Status &= ~SPRL;

    if (RSTE_Value == SET)
        Status |= RSTE;
    else
        Status &= ~RSTE;

    RR52SSP_WriteEnable();

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }

    RR52SSP_CommandRXTX_8bit(0x01); // command
    RR52SSP_CommandRXTX_8bit(Status);

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_NOT_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }
}

/**
 * @brief  Memory mass erase.
 * @param  None.
 * @retval None.
 */
void RR52SSP_MassErase(void)
{
    uint32_t Cnt;

    RR52SSP_WriteEnable();

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }

    RR52SSP_CommandRXTX_8bit(0x60);

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_NOT_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }
}

/**
 * @brief  Erase sector.
 * @param  Address: sector address.
 * @retval None.
 */
void RR52SSP_SectorErase(uint32_t Address)
{
    uint32_t Cnt;

    RR52SSP_WriteEnable();

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }

    RR52SSP_CommandRXTX_8bit(0xD8);
    RR52SSP_CommandRXTX_8bit(Address >> 16);
    RR52SSP_CommandRXTX_8bit(Address >> 8);
    RR52SSP_CommandRXTX_8bit(Address);

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_NOT_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }
}

/**
 * @brief  Read ID.
 * @param  None.
 * @retval ID.
 */
uint16_t RR52SSP_ReadID(void)
{
    uint32_t Cnt;

    uint16_t id_m = 0x00, id_d = 0x00;

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }

    RR52SSP_CommandRXTX_8bit(0x9F);
    id_m = RR52SSP_CommandRXTX_8bit(0x00);
    id_d = RR52SSP_CommandRXTX_8bit(0x00);

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_NOT_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }

    return ((id_m << 8) | id_d);
}

/**
 * @brief  Write byte.
 * @param  Address: memory address.
 * @param  Data: data to write.
 * @retval None.
 */
void RR52SSP_WriteByte(uint32_t Address, uint8_t Data)
{
    uint32_t Cnt;

    RR52SSP_WriteEnable();

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }

    RR52SSP_CommandRXTX_8bit(0x02);
    RR52SSP_CommandRXTX_8bit(Address >> 16);
    RR52SSP_CommandRXTX_8bit(Address >> 8);
    RR52SSP_CommandRXTX_8bit(Address);
    RR52SSP_CommandRXTX_8bit(Address);

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_NOT_ACTIVE;
    for (Cnt = 0; Cnt < 125; Cnt++) { }
}

/**
 * @brief  Read byte.
 * @param  Address: memory address.
 * @retval Data.
 */
uint8_t RR52SSP_ReadByte(uint32_t Address)
{
    uint32_t Cnt;

    uint8_t Data;

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_ACTIVE;
    for (Cnt = 0; Cnt < 5; Cnt++) { }

    RR52SSP_CommandRXTX_8bit(0x03);
    RR52SSP_CommandRXTX_8bit(Address >> 16);
    RR52SSP_CommandRXTX_8bit(Address >> 8);
    RR52SSP_CommandRXTX_8bit(Address);
    Data = RR52SSP_CommandRXTX_8bit(0x00);

    for (Cnt = 0; Cnt < 5; Cnt++) { }
    nCS_NOT_ACTIVE;
    for (Cnt = 0; Cnt < 125; Cnt++) { }

    return Data;
}

/**
 * @brief  Send and receive data via SPI.
 * @param  data: Data to send.
 * @retval Received data.
 */
uint8_t RR52SSP_CommandRXTX_8bit(uint8_t data)
{
    while (!(MDR_SSP->SR & SSP_FLAG_TFE)) { }
    MDR_SSP->DR = data;

    while (!(MDR_SSP->SR & SSP_FLAG_RNE)) { }
    return (MDR_SSP->DR);
}

/** @} */ /* End of group SSP_RR52_234_Exported_Functions */

/** @} */ /* End of group SSP_RR52_234_DRIVER */

/** @} */ /* End of group SSP_RR52_234 */

/** @} */ /* End of group __K1986VK234_EVAL */

/** @} */ /* End of group __MDR32F2xQI_StdPeriph_Examples */

/*********************** (C) COPYRIGHT 2024 Milandr ****************************
 *
 * END OF FILE RR52ssp.c */


