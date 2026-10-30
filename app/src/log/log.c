#include <stdint.h>
#include <stdbool.h>

#include "stm32f7508_discovery_lcd.h"
#include "usart.h"

#include "config.h"
#include "log.h"

#define LOG_UART_TIMEOUT_MS (100)

const char *TAG = "LOG";

static int log_out(int ch, lwprintf_t *p) {
    uint8_t c = (uint8_t)ch;

    /* Don't print zero */
    if(c == '\0') {
        return ch;
    }

    HAL_UART_Transmit(&huart6, &c, 1, LOG_UART_TIMEOUT_MS);

    return ch;
}

void log_init(void) {
    if(CONFIG_LOG_MODE & UART_LOGGING) {
        lwprintf_init(log_out);
        lwprintf_printf("Initialized logs\r\n");
    }

    if(CONFIG_LOG_MODE & SCREEN_LOGGING) {
        LCD_LOG_Init();
        LCD_LOG_SetHeader((uint8_t *)"LOGGING");
    }
}
