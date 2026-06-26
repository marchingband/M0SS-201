#ifndef DSP_INTERFACE_H_
#define DSP_INTERFACE_H_

#include <stdint.h>
#include <stdbool.h>

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
    const int16_t cc;  
    const int16_t btn;
    const float min;
    const float max;
    const float init;
    const float step;
    const bool numeric;
} DSP_Param_t;

#define NUM_UI_ELS 65

#define NUM_MOD_SRCS 3
#define NUM_MOD_DESTS 10
// I for index, S for source
#define OS2_I_S 0
#define SG_I_S  1
#define ENV_I_S 2

#define RATE_I 0
#define RCL_I  1
#define SMTH_I 2
#define ATT_I  3
#define REL_I  4
#define OS1_I  5
#define OS2_I  6
#define IDX_I  7
#define TIMB_I 8
#define AMP_I  9


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
    float os1_oct;  
    float os1_wave; 
    float os1_qtz;  
    float os1_gld;  
    float os2_wave; 
    float os2_tune; 
    float os2_hilo; 
    float os2_kt;   
    float bus_idx;  
    float bus_sel;  
    float mx_os1;   
    float mx_os2;   
    float mx_sg;    
    float mx_env;   
    float fold_timb;
    float fold_bias;
    float pb_up;
    float pb_down;
    float amp_sel;  
    float vol;      
    float dly_tm;   
    float dly_mx;   
    float dly_fb;
    float dly_flt;
    float legato;   

    float mtx[NUM_MOD_SRCS][NUM_MOD_DESTS];

    // all indicators
    float ind_sg;
    float ind_env;
    float ind_os2;
    float ind_amp;

    // special signals
    float midi_chan;
    float pitch_bend; 
} DSP_All_Signals;

#endif  // DSP_INTERFACE_H_