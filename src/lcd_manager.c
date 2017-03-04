#include "lcd_manager.h"

static void init_display();
static void write_command(uint8_t command);
static void write_data(uint8_t data);
static void write_to_display(uint8_t data, uint8_t mode);
static void clear_lcd();
static void lcd_cs(uint8_t state);
static void lcd_rst(uint8_t state);
static void lcd_dc(uint8_t state);
static void lcd_din(uint8_t state);
static void lcd_sck(uint8_t state);
static void lcd_led(uint8_t state);
static void lcd_bit(uint16_t pin, uint8_t state);

void lcd_demo() {
	uint8_t dot[] = { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E };
	uint8_t line;

	// X = 0 and Y = 0;
	write_command(0x40);
	write_command(0x80);

	for (line = 0; line < 6; line++) {
		write_data(dot[line]);
	}
}


void lcd_init() {
	GPIO_InitTypeDef ports;
	//Declare pins to configure
	ports.GPIO_Pin = CS_PIN | RST_PIN | DC_PIN | DIN_PIN | SCK_PIN | LED_PIN;
	ports.GPIO_Speed = GPIO_Speed_100MHz;
	ports.GPIO_Mode = GPIO_Mode_OUT;
	ports.GPIO_OType = GPIO_OType_PP;
	//Start clock to the selected port
	RCC_AHB1PeriphClockCmd(RCC_GPIO, ENABLE);
	//Init Port
	GPIO_Init(GPIO_PORT, &ports);
	init_display();
}


static void init_display() {
	lcd_led(RESET);
	lcd_rst(RESET);
	lcd_dc(SET); //command mode
	lcd_din(SET);
	lcd_sck(SET);
	lcd_cs(SET);

	systick_delay(10);
	lcd_rst(SET);
	write_command(0x21); //Extended instruction set selected
	write_command(0xB8); //Set LCD voltage // 0xB7 from code
	write_command(0x13); //Set Bias for 1/48 /// 0x14 from code
	write_command(0x06); //Set temperature control (TC2)
	write_command(0x20); //Revert to standard instruction set
	clear_lcd();
	write_command(0x0C);
}


static void write_command(uint8_t command) {
    write_to_display(command, COMMAND);
}

static void write_data(uint8_t data) {
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