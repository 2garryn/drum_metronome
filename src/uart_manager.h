#ifndef UARTMANAGER_H_INCLUDED
#define UARTMANAGER_H_INCLUDED

#include <stm32f4xx.h>

void init_UART();

void print_UART(char * s);

//void UART_puts(USART_TypeDef* USARTx, volatile char *s);

void LOGD(char * text, uint8_t code);
void LOGE(char * text, uint8_t code);
void LOGI(char * text, uint8_t code);

#endif /* UARTMANAGER_H_INCLUDED */