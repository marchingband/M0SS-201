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

#define NUM_VOICES 1

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

float *buf;
size_t buf_len;


void dsp_init(size_t buf_size) {
    init_dsp();
    link_ui();
    reset_params();
    printf("malloc\n");
    buf = (float *)malloc(buf_size * sizeof(float));
    buf_len = buf_size;
}

void dsp_run(uint16_t *dest) {
    compute_dsp(buf_len, buf);
    for (int i = 0; i < buf_len; i++) {
        float val = buf[i];

        int32_t raw = (int32_t)(val * 32767);
        uint16_t capped = (raw < -32768)? -32768 : ((raw > 32767)? 32767 : raw);
        dest[2*i + 0] = capped;
        dest[2*i + 1] = -capped;  // balanced output
    }
    poll_gpio();
}