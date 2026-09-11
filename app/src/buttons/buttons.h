#pragma once

// Right Side
#define FIRE_Pin       GPIO_PIN_6
#define FIRE_GPIO_Port GPIOG

#define ACTIVATE_Pin       GPIO_PIN_1
#define ACTIVATE_GPIO_Port GPIOI

#define RUN_Pin       GPIO_PIN_15
#define RUN_GPIO_Port GPIOH

// Left Side
#define FORWARD_Pin       GPIO_PIN_7
#define FORWARD_GPIO_Port GPIOG

#define BACKWARD_Pin       GPIO_PIN_3
#define BACKWARD_GPIO_Port GPIOI

#define LEFT_Pin       GPIO_PIN_2
#define LEFT_GPIO_Port GPIOI

#define RIGHT_Pin       GPIO_PIN_14
#define RIGHT_GPIO_Port GPIOB

// Left bumpers
#define WEAPON_DOWN_Pin       GPIO_PIN_15
#define WEAPON_DOWN_GPIO_Port GPIOB

// Right bumpers
#define WEAPON_UP_Pin       GPIO_PIN_0
#define WEAPON_UP_GPIO_Port GPIOI

void     buttons_init(void);
void     buttons_check(void);
uint32_t buttons_get_state(void);
