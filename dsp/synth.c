#include <stdint.h>

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
        sg_state.arr[i] = 0.0f;
    }
    sg_state.idx = 0;
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
        // compute_os1(fb_dests.os1, all_sigs.os1_hilo, all_sigs.os1_kt, all_sigs.pitch, all_sigs.os1_wave, &os1_state);

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
        buf[i] = 0.0f; // (sg_state.high)? 0.2f : -0.2f;
    }
    all_sigs.ind_env = all_sigs.gate;
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

float compute_os2_freq() {
    // involves all of these
    // fb_dests.os2, all_sigs.pitch, all_sigs.os2_glide, all_sigs.os2_qu, all_sigs.bus_sel, fb_dests.idx, fb_dests.bus
    return 440.0f * pow2f((all_sigs.pitch - 69.0) * 0.08333f);
}

void compute_sg_rate() {
    const float min_freq = 0.1f;  // increments once every ten seconds
    const float max_freq = 4000.0f;  // 4kHz

    const int curvature = 10;
    float rate_hz = normalized_linear_to_logexp(all_sigs.sg_rate, curvature)*max_freq + min_freq;

    float rate_normalized = rate_hz * SR_INV;
    sg_state.phase += rate_normalized;
    if (sg_state.phase >= 1.0f) sg_state.phase -= 1.0f;
    sg_state.high = sg_state.phase < 0.5f;
}

void compute_sg() {
    // uint16_t len_int = len * (MAX_SG_LENGTH - MIN_SG_LENGTH) + MIN_SG_LENGTH;
    // // rising edge, increment the SG
    // if (gate && !state_p->prev_gate) {
    //     // increment
    //     state_p->idx = (state_p->idx + 1) % len_int;
    //     uint16_t write_idx = (state_p->idx + 1) % len_int;

    //     // write new random value
    //     if (recall >= randf()) {
    //         state_p->arr[write_idx] = randf();
    //     }
    // }
    // TODO: shouldn't be all_sigs.gate, should be 
    bool gate = (all_sigs.gate_sel == GATESEL_ENV)? all_sigs.gate : sg_state.high;

    uint16_t len_int = 2;
    if (gate && !sg_state.prev_gate) {
        sg_state.idx = (sg_state.idx + 1) % len_int;
        // uint16_t write_idx = (state_p->idx + 1) % len_int;
        
        if (all_sigs.sg_rcl <= randf()) sg_state.arr[sg_state.idx] = randf();
        sg_state.val = sg_state.arr[sg_state.idx];
    }

    // // smooth out value
    // // calculate coefficient
    // float smooth_amt = (smooth >= 0.01f)? expf(-4.0f * state_p->rate_normalized / smooth) : 0.0f;
    // // simple LPF
    // state_p->lpf_state = (1 - smooth_amt)*state_p->arr[state_p->idx] + smooth_amt*state_p->lpf_state;

    // // output new value
    // state_p->val = state_p->lpf_state;

    sg_state.prev_gate = gate;
}

void compute_env() {

}

void compute_os1() {

}

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