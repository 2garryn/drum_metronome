#include <stm32f4xx.h>

void init_UART();

void print_UART(char * s);

void UART_puts(USART_TypeDef* USARTx, volatile char *s);
