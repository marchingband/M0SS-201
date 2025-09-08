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

#define MIDI_NOTE_OFF 8
#define MIDI_NOTE_ON 9
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
int alt_switch_state = 0;
bool encoder_pressed = false;

static struct CC CCs[NUM_CC] = {0};
static struct Mod Mods[NUM_BUTTONS] = {0};
static struct Alt Alts[NUM_BUTTONS] = {0};
static struct Sel Sels[NUM_BUTTONS] = {0};
static struct AltSel AltSels[NUM_BUTTONS] = {0};
static struct Asb Asbs[NUM_BUTTONS] = {0};
static struct Sigs Signals = {
    .pitch = 0,
    .gate = 0,
    // .lfo = 0,
    // .env = 0,
    // .pitch_bend = 0,
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

void extract(const char *name, float *p, float init, float min, float max, float step)
{
    // we can ignore the pitch and gate, and just calibrate for CC's
    bool found = false;
    for(int i=0; i<NUM_CC; i++)
    {
        struct CC *cc = &CCs[i];
        if(cc->p == p){
            cc->min = min;
            cc->max = max;
            cc->init = init;
            cc->step = step;
            float steps = (max - min) / step;
            float per = steps / 128.0;
            cc->mul = per;
            strcpy(cc->name, name);
            found = true;
        }
    }

    for(int i=0; i<NUM_BUTTONS; i++)
    {
        struct Mod *mod = &Mods[i];
        if(mod->p == p){
            mod->min = min;
            mod->max = max;
            mod->init = init;
            mod->step = step;
            float steps = (max - min) / step;
            float per = steps / 128.0;
            mod->mul = per;
            strcpy(mod->name, name);
            found = true;
        }
    }

    for(int i=0; i<NUM_BUTTONS; i++)
    {
        struct Alt *alt = &Alts[i];
        if(alt->p == p){
            alt->min = min;
            alt->max = max;
            alt->init = init;
            alt->step = step;
            float steps = (max - min) / step;
            float per = steps / 128.0;
            alt->mul = per;
            strcpy(alt->name, name);
            found = true;
        }
    }

    for(int i=0; i<NUM_BUTTONS; i++)
    {
        struct Sel *sel = &Sels[i];
        if(sel->p == p){
            sel->min = min;
            sel->max = max;
            sel->init = init;
            strcpy(sel->name, name);
            found = true;
        }
    }

    for(int i=0; i<NUM_BUTTONS; i++)
    {
        struct AltSel *alt_sel = &AltSels[i];
        if(alt_sel->p == p){
            alt_sel->min = min;
            alt_sel->max = max;
            alt_sel->init = init;
            strcpy(alt_sel->name, name);
            found = true;
        }
    }

    for(int i=0; i<NUM_BUTTONS; i++)
    {
        struct Asb *asb = &Asbs[i];
        if(asb->p == p){
            asb->min = min;
            asb->max = max;
            asb->init = init;
            asb->step = step;
            float steps = (max - min) / step;
            float per = steps / 128.0;
            asb->mul = per;
            strcpy(asb->name, name);
            found = true;
        }
    }
    
    if(!found){
        printf("failed to find pointer for %s\n", name);
    }
}

void declare(UIGlue *uiInterface, float *p, const char* key, const char* val){
    if(strcmp("midi", key) == 0) // its a cc
    {
        printf("got midi %s:%s\n", key, val);
        long num = strtol(val, NULL, 10);

        struct CC *cc = &CCs[num];
        cc->p = p;
    }
    if(strcmp("mod", key) == 0) // its a cc
    {
        printf("got mod %s:%s\n", key, val);
        long num = strtol(val, NULL, 10);

        struct Mod *mod = &Mods[num];
        mod->p = p;
    }
    if(strcmp("alt", key) == 0) // its a cc
    {
        printf("got alt %s:%s\n", key, val);
        long num = strtol(val, NULL, 10);

        struct Alt *alt = &Alts[num];
        alt->p = p;
    }
    if(strcmp("sel", key) == 0) // its a selection toggle
    {
        printf("got sel %s:%s\n", key, val);
        long num = strtol(val, NULL, 10);

        struct Sel *sel = &Sels[num];
        sel->p = p;
    }
    if(strcmp("alt-sel", key) == 0) // its a selection toggle with alt held
    {
        printf("got alt-sel %s:%s\n", key, val);
        long num = strtol(val, NULL, 10);

        struct AltSel *alt_sel = &AltSels[num];
        alt_sel->p = p;
    }
    if(strcmp("asb", key) == 0) // its a modifuer with alt held
    {
        printf("got asb %s:%s\n", key, val);
        long num = strtol(val, NULL, 10);

        struct Asb *asb = &Asbs[num];
        asb->p = p;
    }
    if(strcmp("pitch", key) == 0) // its the pitch
    {
        printf("got pitch pointer [%s]\n", key);
        Signals.pitch = p;
    }
    if(strcmp("gate", key) == 0) // its the gate
    {
        printf("got gate pointer [%s]\n", key);
        Signals.gate = p;
        *Signals.gate = 0;
    }
    // if(strcmp("vca", key) == 0) // its the gate
    // {
    //     // printf("got vca pointer\n\r\r");
    //     Signals.vca = p;
    // }
    // if(strcmp("pitch-bend", key) == 0) // its the pitch bend
    // {
    //     // printf("got pitch bend pointer\n\r\r");
    //     Signals.pitch_bend = p;
    // }
    // if(strcmp("fine-tune", key) == 0) // its the fine tuning
    // {
    //     // printf("got fine-tune pointer\n\r\r");
    //     Signals.fine_tune = p;
    // }
    // if(strcmp("transpose", key) == 0) // its the transpose
    // {
    //     // printf("got transpose pointer\n\r\r");
    //     Signals.transpose = p;
    // }
    // if(strcmp("lfo", key) == 0) // its the lfo
    // {
    //     // printf("got lfo pointer\n\r\r");
    //     Signals.lfo = p;
    // }
    // if(strcmp("env", key) == 0) // its the env
    // {
    //     // printf("got env pointer\n\r\r");
    //     Signals.env = p;
    // }
};

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
            printf("found alt %s\n", alt.name);
            p = alt.p;
        } else if (alt_sel.p != 0){
            printf("found alt_sel %s\n", alt_sel.name);
            p = alt_sel.p;
        } else if (asb.p != 0){
            printf("found asb %s\n", asb.name);
            p = asb.p;
        }
    } else {
        struct Mod *Mods = get_mods();
        struct Sel *Sels = get_sels();
        struct Mod mod = Mods[num];
        struct Sel sel = Sels[num];
        if(mod.p != 0){
            printf("found mod %s\n", mod.name);
            p = mod.p;
        } else if(sel.p != 0){
            printf("found sel %s\n", sel.name);
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
            printf("matched cc:%d %s\n", i, CCs[i].name);
            return i;
        }
    }
    printf("found something but no CC found for that pointer\n");
    return -1;
}

void handle_encoder_press(bool down)
{
    *Signals.gate = down ? 1 : 0;
    encoder_pressed = down;
}

void handle_button_press(int num)
// handles load, save, learn and cancelling the mode by pressing alt again
{
    printf("press %d\n", num);
    int ui_mode = get_ui_mode();
    if(num == BUTTON_NUM_ALT) // alt button
    {
        set_ui_mode(UI_MODE_PLAY);
    }
    else if(alt_held())
    {
        printf("alt held\n");
        if(ui_mode == UI_MODE_LEARN)
        {
            int cc_num = get_cc(num, true);
            printf("learn cc %d\n", cc_num);
            if(cc_num != -1){
                mode_learn_current_cc = cc_num;
                learn_mode_state = LEARN_MODE_SELECTED;
            }

        } else {
            if(num == BUTTON_NUM_LOAD)
            {
                set_ui_mode(UI_MODE_LOAD);
            }
            else if(num == BUTTON_NUM_SAVE)
            {
                set_ui_mode(UI_MODE_SAVE);
            }
            else if(num == BUTTON_NUM_LEARN)
            {
                set_ui_mode(UI_MODE_LEARN);
                mode_learn_current_cc = 0;
                learn_mode_state = LEARN_MODE_READY;
            }
            else
            {
                struct AltSel *alt_sel = &AltSels[num];
                if(alt_sel->p == 0) // no alt-sel to do for that button press
                {
                    printf("nothing\n");
                    return;
                }
                printf("toggling\n");
                float cur = *alt_sel->p;
                if(cur < alt_sel->max)
                {
                    *alt_sel->p += 1;
                }
                else
                {
                    *alt_sel->p = alt_sel->min;
                }
                // check for switch displays
                if(num == 7 || num == 12 || num == 13 || num == 15){
                    if(*alt_sel->p == 0){
                        alt_switch_state = ALT_SWITCH_STATE_OFF;
                    } else {
                        alt_switch_state = ALT_SWITCH_STATE_ON;
                    }
                }
            }
        }
    } 
    else if(ui_mode == UI_MODE_LEARN)
    {
        // int label = button_label(num);
        // if(label < 8)
        // {
        //     mode_learn_current_cc = label + 1;
        // }
        int cc_num = get_cc(num, alt_held());
        printf("learn cc %d\n", cc_num);
        if(cc_num != -1){
            mode_learn_current_cc = cc_num;
            learn_mode_state = LEARN_MODE_SELECTED;
        }

    }
    else if(ui_mode == UI_MODE_PLAY)
    {
        struct Sel *sel = &Sels[num];
        if(sel->p == 0) // no sel for that button press
        {
            return;
        }
        float cur = *sel->p;
        if(cur < sel->max)
        {
            *sel->p += 1;
        }
        else
        {
            *sel->p = sel->min;
        }
    }
    else if(ui_mode == UI_MODE_LOAD)
    {
        int label = button_label(num);
        if(label)
        {
            if(label < NUM_PRESETS)
            {
                // load(label - 1);
                load_preset(label - 1);
            }
        }
        set_ui_mode(UI_MODE_PLAY);
    }
    else if(ui_mode == UI_MODE_SAVE)
    {
        int label = button_label(num); // label is 1 indexed
        if(label)
        {
            label--; // label is 1 indexed
            if(label < NUM_PRESETS)
            {
                save(label);
            }
        }
        set_ui_mode(UI_MODE_PLAY);
    }
}

uint8_t note_lifo[128] = {0};
size_t note_lifo_p = 0;

void push_note(uint8_t note){
    note_lifo[note_lifo_p++] = note;
}

void pop_note(void){
    note_lifo_p--;
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

void handle_midi(uint8_t *msg, uint8_t len)
{
    // printf("msg len %d -> %d %d %d\n\r\r", len, msg[0], msg[1], msg[2]);
    
    uint8_t channel = msg[0] & 0b00001111;
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
    
    switch (code)
    {
    case MIDI_NOTE_ON:
    {
        uint8_t note = msg[1] & 0b01111111;
        // uint8_t velocity = msg[2] & 0b01111111;
        // set_note(note);
        // play(note);
        *Signals.gate = 1;
        *Signals.pitch = note; // the dsp converts to hz
        push_note(note);
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
        if((cc_num_in - 1) >= NUM_CC_MAP){
            // printf("CC %d is too high", cc_num_in);
            break;
        }
        uint8_t cc_val = msg[2] & 0b01111111;
        uint8_t *cc_map = &get_config()->cc_map[0];
        uint8_t cc_num = cc_map[cc_num_in - 1]; // -1 because ccs are indexed at 1

        int ui_mode = get_ui_mode();

        if(ui_mode == UI_MODE_LEARN)
        {
            if(mode_learn_current_cc > 0)
            {
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
                            set_led_display(alt.p, alt.min, alt.max);
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
                            set_led_display(mod.p, mod.min, mod.max);
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
        float steps = cc.mul * (float)cc_val;
        int whole_steps = round(steps);
        float val = (cc.step * whole_steps) + cc.min;
        *cc.p = val;
        // printf("cc%-2d::%12f -> %s\n\r\r", cc_num, val, cc.name );
        break;
    }
    case MIDI_PITCH_BEND:
    {
        uint16_t val = msg[1] | (msg[2] << 7);  // 0 to 16,383
        *Signals.pitch_bend = (float)val;
        // printf("pitchbend %d", val) ;
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
            uint8_t ch = bflb_uart_getchar(uart0);
            // int ret = parse(ch);
            // // printf("got %d, ret is %d", ch, ret);
            // if (ret)
            // {
            //     handle_midi(getMidiMsg(), getMidiMsgLen());
            // }
            // // printf("0x%02x\r\n\r", ch);
            xQueueSendToBackFromISR(uart_queue, &ch, NULL);
        }
    }

    if (intstatus & UART_INTSTS_RTO)
    {
        bflb_uart_int_clear(uart0, UART_INTCLR_RTO);
    }
}

void poll_uart(void)
{
    ch = bflb_uart_getchar(uart0);
    if (ch != -1)
    {
        int ret = parse(ch);
        if (ret)
        {
            handle_midi(getMidiMsg(), getMidiMsgLen());
        }
    }
}

void usb_midi_in(char ch)
{
    bflb_uart_putchar(uart0, ch); // send to midi out
    int ret = parse(ch);
    if (ret == 1)
    {
        handle_midi(getMidiMsg(), getMidiMsgLen());
    }
}

void uart_task(void *args){
    uint8_t ch;
    while(1){
        if(xQueueReceive(uart_queue, &ch, portMAX_DELAY)){
            bflb_uart_putchar(uart0, ch); // send to midi out
            int ret = parse(ch);
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
    xTaskCreate(uart_task, "uart_task", 2048, NULL, 2, NULL);

    init_midi_parser();

    uart0 = bflb_device_get_by_name("uart0");
    bflb_uart_feature_control(uart0, UART_CMD_SET_RX_FIFO_THREHOLD, 0);

    bflb_irq_attach(uart0->irq_num, uart_receive_isr, uart0);
    bflb_irq_enable(uart0->irq_num);
    bflb_uart_rxint_mask(uart0, false);
}
