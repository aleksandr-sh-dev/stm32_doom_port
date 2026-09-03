#include <stdio.h>
#include <string.h>

// Doomgeneric STM32 application
#include "doomgeneric_stm32.h"

#include "log.h"

// Doomgeneric lib
#include "doomkeys.h"
#include "m_argv.h"
#include "doomgeneric.h"
#include "i_system.h"

static bool check_input_devs(void) {
    return true;
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
}
