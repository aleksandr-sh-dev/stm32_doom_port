#pragma once

#include <stdint.h>

#include "stm32f7508_discovery_audio.h"
#include "stm32f7xx_hal_i2s.h"

typedef enum {
    AUDIO_APP_VOLUME_MIN = 0,
    AUDIO_APP_VOLUME_MAX = 100,
} audio_app_volume_level_t;

typedef enum {
    AUDIO_APP_OUTPUT_DEVICE_SPEAKER    = OUTPUT_DEVICE_SPEAKER,
    AUDIO_APP_OUTPUT_DEVICE_HEADPHONES = OUTPUT_DEVICE_HEADPHONE,
    AUDIO_APP_OUTPUT_BOTH              = OUTPUT_DEVICE_BOTH,
    AUDIO_APP_OUTPUT_AUTO              = OUTPUT_DEVICE_AUTO,

    AUDIO_APP_OUTPUT_UNKNOWN,
} audio_app_output_device_t;

typedef enum {
    AUDIO_APP_MODE_RESUME,
    AUDIO_APP_MODE_STOP_HARDWARE = CODEC_PDWN_HW,
    AUDIO_APP_MODE_STOP_SOFTWARE = CODEC_PDWN_SW,
    AUDIO_APP_MODE_PAUSE,

    AUDIO_APP_MODE_UNKNOWN,
} audio_app_mode_t;

typedef enum {
    AUDIO_APP_OK,
    AUDIO_APP_NULL_PTR_ERR,
    AUDIO_APP_BSP_INIT_ERR,
    AUDIO_APP_VOLUME_ERR,
    AUDIO_APP_PLAY_AUDIO_ERR,
    AUDIO_APP_PAUSE_AUDIO_ERR,
    AUDIO_APP_RESUME_AUDIO_ERR,
    AUDIO_APP_STOP_AUDIO_ERR,

    AUDIO_APP_UNKNOWN_ERR,
} audio_app_err_t;

typedef struct {
    audio_app_output_device_t out_dev;
    audio_app_mode_t          mode;
    uint8_t                   volume;
} audio_app_params_t;

typedef uint8_t audio_app_volume_t;

int audio_app_init(audio_app_output_device_t output_device, audio_app_volume_t volume);
int audio_app_change_volume(audio_app_volume_t volume);
int audio_app_play(uint16_t *buf, uint32_t size);
int audio_app_change_mode(audio_app_mode_t mode);
