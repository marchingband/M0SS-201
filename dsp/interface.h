#ifndef DSP_INTERFACE_H_
#define DSP_INTERFACE_H_

#include "stdint.h"

typedef enum {
    PARAM_MOD,      // main parameter
    PARAM_ALT,      // alt parameter
    PARAM_SEL,      // selector
    PARAM_ALT_SEL,  // alt selector
    PARAM_MTX       // matrix parameter
} DSP_Param_Type_t;


typedef struct {
    const DSP_Param_Type_t type;
    float* p;  
    const int16_t btn;
    const float min;
    const float max;
    const float init;
    const float step;
} DSP_Param_t;

#define NUM_MOD_SRCS 3
#define NUM_MOD_DESTS 12
// I for index, S for source
#define OS1_I_S 0
#define SG_I_S  1
#define ENV_I_S 2

#define LEN_I  0
#define RATE_I 1
#define RCL_I  2
#define SMTH_I 3
#define ATT_I  4
#define REL_I  5
#define OS1_I  6
#define OS2_I  7
#define BUS_I  8
#define IDX_I  9
#define TIMB_I 10
#define AMP_I  11


typedef struct {
    // all parameters
    float pitch;
    float gate;

    float sg_len;
    float sg_smth;
    float sg_rate;
    float sg_rcl;
    float gate_sel;
    float env_mode;
    float env_att;
    float env_rel;
    float env_crv;

    float os1_tune;
    float os1_fine;
    float os1_hilo;
    float os1_kt;  
    float os1_wave;
    float bus_idx; 
    float bus_sel; 
    float os2_tune;
    float os2_wave;    
    float os2_glide;
    float os2_qu;   

    float mx_os1;
    float mx_os2;
    float mx_sg; 
    float mx_env;

    float fold_timb;
    float fold_bias;
    float amp_sel;  
    float vol;      

    float dly_tm;
    float dly_mx;
    float dly_fb;

    float mtx[NUM_MOD_SRCS][NUM_MOD_DESTS];

    // all indicators
    float ind_sg;
    float ind_env;
    float ind_os1;
    float ind_os1_freq;
    float ind_amp;

    // audio out
    float audio;
} DSP_All_Signals;

#endif  // DSP_INTERFACE_H_