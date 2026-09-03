#pragma once

#include "log.h"

#define SCREEN_LOGGING (1 << 0)
#define UART_LOGGING   (1 << 1)

// LOGGING
#define LOG_MODE         (UART_LOGGING | SCREEN_LOGGING)
#define LOG_ACTIVE_LEVEL (LOG_LEVEL_DEBUG)

// SDRAM
#define USE_SDRAM_MEMORY
