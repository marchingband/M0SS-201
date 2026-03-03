#include <stdint.h>

#include "synth.h"
#include "interface.h"
extern DSP_All_Signals all_sigs;

FB_Dests fb_dests;
EnvState_t env_state;
SGState_t sg_state;
OscState_t os1_state;
OscState_t os2_state;

void compute_dsp(size_t len, float* buf) {
    for (size_t i = 0; i < len; i++) {
        // compute parameter amounts through matrix
        // takes matrix signals from all_sigs as well as previous outputs (stored in peripheral structs)
        // and writes to fb_dests
        compute_matrix();

        // compute modulator signals
        // SG, ENV, OS1
        Gate_Sel_t gate_select = all_sigs.gate_sel;
        compute_sg_rate(fb_dests.rate, &sg_state);
        float env_gate_in = (gate_select == GATESEL_SG)? sg_state.gate : all_sigs.gate;
        float sg_gate_in = (gate_select == GATESEL_ENV)? env_state.gate : sg_state.gate;
        compute_env(fb_dests.att, fb_dests.rel, env_gate_in, &env_state);
        compute_sg(fb_dests.len, fb_dests.rcl, fb_dests.smth, sg_gate_in, &sg_state);
        compute_os1(fb_dests.os1, all_sigs.os1_hilo, all_sigs.os1_kt, all_sigs.pitch, all_sigs.os1_wave, &os1_state);

        // compute OS2 output
        CmplxModType_t mod_type = all_sigs.bus_sel;
        float os2_freq = compute_os2_freq(mod_type); // takes into account mod_type
        compute_os2(os2_freq, all_sigs.os2_wave, &os2_state);
        if (mod_type == MOD_TYPE_AM) {
            // perform AM
        }

        // process signal path
        // mixer -> folder -> amp -> delay
        float mixed_signal = compute_mixer();
        float timb = fb_dests.timb; 
        if (mod_type == MOD_TYPE_TM) {
            // perform TM by modifying timb
        }
        float folded_signal = compute_wavefolder(mixed_signal, timb, all_sigs.fold_bias);
        float amped_signal = compute_amplifier(folded_signal, fb_dests.amp, all_sigs.amp_sel);
        if (i == len - 1) all_sigs.ind_amp = amped_signal;
        float delayed_signal = compute_delay(amped_signal, all_sigs.dly_tm, all_sigs.dly_fb, all_sigs.dly_mx);

        buf[i] = delayed_signal;
    }
    all_sigs.ind_env = env_state.val;
    all_sigs.ind_sg = sg_state.val;
    all_sigs.ind_os1 = os1_state.val;
}

void compute_matrix() {

}

float compute_os2_freq() {
    // involves all of these
    // fb_dests.os2, all_sigs.pitch, all_sigs.os2_glide, all_sigs.os2_qu, all_sigs.bus_sel, fb_dests.idx, fb_dests.bus
}