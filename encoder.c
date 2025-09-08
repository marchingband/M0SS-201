#include "stdbool.h"

#include <FreeRTOS.h>
#include "queue.h"
#include "portmacro.h"
#include "bflb_dma.h"
#include "bl616_glb_gpio.h"
#include "bl616_irq.h"
#include "bflb_mtimer.h"
#include "board.h"

#include "rotary_encoder.h"
#include "gpio.h"
#include "midi.h"
#include "rgb_led.h"

#define ROT_ENC_A_GPIO 16
#define ROT_ENC_B_GPIO 17
#define ENABLE_HALF_STEPS false  // Set to true to enable tracking of rotary encoder at half step resolution
#define RESET_AT          0      // Set to a positive non-zero number to reset the position if this value is exceeded
#define FLIP_DIRECTION    false  // Set to true to reverse the clockwise/counterclockwise sense

QueueHandle_t event_queue;

uint64_t last_click = 0;

#define CLICK_SLOW 100000
#define CLICK_MED  60000
#define CLICK_FAST 15000

extern bool encoder_pressed;
extern bool asb_active;
extern int cur_asb;
extern bool set_midi_chan_active;
extern int cur_midi_chan;
extern int cur_disp_num;


static void on_encoder(bool up){
    uint64_t now = bflb_mtimer_get_time_us();
    uint64_t passed = now - last_click;
    last_click = now;
    int click_multiplier = passed > 100000 ? 1 : passed > 50000 ? 4 : passed > 10000 ? 10 : 30;
    // printf("time %llu\n\r", passed);
    // printf("mul %d\n\r", click_multiplier);
    // uint8_t *button_states = get_button_states();

    if(asb_active)
    {
        //asignable held
        struct Asb asb = get_asbs()[cur_asb];
        float cur = *asb.p;
        float step = asb.step * asb.mul * click_multiplier;
        float next = up ? cur + step : cur - step;
        float clamped = next > asb.max ? asb.max : next < asb.min ? asb.min : next;
        *asb.p = clamped;
        cur_disp_num = clamped;
    } 
    else if(set_midi_chan_active)
    {
        int new_chan = up ? cur_midi_chan + 1 : cur_midi_chan - 1;
        if(new_chan >= 1 && new_chan <= MIDI_CHANNEL_OMNI){
            cur_midi_chan = new_chan;
            cur_disp_num = new_chan;
        }
    } 
    else if(alt_held()) // pitch bend
    { 
        struct Sigs *sigs = get_signals();
        if(encoder_pressed){
            float cur = *sigs->transpose;
            float step = click_multiplier;
            float next = up ? cur + step : cur - step;
            float clamped = next > 127 ? 127 : next < -127 ? -127 : next;
            *sigs->transpose = clamped;
        } else {
            float cur = *sigs->fine_tune;
            float step = click_multiplier;
            float next = up ? cur + step : cur - step;
            float clamped = next > 127 ? 127 : next < -127 ? -127 : next;
            *sigs->fine_tune = clamped;
        }
        return;
    }
    else
    {

        struct Param param = get_param_selected();

        if(param.button == -1)
        {
            // printf("nothing selected\n\r");
            return;
        }

        if(param.alt)
        {
            struct Alt *Alts = get_alts();
            struct Alt alt = Alts[param.button];
            if(alt.p == 0)
            {
                // printf("got unknown Alt %d\n\r", param.button);
            }
            else
            {
                float cur = *alt.p;
                float step = alt.step * alt.mul * click_multiplier;
                float next = up ? cur + step : cur - step;
                float clamped = next > alt.max ? alt.max : next < alt.min ? alt.min : next;
                *alt.p = clamped;
                // printf("set led %f %f %f\n\r", clamped, alt.min, alt.max);
                set_led_display(alt.p, alt.min, alt.max);
            }

        }
        else
        {
            struct Mod *Mods = get_mods();
            struct Mod mod = Mods[param.button];
            if(mod.p == 0)
            {
                // printf("got unknown Mod %d\n\r", param.button);
            }
            else
            {
                float cur = *mod.p;
                float step = mod.step * mod.mul * click_multiplier;
                float next = up ? cur + step : cur - step;
                float clamped = next > mod.max ? mod.max : next < mod.min ? mod.min : next;
                *mod.p = clamped;
                // printf("set led %f %f %f\n\r", clamped, mod.min, mod.max);
                set_led_display(mod.p, mod.min, mod.max);
            }
        }
    }
}

void process_encoder(void)
{
    rotary_encoder_event_t event = { 0 };
    if (xQueueReceive(event_queue, &event, 0))
    {
        on_encoder(event.state.direction == ROTARY_ENCODER_DIRECTION_CLOCKWISE);
        // printf("Event: position %d, direction %s\n\r", event.state.position,
        //          event.state.direction ? (event.state.direction == ROTARY_ENCODER_DIRECTION_CLOCKWISE ? "CW" : "CCW") : "NOT_SET");
    }    
}

void encoder_init(void)
{
    rotary_encoder_init(ROT_ENC_A_GPIO, ROT_ENC_B_GPIO);
    rotary_encoder_enable_half_steps(ENABLE_HALF_STEPS);
#if FLIP_DIRECTION
    rotary_encoder_flip_direction(&info);
#endif
    event_queue = rotary_encoder_create_queue();
    rotary_encoder_set_queue(event_queue);
}
