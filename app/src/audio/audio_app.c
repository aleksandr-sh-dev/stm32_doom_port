#include "audio_app.h"
#include "log.h"

static const char *TAG = "AUDIO_APP";

int audio_app_play(uint16_t *buf, uint32_t size) {
    if(buf == NULL) {
        LOG_ERROR(TAG, "Failed to play audio: NULL pointer");
        return AUDIO_APP_NULL_PTR_ERR;
    }

    int res = BSP_AUDIO_OUT_Play(buf, size);
    if(res != AUDIO_OK) {
        LOG_ERROR(TAG, "Failed to play audio: %d", res);
        return AUDIO_APP_PLAY_AUDIO_ERR;
    }

    return AUDIO_APP_OK;
}

int audio_app_change_volume(audio_app_volume_t volume) {
    if(volume < AUDIO_APP_VOLUME_MIN || volume > AUDIO_APP_VOLUME_MAX) {
        LOG_ERROR(TAG, "Wrong audio volume: %d", volume);
        return AUDIO_APP_VOLUME_ERR;
    }

    BSP_AUDIO_OUT_SetVolume((uint8_t)volume);

    return AUDIO_APP_OK;
}

int audio_app_change_mode(audio_app_mode_t mode) {
    if(mode >= AUDIO_APP_MODE_UNKNOWN) {
        LOG_ERROR(TAG, "Unknown audio mode: %d", mode);
        return AUDIO_APP_STOP_AUDIO_ERR;
    }

    int res = AUDIO_APP_OK;

    switch(mode) {
    case AUDIO_APP_MODE_RESUME:
        res = BSP_AUDIO_OUT_Resume();
        if(res != AUDIO_OK) {
            LOG_ERROR(TAG, "Failed to resume audio: %d", res);
            return AUDIO_APP_RESUME_AUDIO_ERR;
        }
        break;
    case AUDIO_APP_MODE_PAUSE:
        res = BSP_AUDIO_OUT_Pause();
        if(res != AUDIO_OK) {
            LOG_ERROR(TAG, "Failed to pause audio: %d", res);
            return AUDIO_APP_PAUSE_AUDIO_ERR;
        }
        break;
    case AUDIO_APP_MODE_STOP_HARDWARE:
        res = BSP_AUDIO_OUT_Stop(AUDIO_APP_MODE_STOP_HARDWARE);
        if(res != AUDIO_OK) {
            LOG_ERROR(TAG, "Failed to stop audio: %d", res);
            return AUDIO_APP_STOP_AUDIO_ERR;
        }
        break;
    case AUDIO_APP_MODE_STOP_SOFTWARE:
        res = BSP_AUDIO_OUT_Stop(AUDIO_APP_MODE_STOP_SOFTWARE);
        if(res != AUDIO_OK) {
            LOG_ERROR(TAG, "Failed to stop audio: %d", res);
            return AUDIO_APP_STOP_AUDIO_ERR;
        }
        break;
    default:
        LOG_ERROR(TAG, "Wrong audio mode: %d", mode);
        break;
    }

    return res;
}

int audio_app_init(audio_app_output_device_t output_device, audio_app_volume_t volume) {
    if(volume < AUDIO_APP_VOLUME_MIN || volume > AUDIO_APP_VOLUME_MAX) {
        LOG_ERROR(TAG, "Wrong audio volume: %d", volume);
        return AUDIO_APP_VOLUME_ERR;
    }

    int res = BSP_AUDIO_OUT_Init(output_device, volume, DEFAULT_AUDIO_IN_FREQ);
    if(res != AUDIO_OK) {
        LOG_ERROR(TAG, "Failed to init BSP for audio out: %d", res);
        return AUDIO_APP_BSP_INIT_ERR;
    }

    return AUDIO_APP_OK;
}
