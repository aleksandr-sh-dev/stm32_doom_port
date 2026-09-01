#pragma once

#define SCREEN_LOGGING (1 << 0)
#define UART_LOGGING   (1 << 1)

#define LOG_MODE (SCREEN_LOGGING | UART_LOGGING)
