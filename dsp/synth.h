#ifndef DSP_SYNTH_H_
#define DSP_SYNTH_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SR 48000U
#define SR_INV 2.08333e-5f

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

typedef enum {
    OS1_WAV_SINE,
    OS1_WAV_TRI,
    OS1_WAV_BOTH
} OS1_Wave_Sel_t;

typedef struct {
    bool gate;
    float val;
} EnvState_t;

#define MAX_SG_LENGTH 32
#define MIN_SG_LENGTH 4
typedef struct {
    float phase;
    bool high;
    
    bool prev_gate;
    float arr[MAX_SG_LENGTH];
    uint16_t idx;
    float val;
    float lpf_state;
    float rate_normalized;
} SGState_t;

typedef struct {
    bool high;
    float phase;
    float next_tri;
    float next_saw;
    float next_sqr;
    float val;
} OscState_t;

void compute_matrix();
float compute_os2_freq(CmplxModType_t mod_type);

void compute_sg_rate(float rate, SGState_t* state_p);
void compute_sg(float len, float recall, float smooth, bool gate, SGState_t* state_p);
void compute_env(float att, float rel, bool gate, EnvState_t* state_p);
void compute_os1(float freq, float hilo, float kt, float midi_pitch, float wav_sel, OscState_t* state_p);
void compute_os2(float freq, float wav_sel, OscState_t* state_p);
float compute_mixer();
float compute_wavefolder(float in, float timbre, float bias);
float compute_amplifier(float in, float gain, float mode_sel);
float compute_delay(float in, float time, float fb, float mix);
#endif  // DSP_SYNTH_H_