/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;

void MX_USART1_UART_Init(void);
void MX_USART6_UART_Init(void);

void usart6_transmit(uint8_t *data, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
