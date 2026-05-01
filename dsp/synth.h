#ifndef DSP_SYNTH_H_
#define DSP_SYNTH_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SR 48000U
#define SR_INV 2.08333e-5f

#define MAX_ENV_STAGE_LEN_SEC 2.0f
#define MAX_ENV_STAGE_LEN_SAMPLES 98000.0f  // MAX_ENV_STAGE_LEN_SEC * SR
#define MIN_ENV_STAGE_LEN_SAMPLES 2.0f

void compute_dsp(size_t len, float* buf);
void init_dsp();

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
    ENVMODE_AR,
    ENVMODE_ASR,
    ENVMODE_ARL
} EnvMode_t;

typedef enum {
    MOD_TYPE_AM,
    MOD_TYPE_FM,
    MOD_TYPE_TM
} CmplxModType_t;

typedef enum {
    OS1_WAV_TRI,
    OS1_WAV_SQR,
    OS1_WAV_SAW
} OS1_Wave_Sel_t;

typedef enum {
    OS2_WAV_SINE,
    OS2_WAV_TRI,
    OS2_WAV_BOTH
} OS2_Wave_Sel_t;

typedef enum {
    OS1_RANGE_LO,
    OS1_RANGE_HI
} OS1_Range_Sel_t;

typedef enum {
    ENV_OFF,
    ENV_ATT,
    ENV_SUS,
    ENV_REL
} EnvStages_t;

typedef struct {
    bool prev_gate;
    float val;
    EnvStages_t stage;
} EnvState_t;

#define MAX_SG_LENGTH 32
#define MIN_SG_LENGTH 4
typedef struct {
    float phase;
    bool high;
    float rate_normalized;
    bool prev_gate;
    float arr[MAX_SG_LENGTH];
    uint16_t idx;
    float val;
    float lpf_state;
} SGState_t;

typedef struct {
    bool high;
    float phase;
    float next_tri;
    float next_saw;
    float next_sqr;
    float val;
} OscState_t;

void compute_midi_freq();

void compute_matrix();
// float compute_os2_rate();

void compute_sg_rate();
void compute_sg();
void compute_env();
void compute_os1();
void compute_os2();
float compute_mixer();
float compute_wavefolder(float in, float timbre, float bias);
float compute_amplifier(float in, float gain, float mode_sel);
float compute_delay(float in, float time, float fb, float mix);
#endif  // DSP_SYNTH_H_