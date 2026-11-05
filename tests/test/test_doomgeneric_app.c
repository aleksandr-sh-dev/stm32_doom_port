#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "unity.h"
#include "config.h"
#include "doomgeneric.h"
#include "doomgeneric_app.h"

#include "i_video.h"

#include "mock_stm32f7508_discovery_lcd.h"
#include "mock_buttons.h"

#define TEST_STRETCH_FRAME_TOP_LEFT_PIXELS     (0xAA)
#define TEST_STRETCH_FRAME_BOTTOM_RIGHT_PIXELS (0xBB)

void utils_sleep_ms(uint32_t ms) {}
uint32_t utils_get_ticks_ms(void) { return 0; }

// Stretch Frame test
__attribute__((aligned(32))) static pixel_t mock_screen_memory[DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4];
pixel_t *DG_ScreenBuffer = mock_screen_memory;
struct color colors[256];
__attribute__((aligned(32))) static uint8_t stretched_buffer[CONFIG_DISPLAY_RES_X * CONFIG_DISPLAY_RES_Y];
static uint16_t x_lut[CONFIG_DISPLAY_RES_X];
static uint16_t y_lut[CONFIG_DISPLAY_RES_Y];
static bool     lut_init = false;

// DG GetKey test
static int pressed;
static unsigned char *doomkey;

void setUp(void) {
    doomkey = malloc(sizeof(unsigned char*));
}

void tearDown(void) {
    free(doomkey);
}

int lwprintf_printf_ex(void* lwp, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    return 0;
}

void test_stretch_frame(void) {
    TEST_ASSERT_EQUAL_INT(DOOMGENERIC_APP_OK, doomgeneric_app_cbs.stretch_frame((uint8_t *)DG_ScreenBuffer, stretched_buffer));
    TEST_ASSERT_EQUAL_INT(DOOMGENERIC_APP_NULL_PTR, doomgeneric_app_cbs.stretch_frame((uint8_t *)DG_ScreenBuffer, NULL));
    TEST_ASSERT_EQUAL_INT(DOOMGENERIC_APP_NULL_PTR, doomgeneric_app_cbs.stretch_frame(NULL, stretched_buffer));

    TEST_MESSAGE("DOOMGENERIC APP STRETCH FRAME OK");
}

void test_stretch_frame_scale_pixels_correctly(void) {
    memset(mock_screen_memory, 0, sizeof(mock_screen_memory));
    memset(stretched_buffer, 0, sizeof(stretched_buffer));

    uint8_t *src_8bit = (uint8_t *)DG_ScreenBuffer;

    src_8bit[0] = TEST_STRETCH_FRAME_TOP_LEFT_PIXELS; // top left pixels
    src_8bit[(DOOMGENERIC_RESY - 1) * DOOMGENERIC_RESX + (DOOMGENERIC_RESX - 1)] = TEST_STRETCH_FRAME_BOTTOM_RIGHT_PIXELS; // bottom right pixels of 320x200

    TEST_ASSERT_EQUAL_INT(DOOMGENERIC_APP_OK, doomgeneric_app_cbs.stretch_frame((uint8_t *)DG_ScreenBuffer, stretched_buffer));
    TEST_ASSERT_EQUAL_HEX8(TEST_STRETCH_FRAME_TOP_LEFT_PIXELS, stretched_buffer[0]); // top left should remain same

    int bottom_right_scaled_index = ((CONFIG_DISPLAY_RES_Y - 1) * CONFIG_DISPLAY_RES_X) + (CONFIG_DISPLAY_RES_X - 1);
    TEST_ASSERT_EQUAL_HEX8(TEST_STRETCH_FRAME_BOTTOM_RIGHT_PIXELS, stretched_buffer[bottom_right_scaled_index]); // bottom right should scale to x=479 and y=271

    TEST_MESSAGE("DOOMGENERIC APP STRETCH FRAME PIXELS ARE CORRECT");
}

// void test_dg_getkey(void) {
//     buttons_get_buttons_CMockExpectAndReturn();

//     DG_GetKey(&pressed, doomkey);
// }