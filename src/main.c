#include "fs_manager.h"
#include "uart_manager.h"
#include "dac_manager.h"
#include "fs_manager.h"
#include "btn_manager.h"
#include "clock_timer.h"
#include "bar_player.h"
#include "common.h"
#include "btn_manager.h"
#include "lcd_manager.h"
#include "systick_manager.h"
#include "main_manager.h"


#include "stm32f4xx.h"       // file header basic system 
#include <math.h>

//#define LED_BLUE_ON   GPIOD->BSRRL = GPIO_Pin_15;  // led biru on
//#define LED_BLUE_OFF  GPIOD->BSRRH = GPIO_Pin_15;  // led biru off


int main(void) {
//Enable HSE clock
    RCC_HSEConfig(RCC_HSE_ON); 
//Wait for clock to stabilize
    while (!RCC_WaitForHSEStartUp());
    init_UART();    
    systick_init();
 //   fsm_sdcard_test();
  //  fsm_init();
  //  test_fsm();
 //   test_dac_manager();
 //   clt_init();
 //  dac_init();
    btn_init();
  //  btn_enable();
  //  systick_init();
  //  lcd_init();
  //  main_mgr_init();
 // test_fsm();

    fsm_sample sample;
    uint8_t track[] = {
        DOWNBEAT,
        DOWNBEAT,
        DOWNBEAT,
        DOWNBEAT,
        DOWNBEAT,
        DOWNBEAT,
        DOWNBEAT,
        DOWNBEAT
    };

  //  fsm_open_sample("wav1", &sample);
  //  bpl_start_sample(&sample, 60, &track);
    btn_set_button_mode_multiclick(BTN_PORT_0, BTN_PORT_0_PIN_0);
    btn_set_button_mode_long(BTN_PORT_0, BTN_PORT_0_PIN_1);
    btn_timer_enable();

    while(1) {
        btn_loop();
  //     bpl_loop();
    }
  /*
    fsm_init();
    clt_init();
    dac_init();

    fsm_sample sample;
    uint8_t track[] = {
        DOWNBEAT,
        OFFBEAT,
        OFFBEAT,
        DOWNBEAT,
        OFFBEAT,
        NOTHING,
        NOTHING,
        NOTHING,
        NOTHING
    };


    fsm_open_sample("frank", &sample);
    bpl_start_sample(&sample, 200, &track);




    while(1) {
        bpl_loop();
    }
    */
/*
  //  test_fsm();
    test_dac_manager();

    clt_init();
    clt_tim_enable();
    clt_ms_trigger_set(2000);
    
    uint16_t result = (uint16_t) lroundf(60000.0f/161);
    LOGI("division 161", result); 
 
    
    inisialisasi();
    uint8_t light = 0;
    while(1) {
 //       bpl_execute();
        if(clt_ms_trigger_is_set() && !light) {
            clt_ms_trigger_clear();
            LED_BLUE_ON;
            light = 1;
        }
        if(clt_ms_trigger_is_set()  && light) {
            clt_ms_trigger_clear();
            LED_BLUE_OFF;
            light = 0;
        }

    };

    */
    return 0;
    
}
/*
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
*/