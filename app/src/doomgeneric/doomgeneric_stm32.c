#include <stdint.h>

// HAL
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_dma2d.h"
#include "stm32f7xx_hal_ltdc.h"
#include "stm32f7508_discovery_lcd.h"

// FreeRTOS
#include "FreeRTOS.h"

// Doomgeneric STM32 application
#include "doomgeneric_stm32.h"

// Logs
#include "log.h"

#include "utils.h"
#include "fatfs.h"

// Doomgeneric lib
#include "doomkeys.h"
#include "m_argv.h"
#include "doomgeneric.h"
#include "i_system.h"
#include "z_zone.h"
#include "i_video.h"

#define BITS_PER_PIXEL (32)

static const char *TAG = "DOOM_GENERIC";

static osThreadId_t         doomgeneric_task_h;
static const osThreadAttr_t doomgeneric_task_attributes = {
    .name       = "doomgeneric_task",
    .stack_size = 2048 * 4,
    .priority   = (osPriority_t)osPriorityAboveNormal,
};

static uint8_t     *fbPtr;
static unsigned int fbWidth, fbHeight, fbStride, fbBytesPerPixel, fbOffsetX, fbOffsetY;

extern pixel_t *DG_ScreenBuffer;

static bool check_input_devs(void) {
    return true;
}

int mkdir(const char *path, uint32_t mode) {
    return -1; // Return -1 to indicate directory creation is unsupported
}

void DG_SetWindowTitle(const char *title) {
    (void)title;
}

extern struct color colors[256];
uint32_t            dma2d_clut[256];

void DG_DrawFrame(void) {
    for(int i = 0; i < 256; i++) {
        dma2d_clut[i] = (0xFF << 24) | (colors[i].r << 16) | (colors[i].g << 8) | colors[i].b;
    }

    // LCD resolution
    uint32_t lcd_width  = 480;
    uint32_t lcd_height = 272;

    // (480 - 320) / 2 = 80 pixels padding on the left/right
    uint32_t x_offset = (lcd_width - DOOMGENERIC_RESX) / 2;

    // (272 - 200) / 2 = 36 pixels padding on the top/bottom
    uint32_t y_offset = (lcd_height - DOOMGENERIC_RESY) / 2;

    // Skip 36 rows of 480 pixels, then shift right by 80 pixels
    uint32_t *pDst = (uint32_t *)fbPtr + (y_offset * lcd_width) + x_offset;

    BSP_DrawFrame(DG_ScreenBuffer, pDst, DOOMGENERIC_RESX, DOOMGENERIC_RESY, dma2d_clut);
}

int DG_GetKey(int *pressed, uint8_t *doom_key) {
    return 0;
}

void DG_SleepMs(uint32_t ms) {
    osDelay(pdMS_TO_TICKS(ms));
}

uint32_t DG_GetTicksMs(void) {
    return osKernelGetTickCount();
}

void DG_Init(void) {
    if(!check_input_devs()) {
        return;
    }

    // BSP LCD
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
    BSP_LCD_DisplayOn();
    BSP_LCD_SelectLayer(0);
    BSP_LCD_Clear(LCD_COLOR_BLACK);

    __HAL_RCC_DMA2D_CLK_ENABLE();

    // fbPtr points to LCD framebuffer memory
    fbPtr = (void *)LCD_FB_START_ADDRESS;

    LOG_DEBUG(TAG, "Width: %d, Height: %d", DOOMGENERIC_RESX, DOOMGENERIC_RESY);
}

void doomgeneric_task(void *arg) {
    // unused
    (void)arg;

    LOG_DEBUG(TAG, "Creating DOOM Generic");

    FRESULT res = f_mount(&SDFatFS, (TCHAR const *)SDPath, 0);
    if(res != FR_OK) {
        LOG_ERROR(TAG, "Failed to mount filesystem: %d", res);
        configASSERT(0);
    }

    res = f_open(&SDFile, "doom1.wad", FA_READ);
    if(res != FR_OK) {
        LOG_ERROR(TAG, "Failed to read DOOM file");
        configASSERT(0);
    }

    f_close(&SDFile);

    char *doom_args[] = {"doom", "-mb", "4"};
    doomgeneric_Create(3, doom_args);

    while(1) {
        doomgeneric_Tick();
    }

    osThreadExit();
}

void doomgeneric_init(void) {
    doomgeneric_task_h = osThreadNew(doomgeneric_task, NULL, &doomgeneric_task_attributes);
    configASSERT(doomgeneric_task_h != NULL);
}
