/**
 ******************************************************************************
 * @file    sai.h
 * @brief   This file contains all the function prototypes for
 *          the sai.c file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SAI_H__
#define __SAI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern SAI_HandleTypeDef hsai_BlockA2;

extern SAI_HandleTypeDef hsai_BlockB2;

void MX_SAI2_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __SAI_H__ */
