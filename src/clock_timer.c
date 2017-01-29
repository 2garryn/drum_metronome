#include "clock_timer.h"

uint8_t clt_click_flag = 0;

void clt_init(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef TIM2_TimeBase;
    NVIC_InitTypeDef nvicStructure;

    TIM_TimeBaseStructInit(&TIM2_TimeBase); 
    TIM2_TimeBase.TIM_Period        = 1000 - 1;         
    TIM2_TimeBase.TIM_Prescaler     = 42 - 1;       
    TIM2_TimeBase.TIM_ClockDivision = 0;  
    TIM2_TimeBase.TIM_CounterMode   = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM2, &TIM2_TimeBase);



    nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void clt_tim_enable(void) {
    TIM_Cmd(TIM2, ENABLE);
    NVIC_EnableIRQ(TIM2_IRQn);
}

void clt_tim_disable(void) {
    TIM_Cmd(TIM2, DISABLE);
    NVIC_DisableIRQ(TIM2_IRQn);
}

uint8_t clt_flag_is_set(void) {
    return clt_click_flag;
}

void clt_flag_reset(void) {
    clt_click_flag = 0;
} 

void TIM2_IRQHandler(void) {
  TIM2->SR &= ~TIM_SR_UIF; //Сбрасываем флаг UIF
  clt_click_flag = 1;

}



