#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "bflb_mtimer.h"
#include "dsp.h"
#include "gpio.h"
#include "rgb_led.h"
#include "midi.h"
#include "dsp/synth.h"

// #include "square-diode-lfo.h"
// #include "square-diode.h"
// #include "moog-poly-8.h"
// #include "moog-poly-8-verb.h"
// #include "mg1.h"
#include "M0SS-201.h"

#define NUM_VOICES 1

// from midi.c
void extract(const char *name, float *p, float init, float min, float max, float step);
void *declare(UIGlue *uiInterface, float *p, const char* key, const char* val);

// from the dsp .h file
void buildUserInterfacemydsp(mydsp* dsp, UIGlue* ui_interface);

// from midi.c
void poll_uart(void);

struct voice_s
{
    bool on;
    uint8_t note;
    float *pitch;
    float *gain;
    float *gate;
};

struct voice_s voices[NUM_VOICES];

int get_next_voice(uint8_t note)
{    
    for (int i = 0; i < NUM_VOICES; i++)
    {
        if (voices[i].note == note) // retrigger
        {
            return i;
        }
    }

    for (int i = 0; i < NUM_VOICES; i++)
    {
        if ((voices[i].on == false) && (*voices[i].gain == 0.0f)) // completely open
        {
            // printf("next voice is %d\n", i);
            return i;
        }
    }

    for (int i = 0; i < NUM_VOICES; i++)
    {
        if (*voices[i].gain == 0.0f) // has fully decayed
        {
            // printf("next voice is %d\n", i);
            return i;
        }
    }

    for (int i = 0; i < NUM_VOICES; i++)
    {
        if (voices[i].on == false) // open but not yet done
        {
            // printf("next voice is %d\n", i);
            return i;
        }
    }

    // printf("no freee voices\n");
    return -1;
}

int get_voice_by_note(uint8_t note)
{
    for (int i = 0; i < NUM_VOICES; i++)
    {
        if (voices[i].note == note)
        {
            // printf("%d, found voice is %d\n", note, i);
            return i;
        }
    }
    // printf("no voices for that note\n");
    return -1;
}

float note_to_freq(int note)
{
    float a = 440; // frequency of A (coomon value is 440Hz)
    return (a / 32) * pow(2, ((note - 9) / 12.0));
}

mydsp dsp;

float *buf[1];
size_t buf_len;

void openVerticalBox(UIGlue *uiInterface, char* key){};
void openHorizontalBox(UIGlue *uiInterface, char* key){};
void openTabBox(UIGlue *uiInterface, char* key){};
void *closeBox(UIGlue *uiInterface){};
void *addVerticalBargraph(UIGlue *uiInterface, const char *name, float *p, float f1, float f2){};
void *addButton(UIGlue *uiInterface, const char *name, float *p){extract(name, p, 0, 0, 0, 0);};
void *addVerticalSlider(UIGlue *uiInterface, const char *name, float *p, float init, float min, float max, float step){extract(name, p, init, min, max, step);};
void *addHorizontalSlider(UIGlue *uiInterface, const char *name, float *p, float init, float min, float max, float step){extract(name, p, init, min, max, step);};
void *addNumEntry(UIGlue *uiInterface, const char *name, float *p, float init, float min, float max, float step){extract(name, p, init, min, max, step);};
void *addCheckButton(UIGlue *uiInterface, const char *name, float *p){extract(name, p, 0, 0, 1, 1);};

UIGlue ui_glue = {
    .openVerticalBox = openVerticalBox,
    .openHorizontalBox = openHorizontalBox,
    .closeBox = closeBox,
    .declare = declare,
    .addVerticalSlider = addVerticalSlider,
    .addHorizontalSlider = addHorizontalSlider,
    .addNumEntry = addNumEntry,
    .addVerticalBargraph = addVerticalBargraph,
    .addButton = addButton,
    .openTabBox = openTabBox,
    .addCheckButton = addCheckButton
};

void dsp_init(size_t buf_size)
{

    // size_t cnt = buf_size / 2;
    printf("initmydsp\n");
    initmydsp(&dsp, 48000);
    // initmydsp(&dsp, 32000);
    // initmydsp(&dsp, 8000);
    printf("buildUserInterfacemydsp\n");
    // buildUserInterfacemydsp(&dsp, &ui_glue);
    link_ui();
    printf("malloc\n");
    buf[0] = (float *)malloc(buf_size * sizeof(float));
    buf_len = buf_size;
    // buf[0] = (float *)malloc(cnt * sizeof(float));
    // buf_len = cnt;
}

int last_start = 0;
int consumed = 0;
int period = 0;

void print_bench(void)
{
    // printf("%d of %d [us]\n", consumed, period);
}

int loop_cnt = 0;

void dsp_run(int16_t *dest)
{
    int now = bflb_mtimer_get_time_us();
    period = now - last_start;
    last_start = now;
    // poll_uart();
    compute_dsp(buf_len, buf);
    // computemydsp(&dsp, buf_len, NULL, buf);
    for (int i = 0; i < buf_len; i++)
    {
        float val = buf[0][i];
        // if(val > 1.0 || val < -1.0)
        // {
            // printf("clip :%f", val);
        // }
        // printf("%f\n", val);
        // dest[i] = (uint16_t)((val + 1) * 32767);
        // the AUDAC expects int16_t but the buffer is typed uint16_t so do this awkward thing.
        // dest[i] = (uint16_t)(int16_t)(val * 32767);
        int32_t raw = (int32_t)(val * 32767);
        dest[i] = (uint16_t)(raw < -32768 ? -32768 : raw > 32767 ? 32767 : raw);
    }
    consumed = bflb_mtimer_get_time_us() - now;
    poll_gpio();

    // loop_cnt++;
    // if(loop_cnt >= 20)
    // {
    //     render_lights();
    //     loop_cnt = 0;
    // }
}

void play(uint8_t note)
{
#ifdef POLY_MODE
    int ret = get_next_voice(note);
    if (ret == -1)
    {
        return;
    }
    float hz = note_to_freq(note);
    // printf("playing %d [%fhz] on voice %d\n", note, hz, ret);
    voices[ret].on = true;
    voices[ret].note = note;
    *voices[ret].pitch = hz;
    *voices[ret].gate = 1.0f;
    return;
#endif

}

void stop(uint8_t note)
{
#ifdef POLY_MODE
    for (int i = 0; i < NUM_VOICES; i++)
    {
        if (voices[i].note == note)
        {
            voices[i].on = false;
            *voices[i].gate = 0.0f;
        }
    }
    return;
#endif

}

// void set_note(uint8_t note){
//     float hz = note_to_freq(note);
//     dsp.fEntry1 = hz;
// }

// void set_filter1(uint8_t val)
// {
//     float freq = (float)val / 127.0f; // 0 ~ 1
//     dsp.fHslider0 = freq;
// }

// void set_filter2(uint8_t val)
// {
//     float freq = ((float)val / 127.0f) * 2000; // 0 ~ 2000
//     dsp.fHslider1 = freq;
// }

// void set_filter3(uint8_t val)
// {
    // float freq = ((float)val / 127.0f) * 40;
    // dsp.fHslider2 = freq;
// }