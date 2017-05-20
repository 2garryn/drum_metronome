#ifndef UARTMANAGER_H_INCLUDED
#define UARTMANAGER_H_INCLUDED

#include <stm32f4xx.h>

void init_UART();

void print_UART(char * s);

void LOGD(char * text, uint16_t code);
void LOGE(char * text, uint16_t code);
void LOGI(char * text, uint16_t code);

#endif /* UARTMANAGER_H_INCLUDED */