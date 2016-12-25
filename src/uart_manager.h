#include <stm32f4xx.h>

void init_USART();

void print_USART(char * s);

void USART_puts(USART_TypeDef* USARTx, volatile char *s);
