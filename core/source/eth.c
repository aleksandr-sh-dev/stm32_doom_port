/**
 ******************************************************************************
 * @file    eth.c
 * @brief   This file provides code for the configuration
 *          of the ETH instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "eth.h"
#include "string.h"

#if defined(__ICCARM__) /*!< IAR Compiler */

#pragma location = 0x2004c000
ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
#pragma location = 0x2004c0a0
ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

#elif defined(__CC_ARM) /* MDK ARM Compiler */

__attribute__((at(0x2004c000))) ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
__attribute__((at(0x2004c0a0))) ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

#elif defined(__GNUC__) /* GNU Compiler */

ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT]
    __attribute__((section(".RxDecripSection"))); /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT]
    __attribute__((section(".TxDecripSection"))); /* Ethernet Tx DMA Descriptors */

#endif
ETH_BufferTypeDef  Txbuffer[ETH_TX_DESC_CNT * 2U];
ETH_TxPacketConfig TxConfig;

ETH_HandleTypeDef heth;

/* ETH init function */
void MX_ETH_Init(void) {
    static uint8_t MACAddr[6];
    heth.Instance            = ETH;
    MACAddr[0]               = 0x00;
    MACAddr[1]               = 0x80;
    MACAddr[2]               = 0xE1;
    MACAddr[3]               = 0x00;
    MACAddr[4]               = 0x00;
    MACAddr[5]               = 0x00;
    heth.Init.MACAddr        = &MACAddr[0];
    heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
    heth.Init.TxDesc         = DMATxDscrTab;
    heth.Init.RxDesc         = DMARxDscrTab;
    heth.Init.RxBuffLen      = 1524;

    if(HAL_ETH_Init(&heth) != HAL_OK) {
        Error_Handler();
    }

    memset(&TxConfig, 0, sizeof(ETH_TxPacketConfig));
    TxConfig.Attributes   = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
    TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
    TxConfig.CRCPadCtrl   = ETH_CRC_PAD_INSERT;
}

void HAL_ETH_MspInit(ETH_HandleTypeDef *ethHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(ethHandle->Instance == ETH) {
        /* ETH clock enable */
        __HAL_RCC_ETH_CLK_ENABLE();

        __HAL_RCC_GPIOG_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**ETH GPIO Configuration
        PG14     ------> ETH_TXD1
        PG13     ------> ETH_TXD0
        PG11     ------> ETH_TX_EN
        PC1     ------> ETH_MDC
        PA1     ------> ETH_REF_CLK
        PC4     ------> ETH_RXD0
        PA2     ------> ETH_MDIO
        PC5     ------> ETH_RXD1
        PA7     ------> ETH_CRS_DV
        */
        GPIO_InitStruct.Pin       = RMII_TXD1_Pin | RMII_TXD0_Pin | RMII_TX_EN_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = RMII_MDC_Pin | RMII_RXD0_Pin | RMII_RXD1_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = RMII_REF_CLK_Pin | RMII_MDIO_Pin | RMII_CRS_DV_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

void HAL_ETH_MspDeInit(ETH_HandleTypeDef *ethHandle) {
    if(ethHandle->Instance == ETH) {
        /* Peripheral clock disable */
        __HAL_RCC_ETH_CLK_DISABLE();

        /**ETH GPIO Configuration
        PG14     ------> ETH_TXD1
        PG13     ------> ETH_TXD0
        PG11     ------> ETH_TX_EN
        PC1     ------> ETH_MDC
        PA1     ------> ETH_REF_CLK
        PC4     ------> ETH_RXD0
        PA2     ------> ETH_MDIO
        PC5     ------> ETH_RXD1
        PA7     ------> ETH_CRS_DV
        */
        HAL_GPIO_DeInit(GPIOG, RMII_TXD1_Pin | RMII_TXD0_Pin | RMII_TX_EN_Pin);
        HAL_GPIO_DeInit(GPIOC, RMII_MDC_Pin | RMII_RXD0_Pin | RMII_RXD1_Pin);
        HAL_GPIO_DeInit(GPIOA, RMII_REF_CLK_Pin | RMII_MDIO_Pin | RMII_CRS_DV_Pin);
    }
}
