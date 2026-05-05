#ifndef AUDAC_H
#define AUDAC_H

// #define BUF_SIZE 32
// #define BUF_SIZE 64 // OG
// #define BUF_SIZE 1024
#define BUF_SIZE 256

#define AUDIO_SAMPLE_RATE_HZ 48000
#define AUDIO_FRAME_BITS     16
#define AUDIO_CHANNELS       2

#include "bflb_i2s.h"

void audio_dac_init();

#endif