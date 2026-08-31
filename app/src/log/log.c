#include <stdint.h>

#include "lwprintf/lwprintf.h"
#include "usart.h"

#define LOG_UART_TIMEOUT_MS (100)

static int log_out(int ch, lwprintf_t* p) {
    uint8_t c = (uint8_t)ch;

    /* Don't print zero */
    if (c == '\0') {
        return ch;
    }

    HAL_UART_Transmit(&huart6, &c, 1, LOG_UART_TIMEOUT_MS);

    return ch;
}

void log_init(void) {
    lwprintf_init(log_out);
    lwprintf_printf("Initialized logs\r\n");
}
