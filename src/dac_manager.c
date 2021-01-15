#include "dac_manager.h"   

static uint8_t copy_data(uint8_t id, uint8_t beat, uint8_t volume, uint16_t * buffer, uint16_t copy_size);
                                       
uint16_t dac_buff[512];
uint8_t loop_go = FALSE;

void dac_init(void) {
    LOGD("dac_init", 0);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); 
    
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitStructure;
    DMA_InitTypeDef DMA_INIT;
    TIM_TimeBaseInitTypeDef TIM6_TimeBase;
    
    // Init GPIO with AF function
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    TIM_TimeBaseStructInit(&TIM6_TimeBase); 
    TIM6_TimeBase.TIM_Period        = 1;         
    TIM6_TimeBase.TIM_Prescaler     = 875 - 1;       
    TIM6_TimeBase.TIM_ClockDivision = 0;  
    TIM6_TimeBase.TIM_CounterMode   = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM6, &TIM6_TimeBase);
    TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
    
             // Init DAC
    DAC_StructInit(&DAC_InitStructure);
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO; //DAC_Trigger_None; 
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable; // try DAC_OutputBuffer_Enable later
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    
    // Init DMA 
    DMA_DeInit(DMA1_Stream5);
    DMA_INIT.DMA_Channel            = DMA_Channel_7; 
    DMA_INIT.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12L1;//->DHR12R1;// &DAC->DHR12L1; DHR8R1;
    DMA_INIT.DMA_Memory0BaseAddr    = (uint32_t)&dac_buff;
    DMA_INIT.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_INIT.DMA_BufferSize         = 512;
    DMA_INIT.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_INIT.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_INIT.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_INIT.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
    DMA_INIT.DMA_Mode               = DMA_Mode_Circular;
    DMA_INIT.DMA_Priority           = DMA_Priority_High;
    DMA_INIT.DMA_FIFOMode           = DMA_FIFOMode_Disable;         
   // DMA_INIT.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
    DMA_INIT.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_INIT.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream5, &DMA_INIT);
}    


void dac_enable(void) {
    LOGD("dac_enable", 0);
    DMA_Cmd(DMA1_Stream5, ENABLE);
    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_DMACmd(DAC_Channel_1, ENABLE);
}

void dac_disable(void) {
    LOGD("dac_disable", 0);
    DAC_DMACmd(DAC_Channel_1, DISABLE);
    DAC_Cmd(DAC_Channel_1, DISABLE);
    DMA_Cmd(DMA1_Stream5, DISABLE);
}

uint32_t copied_size_bytes = 0;
uint8_t current_beat;
uint8_t current_sample_id;
uint8_t current_volume;

void dac_play_sample(uint8_t id, uint8_t beat, uint8_t volume) {
    TIM_Cmd(TIM6, DISABLE);
    loop_go = FALSE;
    DMA_Cmd(DMA1_Stream5, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream5, 512);
    DMA_Cmd(DMA1_Stream5, ENABLE);
    copied_size_bytes = 0;
    current_beat = beat;
    current_sample_id = id;
    current_volume = volume;
    copy_data(id, beat, volume, dac_buff, 512);
    TIM_Cmd(TIM6, ENABLE);
    loop_go = TRUE;
}



uint8_t copy_data(uint8_t id, uint8_t beat, uint8_t volume, uint16_t * buffer, uint16_t copy_size) {
    uint32_t start_offset;
    uint32_t sample_size;
    uint16_t i = 0;
    if(beat == DOWNBEAT) {
        ifl_downbeat_pos(id, &start_offset, &sample_size);
    } else {
        ifl_offbeat_pos(id, &start_offset, &sample_size);
    };
    start_offset += copied_size_bytes;
    for(i = 0; i < copy_size; i++) {
        buffer[i] = *(uint16_t *)(start_offset + i * 2);
        buffer[i] += 32768;
        copied_size_bytes += 2;
        if(copied_size_bytes >= sample_size) {
            for(i = 0; i <= copy_size; i++) {
                buffer[i] = 32768;
            }
            return TRUE;
        }
    }
    return FALSE;
}


void dac_loop(void) {
    uint8_t res;
    if(!loop_go) return;
    if(DMA1->HISR & DMA_HISR_HTIF5) {
        res = copy_data(current_sample_id, current_beat, current_volume, &dac_buff[0], 256);
        DMA1->HIFCR |= DMA_HIFCR_CHTIF5;
        if(res) {
            loop_go = FALSE;
            TIM_Cmd(TIM6, DISABLE);
            return;
        }
    }
    if(DMA1->HISR & DMA_HISR_TCIF5) {
        res = copy_data(current_sample_id, current_beat, current_volume, &dac_buff[256], 256);
        DMA1->HIFCR |= DMA_HIFCR_CTCIF5;
        if(res) {
            loop_go = FALSE;
            TIM_Cmd(TIM6, DISABLE);
            return;
        }
    }
}

