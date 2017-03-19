#ifndef _SPI_H_
#define _SPI_H_

#include <stm32f4xx.h>

void SPI_init(uint16_t prescaler);
void CS_init();
uint8_t SPI_send_single(uint8_t data);
uint8_t SPI_receive_single();
void SPI_send(uint8_t* data, unsigned int length);
void SPI_receive(uint8_t* data, unsigned int length);
void SPI_transmit(uint8_t* txbuf, uint8_t* rxbuf, unsigned int len);

void CS_set();
void CS_reset();
void SPI_BaudRate(uint16_t prescaler);

#define	_BV(bit) (1<<(bit))


#endif // _SPI_H_
