/**
 ******************************************************************************
 * @file    stm32f7xx_it.c
 * @brief   Interrupt Service Routines.
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
#include "main.h"
#include "stm32f7xx_it.h"
#include "stm32f7508_discovery_sd.h"

/* External variables --------------------------------------------------------*/
extern DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef  hltdc;
extern TIM_HandleTypeDef   htim6;
extern TIM_HandleTypeDef   htim8;
extern SD_HandleTypeDef    uSdHandle;
extern SAI_HandleTypeDef   haudio_out_sai;

/******************************************************************************/
/*           Cortex-M7 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void) {
    while(1) {
    }
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void) {
    while(1) {
    }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void) {
    while(1) {
    }
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void) {
    while(1) {
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void) {
    while(1) {
    }
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void) {
}

/******************************************************************************/
/* STM32F7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f7xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles SDMMC1 global interrupt.
 */
void BSP_SDMMC_IRQHandler(void) {
    HAL_SD_IRQHandler(&uSdHandle);
}

/**
 * @brief This function handles TIM8 update interrupt and TIM13 global interrupt.
 */
void TIM8_UP_TIM13_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim8);
}

/**
 * @brief This function handles TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts.
 */
void TIM6_DAC_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim6);
}

/**
 * @brief  This function handles DMA2 Stream 6 interrupt request.
 * @param  None
 * @retval None
 */
void BSP_SDMMC_DMA_Tx_IRQHandler(void) {
    HAL_DMA_IRQHandler(uSdHandle.hdmatx);
}

/**
 * @brief  This function handles DMA2 Stream 3 interrupt request.
 * @param  None
 * @retval None
 */
void BSP_SDMMC_DMA_Rx_IRQHandler(void) {
    HAL_DMA_IRQHandler(uSdHandle.hdmarx);
}

/**
 * @brief This function handles LTDC global interrupt.
 */
void LTDC_IRQHandler(void) {
    HAL_LTDC_IRQHandler(&hltdc);
}

/**
 * @brief This function handles DMA2D global interrupt.
 */
void DMA2D_IRQHandler(void) {
    HAL_DMA2D_IRQHandler(&hdma2d);
}

void DMA2_Stream4_IRQHandler(void) {
    HAL_DMA_IRQHandler(haudio_out_sai.hdmatx);
}
