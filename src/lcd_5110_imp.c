#include "lcd_5110_imp.h"

static void init_display();
static void write_to_display(uint8_t data, uint8_t mode);
static void clear_lcd();
static void lcd_cs(uint8_t state);
static void lcd_rst(uint8_t state);
static void lcd_dc(uint8_t state);
static void lcd_din(uint8_t state);
static void lcd_sck(uint8_t state);
static void lcd_led(uint8_t state);
static void lcd_bit(uint16_t pin, uint8_t state);


void lcd_implementation_init() {
	GPIO_InitTypeDef ports;
    RCC_AHB1PeriphClockCmd(RCC_GPIO, ENABLE);
	//Declare pins to configure
	ports.GPIO_Pin = CS_PIN | RST_PIN | DC_PIN | DIN_PIN | SCK_PIN | LED_PIN;
	ports.GPIO_Speed = GPIO_Speed_100MHz;
	ports.GPIO_Mode = GPIO_Mode_OUT;
	ports.GPIO_OType = GPIO_OType_PP;
	//Init Port
	GPIO_Init(GPIO_PORT, &ports);
	init_display();
}

static void init_display() {
	lcd_led(RESET);
	lcd_rst(RESET);

	systick_delay(10);
	lcd_rst(SET);
	write_command(0x21); //Extended instruction set selected
	write_command(0xC0); //Set LCD voltage // 0xB7 from code
    write_command(0x06); //Set temperature control (TC2)
	write_command(0x14); //Set Bias for 1/48 /// 0x14 from code

	write_command(0x20); //Revert to standard instruction set
    write_command(0x0C);

	clear_lcd();
}


void write_command(uint8_t command) {
    write_to_display(command, COMMAND);
}

void write_data(uint8_t data) {
    write_to_display(data, DATA);
}

static void write_to_display(uint8_t data, uint8_t mode) {
	uint8_t i;

	lcd_cs(RESET);
	if(mode == COMMAND) {
		lcd_dc(RESET);
	} else {
		lcd_dc(SET);
	}
	// SPI emulation magic
	for (i = 0; i < 8; i++) {
		lcd_din(data & 0x80 ? SET : RESET);
		data = data << 1;
		lcd_sck(RESET);
		lcd_sck(SET);
	}
	lcd_cs(SET);
}


static void clear_lcd() {
	uint8_t i, j;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 84; j++) {
			write_data(0);
		}
	}
}

static void lcd_cs(uint8_t state) {
	lcd_bit(CS_PIN, state);
}
static void lcd_rst(uint8_t state) {
	lcd_bit(RST_PIN, state);
}
static void lcd_dc(uint8_t state) {
	lcd_bit(DC_PIN, state);
}
static void lcd_din(uint8_t state) {
	lcd_bit(DIN_PIN, state);
}
static void lcd_sck(uint8_t state) {
	lcd_bit(SCK_PIN, state);
}
static void lcd_led(uint8_t state) {
	lcd_bit(LED_PIN, state);
}
static void lcd_bit(uint16_t pin, uint8_t state) {
	if(state == SET) {
		GPIO_SetBits(GPIO_PORT, pin);
	} else {
		GPIO_ResetBits(GPIO_PORT, pin);
	}
}