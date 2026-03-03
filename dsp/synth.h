#ifndef DSP_SYNTH_H_
#define DSP_SYNTH_H_

#include <stddef.h>

void compute_dsp(size_t len, float* buf);

typedef struct {
    float len;
    float rate;
    float rcl;
    float smth;
    float att;
    float rel;
    float os1;
    float os2;
    float bus;
    float idx;
    float timb;
    float amp;
} FB_Dests;

typedef enum {
    GATESEL_SEPERATE,
    GATESEL_SG,
    GATESEL_ENV
} Gate_Sel_t;

typedef enum {
    MOD_TYPE_AM,
    MOD_TYPE_FM,
    MOD_TYPE_TM
} CmplxModType_t;

typedef struct {
    float gate;
    float val;
} EnvState_t;

#define MAX_SG_LENGTH 32
typedef struct {
    float gate;
    float arr[MAX_SG_LENGTH];
    float val;
} SGState_t;

typedef struct {
    bool high;
    float phase;
    float next_tri;
    float next_saw;
    float next_sqr;
    float val;
} OscState_t;

#endif  // DSP_SYNTH_H_