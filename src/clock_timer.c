#include "clock_timer.h"

int16_t clt_ms_trigger = -1;
uint16_t clt_ms_counter = 0;
uint8_t clt_ms_counter_flag = FALSE;

void clt_init(void) {
    LOGD("clt_init", 0);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef TIM2_TimeBase;
    NVIC_InitTypeDef nvicStructure;

    TIM_TimeBaseStructInit(&TIM2_TimeBase); 
    TIM2_TimeBase.TIM_Period        = 1000 - 1;         
    TIM2_TimeBase.TIM_Prescaler     = 84 - 1;       
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
    LOGD("clt_tim_enable", 0);
    TIM_Cmd(TIM2, ENABLE);
    NVIC_EnableIRQ(TIM2_IRQn);
    clt_ms_counter_flag = TRUE;
}

void clt_tim_disable(void) {
    LOGD("clt_tim_disable", 0);
    TIM_Cmd(TIM2, DISABLE);
    NVIC_DisableIRQ(TIM2_IRQn);
}

void clt_ms_trigger_interval_set(uint16_t ms) {
    LOGD("clt_ms_trigger_set", ms);
    clt_ms_trigger = ms;
    clt_ms_counter = 0;
}

uint8_t clt_ms_trigger_is_set(void) {
    return clt_ms_counter_flag;
}

void clt_ms_trigger_clear(void) {
    clt_ms_counter_flag = FALSE;
    clt_ms_counter = 0;
}


void TIM2_IRQHandler(void) {
  TIM2->SR &= ~TIM_SR_UIF; 
  if(clt_ms_trigger != -1){
    clt_ms_counter++;
    if(clt_ms_counter == clt_ms_trigger) {
      clt_ms_counter_flag = TRUE;
    }
  }
}



