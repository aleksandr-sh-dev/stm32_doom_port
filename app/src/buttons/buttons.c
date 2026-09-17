#include "gpio.h"

#include "buttons.h"

volatile uint32_t gamepad_state = 0;

uint32_t buttons_get_state(void) {
    return gamepad_state;
}

// 100Hz timer (10 ms)
void buttons_check(void) {
    uint32_t current_state = 0;

    // Read all pins
    if(!HAL_GPIO_ReadPin(FIRE_GPIO_Port, FIRE_Pin))
        current_state |= FIRE_KEY;
    if(!HAL_GPIO_ReadPin(ACTIVATE_GPIO_Port, ACTIVATE_Pin))
        current_state |= ACTIVATE_KEY;
    if(!HAL_GPIO_ReadPin(RUN_GPIO_Port, RUN_Pin))
        current_state |= RUN_KEY;
    if(!HAL_GPIO_ReadPin(FORWARD_GPIO_Port, FORWARD_Pin))
        current_state |= FORWARD_KEY;
    if(!HAL_GPIO_ReadPin(BACKWARD_GPIO_Port, BACKWARD_Pin))
        current_state |= BACKWARD_KEY;
    if(!HAL_GPIO_ReadPin(LEFT_GPIO_Port, LEFT_Pin))
        current_state |= LEFT_KEY;
    if(!HAL_GPIO_ReadPin(RIGHT_GPIO_Port, RIGHT_Pin))
        current_state |= RIGHT_KEY;
    if(!HAL_GPIO_ReadPin(WEAPON_DOWN_GPIO_Port, WEAPON_DOWN_Pin))
        current_state |= WEAPON_DOWN_KEY;
    if(!HAL_GPIO_ReadPin(WEAPON_UP_GPIO_Port, WEAPON_UP_Pin))
        current_state |= WEAPON_UP_KEY;

    // Update the global state
    gamepad_state = current_state;
}

void buttons_init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // GPIO Clocks are enabled in MX_GPIO_Init on start

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;

    GPIO_InitStruct.Pin = RIGHT_Pin | WEAPON_DOWN_Pin;
    HAL_GPIO_Init(RIGHT_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = FIRE_Pin | FORWARD_Pin;
    HAL_GPIO_Init(FIRE_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = RUN_Pin;
    HAL_GPIO_Init(RUN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = WEAPON_UP_Pin | ACTIVATE_Pin | LEFT_Pin | BACKWARD_Pin;
    HAL_GPIO_Init(WEAPON_UP_GPIO_Port, &GPIO_InitStruct);
}
