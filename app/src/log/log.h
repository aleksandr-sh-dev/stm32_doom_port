#pragma once

#include <stdio.h>

#include "lwprintf/lwprintf.h"
#include "lcd_log.h"
#include "config.h"

// Log colors
#define LOG_COLOR_RESET   "\x1B[0m"
#define LOG_COLOR_RED     "\x1B[31m"
#define LOG_COLOR_ORANGE  "\x1B[33m"
#define LOG_COLOR_MAGENTA "\x1B[35m"
#define LOG_COLOR_WHITE   "\x1B[37m"

// Log levels
#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3

#define LOG_PRINT_CORE(color, level_str, tag, fmt, ...)                                                     \
    do {                                                                                                    \
        lwprintf_printf("%s[%s] %s: " fmt "%s\r\n", color, level_str, tag, ##__VA_ARGS__, LOG_COLOR_RESET); \
    } while(0)

#if(LOG_MODE & SCREEN_LOGGING)
#define LOG_SCREEN_INFO(fmt, ...)              \
    do {                                       \
        LCD_UsrLog(fmt "\r\n", ##__VA_ARGS__); \
    } while(0)
#else
#define LOG_SCREEN_INFO(fmt, ...)
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_DEBUG
#define LOG_DEBUG(tag, fmt, ...) LOG_PRINT_CORE(LOG_COLOR_MAGENTA, "D", tag, fmt, ##__VA_ARGS__)
#else
#define LOG_DEBUG(tag, fmt, ...)
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_INFO
#define LOG_INFO(tag, fmt, ...) LOG_PRINT_CORE(LOG_COLOR_WHITE, "I", tag, fmt, ##__VA_ARGS__)
#else
#define LOG_INFO(tag, fmt, ...)
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_WARN
#define LOG_WARN(tag, fmt, ...) LOG_PRINT_CORE(LOG_COLOR_ORANGE, "W", tag, fmt, ##__VA_ARGS__)
#else
#define LOG_WARN(tag, fmt, ...)
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_ERROR
#define LOG_ERROR(tag, fmt, ...)                                     \
    do {                                                             \
        LOG_PRINT_CORE(LOG_COLOR_RED, "E", tag, fmt, ##__VA_ARGS__); \
        LOG_SCREEN_INFO("ERR [%s]: " fmt, tag, ##__VA_ARGS__);       \
    } while(0)
#else
#define LOG_ERROR(tag, fmt, ...)
#endif

void log_init(void);
