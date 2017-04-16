#include "dac_test_demo.h"

#define PLAY_LENGTH 20

static volatile uint16_t play_length = PLAY_LENGTH;

void dac_init(void) {
    LOGD("dac_test_demo::init", 0);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_TimeBaseInitTypeDef TIM4_TimeBase;
    NVIC_InitTypeDef nvicStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    TIM_TimeBaseStructInit(&TIM4_TimeBase); 
    TIM4_TimeBase.TIM_Period        = 1000 - 1;         
    TIM4_TimeBase.TIM_Prescaler     = 84 - 1;       
    TIM4_TimeBase.TIM_ClockDivision = 0;  
    TIM4_TimeBase.TIM_CounterMode   = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM4, &TIM4_TimeBase);

    nvicStructure.NVIC_IRQChannel = TIM4_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}


void play_sample_dac(fsm_sample * sample, uint8_t beat, uint8_t volume) {
    play_length = PLAY_LENGTH;
    if(beat == DOWNBEAT) {
        GPIO_SetBits(GPIOD, GPIO_Pin_14);
    } else if(beat == OFFBEAT && volume == 3) {
        GPIO_SetBits(GPIOD, GPIO_Pin_12);
    } else {
        GPIO_SetBits(GPIOD, GPIO_Pin_15);
    }
    TIM_Cmd(TIM4, ENABLE);
}

void TIM4_IRQHandler(void) {
    TIM4->SR &= ~TIM_SR_UIF; 
    play_length--;
    if(play_length <= 0) {
        TIM_Cmd(TIM4, DISABLE);
        GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15);
        return;
    };

}