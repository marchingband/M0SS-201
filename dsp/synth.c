#include <stdint.h>
#include <math.h>

#include "synth.h"
#include "interface.h"
#include "utils.h"

extern DSP_All_Signals all_sigs;

FB_Dests fb_dests;
EnvState_t env_state;
SGState_t sg_state;
OscState_t os1_state;
OscState_t os2_state;

void init_dsp() {
    for (uint16_t i = 0; i < MAX_SG_LENGTH; i++) {
        sg_state.arr[i] = randf();
    }
    sg_state.idx = 0;
}

float midi_freq;

void compute_midi_freq() {
    static float lp_state;
    float midi_note_freq = 440.0f * pow2f((all_sigs.pitch - 69.0f) * 0.08333f);
    // TODO: midi pitch bend
    float glide_s = linear_map(all_sigs.os2_glide, 0.0f, 1.0f, 0.01f, 0.6f);
    float coeff = pow2f(-1.0f * M_LOG2_E * SR_INV / glide_s);
    lp_state += (midi_note_freq - lp_state) * coeff;
    midi_freq = lp_state;
}

void compute_dsp(size_t len, float* buf) {
    for (size_t i = 0; i < len; i++) {
        // compute parameter amounts through matrix
        // takes matrix signals from all_sigs as well as previous outputs (stored in peripheral structs)
        // and writes to fb_dests
        compute_matrix();

        // compute modulator signals
        // SG, ENV, OS1

        compute_sg_rate();
        compute_env();
        compute_sg();

        compute_midi_freq();

        compute_os1();
        compute_os1_rate();

        // // compute OS2 output
        // CmplxModType_t mod_type = all_sigs.bus_sel;
        // float os2_freq = compute_os2_freq(mod_type); // takes into account mod_type
        // compute_os2(os2_freq, all_sigs.os2_wave, &os2_state);
        // if (mod_type == MOD_TYPE_AM) {
        //     // perform AM
        // }

        // // process signal path
        // // mixer -> folder -> amp -> delay
        // float mixed_signal = compute_mixer();
        // float timb = fb_dests.timb; 
        // if (mod_type == MOD_TYPE_TM) {
        //     // perform TM by modifying timb
        // }
        // float folded_signal = compute_wavefolder(mixed_signal, timb, all_sigs.fold_bias);
        // float amped_signal = compute_amplifier(folded_signal, fb_dests.amp, all_sigs.amp_sel);
        // if (i == len - 1) all_sigs.ind_amp = amped_signal;
        // float delayed_signal = compute_delay(amped_signal, all_sigs.dly_tm, all_sigs.dly_fb, all_sigs.dly_mx);

        // buf[i] = delayed_signal;
        buf[i] = os1_state.val;
    }
    all_sigs.ind_env = env_state.val;
    all_sigs.ind_sg = sg_state.val;
    all_sigs.ind_os1 = os1_state.val;
}

void compute_matrix() {
    fb_dests.len  = all_sigs.sg_len;
    fb_dests.rate = all_sigs.sg_rate;
    fb_dests.rcl  = all_sigs.sg_rcl;
    fb_dests.smth = all_sigs.sg_smth;
    fb_dests.att  = all_sigs.env_att;
    fb_dests.rel  = all_sigs.env_rel;
    fb_dests.os1  = all_sigs.os1_tune;
    fb_dests.os2  = all_sigs.os2_tune;
    fb_dests.bus  = 0.0f;
    fb_dests.idx  = all_sigs.bus_idx;
    fb_dests.timb = all_sigs.fold_timb;
    fb_dests.amp  = all_sigs.vol;
}

void compute_sg_rate() {
    const float min_freq = 0.1f;  // increments once every ten seconds
    const float max_freq = 4000.0f;  // 4kHz

    const int curvature = 10;
    float rate_hz = normalized_linear_to_logexp(all_sigs.sg_rate, curvature)*max_freq + min_freq;

    sg_state.rate_normalized = rate_hz * SR_INV;
    sg_state.phase += sg_state.rate_normalized;
    if (sg_state.phase >= 1.0f) sg_state.phase -= 1.0f;
    sg_state.high = sg_state.phase < 0.5f;
}

void compute_sg() {
    // TODO: shouldn't be all_sigs.gate, should be start of envelope cycle
    bool gate = (all_sigs.gate_sel == GATESEL_ENV)? all_sigs.gate : sg_state.high;

    uint16_t len_int = all_sigs.sg_len;
    len_int = CLAMP(len_int, MIN_SG_LENGTH, MAX_SG_LENGTH);
    if (gate && !sg_state.prev_gate) {
        sg_state.idx = (sg_state.idx + 1) % len_int;
        
        if (all_sigs.sg_rcl <= randf()) sg_state.arr[sg_state.idx] = randf();
        // sg_state.val = sg_state.arr[sg_state.idx];
    }

    // smooth out value
    // calculate coefficient
    float smooth_amt = (all_sigs.sg_smth >= 0.01f)? expf(-4.0f * sg_state.rate_normalized / all_sigs.sg_smth) : 0.0f;
    // simple LPF
    sg_state.lpf_state = (1 - smooth_amt)*sg_state.arr[sg_state.idx] + smooth_amt*sg_state.lpf_state;

    // output new value
    sg_state.val = sg_state.lpf_state;

    sg_state.prev_gate = gate;
}

// bias goes between 0 and 1, (can't be exactly 0)
// 0.5 is linear, less than 0.5 is exponential-ish, 
// greather than 0.5 is logarithmic-ish
inline float bias_curve(float b, float x) {
    return (x / ((((1.0f/b) - 2.0f)*(1.0f - x))+1.0f));
}

inline float bias_curve_d_dx(float b, float x) {
    float val1 = (1.0f-x+b*(-1.0f+2.0f*x));
    return -(-1+b)*b/(val1 * val1);
}

inline float bias_curve_inverse(float b, float y) {
    return (b-1.0f)*y / (2.0f*b*y-b-y);
}

inline float bias_clip(float b) {
    return CLAMP(b, 0.03f, 0.97f);
}

void compute_env() {
    bool trig = all_sigs.gate && !env_state.prev_gate;
    env_state.prev_gate = all_sigs.gate;

    if (trig || (env_state.stage == ENV_OFF && all_sigs.env_mode == ENVMODE_ARL)) {
        env_state.stage = ENV_ATT;
    }

    float bias = bias_clip(all_sigs.sg_smth);

    switch (env_state.stage) {
        case ENV_OFF: {
            env_state.val = 0.0f;
            break;
        }
        case ENV_ATT: {
            float att_samples = all_sigs.env_att * MAX_ENV_STAGE_LEN_SAMPLES;
            att_samples = CLAMP(att_samples, MIN_ENV_STAGE_LEN_SAMPLES, MAX_ENV_STAGE_LEN_SAMPLES);
            float slope = 1.0f / att_samples;
            
            float x = bias_curve_inverse(bias, env_state.val);
            slope *= bias_curve_d_dx(bias, x);

            env_state.val += slope;
            if (env_state.val >= 1.0f) {
                env_state.val = 1.0f;
                env_state.stage = (all_sigs.env_mode == ENVMODE_ASR)? ENV_SUS : ENV_REL;
            }
            break;
        }
        case ENV_SUS: {
            env_state.val = 1.0f;
            if (all_sigs.gate != 1.0f) env_state.stage = ENV_REL;
            break;
        }
        case ENV_REL: {
            float rel_samples = all_sigs.env_rel * MAX_ENV_STAGE_LEN_SAMPLES;
            rel_samples = CLAMP(rel_samples, MIN_ENV_STAGE_LEN_SAMPLES, MAX_ENV_STAGE_LEN_SAMPLES);
            float slope = 1.0f / rel_samples;
            
            float x = bias_curve_inverse(1.0f - bias, env_state.val);
            slope *= bias_curve_d_dx(1.0f - bias, x);

            env_state.val -= slope;
            if (env_state.val <= 0.0f) {
                env_state.val = 0.0f;
                env_state.stage = (all_sigs.env_mode == ENVMODE_ARL)? ENV_ATT : ENV_OFF;
            }
            break;
        }
    }
}

void compute_os1() {
    // parameters that affect os1: 
    // slow/fast mode, key tracking on/off, 
    // tune (fine/course), waveform

    // intervals determine number of octaves range
    const float fast_mode_course_max_interval = 6.0f;
    const float fast_mode_fine_max_interval = 1.0f;
    const float fast_mode_base_freq_hz = 20.0f;

    // intervals determine number of octaves range
    const float slow_mode_course_max_interval = 10.0f;
    const float slow_mode_fine_max_interval = 1.0f;
    const float slow_mode_base_freq_hz = 0.1f;

    float freq_hz = 0.0f;
    if (all_sigs.os1_hilo == OS1_RANGE_LO) {
        float total_interval = all_sigs.os1_tune * slow_mode_course_max_interval + all_sigs.os1_fine * slow_mode_fine_max_interval;
        float multiplier = pow2f(total_interval);
        freq_hz = ((all_sigs.os1_kt)? midi_freq * 0.125f : fast_mode_base_freq_hz) * multiplier;
    } else {
        float total_interval = all_sigs.os1_tune * fast_mode_course_max_interval + all_sigs.os1_fine * fast_mode_fine_max_interval;
        float multiplier = pow2f(total_interval);
        freq_hz = ((all_sigs.os1_kt)? midi_freq * 0.001953f : slow_mode_base_freq_hz) * multiplier;
    }

    float freq_normalized = freq_hz * SR_INV;

    float this_tri = os1_state.next_tri;
    float this_saw = os1_state.next_saw;
    float this_sqr = os1_state.next_sqr;
    os1_state.next_tri = 0.0f;
    os1_state.next_saw = 0.0f;
    os1_state.next_sqr = 0.0f;

    os1_state.phase += freq_normalized;

    if (os1_state.high && os1_state.phase > 0.5f) {
        float t = (os1_state.phase - 0.5f) / freq_normalized;
        float discontinuity = 4.0f * freq_normalized;
        this_tri -= this_iblep(t) * discontinuity;
        os1_state.next_tri -= next_iblep(t) * discontinuity;

        this_sqr -= this_blep(t);
        os1_state.next_sqr -= next_blep(t);

        os1_state.high = false;
    }

    if (os1_state.phase >= 1.0f) {
        os1_state.phase -= 1.0f;

        float t = os1_state.phase / freq_normalized;
        float discontinuity = 4.0f * freq_normalized;
        this_tri += this_iblep(t) * discontinuity;
        os1_state.next_tri += next_iblep(t) * discontinuity;

        this_sqr += this_blep(t);
        os1_state.next_sqr += next_blep(t);

        this_saw -= this_blep(t);
        os1_state.next_saw -= next_blep(t);

        os1_state.high = true;
    }

    if (all_sigs.os1_hilo == OS1_RANGE_LO) {
        this_tri = os1_state.high? 2.0f * os1_state.phase : 2.0f * (1.0f - os1_state.phase);
        this_saw = os1_state.phase;
        this_sqr = os1_state.high? 1.0f : 0.0f;
    } else {
        this_tri += os1_state.high? 2.0f * os1_state.phase : 2.0f * (1.0f - os1_state.phase);
        this_saw += os1_state.phase;
        this_sqr += os1_state.high? 1.0f : 0.0f;
    }

    switch ((uint32_t)all_sigs.os1_wave) {
        case OS1_WAV_TRI:
            os1_state.val = 2.0f * this_tri - 1.0f;
            break;
        case OS1_WAV_SQR:
            os1_state.val = 2.0f * this_sqr - 1.0f;
            break;
        case OS1_WAV_SAW:
            os1_state.val = 2.0f * this_saw - 1.0f;
            break;
    }
    // if (os1_state.val > 1.0f || os1_state.val < -1.0f) printf("max value exceeded!: %0.4f\n", os1_state.val);
}

// float compute_os2_rate() {
//     // care about midi frequency, tuning, and mod bus
//     // also about quantization and glide
//     float freq_hz = midi_freq;
//     float tuning_range_octaves = 1.0f;  // one octave up and down
//     freq_hz *= pow2f(tuning_range_octaves * all_sigs.os2_tune);
// }

void compute_os2() {
    // float freq_normalized = freq * SR_INV;

    // float this_tri = state_p->next_tri;
    // state_p->next_tri = 0.0f;

    // state_p->phase += freq_normalized;

    // // handle down transition
    // if (state_p->high && state_p->phase > 0.5f) {
    //     float t = (state_p->phase - 0.5f) / freq_normalized;
    //     float discontinuity = 4.0f * freq_normalized;
    //     this_tri -= this_iblep(t) * discontinuity;
    //     state_p->next_tri -= next_iblep(t) * discontinuity;
    //     state_p->high = false;
    // }

    // // handle up transition
    // if (state_p->phase >= 1.0f) {
    //     state_p->phase -= 1.0f;
    //     float t = state_p->phase / freq_normalized;
    //     float discontinuity = 4.0f * freq_normalized;
    //     this_tri += this_iblep(t) * discontinuity;
    //     state_p->next_tri += next_iblep(t) * discontinuity;
    //     state_p->high = true;
    // }

    // state_p->next_tri += state_p->high? 2.0f * state_p->phase :
    //                                     2.0f * (1.0f - state_p->phase);
    
    // float this_sine = sin2pif(state_p->phase);
    // this_tri = 2.0f * this_tri - 1.0f;

    // OS1_Wave_Sel_t wav_sel_ = wav_sel;  // cast to enum
    
    // state_p->val = (wav_sel_ == OS1_WAV_SINE)? this_sine :
    //                (wav_sel_ == OS1_WAV_TRI)? this_tri : (this_sine + this_tri) * 0.5f;
}

float compute_mixer() {
    return sg_state.val * all_sigs.mx_sg + env_state.val * all_sigs.mx_env \
        + os1_state.val * all_sigs.mx_os1 + os2_state.val * all_sigs.mx_os2;
}

float compute_wavefolder(float in, float timbre, float bias) {
    // TODO
    return in;
}

float compute_amplifier(float in, float gain, float mode_sel) {
    // TODO
    return in * all_sigs.gate;
}

float compute_delay(float in, float time, float fb, float mix) {
    // TODO
    return in;
}