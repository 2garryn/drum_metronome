#include "btn_manager.h"

/* 2 modes:

1. Simple click mode
2. Long Click mode
3. Multiclick mode

callback: btn_callback_clicked_0_0(Event)

Event: 
 KEY_DOWN
 KEY_UP_CLICK
 KEY_UP_MULTICLICK
 KEY_UP_LONGCLICK
 KEY_CLICK_MULTICLICK

1. Simple mode:
 KEY_DOWN -> KEY_UP_CLICK // does not matter how much time user holds button

2. Long click Mode:
 a) User holds button short time:
  KEY_DOWN -> KEY_UP_CLICK
 b) User holds button long time:
  KEY_DOWN -> KEY_UP_LONGCLICK

3) Multi click 
 KEY_DOWN -> KEY_CLICK_MULTICLICK -> ... -> KEY_CLICK_MULTICLICK ->  KEY_UP_CLICK

*/

typedef struct port_mode_st {
    uint16_t pin;
    uint8_t mode;
} port_mode_st;

typedef struct stack_event_st {
    uint16_t pin;
    GPIO_TypeDef* port;
    uint8_t event;
} stack_event_st;

static uint8_t get_current_pressed();
static void process_timer();
static void btn_timer_init();

static void set_button_mode_do(port_mode_st* port_modes, uint8_t port_length, uint16_t pin, uint8_t mode);
static uint8_t get_button_mode_do(port_mode_st* port_modes, uint8_t port_length, uint16_t pin);
static void continue_process_simple_mode();
static void continue_process_long_mode();
static void continue_process_multiclick_mode();
static uint8_t is_button_pressed(GPIO_TypeDef* volatile port, volatile uint16_t pin);
static void reset_button_processing();
static uint8_t push_to_queue(stack_event_st st);
static uint8_t pop_from_queue(stack_event_st * st);
static void process_events_from_queue();
static void send_event(uint8_t event, GPIO_TypeDef* volatile port, volatile uint16_t pin);

struct port_mode_st port0_modes[] = {
    {BTN_PORT_0_PIN_0, SIMPLE_MODE},
    {BTN_PORT_0_PIN_1, SIMPLE_MODE},
    {BTN_PORT_0_PIN_2, SIMPLE_MODE},
    {BTN_PORT_0_PIN_3, SIMPLE_MODE}
};

struct port_mode_st port1_modes[] = {
    {BTN_PORT_1_PIN_0, SIMPLE_MODE},
    {BTN_PORT_1_PIN_1, SIMPLE_MODE},
    {BTN_PORT_1_PIN_2, SIMPLE_MODE},
    {BTN_PORT_1_PIN_3, SIMPLE_MODE}
};



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
  //  TIM3_TimeBase.TIM_Period        = 50000 - 1;         
  //  TIM3_TimeBase.TIM_Prescaler     = 336 - 1;       
    TIM3_TimeBase.TIM_Period = 84 - 1; // 1 Mhz  
    TIM3_TimeBase.TIM_Prescaler = 1000 - 1 ;
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
    btn_init_port_pins(BTN_PORT_0, BTN_PORT_0_RCC, BTN_PORT_0_PINS);
    btn_init_port_pins(BTN_PORT_1, BTN_PORT_1_RCC, BTN_PORT_1_PINS);
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


void btn_set_button_mode_simple(GPIO_TypeDef* port, uint16_t pin) {
    btn_set_button_mode(port, pin, SIMPLE_MODE);
};
void btn_set_button_mode_multiclick(GPIO_TypeDef* port, uint16_t pin) {
    btn_set_button_mode(port, pin, MULTICLICK_MODE);
};
void btn_set_button_mode_long(GPIO_TypeDef* port, uint16_t pin) {
    btn_set_button_mode(port, pin, LONG_MODE);
};

void btn_timer_enable() {
    TIM_Cmd(TIM3, ENABLE);
}

void btn_timer_disable() {
    TIM_Cmd(TIM3, DISABLE);
}

void btn_set_button_mode(GPIO_TypeDef* port, uint16_t pin, uint8_t mode) {
    if(port == BTN_PORT_0) {
        set_button_mode_do(port0_modes, 4, pin, mode);
    } else if(port == BTN_PORT_1) {
        set_button_mode_do(port1_modes, 4, pin, mode);
    }
}

void set_button_mode_do(port_mode_st* port_modes, uint8_t port_length, uint16_t pin, uint8_t mode) {
    LOGD("Needed Pin:: ", pin);
    for(uint8_t i = 0; i < port_length; i++) {
        LOGD("For Pin:: ", port_modes[i].pin);
        if(port_modes[i].pin == pin) {
           port_modes[i].mode = mode; 
        }
    }
}

uint8_t get_button_mode(volatile GPIO_TypeDef* port, volatile uint16_t pin) {
    if(port == BTN_PORT_0) {
        return get_button_mode_do(port0_modes, 4, pin);
    };
    if(port == BTN_PORT_1) {
        return get_button_mode_do(port1_modes, 4, pin);
    }
    return SIMPLE_MODE;
}

uint8_t get_button_mode_do(port_mode_st* port_modes, uint8_t port_length, volatile uint16_t pin) {
    for(uint8_t i = 0; i < port_length; i++) {
        if(port_modes[i].pin == pin) {
           return port_modes[i].mode;
        }
    }
    return SIMPLE_MODE;
}

void btn_loop() {
    //send_next_event();
    process_events_from_queue();
}


struct stack_event_st temp_st; 

void process_events_from_queue() {
    if(pop_from_queue(&temp_st) == TRUE) {
        if(temp_st.port == BTN_PORT_0) {
            if(temp_st.pin == BTN_PORT_0_PIN_0) btn_callback_clicked_0_0(temp_st.event); 
            else if(temp_st.pin == BTN_PORT_0_PIN_1) btn_callback_clicked_0_1(temp_st.event); 
            else if(temp_st.pin == BTN_PORT_0_PIN_2) btn_callback_clicked_0_2(temp_st.event);
            else if(temp_st.pin == BTN_PORT_0_PIN_3) btn_callback_clicked_0_3(temp_st.event);
        } else if(temp_st.port == BTN_PORT_1) {
            if(temp_st.pin == BTN_PORT_1_PIN_0) btn_callback_clicked_1_0(temp_st.event); 
            else if(temp_st.pin == BTN_PORT_1_PIN_1) btn_callback_clicked_1_1(temp_st.event); 
            else if(temp_st.pin == BTN_PORT_1_PIN_2) btn_callback_clicked_1_2(temp_st.event);
            else if(temp_st.pin == BTN_PORT_1_PIN_3) btn_callback_clicked_1_3(temp_st.event);  
        }
    } 
}

void TIM3_IRQHandler(void) {
    TIM3->SR &= ~TIM_SR_UIF; 
    process_timer();
}


GPIO_TypeDef* volatile current_port;
volatile uint16_t current_pin;
volatile uint8_t current_mode;
volatile uint8_t is_button_processing;

void process_timer() {
    if(is_button_processing == FALSE) {
        if(get_current_pressed() == TRUE){
            is_button_processing = TRUE;
            current_mode = get_button_mode(current_port, current_pin);
            send_event(KEY_DOWN, current_port, current_pin);
        }
    } else {
        switch(current_mode) {
            case SIMPLE_MODE:
                continue_process_simple_mode();
                break;
            case LONG_MODE:
                continue_process_long_mode();
                break;
            case MULTICLICK_MODE:
                continue_process_multiclick_mode();
                break;
        }
    }
}


uint8_t get_current_pressed() {
    uint16_t port0 = ~(GPIO_ReadInputData(BTN_PORT_0));
    if(port0 & BTN_PORT_0_PIN_0)      { current_port = BTN_PORT_0; current_pin = BTN_PORT_0_PIN_0; return TRUE; }
    else if(port0 & BTN_PORT_0_PIN_1) { current_port = BTN_PORT_0; current_pin = BTN_PORT_0_PIN_1; return TRUE; }
    else if(port0 & BTN_PORT_0_PIN_2) { current_port = BTN_PORT_0; current_pin = BTN_PORT_0_PIN_2; return TRUE; }
    else if(port0 & BTN_PORT_0_PIN_3) { current_port = BTN_PORT_0; current_pin = BTN_PORT_0_PIN_3; return TRUE; }

    uint16_t port1 = ~(GPIO_ReadInputData(BTN_PORT_1));
    if(port1 & BTN_PORT_1_PIN_0)      { current_port = BTN_PORT_1; current_pin = BTN_PORT_1_PIN_0; return TRUE; }
    else if(port1 & BTN_PORT_1_PIN_1) { current_port = BTN_PORT_1; current_pin = BTN_PORT_1_PIN_1; return TRUE; }
    else if(port1 & BTN_PORT_1_PIN_2) { current_port = BTN_PORT_1; current_pin = BTN_PORT_1_PIN_2; return TRUE; }
    else if(port1 & BTN_PORT_1_PIN_3) { current_port = BTN_PORT_1; current_pin = BTN_PORT_1_PIN_3; return TRUE; }

    return FALSE;
}

#define SIMPLE_MODE_MS 5

volatile uint8_t simple_mode_counter = SIMPLE_MODE_MS;
volatile uint8_t simple_mode_key_up_sent = FALSE;

void continue_process_simple_mode() {
    if(!simple_mode_key_up_sent) {
        send_event(KEY_UP_CLICK, current_port, current_pin);
        simple_mode_key_up_sent = TRUE;
    }
    if(is_button_pressed(current_port, current_pin) == TRUE) {
        return;
    }
    simple_mode_counter--;
    if(simple_mode_counter == 0) {
        simple_mode_counter = SIMPLE_MODE_MS;
        simple_mode_key_up_sent = FALSE;
        reset_button_processing();
    }
}


#define LONG_MODE_MS 1500
#define LONG_MODE_SHORT_MS 5

volatile uint16_t long_mode_counter = 0;
volatile uint8_t long_mode_short_counter = 0;
volatile uint8_t long_click_event_sent = FALSE;

void continue_process_long_mode() {
    if(is_button_pressed(current_port, current_pin) == TRUE) {
        long_mode_counter++;
        long_mode_short_counter = 0;
    } else {
        long_mode_short_counter++;
    }
    if(long_mode_counter < LONG_MODE_MS && long_mode_short_counter >= LONG_MODE_SHORT_MS) {
        long_mode_counter = 0;
        long_mode_short_counter = 0;
        long_click_event_sent = FALSE;
        send_event(KEY_UP_CLICK, current_port, current_pin);
        reset_button_processing();
        return;
    } else if (long_mode_counter >= LONG_MODE_MS && long_click_event_sent == FALSE) {
        send_event(KEY_UP_LONGCLICK, current_port, current_pin);
        long_click_event_sent = TRUE;
    } 
    if(long_mode_short_counter >= LONG_MODE_SHORT_MS)  {
        long_mode_short_counter = 0;
        long_mode_counter = 0;
        long_click_event_sent = FALSE;
        reset_button_processing();
    }
}

#define MULTICLICK_MODE_FIRST_DELAY 1000
#define MULTICLICK_MODE_INTERVAL 50
#define MULTICLICK_KEYUP_INTERVAL 5

volatile uint16_t multiclick_first_delay_counter = 0;
volatile uint16_t multiclick_interval_counter = 0;
volatile uint8_t multiclick_keyup_interval_counter = 0;
//uint8_t multiclick_first_delay_passed = FALSE;

void continue_process_multiclick_mode() {
    if(is_button_pressed(current_port, current_pin) == TRUE) {
        multiclick_keyup_interval_counter = 0;
        if(multiclick_first_delay_counter < MULTICLICK_MODE_FIRST_DELAY) {
            multiclick_first_delay_counter++;
            return;
        }
        multiclick_interval_counter++;
        if(multiclick_interval_counter < MULTICLICK_MODE_INTERVAL) {
            return;
        }
        send_event(KEY_UP_MULTICLICK, current_port, current_pin);
        multiclick_interval_counter = 0;
    } else {
        multiclick_keyup_interval_counter++;
        if(multiclick_keyup_interval_counter < MULTICLICK_KEYUP_INTERVAL) {
            return;
        } else {
            multiclick_first_delay_counter = 0;
            multiclick_interval_counter = 0;
            multiclick_keyup_interval_counter = 0;
            send_event(KEY_UP_CLICK, current_port, current_pin);
            reset_button_processing();
        }
    }

}

void reset_button_processing() {
    LOGD("Reset button processing", 0);
    is_button_processing = FALSE;
}

uint8_t is_button_pressed(GPIO_TypeDef*  volatile port, volatile uint16_t pin) {
    uint16_t port_result = ~(GPIO_ReadInputData(port));
    if(port_result & pin) return TRUE;
    return FALSE;
}

void send_event(uint8_t event, GPIO_TypeDef* volatile port, volatile uint16_t pin) {
    struct stack_event_st st;
    st.event = event;
    st.port = port;
    st.pin = pin;
    push_to_queue(st);
}


#define EVENTS_STACK_SIZE 8
volatile struct  stack_event_st events_stack[EVENTS_STACK_SIZE];
volatile uint8_t events_stack_counter = 0;
volatile uint8_t  write_pos = 0;
volatile uint8_t  read_pos  = 0;

uint8_t push_to_queue(stack_event_st st) {
    uint8_t next_write_pos = write_pos + 1;
    if(next_write_pos >= EVENTS_STACK_SIZE) {
        next_write_pos = 0;
    };
    if(next_write_pos == read_pos) {
        LOGE("btn_manager: Event buffer is full", 0);
        return FALSE;
    }
    events_stack[write_pos] = st;
    write_pos = next_write_pos;
 //   LOGD("Write pos", write_pos);
    return TRUE;
}


uint8_t pop_from_queue(stack_event_st * st) {
    if(write_pos == read_pos) {
        return FALSE;
    }
    uint8_t next_read_pos = read_pos + 1;
    if(next_read_pos >= EVENTS_STACK_SIZE) {
        next_read_pos = 0;
    }
    *st = events_stack[read_pos];
    read_pos = next_read_pos;
  //  LOGD("Read pos", read_pos);
    return TRUE;
}