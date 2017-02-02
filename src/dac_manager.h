#ifndef DACMANAGER_H_INCLUDED
#define DACMANAGER_H_INCLUDED

#include <stm32f4xx.h>
#include "uart_manager.h"
#include "fs_manager.h"
/*
#define   OUT_FREQ          2500                                 // Output waveform frequency
#define   SINE_RES          128                                  // Waveform resolution
#define   DAC_DHR12R1_ADDR  0x40007408                           // DMA writes into this reg on every request
#define   CNT_FREQ          42000000                             // TIM6 counter clock (prescaled APB1)
#define   TIM_PERIOD        ((CNT_FREQ)/((SINE_RES)*(OUT_FREQ))) // Autoreload reg value



void test_dac_manager1(void);
void test_dac_manager(void);

static void TIM6_Config(void);
static void DAC1_Config(void);   
*/

void dac_init(void);
void dac_enable(void);
void dac_disable(void);
uint8_t dac_load_file(FIL * fil);
uint8_t dac_loop(void);
uint8_t dac_file_reset();

#endif /* DACMANAGER_H_INCLUDED */