#include <stdint.h>

#include "lwprintf/lwprintf.h"
#include "usart.h"

#define LOG_UART_TIMEOUT_MS (100)

static int log_out(int ch, lwprintf_t *lwp) {
    if (ch != '\0') {
        HAL_UART_Transmit(&huart6, (uint8_t *)&ch, sizeof(char), LOG_UART_TIMEOUT_MS);
    }
    
    return ch;
}

void log_init(void) {
    lwprintf_init(log_out);
    lwprintf_printf("Initialized logs\r\n");
}
