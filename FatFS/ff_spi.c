#include "ff_spi.h"

#define SPI_PERIPH SPI2
#define SPI_PERIPH_CLOCK RCC_APB1Periph_SPI2

void SPI_init(uint16_t prescaler)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	// enable clock for used IO pins
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	/* configure pins used by SPI1
	 * PA5 = SCK  = PB10
	 * PA6 = MISO = PC2
	 * PA7 = MOSI = PC3
	 */
     /*
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	*/
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	// connect SPI1 pins to SPI alternate function
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);
	
	// enable peripheral clock
	RCC_APB1PeriphClockCmd(SPI_PERIPH_CLOCK, ENABLE);
	
	/* configure SPI1 in Mode 0 
	 * CPOL = 0 --> clock is low when idle
	 * CPHA = 0 --> data is sampled at the first edge
	 */
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to full duplex mode, seperate MOSI and MISO lines
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;     // transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;        // clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;      // data sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_BaudRatePrescaler = prescaler; // SPI frequency is APB2 frequency / 4
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB 
    SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI_PERIPH, &SPI_InitStruct); 
	
	SPI_Cmd(SPI_PERIPH, ENABLE); // enable SPI1
}

void CS_init() {
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIOC->ODR |= GPIO_Pin_4;
}

void CS_set() {
    GPIOC->ODR |= GPIO_Pin_4;
}

void CS_reset() {
    GPIOC->ODR &= ~GPIO_Pin_4;
}

void SPI_BaudRate(uint16_t prescaler) {
    SPI_PERIPH->CR1 |= prescaler;
}

uint8_t SPI_send_single(uint8_t data)
{
	//unsigned char tmp;
	SPI_PERIPH->DR = data; // write data to be transmitted to the SPI data register
	while( !(SPI_PERIPH->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
	while( !(SPI_PERIPH->SR & SPI_I2S_FLAG_RXNE) ); // wait until receive complete
	while( SPI_PERIPH->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore
	return SPI_PERIPH->DR; // return received data from SPI data register	
}

uint8_t SPI_receive_single()
{
	SPI_PERIPH->DR = 0xFF; // write data to be transmitted to the SPI data register
	while( !(SPI_PERIPH->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
	while( !(SPI_PERIPH->SR & SPI_I2S_FLAG_RXNE) ); // wait until receive complete
	while( SPI_PERIPH->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore
	return SPI_PERIPH->DR; // return received data from SPI data register
}

void SPI_send(uint8_t* data, unsigned int length)
{
	while (length--)
	{
		SPI_send_single(*data);
		data++;
	}
}

void SPI_receive(uint8_t* data, unsigned int length)
{
	while (length--)
	{
		*data = SPI_receive_single();
		data++;
	}
}

void SPI_transmit(uint8_t* txbuf, uint8_t* rxbuf, unsigned int len)
{
	while (len--)
	{
		SPI_PERIPH->DR = *txbuf; // write data to be transmitted to the SPI data register
		while( !(SPI_PERIPH->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
		while( !(SPI_PERIPH->SR & SPI_I2S_FLAG_RXNE) ); // wait until receive complete
		while( SPI_PERIPH->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore
		*rxbuf = SPI_PERIPH->DR; // return received data from SPI data register
		txbuf++;
		rxbuf++;	 
	}
}
