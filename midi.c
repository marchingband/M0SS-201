#include <FreeRTOS.h>
#include "queue.h"
#include "bflb_mtimer.h"
#include "bflb_uart.h"
#include "bflb_gpio.h"
#include <string.h>
#include "math.h"
#include "dsp.h"
#include "gpio.h"
#include "midi.h"
#include "rgb_led.h"
#include "flash.h"
#include "dsp/interface.h"
extern DSP_Param_t dsp_ui[];
extern DSP_All_Signals all_sigs_raw;

#define MIDI_NOTE_OFF 8
#define MIDI_NOTE_ON 9
#define MIDI_AFTERTOUCH 0xD
#define MIDI_PITCH_BEND 0xE
#define MIDI_PROGRAM_CHANGE 12
#define MIDI_CC 11
#define MIDI_CC_VOLUME 7
#define MIDI_CC_PAN 10
#define MIDI_CC_EXP 11
#define MIDI_CC_SUSTAIN 64
#define MIDI_CC_RELEASE 72
#define MIDI_CC_MUTE 120
#define MIDI_CC_RESET 121

#define MIDI_CC_EQ_BASS 20
#define MIDI_CC_EQ_TREBLE 21

uint8_t *getMidiMsg(void);
uint8_t getMidiMsgLen(void);
bool parse(uint8_t byte);
void init_midi_parser(void);

void play(uint8_t note);
void stop(uint8_t note);
void set_note(uint8_t note);
void set_filter(uint8_t val);

static struct bflb_device_s *uart0;
QueueHandle_t uart_queue;

int cur_midi_chan = MIDI_CHANNEL_OMNI;

int ch;
int mode_learn_current_cc = 0;
int learn_mode_state = LEARN_MODE_READY;

static struct CC CCs[NUM_CC] = {0};
static struct Mod Mods[NUM_BUTTONS] = {0};
static struct Alt Alts[NUM_BUTTONS] = {0};
static struct Sel Sels[NUM_BUTTONS] = {0};
static struct AltSel AltSels[NUM_BUTTONS] = {0};
static struct Asb Asbs[NUM_BUTTONS] = {0};
static struct Sigs Signals = {
    .pitch = 0,
    .gate = 0,
    .sg = 0,
    .le = 0,
    .mo = 0,
    .mo_freq = 0,
    .amp = 0,
    // .lfo = 0,
    // .env = 0,
    .pitch_bend = 0
    // .vca = 0,
};

struct Sel *get_sels(void)
{
    return &Sels[0];
}

struct AltSel *get_alt_sels(void)
{
    return &AltSels[0];
}

struct Asb *get_asbs(void)
{
    return &Asbs[0];
}

struct Mod *get_mods(void)
{
    return &Mods[0];
}

struct Alt *get_alts(void)
{
    return &Alts[0];
}

struct CC *get_ccs(void)
{
    return &CCs[0];
}

struct Sigs *get_signals(void)
{
    return &Signals;
}

int get_mode_learn_current_cc(void)
{
    return mode_learn_current_cc;
}

void link_ui() {
    // attach parameters to their respective arrays
    printf("linking UI...\n");
    for (int i = 0; i < NUM_UI_ELS; i++) {
        DSP_Param_t param = dsp_ui[i];

        printf("idx %d, ptr %p\n", i, (void*)param.p);
        if (param.cc != 0) {
            struct CC *cc = &CCs[param.cc];
            cc->p = param.p;
            cc->min = param.min;
            cc->max = param.max;
            cc->init = param.init;
            cc->step = param.step;
            float steps = (param.max - param.min) / param.step;
            float per = steps / 1024.0;
            cc->mul = per;
            cc->numeric = param.numeric;
        }

        switch (param.type) {
            case PARAM_MOD: {
                struct Mod *mod = &Mods[param.btn];
                mod->p = param.p;
                mod->min = param.min;
                mod->max = param.max;
                mod->init = param.init;
                mod->step = param.step;
                float steps = (param.max - param.min) / param.step;
                float per = steps / 1024.0;
                mod->mul = per;
                mod->numeric = param.numeric;
                break;
            };
            case PARAM_ALT: {
                struct Alt *alt = &Alts[param.btn];
                alt->p = param.p;
                alt->min = param.min;
                alt->max = param.max;
                alt->init = param.init;
                alt->step = param.step;
                float steps = (param.max - param.min) / param.step;
                float per = steps / 1024.0;
                alt->mul = per;
                alt->numeric = param.numeric;
                break;
            };
            case PARAM_SEL: {
                struct Sel *sel = &Sels[param.btn];
                sel->p = param.p;
                sel->min = param.min;
                sel->max = param.max;
                sel->init = param.init;
                sel->numeric = param.numeric;
                break;
            };
            case PARAM_ALT_SEL: {
                struct AltSel *altsel = &AltSels[param.btn];
                altsel->p = param.p;
                altsel->min = param.min;
                altsel->max = param.max;
                altsel->init = param.init;
                altsel->numeric = param.numeric;
                break;
            };
            case PARAM_MTX: {
                // assigned as a CC, no other
                break;
            };
        }
    }
    // attach special signals
    all_sigs_raw.midi_chan = MIDI_CHANNEL_OMNI;
    Signals.pitch = &all_sigs_raw.pitch;
    *Signals.pitch = 48.0f; // initial value
    Signals.gate = &all_sigs_raw.gate;
    Signals.amp = &all_sigs_raw.ind_amp;
    Signals.le = &all_sigs_raw.ind_env;
    Signals.sg = &all_sigs_raw.ind_sg;
    Signals.mo = &all_sigs_raw.ind_os2;
    Signals.mo_freq = &all_sigs_raw.ind_os2_freq;
    Signals.pitch_bend = &all_sigs_raw.pitch_bend;
    *Signals.pitch_bend = 8192.0f;
}

void reset_params(void){
    int i;
    for(i=0; i<NUM_CC; i++){
        *(CCs[i].p) = CCs[i].init;
    }
}

int get_cc(int num, bool alt){
    printf("get_cc num:%d alt:%s\n", num, alt?"true":"false");
    float *p = NULL;
    if(alt){
        printf("get_cc alt held\n");
        struct Alt *Alts = get_alts();
        struct AltSel *AltSels = get_alt_sels();
        struct Asb *Asbs = get_asbs();
        struct Alt alt = Alts[num];
        struct AltSel alt_sel = AltSels[num];
        struct Asb asb = Asbs[num];
        if(alt.p != 0){
            p = alt.p;
        } else if (alt_sel.p != 0){
            p = alt_sel.p;
        } else if (asb.p != 0){
            p = asb.p;
        }
    } else {
        struct Mod *Mods = get_mods();
        struct Sel *Sels = get_sels();
        struct Mod mod = Mods[num];
        struct Sel sel = Sels[num];
        if(mod.p != 0){
            p = mod.p;
        } else if(sel.p != 0){
            p = sel.p;
        }
    }
    if(p == NULL){
        printf("nothing found for that button\n");
        return -1; // nothing found on that button press
    } 
    struct CC *CCs = get_ccs();
    // search all the CCs to find the matching pointer
    for(int i=0; i<NUM_CC; i++){
        if(CCs[i].p == p){
            return i;
        }
    }
    printf("found something but no CC found for that pointer\n");
    return -1;
}

uint8_t note_lifo[128] = {0};
size_t note_lifo_p = 0;

void push_note(uint8_t note){
    note_lifo[note_lifo_p++] = note;
}

void pop_note(void) {
    if (note_lifo_p > 0) note_lifo_p--;
}

uint8_t peek_note(void){
    return note_lifo[note_lifo_p - 1];
}

void remove_note(uint8_t note){
    int idx = -1;
    for(int i=0; i< note_lifo_p; i++){
        if(note_lifo[i] == note){
            idx = i;
            break;
        }
    }
    if(idx == -1) return;
    for(int i=idx; i< note_lifo_p; i++){
        note_lifo[i] = note_lifo [i + 1];
    }
    note_lifo_p --;
}

void midi_clear() {
    note_lifo_p = 0;
    *Signals.gate = 0.0f;
}

void handle_midi(uint8_t *msg, uint8_t len)
{
    // printf("msg len %d -> %d %d %d\n\r\r", len, msg[0], msg[1], msg[2]);
    
    uint8_t channel = msg[0] & 0b00001111;
    if (cur_midi_chan != (int)all_sigs_raw.midi_chan) {
        note_lifo_p = 0;
        *Signals.gate = 0;
        cur_midi_chan = (int)all_sigs_raw.midi_chan;
    }
    cur_midi_chan = (int)all_sigs_raw.midi_chan;
    if(cur_midi_chan != MIDI_CHANNEL_OMNI && cur_midi_chan != (channel + 1)) // channels are 0 indexed but we use 1 indexed
    {
        return;
    }

    // check for stupid note-offs
    uint8_t code = (msg[0] >> 4) & 0b00001111;
    if(code == MIDI_NOTE_ON){
        uint8_t velocity = msg[2] & 0b01111111;
        if(velocity == 0){
            code = MIDI_NOTE_OFF;
        }
    }

    int ui_mode = get_ui_mode();
    uint8_t *cc_map = &get_config()->cc_map[0];
    
    switch (code)
    {
    case MIDI_NOTE_ON:
    {
        uint8_t note = msg[1] & 0b01111111;
        // uint8_t velocity = msg[2] & 0b01111111;
        // set_note(note);
        // play(note);
        // *Signals.gate = 1;
        // *Signals.pitch = note; // the dsp converts to hz
        // push_note(note);
        uint8_t velocity = msg[2] & 0b01111111;
        if (ui_mode == UI_MODE_LEARN) {
            if (mode_learn_current_cc > 0) {
                cc_map[CC_MAP_VELOCITY_IDX] = mode_learn_current_cc;
                learn_mode_state = LEARN_MODE_LEARNED;
            }
        } else {
            *Signals.gate = 1;
            *Signals.pitch = note;
            push_note(note);

            // handle velocity
            uint8_t cc_num = cc_map[CC_MAP_VELOCITY_IDX];
            struct CC cc = CCs[cc_num];
            if (cc.p != 0) {
                float steps = cc.mul * (float)velocity * 8.0f;
                int whole_steps = round(steps);
                float val = (cc.step * whole_steps) + cc.min;
                *cc.p = val;
            }
        }
        // printf("noteon %d", note);
        break;
    }
    case MIDI_NOTE_OFF:
    {
        uint8_t note = msg[1] & 0b01111111;
        // stop(note);
        if(*Signals.pitch == note)
        {
            // the last note played has been stopped
            pop_note(); // remove the last note.
            if(note_lifo_p > 0){ // we have decremented note_lifo_p, if it is still positive, there are more notes.
                uint8_t next_note = peek_note();
                *Signals.pitch = next_note;
            } else {
                *Signals.gate = 0;
            }
        } else {
            remove_note(note);
        }
        break;
    }
    case MIDI_PROGRAM_CHANGE:
    {
        uint8_t voice = msg[1] & 0b01111111;
        load_preset(voice);
        break;
    }
    case MIDI_CC:
    {
        uint8_t cc_num_in = msg[1] & 0b01111111;
        uint8_t cc_val = msg[2] & 0b01111111;
        uint8_t cc_num = cc_map[cc_num_in - 1]; // -1 because ccs are indexed at 1

        if(ui_mode == UI_MODE_LEARN)
        {
            if(mode_learn_current_cc > 0)
            {
                if (cc_num_in <= 0) break;
                cc_map[cc_num_in - 1] = mode_learn_current_cc; // -1 because ccs are indexed at 1
                learn_mode_state = LEARN_MODE_LEARNED;
            }
            // printf("no cc_learn selected\n\r\r");
            break;
        }
        
        if(cc_num >= NUM_CC)
        {
            // printf("CC %d is too high", cc_num);
            break;
        }
        
        if(cc_num == 8) // maybe a modifier
        {
            uint8_t should_set_volume = true;
            uint8_t *button_states = get_button_states();
            for(int i=0; i<NUM_BUTTONS; i++)
            {
                if(button_states[i] == 0) // button is pressed means 0
                {
                    if(alt_held())
                    {
                        struct Alt alt = Alts[i];
                        if(alt.p == 0)
                        {
                            // printf("got unknown Alt %d\n\r\r", i);
                        }
                        else
                        {
                            should_set_volume = false; // data is redirected, do not change volume
                            float steps = alt.mul * (float)cc_val;
                            int whole_steps = round(steps);
                            float val = (alt.step * whole_steps) + alt.min;
                            *alt.p = val;
                            // printf("set led %f %f %f\n\r", val, alt.min, alt.max);
                            set_led_display(alt.p, alt.min, alt.max, alt.numeric);
                        }
                    }
                    else
                    {
                        struct Mod mod = Mods[i];
                        if(mod.p == 0)
                        {
                            // printf("got unknown Mod %d\n\r\r", i);
                        }
                        else
                        {
                            should_set_volume = false; // data is redirected, do not change volume
                            float steps = mod.mul * (float)cc_val;
                            int whole_steps = round(steps);
                            float val = (mod.step * whole_steps) + mod.min;
                            *mod.p = val;
                            // printf("set led %f %f %f\n\r", val, mod.min, mod.max);
                            set_led_display(mod.p, mod.min, mod.max, mod.numeric);
                        }
                    }
                }
            }
            if(!should_set_volume)
            {
                break;
            }
        }
        struct CC cc = CCs[cc_num];
        if(cc.p == 0)
        {
            // printf("got unknown CC %d\n\r\r", cc_num);
            break;
        }
        float steps = cc.mul * (float)cc_val * 8.0f;
        int whole_steps = round(steps);
        float val = (cc.step * whole_steps) + cc.min;
        *cc.p = val;
        break;
    }
    case MIDI_PITCH_BEND:
    {
        uint8_t pb_up = msg[1];
        uint8_t pb_down = msg[2];
        uint16_t val = msg[1] | (msg[2] << 7);  // 0 to 16,383
        *Signals.pitch_bend = (float)val;
        printf("pb: %d\n", val);
        break;
    }
    case MIDI_AFTERTOUCH: {
        uint8_t cc_val = msg[1] & 0b01111111;
        uint8_t cc_num = cc_map[CC_MAP_AFTERTOUCH_IDX];

        if (ui_mode == UI_MODE_LEARN) {
            if (mode_learn_current_cc > 0) {
                cc_map[CC_MAP_AFTERTOUCH_IDX] = mode_learn_current_cc;
                learn_mode_state = LEARN_MODE_LEARNED;
            }
        } else {
            struct CC cc = CCs[cc_num];
            if (cc.p != 0) {
                float steps = cc.mul * (float)cc_val * 8.0f;
                int whole_steps = round(steps);
                float val = (cc.step * whole_steps) + cc.min;
                *cc.p = val;
            }
        }
        break;
    }
    default:
        break;
    }
}

static void uart_receive_isr(int irq, void *arg)
{
    uint32_t intstatus = bflb_uart_get_intstatus(uart0);

    if (intstatus & UART_INTSTS_RX_FIFO)
    {
        while (bflb_uart_rxavailable(uart0))
        {
            int ret = bflb_uart_getchar(uart0);
            if (ret >= 0) {
                uint8_t ch = (uint8_t)ret;
                xQueueSendToBackFromISR(uart_queue, &ch, NULL);
            }
        }
    }

    if (intstatus & UART_INTSTS_RTO)
    {
        bflb_uart_int_clear(uart0, UART_INTCLR_RTO);
    }
}

void usb_midi_in(char ch)
{
    xQueueSendToBackFromISR(uart_queue, &ch, NULL);
}

void uart_task(void *args){
    uint8_t ch;
    while(1){
        if(xQueueReceive(uart_queue, &ch, portMAX_DELAY)){
            bflb_uart_putchar(uart0, ch); // send to midi out
            bool ret = parse(ch);
            // printf("TRS: %02X\n", ch);
            if (ret)
            {
                handle_midi(getMidiMsg(), getMidiMsgLen());
            }
        }
    }
}

void uart_receive_init(void)
{
    uart_queue = xQueueCreate(4096, sizeof(uint8_t));
    xTaskCreate(uart_task, "uart_task", 1024 * 2, NULL, 2, NULL);

    init_midi_parser();

    uart0 = bflb_device_get_by_name("uart0");
    bflb_uart_feature_control(uart0, UART_CMD_SET_RX_FIFO_THREHOLD, 0);

    bflb_irq_attach(uart0->irq_num, uart_receive_isr, uart0);
    bflb_irq_enable(uart0->irq_num);
    bflb_uart_rxint_mask(uart0, false);
}