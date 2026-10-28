#pragma once

// Doomgeneric includes
#include "doomkeys.h"

// Project includes
#include "buttons.h"
#include "log.h"

// Buttons default binding
static const button_binding_t button_default_bindings[] = {
    {"FIRE", FIRE_GPIO_Port, FIRE_Pin, KEY_FIRE},
    {"ACTIVATE", ACTIVATE_GPIO_Port, ACTIVATE_Pin, KEY_USE},
    {"RUN", RUN_GPIO_Port, RUN_Pin, KEY_ENTER},
    {"FORWARD", FORWARD_GPIO_Port, FORWARD_Pin, KEY_UPARROW},
    {"BACKWARD", BACKWARD_GPIO_Port, BACKWARD_Pin, KEY_DOWNARROW},
    {"LEFT", LEFT_GPIO_Port, LEFT_Pin, KEY_LEFTARROW},
    {"RIGHT", RIGHT_GPIO_Port, RIGHT_Pin, KEY_RIGHTARROW},
    {"WEAPON_DOWN", WEAPON_DOWN_GPIO_Port, WEAPON_DOWN_Pin, KEY_STRAFE_L},
    {"WEAPON_UP", WEAPON_UP_GPIO_Port, WEAPON_UP_Pin, KEY_STRAFE_R}};

#define CONFIG_BUTTONS_NUM (sizeof(button_default_bindings) / sizeof(button_default_bindings[0]))

#define SCREEN_LOGGING (1 << 0)
#define UART_LOGGING   (1 << 1)

// SD and FatFS
#define CONFIG_ENABLE_SD_DMA_CACHE_MAINTENANCE (1)

// LOGGING
#define CONFIG_LOG_MODE         (UART_LOGGING)
#define CONFIG_LOG_ACTIVE_LEVEL (LOG_LEVEL_DEBUG)

// SDRAM
#define CONFIG_USE_SDRAM_MEMORY
