#include <stdio.h>
#include <stdarg.h>

#include "unity.h"
#include "audio_app.h"
#include "config.h"

// Generated automatically by CMock
#include "mock_stm32f7508_discovery_audio.h"

static uint16_t test_audio_buf[64] = {0};
#define TEST_BUF_SIZE (sizeof(test_audio_buf) / sizeof(test_audio_buf[0]))

void setUp(void) {}

void tearDown(void) {}

int lwprintf_printf_ex(void* lwp, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    return 0;
}

void test_audio_app_init_errors(void) {
    /* Volume above AUDIO_APP_VOLUME_MAX (100) should fail before calling BSP */
    int res = audio_app_init(AUDIO_APP_OUTPUT_DEVICE_HEADPHONES, AUDIO_APP_VOLUME_MAX + 1);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_VOLUME_ERR, res);

    /* BSP hardware init failure should return AUDIO_APP_BSP_INIT_ERR */
    BSP_AUDIO_OUT_Init_ExpectAndReturn(
        AUDIO_APP_OUTPUT_DEVICE_HEADPHONES, 50, DEFAULT_AUDIO_IN_FREQ, AUDIO_ERROR
    );
    res = audio_app_init(AUDIO_APP_OUTPUT_DEVICE_HEADPHONES, 50);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_BSP_INIT_ERR, res);

    TEST_MESSAGE("AUDIO APP INIT ERRORS OK");
}

void test_audio_app_init_success(void) {
    BSP_AUDIO_OUT_Init_ExpectAndReturn(
        AUDIO_APP_OUTPUT_DEVICE_BOTH, AUDIO_APP_VOLUME_MAX, DEFAULT_AUDIO_IN_FREQ, AUDIO_OK
    );

    int res = audio_app_init(AUDIO_APP_OUTPUT_DEVICE_BOTH, AUDIO_APP_VOLUME_MAX);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_OK, res);

    TEST_MESSAGE("AUDIO APP INIT SUCCESS OK");
}

void test_audio_app_play_errors(void) {
    /* NULL buffer check */
    int res = audio_app_play(NULL, TEST_BUF_SIZE);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_NULL_PTR_ERR, res);

    /* BSP play error check */
    BSP_AUDIO_OUT_Play_ExpectAndReturn(test_audio_buf, TEST_BUF_SIZE, AUDIO_ERROR);
    res = audio_app_play(test_audio_buf, TEST_BUF_SIZE);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_PLAY_AUDIO_ERR, res);

    TEST_MESSAGE("AUDIO APP PLAY ERRORS OK");
}

void test_audio_app_play_success(void) {
    BSP_AUDIO_OUT_Play_ExpectAndReturn(test_audio_buf, TEST_BUF_SIZE, AUDIO_OK);

    int res = audio_app_play(test_audio_buf, TEST_BUF_SIZE);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_OK, res);

    TEST_MESSAGE("AUDIO APP PLAY SUCCESS OK");
}

void test_audio_app_change_volume_errors(void) {
    int res = audio_app_change_volume(AUDIO_APP_VOLUME_MAX + 1);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_VOLUME_ERR, res);

    res = audio_app_change_volume(255);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_VOLUME_ERR, res);

    TEST_MESSAGE("AUDIO APP CHANGE VOLUME ERRORS OK");
}

void test_audio_app_change_volume_success(void) {
    /* Note: If BSP_AUDIO_OUT_SetVolume returns uint8_t (AUDIO_OK) in your BSP header,
       change _Expect to _ExpectAndReturn(..., AUDIO_OK) */
    BSP_AUDIO_OUT_SetVolume_ExpectAndReturn(AUDIO_APP_VOLUME_MIN, AUDIO_OK);
    int res = audio_app_change_volume(AUDIO_APP_VOLUME_MIN);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_OK, res);

    BSP_AUDIO_OUT_SetVolume_ExpectAndReturn(AUDIO_APP_VOLUME_MAX, AUDIO_OK);
    res = audio_app_change_volume(AUDIO_APP_VOLUME_MAX);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_OK, res);

    TEST_MESSAGE("AUDIO APP CHANGE VOLUME SUCCESS OK");
}

void test_audio_app_change_mode_errors(void) {
    /* Out-of-bounds mode check */
    int res = audio_app_change_mode(AUDIO_APP_MODE_UNKNOWN);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_STOP_AUDIO_ERR, res);

    /* Resume failure */
    BSP_AUDIO_OUT_Resume_ExpectAndReturn(AUDIO_ERROR);
    res = audio_app_change_mode(AUDIO_APP_MODE_RESUME);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_RESUME_AUDIO_ERR, res);

    /* Pause failure */
    BSP_AUDIO_OUT_Pause_ExpectAndReturn(AUDIO_ERROR);
    res = audio_app_change_mode(AUDIO_APP_MODE_PAUSE);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_PAUSE_AUDIO_ERR, res);

    /* Stop Hardware failure */
    BSP_AUDIO_OUT_Stop_ExpectAndReturn(AUDIO_APP_MODE_STOP_HARDWARE, AUDIO_ERROR);
    res = audio_app_change_mode(AUDIO_APP_MODE_STOP_HARDWARE);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_STOP_AUDIO_ERR, res);

    /* Stop Software failure */
    BSP_AUDIO_OUT_Stop_ExpectAndReturn(AUDIO_APP_MODE_STOP_SOFTWARE, AUDIO_ERROR);
    res = audio_app_change_mode(AUDIO_APP_MODE_STOP_SOFTWARE);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_STOP_AUDIO_ERR, res);

    TEST_MESSAGE("AUDIO APP CHANGE MODE ERRORS OK");
}

void test_audio_app_change_mode_success(void) {
    /* Resume success */
    BSP_AUDIO_OUT_Resume_ExpectAndReturn(AUDIO_OK);
    int res = audio_app_change_mode(AUDIO_APP_MODE_RESUME);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_OK, res);

    /* Pause success */
    BSP_AUDIO_OUT_Pause_ExpectAndReturn(AUDIO_OK);
    res = audio_app_change_mode(AUDIO_APP_MODE_PAUSE);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_OK, res);

    /* Stop Hardware success */
    BSP_AUDIO_OUT_Stop_ExpectAndReturn(AUDIO_APP_MODE_STOP_HARDWARE, AUDIO_OK);
    res = audio_app_change_mode(AUDIO_APP_MODE_STOP_HARDWARE);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_OK, res);

    /* Stop Software success */
    BSP_AUDIO_OUT_Stop_ExpectAndReturn(AUDIO_APP_MODE_STOP_SOFTWARE, AUDIO_OK);
    res = audio_app_change_mode(AUDIO_APP_MODE_STOP_SOFTWARE);
    TEST_ASSERT_EQUAL_INT(AUDIO_APP_OK, res);

    TEST_MESSAGE("AUDIO APP CHANGE MODE SUCCESS OK");
}
