#include <stdint.h>

#include "bflb_gpio.h"

#include "synth.h"
#include "interface.h"
#include "utils.h"
#include "fold_lut.h"

extern DSP_All_Signals all_sigs_raw;

FB_Dests fb_dests;
EnvState_t env_state;
SGState_t sg_state;
OscState_t os1_state;
OscState_t os2_state;
InterpolatingDelay_t dly;

DSP_All_Signals all_sigs;

Biquad_t dec_filt1, dec_filt2, dec_filt3;

const float param_smooth_coeff = 0.9997917f; //pow2f(-1.0f * LOG2_E * SR_INV / 0.1f);

void smooth_signals() {
    // all parameters
    float c = 1.0f - param_smooth_coeff;
    all_sigs.sg_smth += c * (all_sigs_raw.sg_smth - all_sigs.sg_smth);
    all_sigs.sg_rate += c * (all_sigs_raw.sg_rate - all_sigs.sg_rate);
    all_sigs.sg_rcl += c * (all_sigs_raw.sg_rcl - all_sigs.sg_rcl);
    all_sigs.env_att += c * (all_sigs_raw.env_att - all_sigs.env_att);
    all_sigs.env_rel += c * (all_sigs_raw.env_rel - all_sigs.env_rel);
    all_sigs.env_crv += c * (all_sigs_raw.env_crv - all_sigs.env_crv);
    all_sigs.os2_tune += c * (all_sigs_raw.os2_tune - all_sigs.os2_tune);
    all_sigs.bus_idx += c * (all_sigs_raw.bus_idx - all_sigs.bus_idx); 
    all_sigs.os1_gld += c * (all_sigs_raw.os1_gld - all_sigs.os1_gld);
    all_sigs.mx_os1 += c * (all_sigs_raw.mx_os1 - all_sigs.mx_os1);
    all_sigs.mx_os2 += c * (all_sigs_raw.mx_os2 - all_sigs.mx_os2);
    all_sigs.mx_sg += c * (all_sigs_raw.mx_sg - all_sigs.mx_sg); 
    all_sigs.mx_env += c * (all_sigs_raw.mx_env - all_sigs.mx_env);
    all_sigs.fold_timb += c * (all_sigs_raw.fold_timb - all_sigs.fold_timb);
    all_sigs.fold_bias += c * (all_sigs_raw.fold_bias - all_sigs.fold_bias); 
    all_sigs.vol += c * (all_sigs_raw.vol - all_sigs.vol);      
    all_sigs.dly_tm += c * (all_sigs_raw.dly_tm - all_sigs.dly_tm);
    all_sigs.dly_mx += c * (all_sigs_raw.dly_mx - all_sigs.dly_mx);
    all_sigs.dly_fb += c * (all_sigs_raw.dly_fb - all_sigs.dly_fb);
    for (uint16_t i = 0; i < NUM_MOD_SRCS; i++) {
        for (uint16_t j = 0; j < NUM_MOD_DESTS; j++) {
           all_sigs.mtx[i][j] += c * (all_sigs_raw.mtx[i][j] - all_sigs.mtx[i][j]); 
        }
    }
}

#define OVERSAMPLE_RATIO 4

#define DBG_TIMING
#define DBG_TIMING_PIN 15

void init_dsp() {
    for (uint16_t i = 0; i < MAX_SG_LENGTH; i++) {
        sg_state.arr[i] = randf();
    }
    sg_state.idx = 0;

    dec_filt1 = dec_filt2 = dec_filt3 = generate_lowpass(SR * OVERSAMPLE_RATIO, SR >> 1, 0.707f);

    dly.buffer_size = DELAY_LINE_LEN;
    dly.ptr = 0;

    #ifdef DBG_TIMING
    struct bflb_device_s* gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, DBG_TIMING_PIN, GPIO_OUTPUT | GPIO_SMT_EN | GPIO_DRV_0);
    #endif
}

float midi_note;

void compute_midi_note() {
    static float lp_state;
    // TODO: midi pitch bend
    float glide_s = linear_map(all_sigs.os1_gld, 0.0f, 1.0f, 0.01f, 0.6f);
    float coeff = pow2f(-1.0f * LOG2_E * SR_INV / glide_s);
    lp_state += (all_sigs_raw.pitch - lp_state) * coeff;
    midi_note = lp_state;
}

float phase = 0.0f;
const float freq = 260.0f * SR_INV;

void compute_dsp(size_t len, float* buf) {
    #ifdef DBG_TIMING
    struct bflb_device_s* gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_set(gpio, DBG_TIMING_PIN);
    #endif

    for (size_t i = 0; i < len; i++) {
        // phase += freq;
        // if (phase >= 1.0f) phase -= 1.0f;
        // buf[i] = sin2pif(phase) * 0.1f;
        // continue;


        // compute parameter amounts through matrix
        // takes matrix signals from all_sigs as well as previous outputs (stored in peripheral structs)
        // and writes to fb_dests
        smooth_signals();

        compute_matrix();

        // compute modulator signals
        // SG, ENV, OS1

        compute_sg_rate();
        compute_env();
        compute_sg();

        compute_midi_note();

        compute_os2();
        compute_os1();
        float signal = 0.0f;
        signal = compute_mixer();
        signal = compute_wavefolder(signal);
        signal = compute_amplifier(signal);
        signal = compute_delay(signal);

        float volume = normalized_linear_to_logexp(all_sigs.vol, 4);
        buf[i] = signal * volume;
    }
    all_sigs_raw.ind_env = env_state.val; //  env_state.val;
    all_sigs_raw.ind_sg = sg_state.val;
    all_sigs_raw.ind_os2 = os2_state.val;
    all_sigs_raw.ind_amp = buf[0]; //0.5f*buf[len-1] + 0.5f;

    #ifdef DBG_TIMING
    bflb_gpio_reset(gpio, DBG_TIMING_PIN);
    #endif
}

// os1 is bipolar, sg and env are unipolar
// therefore just by adding the signals the range is [-1, 3]
// os1 should have the same total effect as the other two however,
// so the operation to normalize should be ((os1 + 1)/2 + sg + env) / 3

#define E_MAT env_state.val * all_sigs.mtx[ENV_I_S]
#define S_MAT sg_state.val * all_sigs.mtx[SG_I_S]
#define O_MAT os1_state.val * all_sigs.mtx[OS2_I_S]

#define MAT(x, y) CLAMP((E_MAT[x]) + (S_MAT[x]) + (O_MAT[x]) + (y), 0.0f, 1.0f)

float os1_mod_freq;

void compute_matrix() {
    fb_dests.rate = MAT(RATE_I, all_sigs.sg_rate);
    fb_dests.smth = MAT(SMTH_I, all_sigs.sg_smth);
    fb_dests.rcl  = MAT(RCL_I,  all_sigs.sg_rcl);
    fb_dests.att  = MAT(ATT_I,  all_sigs.env_att);
    fb_dests.rel  = MAT(REL_I,  all_sigs.env_rel);
    fb_dests.os1  = MAT(OS1_I,  os1_mod_freq);
    fb_dests.os2  = MAT(OS2_I,  all_sigs.os2_tune);
    fb_dests.idx  = MAT(IDX_I,  all_sigs.bus_idx);
    fb_dests.timb = MAT(TIMB_I, all_sigs.fold_timb);
    fb_dests.amp  = MAT(AMP_I,  0.0f);
}

void compute_sg_rate() {
    const float min_freq = 0.1f;  // increments once every ten seconds
    const float max_freq = 4000.0f;  // 4kHz

    const int curvature = 10;
    float rate_hz = normalized_linear_to_logexp(fb_dests.rate, curvature)*max_freq + min_freq;

    sg_state.rate_normalized = rate_hz * SR_INV;
    sg_state.phase += sg_state.rate_normalized;
    if (sg_state.phase >= 1.0f) sg_state.phase -= 1.0f;
    sg_state.high = sg_state.phase < 0.5f;
}

void compute_sg() {
    // TODO: shouldn't be all_sigs.gate, should be start of envelope cycle
    bool gate = (all_sigs_raw.gate_sel == GATESEL_ENV)? all_sigs_raw.gate : sg_state.high;

    uint16_t len_int = all_sigs_raw.sg_len;
    len_int = CLAMP(len_int, MIN_SG_LENGTH, MAX_SG_LENGTH);
    if (gate && !sg_state.prev_gate) {
        sg_state.idx = (sg_state.idx + 1) % len_int;
        
        if (fb_dests.rcl <= randf()) sg_state.arr[sg_state.idx] = randf();
        // sg_state.val = sg_state.arr[sg_state.idx];
    }

    // smooth out value
    // calculate coefficient
    float smooth_amt = (fb_dests.smth >= 0.01f)? expf(-4.0f * sg_state.rate_normalized / fb_dests.smth) : 0.0f;
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
    bool trig = all_sigs_raw.gate && !env_state.prev_gate;
    env_state.prev_gate = all_sigs_raw.gate;

    if (trig || (env_state.stage == ENV_OFF && all_sigs_raw.env_mode == ENVMODE_ARL)) {
        env_state.stage = ENV_ATT;
    }

    float bias = bias_clip(1.0f - all_sigs.env_crv);

    switch (env_state.stage) {
        case ENV_OFF: {
            env_state.val = 0.0f;
            break;
        }
        case ENV_ATT: {
            float att_samples = fb_dests.att * MAX_ENV_STAGE_LEN_SAMPLES;
            att_samples = CLAMP(att_samples, MIN_ENV_STAGE_LEN_SAMPLES, MAX_ENV_STAGE_LEN_SAMPLES);
            float slope = 1.0f / att_samples;
            
            float x = bias_curve_inverse(bias, env_state.val);
            slope *= bias_curve_d_dx(bias, x);

            env_state.val += slope;
            if (env_state.val >= 1.0f) {
                env_state.val = 1.0f;
                env_state.stage = (all_sigs_raw.env_mode == ENVMODE_ASR)? ENV_SUS : ENV_REL;
            }
            break;
        }
        case ENV_SUS: {
            env_state.val = 1.0f;
            if (all_sigs_raw.gate != 1.0f) env_state.stage = ENV_REL;
            break;
        }
        case ENV_REL: {
            float rel_samples = fb_dests.rel * MAX_ENV_STAGE_LEN_SAMPLES;
            rel_samples = CLAMP(rel_samples, MIN_ENV_STAGE_LEN_SAMPLES, MAX_ENV_STAGE_LEN_SAMPLES);
            float slope = 1.0f / rel_samples;
            
            float x = bias_curve_inverse(1.0f - bias, env_state.val);
            slope *= bias_curve_d_dx(1.0f - bias, x);

            env_state.val -= slope;
            if (env_state.val <= 0.0f) {
                env_state.val = 0.0f;
                env_state.stage = (all_sigs_raw.env_mode == ENVMODE_ARL)? ENV_ATT : ENV_OFF;
            }
            break;
        }
    }
}

void compute_os2() {
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
    float midi_freq = midi_note_to_freq_hz(midi_note);
    if (all_sigs_raw.os2_hilo == OS2_RANGE_LO) {
        float total_interval = fb_dests.os2 * slow_mode_course_max_interval;
        float multiplier = pow2f(total_interval);
        freq_hz = ((all_sigs_raw.os2_kt)? midi_freq * 0.001953f : slow_mode_base_freq_hz) * multiplier;
    } else {
        float total_interval = fb_dests.os2 * fast_mode_course_max_interval;
        float multiplier = pow2f(total_interval);
        freq_hz = ((all_sigs_raw.os2_kt)? midi_freq * 0.125f : fast_mode_base_freq_hz) * multiplier;
    }

    float freq_normalized = freq_hz * SR_INV;

    float this_tri = os2_state.next_tri;
    float this_saw = os2_state.next_saw;
    float this_sqr = os2_state.next_sqr;
    os2_state.next_tri = 0.0f;
    os2_state.next_saw = 0.0f;
    os2_state.next_sqr = 0.0f;

    os2_state.phase += freq_normalized;

    if (os2_state.high && os2_state.phase >= 0.5f) {
        float t = (os2_state.phase - 0.5f) / freq_normalized;
        float discontinuity = 4.0f * freq_normalized;
        this_tri -= this_iblep(t) * discontinuity;
        os2_state.next_tri -= next_iblep(t) * discontinuity;

        this_sqr -= this_blep(t);
        os2_state.next_sqr -= next_blep(t);

        os2_state.high = false;
    }

    if (os2_state.phase >= 1.0f) {
        os2_state.phase -= 1.0f;

        float t = os2_state.phase / freq_normalized;
        float discontinuity = 4.0f * freq_normalized;
        this_tri += this_iblep(t) * discontinuity;
        os2_state.next_tri += next_iblep(t) * discontinuity;

        this_sqr += this_blep(t);
        os2_state.next_sqr += next_blep(t);

        this_saw -= this_blep(t);
        os2_state.next_saw -= next_blep(t);

        os2_state.high = true;
    }

    this_tri += os2_state.high? 2.0f * os2_state.phase : 2.0f * (1.0f - os2_state.phase);
    this_saw += os2_state.phase;
    this_sqr += os2_state.high? 1.0f : 0.0f;

    switch ((uint32_t)all_sigs_raw.os2_wave) {
        case OS2_WAV_TRI:
            os2_state.val = 2.0f * this_tri - 1.0f;
            break;
        case OS2_WAV_SQR:
            os2_state.val = 2.0f * this_sqr - 1.0f;
            break;
        case OS2_WAV_SAW:
            os2_state.val = 2.0f * this_saw - 1.0f;
            break;
    }                        
    // if (os1_state.val > 1.0f || os1_state.val < -1.0f) printf("max value exceeded!: %0.4f\n", os1_state.val);
}

float quantize(float freq_hz, const Scale* scale) {
    // the scale contains a root frequency, need to get the
    // input frequency to be in the same octave.
    float rel_octave = pow2f((floorf(log2f(freq_hz / scale->root_freq_hz))));
    float norm_ratio = freq_hz / (scale->root_freq_hz * rel_octave);

    float quantized_ratio = 1.0f;
    float min_ratio_diff = 1e8f;
    for (uint32_t i = 0; i < scale->num_intervals; i++) {
        float ratio_diff = absf(norm_ratio - scale->intervals[i]);
        if (ratio_diff < min_ratio_diff) {
            min_ratio_diff = ratio_diff;
            quantized_ratio = scale->intervals[i];
        }
    }

    return scale->root_freq_hz * rel_octave * quantized_ratio;
}

Scale pentatonic = {.root_freq_hz = 0.0f, .num_intervals = 5, .intervals = {
    1.0f, 1.189207f, 1.3348398f, 1.49830708f, 1.781797f}
};

float compute_os1_rate() {
    // care about midi frequency, tuning, and mod bus
    // also about quantization and glide

    // midi_freq is already computed with glide and pitch bend accounted for
    // so the tuning slider, modulations, and FM need to be added.
    // tuning slider and modulations get quantized, then FM is added.

    float tuning_range_octaves = 1.0f;  // one octave up and down
    float freq_multiplier = pow2f(fb_dests.os1 * 2.0f + tuning_range_octaves * fb_dests.os1); // maximum two octave range for normalized value

    pentatonic.root_freq_hz = midi_note_to_freq_hz(all_sigs_raw.pitch);
    if (all_sigs_raw.os1_qtz != 0) freq_multiplier = quantize(freq_multiplier, &pentatonic);

    float freq_hz = freq_multiplier * midi_note_to_freq_hz(midi_note);
    if (all_sigs_raw.bus_sel == BUS_SEL_FM) {
        float max_fm_index = 0.5f;
        freq_hz *= 1.0f + max_fm_index*fb_dests.idx*os2_state.val;
    }
    return freq_hz;
}

void compute_os1() {
    float freq_normalized = compute_os1_rate() * SR_INV;

    float this_tri = os1_state.next_tri;
    os1_state.next_tri = 0.0f;

    os1_state.phase += freq_normalized;

    if (os1_state.high && os1_state.phase > 0.5f) {
        float t = (os1_state.phase - 0.5f) / freq_normalized;
        float discontinuity = 4.0f * freq_normalized;
        this_tri -= this_iblep(t) * discontinuity;
        os1_state.next_tri -= next_iblep(t) * discontinuity;

        os1_state.high = false;
    }

    if (os1_state.phase >= 1.0f) {
        os1_state.phase -= 1.0f;

        float t = os1_state.phase / freq_normalized;
        float discontinuity = 4.0f * freq_normalized;
        this_tri += this_iblep(t) * discontinuity;
        os1_state.next_tri += next_iblep(t) * discontinuity;

        os1_state.high = true;
    }

    this_tri += 2.0f * (os1_state.high? 2.0f * os1_state.phase : 2.0f * (1.0f - os1_state.phase)) - 1.0f;
    float this_sine = sin2pif(os1_state.phase);

    float selected_signal;

    switch ((uint32_t)all_sigs_raw.os1_wave) {
        case OS1_WAV_SINE:
            selected_signal = this_sine;
            break;
        case OS1_WAV_TRI:
            selected_signal = this_tri;
            break;
        case OS1_WAV_BOTH:
            selected_signal = (this_tri + this_sine) * 0.5f;
            break;
    }

    if (all_sigs_raw.bus_sel == BUS_SEL_AM) {
        selected_signal *= 1.0f - fb_dests.idx*(1.0f - os1_state.val);
    }
    os1_state.val = selected_signal;
}

float compute_mixer() {
    const int16_t power_law_coeff = 4;
    float bipolar_sg = 2.0f*sg_state.val - 1.0f;
    float bipolar_env = 2.0f*env_state.val - 1.0f;
    float power_law_sg = normalized_linear_to_logexp(all_sigs.mx_sg,   power_law_coeff);
    float power_law_env = normalized_linear_to_logexp(all_sigs.mx_env, power_law_coeff);
    float power_law_os1 = normalized_linear_to_logexp(all_sigs.mx_os1, power_law_coeff);
    float power_law_os2 = normalized_linear_to_logexp(all_sigs.mx_os2, power_law_coeff);
    return bipolar_sg * power_law_sg + bipolar_env * power_law_env \
        + os1_state.val * power_law_os1 + os2_state.val * power_law_os2;
}

float parabolic_fold(float x) {
    const float a = 0.6f;
    const float b = 1.0f / (2.0f * a);
    const float xm2 = (x - 1.0f)*(x - 1.0f);
    const float xp2 = (x + 1.0f)*(x + 1.0f);
    if (x < -1.0f-a) return -2.0f - x;
    if (x < -1.0f+a) return b*xp2 - 1.0f + 0.5f*a;
    if (x <  1.0f-a) return x;
    if (x <  1.0f+a) return -b*xm2 + 1.0f - 0.5f*a;
    return 2.0f - x;
}

float plaits_fold(float in) {
    in *= 0.02f;
    if (in > 1.0f) return 1.0f;
    if (in < -1.0f) return -1.0f;
    return InterpolateHermite(fold_lut + 1, in*0.5f + 0.5f, 512.0f);
}

float compute_wavefolder(float in) {
    // originally, signal is normalized to be between -1 and 1.
    // The mixer adds four signals together, so if they all line up
    // the signal could technically be between +/-4
    // generally though, there's just going to be one signal (or maybe two).
    // mixing two together with no folding applied should sound clean,
    // adding the modulators (ENV and SG) it would be acceptable for them
    // to hit the wavefolder (that's intended as an automated bias control).
    // Emilie's LUT has a linear region of around +/-0.02, and the first
    // fold crests at 0.06
    // when the signal gets all the way to +/-1, the output reaches 1,
    // which is our 0dBFS level.

    // gain-staging wise, this is the point in the signal chain where we need
    // to reduce levels from their full-scale representation to their normal
    // version. In this case, that means that +/-0.5 is an acceptable level
    // for normal volume. Increasing to +/-1 would be making it loud, which
    // the wavefolder LUT does do.

    // add bias before gain-staging
    // okay for it to be unipolar, it would sound the same on
    // the negative side anyway
    in += all_sigs.fold_bias;

    // minimum timbre is no change in signal level
    // maximum timbre is all the way to +/-1, so 50x
    // above that value the folder will simply saturate
    float t = fb_dests.timb * 25.0f;
    if (all_sigs_raw.bus_sel == BUS_SEL_TM) {
        const float max_timbre_modulation = 1.0f;
        t += max_timbre_modulation * fb_dests.idx * os2_state.val * 25.0f;
    }
    t += 1.0f;

    in *= t;

    static float prev_in = 0.0f;
    float os_outputs[OVERSAMPLE_RATIO];
    for (uint16_t i = 0; i < OVERSAMPLE_RATIO; i++) {
        // 0.25f for a 4x oversample ratio
        float frac = (float)i * 0.25f;
        float interpolated_sample = prev_in + (in - prev_in) * frac;

        //float folded_sample = plaits_fold(interpolated_sample);
        #define FOLD_STAGES 2
        for (uint16_t j = 0; j < FOLD_STAGES; j++) {
            interpolated_sample = plaits_fold(interpolated_sample);
        }
        float folded_sample = interpolated_sample;

        os_outputs[i] = process_biquad(&dec_filt3, process_biquad(&dec_filt2, process_biquad(&dec_filt1, folded_sample)));
    }
    prev_in = in;
    return os_outputs[OVERSAMPLE_RATIO - 1];
}

Vactrol_t vact;

void compute_vactrol_params(float level, float short_decay, float decay_tail, float hf) {
        float vactrol_input = level;
        float vactrol_error = (vactrol_input - vact.state);
        float vactrol_state_2 = vact.state * vact.state;
        float vactrol_state_4 = vactrol_state_2 * vactrol_state_2;
        float tail = 1.0f - vact.state;
        float tail_2 = tail * tail;
        float vactrol_coefficient = (vactrol_error > 0.0f)? 0.6f \
            : short_decay + (1.0f - vactrol_state_4) * decay_tail;

        vact.state += vactrol_coefficient * vactrol_error;
        vact.gain = vact.state;
        vact.normalized_freq = 0.003f + 0.3f * vactrol_state_4 + hf * 0.04f;
        vact.hf_bleed = (tail_2 + (1.0f - tail_2) * hf) * hf * hf;
    }

const float PI = 3.1415926f;
float compute_amplifier(float in) {
    float gain = fb_dests.amp; // .amp;
    if (all_sigs_raw.amp_sel == AMP_VCA) return in * gain;

    compute_vactrol_params(gain, 0.01f, 0.005f, 0.2f);
    float freq_coeff = 1.0f - expf(-2.0f * PI * vact.normalized_freq);
    in *= vact.gain;

    vact.lpf_state += freq_coeff*(in - vact.lpf_state);

    return vact.lpf_state + (in - vact.lpf_state) * vact.hf_bleed;
}

float compute_delay(float in) {
    // compute parameters
    float delay_time = all_sigs.dly_tm * (float)(dly.buffer_size - 1);
    float feedback = normalized_linear_to_logexp(all_sigs.dly_fb, 4);
    float mix = all_sigs.dly_mx;

    // get read pointer
    float read_ptr = (float)dly.ptr - delay_time;
    read_ptr = fmodf(read_ptr, dly.buffer_size);
    if (read_ptr < 0) read_ptr += dly.buffer_size;

    // find new sample (interpolating)
    MAKE_INTEGRAL_FRACTIONAL(read_ptr);
    float sample1 = dly.buffer[read_ptr_integral];
    float sample2 = dly.buffer[(read_ptr_integral + 1) % dly.buffer_size];
    float delayed_sample = sample1 + read_ptr_fractional*(sample2 - sample1);
    if (absf(delayed_sample) < 1e-15f) delayed_sample = 0.0f;

    // handle feedback
    dly.buffer[dly.ptr] = in + feedback*delayed_sample;

    // update pointer
    dly.ptr = (dly.ptr + 1) % dly.buffer_size;

    // output value
    return in*(1.0f - mix) + delayed_sample*mix;
}