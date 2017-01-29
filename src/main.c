#include "fs_manager.h"
#include "uart_manager.h"
#include "dac_manager.h"
#include "clock_timer.h"

#include "stm32f4xx.h"       // file header basic system STM32F4

#define LED_BLUE_ON   GPIOD->BSRRL = GPIO_Pin_15;  // led biru on
#define LED_BLUE_OFF  GPIOD->BSRRH = GPIO_Pin_15;  // led biru off


int main(void) {
//Enable HSE clock
    RCC_HSEConfig(RCC_HSE_ON);
//Wait for clock to stabilize
     while (!RCC_WaitForHSEStartUp());
    init_UART();
    print_UART("TEST 1111 TEST 2222\n");
  //  test_fsm();
 //   test_dac_manager();
    clt_init();
    clt_tim_enable();
    
    
    
    
    inisialisasi();
    uint8_t light = 0;
    while(1) {
        if(clt_flag_is_set() && !light) {
            clt_flag_reset();
            LED_BLUE_ON;
            light = 1;
        }
        if(clt_flag_is_set() && light) {
            clt_flag_reset();
            LED_BLUE_OFF;
            light = 0;
        }

    };
    return 0;
    
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

void delay111(uint32_t ulang) // procedure delay
{
   while (ulang > 0)
   {ulang--;}
}
