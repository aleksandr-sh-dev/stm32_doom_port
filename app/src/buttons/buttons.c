#include "buttons.h"
#include "log.h"

typedef enum {
    BUTTONS_OK = 0,
    BUTTONS_ERR_NULL_PTR = 1,
    BUTTONS_ERR_NO_BTNS = 2,
} button_err_t;

static buttons_t buttons = {
    .bindings = NULL,
    .state = 0,
    .num = 0,
};

static const char* TAG = "BUTTONS";

uint32_t buttons_get_state(void) {
    return buttons.state;
}

// 100Hz timer (10 ms)
void buttons_check(void) {
    uint32_t current_state = 0;

    // Loop through your elegant array instead of hardcoding every pin!
    for (uint8_t i = 0; i < buttons.num; i++) {
        if (!BUTTON_READ(buttons.bindings[i].gpio_port, buttons.bindings[i].gpio_pin)) {
            // Apply the bitmask defined in the struct
            current_state |= buttons.bindings[i].bitmask; 
        }
    }

    // Update the global state
    buttons.state = current_state;
}

int buttons_init(const button_binding_t *bindings, uint8_t buttons_num) {
    if (bindings == NULL) {
        return BUTTONS_ERR_NULL_PTR;
    }

    if (buttons_num == 0) {
        return BUTTONS_ERR_NO_BTNS;
    }

    // Initialize bindings
    memcpy(buttons.bindings, bindings, sizeof(button_binding_t));
    buttons.num = buttons_num;

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;

    for (uint8_t i = 0; i < buttons.num; i++) {
        GPIO_InitStruct.Pin = buttons.bindings[i].gpio_pin;
        BUTTON_INIT(buttons.bindings[i].gpio_port, &GPIO_InitStruct);
    }

    buttons.initialized = true;

    return BUTTONS_OK;
}

void buttons_deinit(void) {
    if (!buttons.initialized) {
        LOG_WARN(TAG, "Buttons were not initialized");
        return;
    }

    for (uint8_t i = 0; i < buttons.num; i++) {
        BUTTON_DEINIT(buttons.bindings[i].gpio_port, buttons.bindings[i].gpio_pin);
    }

    buttons.initialized = false;

    LOG_INFO(TAG, "Buttons deinitialized");
}
