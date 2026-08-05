#ifndef DSP_SYNTH_H_
#define DSP_SYNTH_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SR 48000U
#define SR_INV 2.08333e-5f

#define LOG2_E 0.693147180559945309417

#define MAX_ENV_STAGE_LEN_SEC 2.0f
#define MAX_ENV_STAGE_LEN_SAMPLES 98000.0f  // MAX_ENV_STAGE_LEN_SEC * SR
#define MIN_ENV_STAGE_LEN_SAMPLES 80.0f

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
    BUS_SEL_AM,
    BUS_SEL_FM,
    BUS_SEL_TM
} Bus_Sel_t;

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
    OS2_WAV_TRI,
    OS2_WAV_SQR,
    OS2_WAV_SAW
} OS2_Wave_Sel_t;

typedef enum {
    OS1_WAV_SINE,
    OS1_WAV_TRI,
    OS1_WAV_BOTH
} OS1_Wave_Sel_t;

typedef enum {
    OS2_RANGE_HI,
    OS2_RANGE_LO
} OS2_Range_Sel_t;

typedef enum {
    ENV_OFF,
    ENV_ATT,
    ENV_SUS,
    ENV_REL
} EnvStages_t;

typedef enum {
    AMP_LPG,
    AMP_VCA
} AmpMode_t;

typedef struct {
    bool prev_gate;
    float val;
    EnvStages_t stage;
} EnvState_t;

typedef struct {
    float gain;
    float threshold;
    float bias;
    float mix;
} BuchlaWF_Cell_t;

#define MAX_SG_LENGTH 32
#define MIN_SG_LENGTH 2
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

typedef enum {
    OS1_OCT_0,
    OS1_OCT_n1,
    OS1_OCT_n2
} OsOct_t;

#define MAX_SCALE_DEGREES 24

typedef struct {
    float root_freq_hz;
    uint32_t num_intervals; 
    float intervals[MAX_SCALE_DEGREES];
} Scale;


typedef struct {
    float threshold;
    float prev_input;
} Folder_Stage_t;

typedef struct {
    Folder_Stage_t stages[16];
    int num_stages;
} Multistage_Wave_Folder_t;

typedef struct {
    float state;
    float normalized_freq;
    float gain;
    float hf_bleed;
    float lpf_state;
} Vactrol_t;


// delay line length in seconds: 1.5
#define DELAY_LINE_LEN 72000  // SR * delay_line_s
#define DELAY_LINE_MIN 960  // SR * 20ms

typedef struct {
    float buffer[DELAY_LINE_LEN];
    int buffer_size;
    int ptr;
    float delay_time_samples;
} InterpolatingDelay_t;

void compute_midi_note();

void compute_matrix();

void compute_sg_rate();
void compute_sg();
void compute_env();
void compute_os1();
void compute_os2();
float compute_mixer();
float compute_wavefolder(float in);
float compute_amplifier(float in);
float compute_delay(float in);
#endif  // DSP_SYNTH_H_