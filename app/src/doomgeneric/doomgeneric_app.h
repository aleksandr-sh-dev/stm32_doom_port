#pragma once

#include <stdint.h>

#define DOOMGENERIC_APP_RESX (320)
#define DOOMGENERIC_APP_RESY (200)

#define DOOMGENERIC_RESX (DOOMGENERIC_APP_RESX)
#define DOOMGENERIC_RESY (DOOMGENERIC_APP_RESY)

typedef void (*doomgeneric_app_sleep_ms_t)(uint32_t ms);
typedef uint32_t (*doomgeneric_app_get_ticks_ms_t)(void);
typedef bool (*doomgeneric_app_check_input_devs_t)(void);
typedef int (*doomgeneric_app_stretch_frame_t)(uint8_t *src, uint8_t *dst);
typedef void (*doomgeneric_app_draw_frame_t)(void *pSrc, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t *pClut);

typedef struct {
    doomgeneric_app_sleep_ms_t     sleep_ms;
    doomgeneric_app_get_ticks_ms_t get_ticks_ms;
    doomgeneric_app_check_input_devs_t check_input_devs;
    doomgeneric_app_stretch_frame_t stretch_frame;
    doomgeneric_app_draw_frame_t draw_frame;
} doomgeneric_app_cbs_t;

typedef enum {
    DOOMGENERIC_APP_OK = 0,
    DOOMGENERIC_APP_NULL_PTR_ERR = 1,
    DOOMGENERIC_APP_LCD_INIT_ERR = 2,
} doomgeneric_app_err_t;

extern doomgeneric_app_cbs_t doomgeneric_app_cbs;

int doomgeneric_app_stretch_frame(uint8_t *src, uint8_t *dst);
