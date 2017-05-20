//#include "fs_manager.h"
#include "uart_manager.h"
#include "dac_manager.h"
// #include "fs_manager.h"
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

int main(void) {
//Enable HSE clock
    RCC_HSEConfig(RCC_HSE_ON); 
//Wait for clock to stabilize
    while (!RCC_WaitForHSEStartUp());
    init_UART();    
    systick_init();
    clt_init();
    dac_init();
    btn_init();
    lcd_init();
    main_mgr_init();
    dac_enable();

    while(1) {
        btn_loop();
        bpl_loop();
    }

    return 0;
    
}
