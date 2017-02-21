#include "btn_manager.h"



uint8_t buttons_enabled = FALSE;
uint8_t debounce_delay = FALSE;

void btn_init(void) {
    btn_pins_init();
    btn_timer_init();
}

void btn_timer_init(void) {
    LOGD("btn_init", 0);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseInitTypeDef TIM3_TimeBase;
    NVIC_InitTypeDef nvicStructure;

    TIM_TimeBaseStructInit(&TIM3_TimeBase); 
    TIM3_TimeBase.TIM_Period        = 50000 - 1;         
    TIM3_TimeBase.TIM_Prescaler     = 336 - 1;       
    TIM3_TimeBase.TIM_ClockDivision = 0;  
    TIM3_TimeBase.TIM_CounterMode   = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM3, &TIM3_TimeBase);

    nvicStructure.NVIC_IRQChannel = TIM3_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 1;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
    NVIC_EnableIRQ(TIM3_IRQn);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}



void btn_pins_init(void) {
#ifdef BTN_PORT_0_ENABLED
    btn_init_port_pins(BTN_PORT_0, BTN_PORT_0_RCC, BTN_PORT_0_PINS);
#endif
#ifdef BTN_PORT_1_ENABLED
    btn_init_port_pins(BTN_PORT_1, BTN_PORT_1_RCC, BTN_PORT_1_PINS);
#endif
}

void btn_init_port_pins(GPIO_TypeDef* GpioPort, uint32_t RCClocking, uint32_t pins) {
    GPIO_InitTypeDef buttons;
    RCC_AHB1PeriphClockCmd(RCClocking, ENABLE);
    buttons.GPIO_Pin = pins;
    buttons.GPIO_Mode = GPIO_Mode_IN;
    buttons.GPIO_Speed = GPIO_Speed_50MHz;
    buttons.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GpioPort, &buttons);
}


void btn_enable(void) {
    LOGD("btn_tim_enable", 0);
    buttons_enabled = TRUE;

}

void btn_disable(void) {
    LOGD("btn_tim_enable", 0);
    buttons_enabled = FALSE;
}

void btn_loop(void) {
    if(!buttons_enabled || debounce_delay) {
        return;
    }
#ifdef BTN_PORT_0_ENABLED
    uint16_t port0 = ~(GPIO_ReadInputData(BTN_PORT_0));
#endif
#ifdef BTN_PORT_1_ENABLED
    uint16_t port1 = GPIO_ReadInputData(BTN_PORT_1);
#endif
#ifdef BTN_PORT_0_ENABLED
    if(port0 & (BTN_PORT_0_PINS)) {
        debounce_delay = TRUE;
        if(port0 & BTN_PORT_0_PIN_0) btn_callback_clicked_0_0(); 
        else if(port0 & BTN_PORT_0_PIN_1) btn_callback_clicked_0_1(); 
        else if(port0 & BTN_PORT_0_PIN_2) btn_callback_clicked_0_2();
        else if(port0 & BTN_PORT_0_PIN_3) btn_callback_clicked_0_3();
        TIM_Cmd(TIM3, ENABLE);
    } 
#endif
#ifdef BTN_PORT_1_ENABLED
    else if(port1 & (BTN_PORT_1_PINS)) {
        if(port1 & BTN_PORT_1_PIN_0) btn_callback_clicked_1_0(); 
        else if(port1 & BTN_PORT_1_PIN_1) btn_callback_clicked_1_1();
        else if(port1 & BTN_PORT_1_PIN_2) btn_callback_clicked_1_2(); 
        else if(port1 & BTN_PORT_1_PIN_3) btn_callback_clicked_1_3();
        TIM_Cmd(TIM3, ENABLE);
    } else {
        return;
    }
#endif
}

void TIM3_IRQHandler(void) {
  TIM3->SR &= ~TIM_SR_UIF; 
  debounce_delay = FALSE;
  TIM_Cmd(TIM3, DISABLE);
}