/**
 ******************************************************************************
 * @file   fatfs.c
 * @brief  Code for fatfs applications
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

#include "fatfs.h"
#include "utils.h"

uint8_t              retSD;     /* Return value for SD */
__ALIGNED_32__ char  SDPath[4]; /* SD logical drive path */
__ALIGNED_32__ FATFS SDFatFS;   /* File system object for SD logical drive */
__ALIGNED_32__ FIL   SDFile;    /* File object for SD */

void MX_FATFS_Init(void) {
    /*## FatFS: Link the SD driver ###########################*/
    retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
}

/**
 * @brief  Gets Time from RTC
 * @param  None
 * @retval Time in DWORD
 */
DWORD get_fattime(void) {
    return 0;
}
