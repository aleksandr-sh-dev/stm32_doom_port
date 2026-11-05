#include <stdint.h>
#include <stdbool.h>

#include "doomgeneric_callbacks.h"

// FreeRTOS
#include "FreeRTOS.h"

// HAL
#include "stm32f7508_discovery_lcd.h"

// Doomgeneric STM32 application
#include "doomgeneric_stm32.h"

// Logs
#include "log.h"

#include "buttons.h"
#include "utils.h"

// Doomgeneric lib
#include "doomgeneric.h"
#include "i_video.h"

static const char *TAG = "DOOMGENERIC_CALLBACKS";

static uint32_t previous_button_state = 0;

extern struct color colors[256];
static uint32_t     dma2d_clut[256];
static uint8_t     *fbPtr;
extern pixel_t     *DG_ScreenBuffer;

/* Global pointer for the 130 KB scaled frame */
__ALIGNED_32__ __SDRAM_MEMORY__ static uint8_t stretched_buffer[480 * 272];

/* Fast pre-computed lookup tables */
static uint16_t x_lut[480];
static uint16_t y_lut[272];
static bool     lut_init = false;

static bool check_input_devs(void) {
    return true;
}

int mkdir(const char *path, uint32_t mode) {
    return -1; // Return -1 to indicate directory creation is unsupported
}

static void stretch_frame_8bit(uint8_t *src, uint8_t *dst) {
    if(!lut_init) {
        for(int x = 0; x < 480; x++) {
            x_lut[x] = (x * 320) / 480;
        }
        for(int y = 0; y < 272; y++) {
            y_lut[y] = ((y * 200) / 272) * 320;
        }
        lut_init = true;
    }

    for(int y = 0; y < 272; y++) {
        uint8_t *src_row = src + y_lut[y];
        uint8_t *dst_row = dst + (y * 480);

        for(int x = 0; x < 480; x++) {
            dst_row[x] = src_row[x_lut[x]];
        }
    }
}

void DG_SetWindowTitle(const char *title) {
    (void)title;
}

void DG_DrawFrame(void) {
    stretch_frame_8bit((uint8_t *)DG_ScreenBuffer, stretched_buffer);

    for(int i = 0; i < 256; i++) {
        dma2d_clut[i] = (0xFF << 24) | (colors[i].r << 16) | (colors[i].g << 8) | colors[i].b;
    }

    SCB_CleanDCache_by_Addr((uint32_t *)stretched_buffer, 480 * 272);

    BSP_DrawFrame(stretched_buffer, fbPtr, 480, 272, dma2d_clut);
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
    // osDelay(pdMS_TO_TICKS(ms));
    doomgeneric_cbs.sleep_ms(ms);
}

uint32_t DG_GetTicksMs(void) {
    // return osKernelGetTickCount();
    return doomgeneric_cbs.get_ticks_ms();
}

void DG_Init(void) {
    assert_param(doomgeneric_cbs != NULL);

    if(!doomgeneric_cbs.check_input_devs()) {
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

doomgeneric_callbacks_t doomgeneric_cbs = {
    .get_ticks_ms = utils_get_ticks_ms,
    .sleep_ms = utils_sleep_ms,
    .check_input_devs = check_input_devs,
};
