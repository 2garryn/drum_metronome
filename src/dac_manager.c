#include "dac_manager.h"   

                                       
unsigned char dac_buff[512];
uint8_t loop_go = FALSE;
FIL * current_file;


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
    DMA_INIT.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR8R1;//->DHR12R1;// &DAC->DHR12L1; DHR8R1;
    DMA_INIT.DMA_Memory0BaseAddr    = (uint32_t)&dac_buff;
    DMA_INIT.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_INIT.DMA_BufferSize         = 512;
    DMA_INIT.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_INIT.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_INIT.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_INIT.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_INIT.DMA_Mode               = DMA_Mode_Circular;
    DMA_INIT.DMA_Priority           = DMA_Priority_High;
    DMA_INIT.DMA_FIFOMode           = DMA_FIFOMode_Disable;         
    DMA_INIT.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
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

void dac_timer_enable() {
    loop_go = TRUE;
    DMA_SetCurrDataCounter(DMA1_Stream5, 0);
    TIM_Cmd(TIM6, ENABLE);
}

void dac_timer_disable() {
    loop_go = FALSE;
    TIM_Cmd(TIM6, DISABLE);
}

uint8_t dac_load_file(FIL * fil) {
    LOGD("dac_play", 0);
    uint8_t result;
    current_file = fil;
    if(fsm_seek_raw_wav(fil)) {          
        return RET_ERROR;
    };
    result = fsm_read_file(fil, &dac_buff[0], 512);
    if(result == RET_ERROR) {
        return RET_ERROR;
    }
    return RET_OK;
}

uint8_t dac_loop(void) {
    uint8_t res = RET_OK;
    if((DMA1->HISR & DMA_HISR_HTIF5) && loop_go) {
        res = fsm_read_file(current_file, &dac_buff[0], 256);    //загрузить ее данными
        DMA1->HIFCR |= DMA_HIFCR_CHTIF5;   
        if(res == RET_FILE_FINISHED || res == RET_ERROR) {
            loop_go = FALSE;
        };
        return res;
    }
    if((DMA1->HISR & DMA_HISR_TCIF5) && loop_go) {
        res = fsm_read_file(current_file, &dac_buff[256], 256);
        DMA1->HIFCR |= DMA_HIFCR_CTCIF5;
        if(res == RET_FILE_FINISHED || res == RET_ERROR) {
            loop_go = FALSE;
        } 
        return res;
    }
    return res;
}


void test_dac_manager(void){
    fsm_sample sample;
    fsm_init();
    dac_init();
    fsm_open_sample("sine", &sample);
    dac_enable();
    dac_load_file(&(sample.downbeat_fil));
    uint8_t res = RET_OK;
    dac_timer_enable();
    while(1) {
        res = dac_loop();
        if(res == RET_ERROR || res == RET_FILE_FINISHED ) {
            break;
        }
    }
    dac_timer_disable();
    dac_load_file(&(sample.offbeat_fil));;
    res = RET_OK;
    dac_timer_enable();
    while(1) {
        res = dac_loop();
        if(res == RET_ERROR || res == RET_FILE_FINISHED ) {
            break;
        }
    }
    dac_timer_disable();
    dac_disable();
    fsm_close_sample(&sample);

}

 /*
int wave_playback(const char *FileName)
{
  FRESULT res;                                //для возвращаемого функциями результата
  FIL file;                                   //файловый объект
  UINT cnt;                                   //количество реально прочитанных байт
  FATFS FatFs;
  
  print_UART("Mounting volume...");
  res = f_mount(&FatFs, "", 1);
  if(res) { 
        print_UART("Can not mount volume\n");
        return 4;
  };
  print_UART("Opening file: \"test.txt\"...");
  res = f_open( &file, FileName, FA_READ );   //открыть файл FileName для чтения
  if(res) { 
      print_UART("File not opened\n");
      return 1;
  };
  print_UART( "success!\n");
  print_UART( "Seek position: \"");
  res = f_lseek(&file,0x2c);        //переместить указатель на начало полезных данных
  current_file = &file;
  if(res) {          
      print_UART("Can not go to file position \n");
      return 2;
  };
  print_UART( "success!\n");
  print_UART( "Reading file: \"");
  res = f_read (&file,&dac_buff[0],512,&cnt);       //загрузить буфер ЦАПа данными
    if(res) {          
        print_UART("Can not read file to buffer\n");
        return 3;
    };
  print_UART( "success!\n");
  //print_UART(dac_buff);
  //TIM6->CR1 |= TIM_CR1_CEN;                   //запустить преобразование
  TIM_Cmd(TIM6, ENABLE);
  //TIM_DMACmd(TIM6, TIM_DMA_Update, ENABLE);

  while(1)
  {
     while(!(DMA1->HISR & DMA_HISR_HTIF5)) {}   //ждем освобождение первой части буфера
     f_read (&file,&dac_buff[0],256,&cnt);    //загрузить ее данными
     DMA1->HIFCR |= DMA_HIFCR_CHTIF5;             //сбросить флаг
     if(cnt<256)break;                        //если конец файла
 
     while(!(DMA1->HISR & DMA_HISR_TCIF5)) {}   //ждем освобождение второй части буфера
     f_read (&file,&dac_buff[256],256,&cnt);  //загрузить ее данными
     DMA1->HIFCR |= DMA_HIFCR_CTCIF5;             //сбросить флаг
     if(cnt<256)break;                        //если конец файла
  }
 
 while(1) {
     dac_loop_do();
 }
  TIM6->CR1 &= ~TIM_CR1_CEN;                  //остановить преобразование
  f_close(&file);                             //закрыть файл
  return 0;                                   //успешное завершение ф-ии
}



unsigned char i=0;
int first = 0;
int second = 0;

int init2(void) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
//     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); 
    
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitStructure;
//    DMA_InitTypeDef DMA_INIT;
    TIM_TimeBaseInitTypeDef TIM6_TimeBase;
    
    // Init GPIO with AF function
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    TIM_TimeBaseStructInit(&TIM6_TimeBase); 
    TIM6_TimeBase.TIM_Period        = 1;         
    TIM6_TimeBase.TIM_Prescaler     = 875 + 1;       
    TIM6_TimeBase.TIM_ClockDivision = 0;  
    TIM6_TimeBase.TIM_CounterMode   = TIM_CounterMode_Up;  
    TIM6_TimeBase.TIM_ClockDivision = TIM_CKD_DIV1;
//    TIM6_TimeBase.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM6, &TIM6_TimeBase);
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    //TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
    
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
    
             // Init DAC
    DAC_StructInit(&DAC_InitStructure);
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None; 
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable; // try DAC_OutputBuffer_Enable later
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    
      FRESULT res;                                //для возвращаемого функциями результата
      FIL file;                                   //файловый объект
      UINT cnt;                                   //количество реально прочитанных байт
      FATFS FatFs;
      
      print_UART("Mounting volume...");
      res = f_mount(&FatFs, "", 1);
      if(res) { print_UART("Can not mount volume\n"); return 4;};
      print_UART("Opening file: \"test.txt\"...");
      res = f_open( &file, "output.wav", FA_READ );   //открыть файл FileName для чтения
      if(res) { print_UART("File not opened\n"); return 1;};
      print_UART( "success!\n");
      print_UART( "Seek position: \"");
      res = f_lseek(&file,0x2c);        //переместить указатель на начало полезных данных
      if(res) {print_UART("Can not go to file position \n"); return 2;};
      print_UART( "Reading file: \"");
      res = f_read (&file,&dac_buff[0],512,&cnt);       //загрузить буфер ЦАПа данными
      if(res) { print_UART("Can not read file to buffer\n"); return 3;};
      print_UART( "success!\n");  
    
    TIM_Cmd(TIM6, ENABLE);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
    DAC_Cmd(DAC_Channel_1, ENABLE);
    while(1){
        while(first == 0) {}   //ждем освобождение первой части буфера
        f_read (&file,&dac_buff[0],256,&cnt);    //загрузить ее данными
        first = 0;
        if(cnt<256)break;                        //если конец файла
 
        while(second == 0) {}    //ждем освобождение второй части буфера
        f_read (&file,&dac_buff[256],256,&cnt);  //загрузить ее данными
        second = 0;
        if(cnt<256)break;                        //если конец файла
    }
 
    TIM6->CR1 &= ~TIM_CR1_CEN;                  //остановить преобразование
    f_close(&file);                             //закрыть файл
    return 0;   
    
}

void TIM6_DAC_IRQHandler(void) {
  //print_UART("Interrupt\n");
  TIM6->SR &= ~TIM_SR_UIF; //Сбрасываем флаг UIF

  DAC->DHR8R1=dac_buff[i]; //Запихиваем в ЦАП очередной элемент массива
//  print_UART(&(dac_buff[i]));
  i += 1;
  if(i == 256) {
      first = 1;
  }
  if( i == 512 ) {
      second = 1;
      i = 0;
  }
 // if (i==512) i=0; //Если вывели в ЦАП все 32 значения то начинаем заново
}












void test_dac_manager1(void){
  GPIO_InitTypeDef gpio_A;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);                  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

  gpio_A.GPIO_Pin  = GPIO_Pin_4;
  gpio_A.GPIO_Mode = GPIO_Mode_AN;
  gpio_A.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &gpio_A);
  
  print_UART("GPIo Inited");

  TIM6_Config();  
  print_UART("TIM6 inited");
  
  DAC1_Config();
  
  print_UART("DAC1 inited");
 
 
}

static void TIM6_Config(void) {
  TIM_TimeBaseInitTypeDef TIM6_TimeBase;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
 
  TIM_TimeBaseStructInit(&TIM6_TimeBase); 
  TIM6_TimeBase.TIM_Period        = (uint16_t)TIM_PERIOD;          
  TIM6_TimeBase.TIM_Prescaler     = 0;       
  TIM6_TimeBase.TIM_ClockDivision = 0;    
  TIM6_TimeBase.TIM_CounterMode   = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM6, &TIM6_TimeBase);
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

  TIM_Cmd(TIM6, ENABLE);
}


static void DAC1_Config(void) {
  DAC_InitTypeDef DAC_INIT;
  DMA_InitTypeDef DMA_INIT;
  
  DAC_StructInit(&DAC_INIT);
  DAC_INIT.DAC_Trigger        = DAC_Trigger_T6_TRGO;
  DAC_INIT.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_INIT.DAC_OutputBuffer   = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_INIT);
  
  

  DMA_DeInit(DMA1_Stream5);
  DMA_INIT.DMA_Channel            = DMA_Channel_7;  
  DMA_INIT.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12L1;//->DHR12R1;// &DAC->DHR8R1;
  DMA_INIT.DMA_Memory0BaseAddr    = (uint32_t)&functionss;
  DMA_INIT.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
  DMA_INIT.DMA_BufferSize         = SINE_RES;
  DMA_INIT.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_INIT.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_INIT.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_INIT.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
  DMA_INIT.DMA_Mode               = DMA_Mode_Circular;
  DMA_INIT.DMA_Priority           = DMA_Priority_High;
  DMA_INIT.DMA_FIFOMode           = DMA_FIFOMode_Disable;         
  DMA_INIT.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_INIT.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_INIT.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream5, &DMA_INIT);

  DMA_Cmd(DMA1_Stream5, ENABLE);
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_DMACmd(DAC_Channel_1, ENABLE);
}
*/