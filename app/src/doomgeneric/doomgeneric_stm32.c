#include <stdint.h>

// FreeRTOS
#include "FreeRTOS.h"

// Doomgeneric STM32 application
#include "doomgeneric_stm32.h"

// Logs
#include "log.h"

#include "utils.h"

// Doomgeneric lib
#include "doomkeys.h"
#include "m_argv.h"
#include "doomgeneric.h"
#include "i_system.h"
#include "z_zone.h"

static osThreadId_t         doomgeneric_task_h;
static const osThreadAttr_t doomgeneric_task_attributes = {
    .name       = "doomgeneric_task",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityAboveNormal,
};

static bool check_input_devs(void) {
    return true;
}

int mkdir(const char *path, uint32_t mode) {
    return -1; // Return -1 to indicate directory creation is unsupported
}

void DG_SetWindowTitle(const char *title) {
    (void)title;
}

void DG_DrawFrame(void) {
}

int DG_GetKey(int *pressed, uint8_t *doom_key) {
    return 0;
}

void DG_SleepMs(uint32_t ms) {
    osDelay(pdMS_TO_TICKS(ms));
}

uint32_t DG_GetTicksMs(void) {
    return osKernelGetTickCount();
}

void DG_Init(void) {
    if(!check_input_devs()) {
        return;
    }
}

void doomgeneric_task(void *arg) {
    // unused
    (void)arg;

    LOG_SCREEN_INFO("Creating DOOM Generic\n");

    doomgeneric_Create(0, NULL);

    while(1) {
        doomgeneric_Tick();
    }

    osThreadExit();
}

void doomgeneric_init(void) {
    doomgeneric_task_h = osThreadNew(doomgeneric_task, NULL, &doomgeneric_task_attributes);
}
