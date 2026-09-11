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

extern struct color colors[256];
static uint32_t     dma2d_clut[256];
static uint8_t     *fbPtr;
extern pixel_t     *DG_ScreenBuffer;

static bool check_input_devs(void) {
    return true;
}

int mkdir(const char *path, uint32_t mode) {
    return -1; // Return -1 to indicate directory creation is unsupported
}

void DG_SetWindowTitle(const char *title) {
    (void)title;
}

/* Global pointer for the 130 KB scaled frame */
__SDRAM_MEMORY__ static uint8_t stretched_buffer[480 * 272];

/* Fast pre-computed lookup tables */
static uint16_t x_lut[480];
static uint16_t y_lut[272];
static bool lut_init = false;

static void stretch_frame_8bit(uint8_t* src, uint8_t* dst) {
    if (!lut_init) {
        for (int x = 0; x < 480; x++) {
            x_lut[x] = (x * 320) / 480;
        }
        for (int y = 0; y < 272; y++) {
            y_lut[y] = ((y * 200) / 272) * 320; 
        }
        lut_init = true;
    }

    for (int y = 0; y < 272; y++) {
        uint8_t* src_row = src + y_lut[y];
        uint8_t* dst_row = dst + (y * 480);
        
        for (int x = 0; x < 480; x++) {
            dst_row[x] = src_row[x_lut[x]];
        }
    }
}

void DG_DrawFrame(void) {
    stretch_frame_8bit((uint8_t *)DG_ScreenBuffer, stretched_buffer);

    for(int i = 0; i < 256; i++) {
        dma2d_clut[i] = (0xFF << 24) | (colors[i].r << 16) | (colors[i].g << 8) | colors[i].b;
    }

    BSP_DrawFrame(stretched_buffer, fbPtr, 480, 272, dma2d_clut);
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
