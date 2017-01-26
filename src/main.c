#include "fs_manager.h"
#include "uart_manager.h"

#include "stm32f4xx.h"       // file header basic system STM32F4

#define LED_BLUE_ON   GPIOD->BSRRL = GPIO_Pin_15;  // led biru on
#define LED_BLUE_OFF  GPIOD->BSRRH = GPIO_Pin_15;  // led biru off

void main(void) {
//Enable HSE clock
    RCC_HSEConfig(RCC_HSE_ON);
//Wait for clock to stabilize
     while (!RCC_WaitForHSEStartUp());
    init_UART();
    print_UART("TEST 1111 TEST 2222\n");
//    test_fs_manager();
    test_dac_manager();
    
    
    
    
    inisialisasi();
    while(1) {
        LED_BLUE_ON;
        delay(100000);
        LED_BLUE_OFF;
        delay(300000);
    };
}

void inisialisasi() // Inisialisasi port D, port tempat LED berada
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void delay(uint32_t ulang) // procedure delay
{
   while (ulang > 0)
   {ulang--;}
}