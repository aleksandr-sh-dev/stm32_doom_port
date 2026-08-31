#include "lwprintf/lwprintf.h"
#include "usart.h"

static int log_out(int ch, lwprintf_t *lwp) {
    if(ch != '\0') {
        HAL_UART_Transmit(&huart6, &ch, sizeof(char), 100);
    }

    return ch;
}

void log_init(void) {
    lwprintf_init(log_out);
    lwprintf_printf("Initialized logs\r\n");
}
