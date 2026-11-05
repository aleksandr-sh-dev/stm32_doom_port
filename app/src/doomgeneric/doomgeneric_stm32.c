// HAL
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_dma2d.h"
#include "stm32f7xx_hal_ltdc.h"
#include "stm32f7508_discovery_lcd.h"

// FreeRTOS
#include "FreeRTOS.h"

// Doomgeneric STM32 application
#include "doomgeneric_stm32.h"

// Logs
#include "log.h"

#include "fatfs.h"
#include "buttons.h"
#include "gpio.h"

// Doomgeneric lib
#include "m_argv.h"
#include "doomgeneric.h"
#include "i_system.h"
#include "z_zone.h"
#include "i_video.h"

static const char *TAG = "DOOMGENERIC_STM32";

static osThreadId_t         doomgeneric_task_h;
static const osThreadAttr_t doomgeneric_task_attributes = {
    .name       = "doomgeneric_task",
    .stack_size = 2048 * 4,
    .priority   = (osPriority_t)osPriorityAboveNormal,
};

void doomgeneric_task(void *arg) {
    // unused
    (void)arg;

    LOG_DEBUG(TAG, "Creating DOOM Generic");

    FRESULT res = f_mount(&SDFatFS, (TCHAR const *)SDPath, 0);
    if(res != FR_OK) {
        LOG_ERROR(TAG, "Failed to mount filesystem: %d", res);
        configASSERT(0);
    }

    res = f_open(&SDFile, "doom1.wad", FA_READ);
    if(res != FR_OK) {
        LOG_ERROR(TAG, "Failed to read DOOM file");
        configASSERT(0);
    }

    f_close(&SDFile);

    char *doom_args[] = {"doom", "-mb", "4"};
    doomgeneric_Create(3, doom_args);

    while(1) {
        doomgeneric_Tick();
    }

    osThreadExit();
}

void doomgeneric_init(void) {
    doomgeneric_task_h = osThreadNew(doomgeneric_task, NULL, &doomgeneric_task_attributes);
    configASSERT(doomgeneric_task_h != NULL);
}
