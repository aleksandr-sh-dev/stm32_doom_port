#pragma once

// Doomgeneric includes
#include "doomkeys.h"

// Project includes
#include "buttons.h"
#include "audio_app.h"

// Buttons default binding
static const button_binding_t button_default_bindings[] = {
    // RIGHT SIDE - ACTIONS
    {"FIRE",         FIRE_GPIO_Port,         FIRE_Pin,         KEY_FIRE      },
    {"ACTIVATE",     ACTIVATE_GPIO_Port,     ACTIVATE_Pin,     KEY_USE       },
    {"RUN",          RUN_GPIO_Port,          RUN_Pin,          KEY_RSHIFT    },
    {"ENTER",        ENTER_GPIO_Port,        ENTER_Pin,        KEY_ENTER     },
    // LEFT SIDE - MOVEMENT
    {"FORWARD",      FORWARD_GPIO_Port,      FORWARD_Pin,      KEY_UPARROW   },
    {"BACKWARD",     BACKWARD_GPIO_Port,     BACKWARD_Pin,     KEY_DOWNARROW },
    {"LEFT",         LEFT_GPIO_Port,         LEFT_Pin,         KEY_LEFTARROW },
    {"RIGHT",        RIGHT_GPIO_Port,        RIGHT_Pin,        KEY_RIGHTARROW},
    // BUMPERS - STRAFE
    {"STRAFE_LEFT",  STRAFE_LEFT_GPIO_Port,  STRAFE_LEFT_Pin,  KEY_STRAFE_L  },
    {"STRAFE_RIGHT", STRAFE_RIGHT_GPIO_Port, STRAFE_RIGHT_Pin, KEY_STRAFE_R  },
    // BUMPERS - WEAPON CYCLE
    {"WEAPON_DOWN",  WEAPON_DOWN_GPIO_Port,  WEAPON_DOWN_Pin,  ','           },
    {"WEAPON_UP",    WEAPON_UP_GPIO_Port,    WEAPON_UP_Pin,    '.'           },
    // MIDDLE - MENU AND MAP
    {"AUTOMAP",      AUTOMAP_GPIO_Port,      AUTOMAP_Pin,      KEY_TAB       },
    {"PAUSE",        PAUSE_GPIO_Port,        PAUSE_Pin,        KEY_ESCAPE    },
};

#define CONFIG_DISPLAY_RES_X (480)
#define CONFIG_DISPLAY_RES_Y (272)

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

// AUDIO
#define CONFIG_AUDIO_OUTPUT_DEVICE  (AUDIO_APP_OUTPUT_BOTH)
#define CONFIG_AUDIO_DEFAULT_VOLUME (10)
