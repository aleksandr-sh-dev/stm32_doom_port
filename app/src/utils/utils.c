#include <stdint.h>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "cmsis_os2.h"

uint32_t utils_get_ticks_ms(void) {
    return osKernelGetTickCount() * (1000 / configTICK_RATE_HZ);
}

void utils_sleep_ms(uint32_t ms) {
    osDelay(pdMS_TO_TICKS(ms));
}
