#include <stdio.h>
#include <stdarg.h>

#include "unity.h"
#include "buttons.h"
#include "config.h"

// Generated automatically by CMock
#include "mock_stm32f7xx_hal_gpio.h"

static const button_binding_t test_bindings[] = {
    {"FIRE",     (GPIO_TypeDef *)0x40021800, GPIO_PIN_6,  KEY_FIRE},
    {"ACTIVATE", (GPIO_TypeDef *)0x40021C00, GPIO_PIN_15, KEY_USE},
    {"RUN",      (GPIO_TypeDef *)0x40020400, GPIO_PIN_7,  KEY_ENTER}
};
#define NUM_TEST_BUTTONS (sizeof(test_bindings) / sizeof(test_bindings[0]))

void setUp(void) {
    // Reset state before every test
    HAL_GPIO_DeInit_Ignore();
    buttons_deinit();
}

void tearDown(void) {}

int lwprintf_printf_ex(void* lwp, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    return 0;
}

void test_buttons_getters(void) {
    TEST_ASSERT_EQUAL_HEX32(0, buttons_get_state());
    TEST_ASSERT_NULL(buttons_get_bindings());
    TEST_ASSERT_NULL(buttons_get_buttons());
}

void test_buttons_init_errors(void) {
    int res = buttons_init(NULL, NUM_TEST_BUTTONS);
    TEST_ASSERT_EQUAL_INT(BUTTONS_ERR_NULL_PTR, res);

    res = buttons_init(test_bindings, 0);
    TEST_ASSERT_EQUAL_INT(BUTTONS_ERR_NO_BTNS, res);
}

void test_buttons_init_and_deinit_success(void) {
    /* Expect Init to be called for all buttons */
    for (int i = 0; i < NUM_TEST_BUTTONS; i++) {
        HAL_GPIO_Init_ExpectAnyArgs();
    }

    int res = buttons_init(test_bindings, NUM_TEST_BUTTONS);
    TEST_ASSERT_EQUAL_INT(BUTTONS_OK, res);

    /* Verify the Getters now return valid pointers to our test array */
    TEST_ASSERT_EQUAL_PTR(test_bindings, buttons_get_bindings());
    TEST_ASSERT_EQUAL_INT(NUM_TEST_BUTTONS, buttons_get_buttons()->num);

    /* Expect DeInit to be called and print LOG_DEBUG("Deinit %s button") */
    for (int i = 0; i < NUM_TEST_BUTTONS; i++) {
        HAL_GPIO_DeInit_ExpectAnyArgs();
    }

    res = buttons_deinit();
    TEST_ASSERT_EQUAL_INT(BUTTONS_OK, res);
}

void test_buttons_check_should_update_state_by_array_index_shift(void) {
    for (int i = 0; i < NUM_TEST_BUTTONS; i++) {
        HAL_GPIO_Init_ExpectAnyArgs();
    }
    buttons_init(test_bindings, NUM_TEST_BUTTONS);

    /* Simulate FIRE (Index 0) and RUN (Index 2) being physically pressed (LOW / GPIO_PIN_RESET).
        Simulate ACTIVATE (Index 1) being released (HIGH / GPIO_PIN_SET). */
    HAL_GPIO_ReadPin_ExpectAndReturn(test_bindings[0].gpio_port, test_bindings[0].gpio_pin, GPIO_PIN_RESET);
    HAL_GPIO_ReadPin_ExpectAndReturn(test_bindings[1].gpio_port, test_bindings[1].gpio_pin, GPIO_PIN_SET);
    HAL_GPIO_ReadPin_ExpectAndReturn(test_bindings[2].gpio_port, test_bindings[2].gpio_pin, GPIO_PIN_RESET);

    /* Run the state machine tick */
    buttons_check();

    /* Verify the state calculation */
    uint32_t expected_state = (1U << 0) | (1U << 2);
    TEST_ASSERT_EQUAL_HEX32(expected_state, buttons_get_state());
}
