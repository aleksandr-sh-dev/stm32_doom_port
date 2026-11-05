#pragma once

#include <stdbool.h>

#include "gpio.h"

// Right Side
#define RUN_Pin            GPIO_PIN_6
#define RUN_GPIO_Port      GPIOG
#define ACTIVATE_Pin       GPIO_PIN_1
#define ACTIVATE_GPIO_Port GPIOI
#define FIRE_Pin           GPIO_PIN_6 // ARD_A5
#define FIRE_GPIO_Port     GPIOF
#define ENTER_Pin          GPIO_PIN_15
#define ENTER_GPIO_Port    GPIOH

// Left Side
#define FORWARD_Pin        GPIO_PIN_7
#define FORWARD_GPIO_Port  GPIOG
#define BACKWARD_Pin       GPIO_PIN_3
#define BACKWARD_GPIO_Port GPIOI
#define LEFT_Pin           GPIO_PIN_2
#define LEFT_GPIO_Port     GPIOI
#define RIGHT_Pin          GPIO_PIN_14
#define RIGHT_GPIO_Port    GPIOB

// Left bumpers
#define STRAFE_LEFT_Pin       GPIO_PIN_8 // ARD_A2
#define STRAFE_LEFT_GPIO_Port GPIOF
#define WEAPON_DOWN_Pin       GPIO_PIN_9 // ARD_A3
#define WEAPON_DOWN_GPIO_Port GPIOF

// Right bumpers
#define WEAPON_UP_Pin          GPIO_PIN_15 // ARD_D11
#define WEAPON_UP_GPIO_Port    GPIOB
#define STRAFE_RIGHT_Pin       GPIO_PIN_0 // ARD_A0
#define STRAFE_RIGHT_GPIO_Port GPIOA

// Middle Side
#define AUTOMAP_Pin       GPIO_PIN_10 // ARD_A1
#define AUTOMAP_GPIO_Port GPIOF
#define PAUSE_Pin         GPIO_PIN_7 // ARD_A4
#define PAUSE_GPIO_Port   GPIOF

#define BUTTON_READ(port, pin)         (HAL_GPIO_ReadPin(port, pin))
#define BUTTON_INIT(port, init_struct) (HAL_GPIO_Init(port, init_struct))
#define BUTTON_DEINIT(port, pin)       (HAL_GPIO_DeInit(port, pin))

typedef struct {
    char         *name;
    GPIO_TypeDef *gpio_port;
    uint16_t      gpio_pin;
    uint8_t       doom_key;
} button_binding_t;

typedef struct {
    const button_binding_t *bindings;
    volatile uint32_t       state;
    uint8_t                 num;
    bool                    initialized;
} buttons_t;

typedef enum {
    BUTTONS_OK                  = 0,
    BUTTONS_ERR_NULL_PTR        = 1,
    BUTTONS_ERR_NO_BTNS         = 2,
    BUTTONS_ERR_NOT_INITIALIZED = 3,
} button_err_t;

int                     buttons_init(const button_binding_t *bindings, uint8_t buttons_num);
int                     buttons_deinit(void);
void                    buttons_check(void);
uint32_t                buttons_get_state(void);
const button_binding_t *buttons_get_bindings(void);
const buttons_t        *buttons_get_buttons(void);
