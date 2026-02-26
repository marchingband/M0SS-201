#include <stdlib.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include "bflb_gpio.h"
#include "rgb_led.h"

#include "midi.h"
#include "gpio.h"
#include "flash.h"

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
uint64_t reset_mode_started = 0;
bool encoder_pressed = false;
int alt_switch_state = 0;

void set_param_selected(int num, bool alt, bool cc)
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
    else if(cc)
    {
        struct CC *ccs = get_ccs();
        struct CC cc = ccs[num];
        if(cc.p == 0){
            return;
        }
        set_led_display(cc.p, cc.min, cc.max);
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
    // if we get here it is a valid Alt, Mod or CC
    param_selected.button = num;
    param_selected.alt = alt;
    param_selected.cc = cc;
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

bool alt_held(void){return button_state[BUTTON_NUM_ALT] == 0;}
bool OS1_held(void){return button_state[BUTTON_NUM_OS1] == 0;}
bool SG_held(void){return button_state[BUTTON_NUM_SG] == 0;}
bool ENV_held(void){return button_state[BUTTON_NUM_ENV] == 0;}

const int param_matrix[3][12] = {
    {54,55,56,57,58,59,60,61,62,63,64,65},
    {42,43,44,45,46,47,48,49,50,51,52,53},
    {30,31,32,33,34,35,36,37,38,39,40,41},
};

void matrix_pick(int num){
    struct CC cc = get_ccs()[num];
    if(cc.p !=0){
        set_param_selected(num, false, true);
    }
}

bool check_martix_press(int num){
    int matrix_column_selected = -1;
    int matrix_row_selected = ENV_held() ? 2 : SG_held() ? 1 : OS1_held() ? 0 : -1;
    if(matrix_row_selected == -1){
        return false;
    }
    // printf("%s held", matrix_row_selected == 2 ? "ENV" : matrix_row_selected == 1 ? "SG" : matrix_row_selected == 0 ? "OS1" : "NONE");
    switch (num){
        case BUTTON_NUM_MATRIX_COLUMN_LEN:
            matrix_column_selected = 0;
            break;
        case BUTTON_NUM_MATRIX_COLUMN_RATE:
            matrix_column_selected = 1;
            break;
        case BUTTON_NUM_MATRIX_COLUMN_RCL:
            matrix_column_selected = 2;
            break;
        case BUTTON_NUM_MATRIX_COLUMN_SMTH:
            matrix_column_selected = 3;
            break;
        case BUTTON_NUM_MATRIX_COLUMN_ATT:
            matrix_column_selected = 4;
            break;
        case BUTTON_NUM_MATRIX_COLUMN_REL:
            matrix_column_selected = 5;
            break;
        case BUTTON_NUM_MATRIX_COLUMN_OS1:
            matrix_column_selected = 6;
            break;
        case BUTTON_NUM_MATRIX_COLUMN_OS2:
            matrix_column_selected = 7;
            break;
        case BUTTON_NUM_MATRIX_COLUMN_BUS :
            matrix_column_selected = 8;
            break;
        case BUTTON_NUM_MATRIX_COLUMN_IDX:
            matrix_column_selected = 9;
            break;
        case BUTTON_NUM_MATRIX_COLUMN_TIMB:
            matrix_column_selected = 10;
            break;
        case BUTTON_NUM_MATRIX_COLUMN_AMP:
            matrix_column_selected = 11;
            break;          
        default:
            break;
    }

    if(matrix_column_selected != -1 && matrix_row_selected != -1){
        int param = param_matrix[matrix_row_selected][matrix_column_selected];
        matrix_pick(param);
        // printf("matrix picked %d", param);
        return true;
    }

    return false;
}

void handle_encoder_press(bool down)
{
    struct Sigs *sigs = get_signals();
    *sigs->gate = down ? 1 : 0;
    encoder_pressed = down;
}

extern int mode_learn_current_cc;
extern int learn_mode_state;

static void process_buttons(){
    for (int i =0; i< NUM_BUTTONS; i++)
    {
        bool state = button_state[i];
        int ui_mode = get_ui_mode();
        uint64_t now = bflb_mtimer_get_time_us();

        // check params reset logic
        if(ui_mode == UI_MODE_RST){
            if(button_state[BUTTON_NUM_ALT] || button_state[8] || button_state[BUTTON_NUM_ENCODER]) // one was released
            {
                set_ui_mode(UI_MODE_PLAY);
            }
             
            if(now - reset_mode_started > ONE_SECOND)
            {
                if(button_state[2] == 0 && button_state[17] == 0) // factory reset
                {
                    // factory reset
                    set_ui_mode(UI_MODE_FACTORY_RST);
                } else {
                    reset_params();
                    set_ui_mode(UI_MODE_PLAY);
                }
            }
            return;
        } else if (ui_mode == UI_MODE_FACTORY_RST) {
            if (button_state[BUTTON_NUM_ALT] || button_state[8] || button_state[BUTTON_NUM_ENCODER]) {
                factory_reset();
            }
            return;
        }

        // check for presses
        if(button_pushes[i] == 0 && state == 0 && ((now - button_time[i]) > DEBOUNCE_TIME)) // not pushed
        {
            button_pushes[i] = 1;
            button_state[i] = 0;
            button_time[i] = now;

            printf("button %d pushed\n", i);

            if( i == BUTTON_NUM_ENCODER ){
                // check for reset command
                if(button_state[BUTTON_NUM_ALT] == 0 && button_state[8] == 0) // alt and idx are held
                {
                    set_ui_mode(UI_MODE_RST);
                    reset_mode_started = bflb_mtimer_get_time_us();
                } else {
                    handle_encoder_press(true);
                }
                continue;
            }

            if(check_martix_press(i)){
                continue;
            };

            if (i == BUTTON_NUM_ALT) alt_consumed = false;
            
            // set ui modes
            if (alt_held() && i == BUTTON_NUM_LEARN) {
                set_ui_mode(UI_MODE_LEARN);
                alt_consumed = true;
                continue;
            }
            // same for load and save

            switch (ui_mode) {
                case UI_MODE_PLAY: {
                    handle_normal_mode_button_press(i);
                    break;
                }
                case UI_MODE_LEARN: {
                    if (alt_held()) {
                        int cc_num = get_cc(i, true);
                        if (cc_num != -1) {
                            mode_learn_current_cc = cc_num;
                            learn_mode_state = LEARN_MODE_SELECTED;
                            alt_consumed = true;
                        } 
                    } else {
                        int cc_num = get_cc(i, false);
                        if (cc_num != -1) {
                            mode_learn_current_cc = cc_num;
                            learn_mode_state = LEARN_MODE_SELECTED;
                        }
                    }
                    break;
                }
            }
        }
        // check for releases
        else if(button_pushes[i] == 1 && state == 1 && (now - button_time[i]) > DEBOUNCE_TIME) 
        {
            button_pushes[i] = 0;
            button_state[i] = 1;
            button_time[i] = now;

            printf("button %d released\n", i);

            if( i == BUTTON_NUM_ENCODER ){
                handle_encoder_press(false);
                continue;
            }else if(i == BUTTON_NUM_ALT){
                alt_switch_state = ALT_SWITCH_STATE_NONE;
                if (ui_mode != UI_MODE_PLAY && !alt_consumed) set_ui_mode(UI_MODE_PLAY);
            }else if(i == 4 && !alt_held()){ // hi-lo
                alt_switch_state = ALT_SWITCH_STATE_NONE;
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

void buttons_init(){
    mux_gpio_init();
    connected_buttons_init();
}

void handle_normal_mode_button_press(int btn_idx) {
    struct Sel *Sels = get_sels();
    struct AltSel *AltSels = get_alt_sels();
    struct Mod *Mods = get_mods();
    struct Alt *Alts = get_alts();

    if(alt_held()) {
        struct AltSel *alt_sel = &AltSels[btn_idx];
        if(alt_sel->p != 0){
            float cur = *alt_sel->p;
            if(cur < alt_sel->max){
                *alt_sel->p += 1;
            }else{
                *alt_sel->p = alt_sel->min;
            }

            // toggle switch state
            if(btn_idx == 4 || btn_idx == 20){ // k-t and quantize
                alt_switch_state = *alt_sel->p == 0 ? ALT_SWITCH_STATE_OFF : ALT_SWITCH_STATE_ON;
            }
        }
        struct Alt *alt = &Alts[btn_idx];
        if(alt->p != 0){
            set_param_selected(btn_idx, true, false);
        }   
    } else {
        struct Sel *sel = &Sels[btn_idx];
        if(sel->p != 0){
            float cur = *sel->p;
            if(cur < sel->max){
                *sel->p += 1;
            }else{
                *sel->p = sel->min;
            }
            // toggle switch state
            if(btn_idx == 4){ // hi-lo
                alt_switch_state = *sel->p == 0 ? ALT_SWITCH_STATE_OFF : ALT_SWITCH_STATE_ON;
            }
        }
        struct Mod *mod = &Mods[btn_idx];
        if(mod->p != 0){
            set_param_selected(btn_idx, false, false);
        }                
    }
}