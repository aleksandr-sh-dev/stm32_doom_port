#include "buttons.h"
#include "log.h"

static buttons_t buttons = {
    .bindings = NULL,
    .state    = 0,
    .num      = 0,
};

static const char *TAG = "BUTTONS";

uint32_t buttons_get_state(void) {
    if(!buttons.initialized) {
        LOG_ERROR(TAG, "Buttons hasn't been initialized");
        return 0;
    }

    return buttons.state;
}

const button_binding_t *buttons_get_bindings(void) {
    if(!buttons.initialized) {
        LOG_ERROR(TAG, "Buttons hasn't been initialized");
        return NULL;
    }

    return buttons.bindings;
}

const buttons_t *buttons_get_buttons(void) {
    if(!buttons.initialized) {
        LOG_ERROR(TAG, "Buttons hasn't been initialized");
        return NULL;
    }
    return &buttons;
}

// 100Hz timer (10 ms)
void buttons_check(void) {
    uint32_t current_state = 0;

    // Check every button
    for(uint8_t button_num = 0; button_num < buttons.num; button_num++) {
        if(!BUTTON_READ(buttons.bindings[button_num].gpio_port, buttons.bindings[button_num].gpio_pin)) {
            current_state |= (1U << button_num);
        }
    }

    // Update the global state
    buttons.state = current_state;
}

int buttons_init(const button_binding_t *bindings, uint8_t buttons_num) {
    if(bindings == NULL) {
        return BUTTONS_ERR_NULL_PTR;
    }

    if(buttons_num == 0) {
        return BUTTONS_ERR_NO_BTNS;
    }

    // Initialize bindings
    buttons.bindings = bindings;
    buttons.num      = buttons_num;

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode             = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull             = GPIO_PULLUP;

    for(uint8_t i = 0; i < buttons.num; i++) {
        GPIO_InitStruct.Pin = buttons.bindings[i].gpio_pin;
        BUTTON_INIT(buttons.bindings[i].gpio_port, &GPIO_InitStruct);
        LOG_DEBUG(TAG, "Init %s button", buttons.bindings[i].name);
    }

    buttons.initialized = true;

    return BUTTONS_OK;
}

int buttons_deinit(void) {
    if(!buttons.initialized) {
        LOG_WARN(TAG, "Buttons were not initialized");
        return BUTTONS_ERR_NOT_INITIALIZED;
    }

    for(uint8_t i = 0; i < buttons.num; i++) {
        BUTTON_DEINIT(buttons.bindings[i].gpio_port, buttons.bindings[i].gpio_pin);
        LOG_DEBUG(TAG, "Deinit %s button", buttons.bindings[i].name);
    }

    buttons.initialized = false;
    buttons.bindings    = NULL;
    buttons.num         = 0;
    buttons.state       = 0;

    LOG_INFO(TAG, "Buttons deinitialized");

    return BUTTONS_OK;
}
