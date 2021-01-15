#include "uart_manager.h"

static void UART_puts(USART_TypeDef* USARTx, volatile char *s);
static void LOG(char * log_level, char * text, uint16_t code);

void init_UART(){
    uint32_t baudrate = 115200;
	GPIO_InitTypeDef GPIO_InitStruct; // this is for the GPIO pins used as TX and RX
	USART_InitTypeDef USART_InitStruct; // this is for the USART1 initilization
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;				// Pins 6 (TX) 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 			// the pins are configured as alternate function so the USART peripheral has access to them
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// this defines the IO speed and has nothing to do with the baudrate!
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			// this defines the output type as push pull mode (as opposed to open drain)
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// this activates the pullup resistors on the IO pins
	GPIO_Init(GPIOB, &GPIO_InitStruct);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); //
	
	USART_InitStruct.USART_BaudRate = baudrate;				// the baudrate is set to the value we passed into this init function
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
	USART_InitStruct.USART_StopBits = USART_StopBits_1;		// we want 1 stop bit (standard)
	USART_InitStruct.USART_Parity = USART_Parity_No;		// we don't want a parity bit (standard)
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
	USART_InitStruct.USART_Mode = USART_Mode_Tx; 			// we want to enable the transmitter
	USART_Init(USART1, &USART_InitStruct);					// again all the properties are passed to the USART_Init function which takes care of all the bit setting

	// finally this enables the complete USART1 peripheral
	USART_Cmd(USART1, ENABLE);
    
    print_UART("\n");
}

void print_UART(char *s) {
    UART_puts(USART1, s);
}

void UART_putc(USART_TypeDef* USARTx, unsigned char c){
    // wait until data register is empty
    while( !(USARTx->SR & 0x00000040) ); 
    USART_SendData(USARTx, c);
}

void UART_puts(USART_TypeDef* USARTx, volatile char *s){
	while(*s){
		UART_putc(USARTx, *s);
		s++;
	}
}

#define STRING_LENGTH 6
void int_to_str( uint16_t word, char * str ) {
  str[STRING_LENGTH - 1] = 0;
  int8_t i;
  for(i = STRING_LENGTH - 2; i >= 0; i--) {
	  str[i] = 48;
  }
  for (i = STRING_LENGTH - 2; i >= 0; i--) {
    str[i] = (word % 10) + 48;
    word /= 10;
  };
}

void LOGD(char * text, uint16_t code) {
	LOG("DEBUG", text, code);
}

void LOGE(char * text, uint16_t code) {
	LOG("ERROR", text, code);
}

void LOGI(char * text, uint16_t code) {
	LOG("INFO", text, code);
}

void LOG(char * log_level, char * text, uint16_t code) {
	char str[STRING_LENGTH];
	int_to_str(code, str);
	print_UART(log_level);
	print_UART(": ");
	print_UART(text);
	print_UART(" - Code: ");
	print_UART(str);
	print_UART("\n");
	
}
