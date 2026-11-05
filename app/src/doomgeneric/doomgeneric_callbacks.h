#pragma once

#include <stdint.h>

typedef void (*sleep_ms_t)(uint32_t ms);
typedef uint32_t (*get_ticks_ms_t)(void);
typedef bool (*check_input_devs_t)(void);

typedef struct {
    sleep_ms_t     sleep_ms;
    get_ticks_ms_t get_ticks_ms;
    check_input_devs_t check_input_devs;
} doomgeneric_callbacks_t;

extern doomgeneric_callbacks_t doomgeneric_cbs;
