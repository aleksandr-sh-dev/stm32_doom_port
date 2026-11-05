#include <stdbool.h>

#include "doomgeneric_app.h"

// HAL
#include "stm32f7508_discovery_lcd.h"

// App
#include "log.h"
#include "buttons.h"
#include "utils.h"

// Doomgeneric lib
#include "doomgeneric.h"
#include "i_video.h"

#define DOOM_COLORS_8BIT (256)

static const char *TAG = "DOOMGENERIC_APP";

static uint32_t previous_button_state = 0;

extern struct color colors[DOOM_COLORS_8BIT];
static uint32_t     dma2d_clut[DOOM_COLORS_8BIT];
static uint8_t     *fbPtr;
extern pixel_t     *DG_ScreenBuffer;

/* Global pointer for the 130 KB scaled frame */
__ALIGNED_32__ __SDRAM_MEMORY__ static uint8_t stretched_buffer[CONFIG_DISPLAY_RES_X * CONFIG_DISPLAY_RES_Y];

/* Fast pre-computed lookup tables */
static uint16_t x_lut[CONFIG_DISPLAY_RES_X];
static uint16_t y_lut[CONFIG_DISPLAY_RES_Y];
static bool     lut_init = false;

static bool check_input_devs(void) {
    return true;
}

int mkdir(const char *path, uint32_t mode) {
    return -1; // Return -1 to indicate directory creation is unsupported
}

int doomgeneric_app_stretch_frame(uint8_t *src, uint8_t *dst) {
    if (src == NULL) {
        return DOOMGENERIC_APP_NULL_PTR;
    }

    if (dst == NULL) {
        return DOOMGENERIC_APP_NULL_PTR;
    }

    // if(!lut_init) {
    //     for(int x = 0; x < CONFIG_DISPLAY_RES_X; x++) {
    //         x_lut[x] = (x * DOOMGENERIC_RESX) / CONFIG_DISPLAY_RES_X;
    //     }
    //     for(int y = 0; y < CONFIG_DISPLAY_RES_Y; y++) {
    //         y_lut[y] = ((y * DOOMGENERIC_RESY) / CONFIG_DISPLAY_RES_Y) * DOOMGENERIC_RESX;
    //     }
    //     lut_init = true;
    // }

    // for(int y = 0; y < CONFIG_DISPLAY_RES_Y; y++) {
    //     uint8_t *src_row = src + y_lut[y];
    //     uint8_t *dst_row = dst + (y * CONFIG_DISPLAY_RES_X);

    //     for(int x = 0; x < CONFIG_DISPLAY_RES_X; x++) {
    //         dst_row[x] = src_row[x_lut[x]];
    //     }
    // }

    if(!lut_init) {
        for(int x = 0; x < CONFIG_DISPLAY_RES_X; x++) {
            x_lut[x] = (x * DOOMGENERIC_RESX) / CONFIG_DISPLAY_RES_X;
        }
        for(int y = 0; y < CONFIG_DISPLAY_RES_Y; y++) {
            y_lut[y] = ((y * DOOMGENERIC_RESY) / CONFIG_DISPLAY_RES_Y) * DOOMGENERIC_RESX;
        }
        lut_init = true;
    }

    for(int y = 0; y < CONFIG_DISPLAY_RES_Y; y++) {
        uint8_t *src_row = src + y_lut[y];
        uint8_t *dst_row = dst + (y * CONFIG_DISPLAY_RES_X);

        for(int x = 0; x < CONFIG_DISPLAY_RES_X; x++) {
            dst_row[x] = src_row[x_lut[x]];
        }
    }

    return DOOMGENERIC_APP_OK;
}

void DG_SetWindowTitle(const char *title) {
    (void)title;
}

void DG_DrawFrame(void) {
    doomgeneric_app_cbs.stretch_frame((uint8_t *)DG_ScreenBuffer, stretched_buffer);

    for(int i = 0; i < DOOM_COLORS_8BIT; i++) {
        dma2d_clut[i] = (0xFF << 24) | (colors[i].r << 16) | (colors[i].g << 8) | colors[i].b;
    }

    BSP_DrawFrame(stretched_buffer, fbPtr, CONFIG_DISPLAY_RES_X, CONFIG_DISPLAY_RES_Y, dma2d_clut);
}

int DG_GetKey(int *pressed, uint8_t *doom_key) {
    const buttons_t *buttons = buttons_get_buttons();
    if(buttons == NULL) {
        return 0;
    }

    uint32_t state           = buttons->state;
    uint32_t changed_buttons = state ^ previous_button_state;
    if(changed_buttons == 0) {
        return 0;
    }

    for(int button = 0; button < buttons->num; button++) {
        uint32_t mask = (1U << button);

        if(changed_buttons & mask) {
            *doom_key = buttons->bindings[button].doom_key;
            *pressed  = (state & mask) ? 1 : 0;
            previous_button_state ^= mask;
            return 1;
        }
    }

    return 0;
}

void DG_SleepMs(uint32_t ms) {
    doomgeneric_app_cbs.sleep_ms(ms);
}

uint32_t DG_GetTicksMs(void) {
    return doomgeneric_app_cbs.get_ticks_ms();
}

void DG_Init(void) {
    if(!doomgeneric_app_cbs.check_input_devs()) {
        return;
    }

    // BSP LCD
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
    BSP_LCD_DisplayOn();
    BSP_LCD_SelectLayer(0);
    BSP_LCD_Clear(LCD_COLOR_BLACK);

    // fbPtr points to LCD framebuffer memory
    fbPtr = (void *)LCD_FB_START_ADDRESS;

    LOG_DEBUG(TAG, "Width: %d, Height: %d", DOOMGENERIC_RESX, DOOMGENERIC_RESY);
}

doomgeneric_app_cbs_t doomgeneric_app_cbs = {
    .get_ticks_ms = utils_get_ticks_ms,
    .sleep_ms = utils_sleep_ms,
    .check_input_devs = check_input_devs,
    .stretch_frame = doomgeneric_app_stretch_frame,
};
