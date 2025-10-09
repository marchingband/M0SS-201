#include <stdlib.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include "bflb_gpio.h"
#include "rgb_led.h"

#include "midi.h"
#include "gpio.h"

extern int alt_switch_state;
extern int cur_midi_chan;
extern int cur_disp_num;

struct bflb_device_s *b_gpio;

const uint8_t mux_code[8][3] = {
    {0,0,0},
    {0,0,1},
    {0,1,0},
    {0,1,1},
    {1,0,0},
    {1,0,1},
    {1,1,0},
    {1,1,1}
};

const uint8_t mux_pins[24] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};

#define MUX_A0_PIN 27
#define MUX_A1_PIN 28
#define MUX_A2_PIN 29

#define MUX_0_DRAIN_PIN 30
#define MUX_1_DRAIN_PIN 0
#define MUX_2_DRAIN_PIN 1

#define ENCODER_BUTTON_PIN 20
#define BUTTON_25_PIN 12
#define BUTTON_26_PIN 13

#define DEBOUNCE_TIME 1000 * 50 // 50 ms

// button roles
// #define BUTTON_NUM_SAVE 3
// #define BUTTON_NUM_LOAD 1
// #define BUTTON_NUM_ALT 22
// #define BUTTON_NUM_LEARN 5
// #define BUTTON_NUM_ENCODER 26
// #define BUTTON_NUM_MIDI_CHAN 0 // ?

uint8_t button_pushes[NUM_BUTTONS]    = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t button_state[NUM_BUTTONS]     = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
uint64_t button_time[NUM_BUTTONS]     = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

struct Param param_selected = {
    .button = -1,
    .alt = false
};

struct Param get_param_selected(void){
    return param_selected;
}

bool set_midi_chan_active = false;
bool asb_active = false;
int cur_asb = 0;

void set_param_selected(int num, bool alt)
{
    if(alt)
    {
        struct Alt *Alts = get_alts();
        struct Alt alt = Alts[num];
        if(alt.p == 0)
        {
            return;
        }
        // printf("set led %f %f %f\n\r", val, alt.min, alt.max);
        set_led_display(alt.p, alt.min, alt.max);
    }
    else
    {
        struct Mod *Mods = get_mods();
        struct Mod mod = Mods[num];
        if(mod.p == 0){
            return;
        }
        // printf("set led %f %f %f\n\r", val, mod.min, mod.max);
        set_led_display(mod.p, mod.min, mod.max);
    }
    // if we get here it is a valid Alt or Mod
    param_selected.button = num;
    param_selected.alt = alt;
}

int button_label(int num){
    switch(num){
        case 0: return 1;
        case 1: return 2;
        case 2: return 3;
        case 3: return 4;
        case 6: return 5;
        case 7: return 6;
        case 8: return 7;
        case 9: return 8;
        case 10: return 9;
        case 11: return 10;
        case 12: return 11;
        case 13: return 12;
        case 15: return 13;
        case 18: return 14;
        case 20: return 15;
        case 24: return 16;
        case 23: return 17;
        default: return 0;
    }
    return 0;
}

static void mux_gpio_init(void){
    b_gpio = bflb_device_get_by_name("gpio");

    // drains need pullups
    bflb_gpio_init(b_gpio, MUX_0_DRAIN_PIN, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(b_gpio, MUX_1_DRAIN_PIN, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(b_gpio, MUX_2_DRAIN_PIN, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);

    bflb_gpio_init(b_gpio, MUX_A0_PIN, GPIO_OUTPUT | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(b_gpio, MUX_A1_PIN, GPIO_OUTPUT | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(b_gpio, MUX_A2_PIN, GPIO_OUTPUT | GPIO_SMT_EN | GPIO_DRV_0);
}

static void connected_buttons_init(){
    bflb_gpio_init(b_gpio, BUTTON_25_PIN, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(b_gpio, BUTTON_26_PIN, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(b_gpio, ENCODER_BUTTON_PIN, GPIO_INPUT | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_0);
}

const uint8_t mux_drain_pins[] = { MUX_0_DRAIN_PIN, MUX_1_DRAIN_PIN, MUX_2_DRAIN_PIN };

void read_buttons(){
    // poll mux
    for(int i=0; i<8; i++) // 8 pins per mux
    {
        // set the mux bits
        for(int j=0; j<3; j++)
        {
            if(mux_code[i][j])
                bflb_gpio_set(b_gpio, MUX_A0_PIN + j);
            else
                bflb_gpio_reset(b_gpio, MUX_A0_PIN + j);
        }
        
        bflb_mtimer_delay_us(5); // let it settle
        // bflb_mtimer_delay_us(2); // let it settle

        // read the pins
        for(int k=0; k<3; k++)
        {
            uint8_t idx = i + (k * 8);
            if(idx == 15 || idx == 21){
                // these are bad buttons
                button_state[idx] = 1;
                continue;
            }
            button_state[idx] = bflb_gpio_read(b_gpio, mux_drain_pins[k]);
        }
    }
    button_state[24] = bflb_gpio_read(b_gpio, BUTTON_25_PIN);
    button_state[25] = bflb_gpio_read(b_gpio, BUTTON_26_PIN);
    button_state[26] = bflb_gpio_read(b_gpio, ENCODER_BUTTON_PIN);
}

bool alt_consumed = false;

bool alt_held(void)
{
    return button_state[BUTTON_NUM_ALT] == 0;
}

static void process_buttons(){
    for (int i =0; i< NUM_BUTTONS; i++)
    {
        bool state = button_state[i];
        int ui_mode = get_ui_mode();

        // check for presses
        if(button_pushes[i] == 0) // not pushed
        {
            if(state == 0) // maybe pushed ( 0 is low so maybe pressed )
            {
                uint64_t now = bflb_mtimer_get_time_us();
                if( (now - button_time[i]) > DEBOUNCE_TIME ) // pressed
                {
                    button_pushes[i] = 1;
                    button_state[i] = 0;
                    button_time[i] = now;
                    if( i == BUTTON_NUM_ENCODER ){
                        handle_encoder_press(true);
                    } else if( i == BUTTON_NUM_ALT ){
                        // handle_button_press(i);
                        alt_consumed = false;
                    } else if( alt_held() && get_asbs()[i].p != 0 ){
                        // its an asb
                        // printf("got asb on button %d\n", i);
                        cur_asb = i;
                        asb_active = true;
                        cur_disp_num = (int)*get_asbs()[i].p;
                        set_ui_mode(UI_MODE_DISP_NUM);
                    } else if( alt_held() && i == BUTTON_NUM_MIDI_CHAN ){
                        set_midi_chan_active = true;
                        cur_disp_num = cur_midi_chan;
                        set_ui_mode(UI_MODE_DISP_NUM);
                    } else if( ui_mode == UI_MODE_PLAY ){
                        set_param_selected(i, alt_held());
                    }
                    printf("button %d pushed\n\r", i);
                }
            }
        }

        // check for releases
        else if(button_pushes[i] == 1) // is pushed
        {
            if(state == 1) // maybe released ( 1 is high so released)
            {
                uint64_t now = bflb_mtimer_get_time_us();
                if( (now - button_time[i]) > DEBOUNCE_TIME ) // released
                {
                    button_pushes[i] = 0;
                    button_state[i] = 1;
                    button_time[i] = now;
                    printf("button %d released\n\r", i);
                    // chack for assignable events

                    if(asb_active && i == cur_asb)
                    {
                        asb_active == false;
                        set_ui_mode(UI_MODE_PLAY);
                    } 
                    else if(set_midi_chan_active && i == BUTTON_NUM_MIDI_CHAN)
                    {
                        set_midi_chan_active == false;
                        set_ui_mode(UI_MODE_PLAY);
                    } 
                    else if(i == BUTTON_NUM_ENCODER)
                    {
                        handle_encoder_press(false);
                    }
                    else if(i == BUTTON_NUM_ALT){
                        alt_switch_state = ALT_SWITCH_STATE_NONE;
                        if(!alt_consumed){
                            handle_button_press(i); // trigger alt event on release
                        }
                        asb_active = false;
                    }
                    else
                    {
                        alt_consumed = true;
                        handle_button_press(i); // trigger event on release
                    }
                }
            }
        }
    }
}

uint8_t *get_button_states(void)
{
    return button_state;
}

uint8_t *get_button_pushes(void)
{
    return button_pushes;
}

void clear_button_pushes(void)
{
    memset(button_pushes, 0, sizeof button_pushes); // clear it on every read
}

void poll_gpio(){
    read_buttons();
    process_buttons();
}

static void button_task(void* arg){
    while(1)
    {
        poll_gpio();
        vTaskDelay(10);
    }
}

void buttons_init(){
    mux_gpio_init();
    connected_buttons_init();
    // xTaskCreate(button_task, "button_task", 2048, NULL, 2, NULL);
}