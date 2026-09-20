#include <stdint.h>

#include "bflb_gpio.h"

#include "synth.h"
#include "interface.h"
#include "utils.h"
#include "biquad.h"
#include "wavefolder.h"

extern DSP_All_Signals all_sigs_raw;

FB_Dests fb_dests;
EnvState_t env_state;
SGState_t sg_state;
OscState_t os1_state;
OscState_t os2_state;
InterpolatingDelay_t dly;

DSP_All_Signals all_sigs;

Biquad_t dec_filt1, dec_filt2, dec_filt3;

const float param_smooth_coeff = 0.99958342f; //pow2f(-1.0f * LOG2_E * SR_INV / 0.1f);

static inline float x3(float x) {
    float x2 = x * x;
    return x2 * x;
}

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
    all_sigs.dly_flt += c * (all_sigs_raw.dly_flt - all_sigs.dly_flt);
    for (uint16_t i = 0; i < NUM_MOD_SRCS; i++) {
        for (uint16_t j = 0; j < NUM_MOD_DESTS; j++) {
           all_sigs.mtx[i][j] += c * (all_sigs_raw.mtx[i][j] - all_sigs.mtx[i][j]); 
        }
    }
}

#define OVERSAMPLE_RATIO 4

void init_dsp() {
    for (uint16_t i = 0; i < MAX_SG_LENGTH; i++) {
        sg_state.arr[i] = randf();
    }
    sg_state.idx = 0;

    dec_filt1 = dec_filt2 = dec_filt3 = generate_lowpass(SR * OVERSAMPLE_RATIO, 0.45f*(float)SR, 0.707f);

    dly.buffer_size = DELAY_LINE_LEN;
    dly.ptr = 0;
}

float midi_note;

size_t counter = 0;

#define LEGATO_MODE false

bool prev_gate_in;
bool retrigger;
float prev_note_in;

void compute_midi_note() {
    static float lp_state;
    // TODO: midi pitch bend
    float glide = x3(all_sigs.os1_gld);
    float glide_s = linear_map(glide, 0.0f, 1.0f, 1e-4f, 0.4f);
    float coeff = 1.0f - pow2f(-1.0f * LOG2_E * SR_INV / glide_s);
    float input_note = CLAMP(all_sigs_raw.pitch, 0, 108);
    lp_state += (input_note - lp_state) * coeff;

    float normalized_pb = all_sigs_raw.pitch_bend - 8192;
    normalized_pb /= 8192.0f;
    float semitone_bend = (normalized_pb > 0.0f)? 
        normalized_pb*all_sigs_raw.pb_up : normalized_pb*all_sigs_raw.pb_down;
    static float bend_state;
    bend_state += (semitone_bend - bend_state) * 1e-3f;

    midi_note = lp_state + bend_state;

    // if the midi note changes but the gate has been on the whole time, that's a retrigger
    retrigger = (prev_note_in != all_sigs_raw.pitch) && prev_gate_in && all_sigs_raw.gate;
    retrigger = retrigger && !all_sigs_raw.legato;  // turn it off if we're in legato mode
    prev_note_in = all_sigs_raw.pitch;
    prev_gate_in = all_sigs_raw.gate;
}

float os2freq;

float phase = 0.0f;
const float freq = 260.0f * SR_INV;

void compute_dsp(size_t len, float* buf) {
    static float amp_ind_lpf_state = 0.0f;
    for (size_t i = 0; i < len; i++) {
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

        float volume = x3(all_sigs.vol);
        signal *= volume;
        signal = soft_clip(signal);
        buf[i] = signal;
        amp_ind_lpf_state += (absf(signal) - amp_ind_lpf_state) * 0.001f;
    }

    all_sigs_raw.ind_env = env_state.val;
    all_sigs_raw.ind_sg = sg_state.val;
    all_sigs_raw.ind_os2 = os2_state.val;
    all_sigs_raw.ind_os2_freq = os2freq;
    all_sigs_raw.ind_amp = amp_ind_lpf_state;
}

// os1 is bipolar, sg and env are unipolar
// therefore just by adding the signals the range is [-1, 3]
// os1 should have the same total effect as the other two however,
// so the operation to normalize should be ((os1 + 1)/2 + sg + env) / 3

#define E_MAT env_state.val * all_sigs.mtx[ENV_I_S]
#define S_MAT sg_state.val * all_sigs.mtx[SG_I_S]
#define O_MAT os2_state.val * all_sigs.mtx[OS2_I_S]

#define MAT(x, y) CLAMP((E_MAT[x]) + (S_MAT[x]) + (O_MAT[x]) + (y), 0.0f, 1.0f)

void compute_matrix() {
    fb_dests.rate = MAT(RATE_I, all_sigs.sg_rate);
    fb_dests.smth = MAT(SMTH_I, all_sigs.sg_smth);
    fb_dests.rcl  = MAT(RCL_I,  all_sigs.sg_rcl);
    fb_dests.att  = MAT(ATT_I,  all_sigs.env_att);
    fb_dests.rel  = MAT(REL_I,  all_sigs.env_rel);
    fb_dests.os1  = CLAMP((E_MAT[OS1_I]) + (S_MAT[OS1_I]) + (O_MAT[OS1_I]), -1.0f, 1.0f);
    fb_dests.os2  = MAT(OS2_I,  all_sigs.os2_tune);
    fb_dests.idx  = MAT(IDX_I,  all_sigs.bus_idx);
    fb_dests.timb = MAT(TIMB_I, all_sigs.fold_timb);

    // this signal specifically behaves differently with modulation
    // the volume control already changes the volume,
    // so there's no point in having the matrix control just reduce the volume.
    // instead, reducing the strength of env->amp decrease the effect env has
    // on amp by increasing the baseline volume level
    // env->amp at 50% means the audio is normally passing through at 50%,
    // and the other 50% is triggered by the envelope.
    // this also works for negative modulations, if env->amp is negative
    // it acts like a ducking effect on a normally open signal
    float env_to_amp = all_sigs.mtx[ENV_I_S][AMP_I];
    float baseline_gain = (env_to_amp >= 0)? (1.0f - env_to_amp) : 1.0f;
    fb_dests.amp  = MAT(AMP_I,  baseline_gain);
}

void compute_sg_rate() {
    const float min_freq = 0.1f;  // increments once every ten seconds
    const float max_freq = 4000.0f;  // 4kHz

    const int curvature = 10;
    float r2 = fb_dests.rate * fb_dests.rate;
    float r6 = r2 * r2 * r2;
    float rate_hz = r6*max_freq + min_freq;

    sg_state.rate_normalized = rate_hz * SR_INV;
    sg_state.phase += sg_state.rate_normalized;
    if (sg_state.phase >= 1.0f) {
        sg_state.phase -= 1.0f;
        sg_state.high = true;
    } else sg_state.high = false;
}

bool env_gate = false;

void compute_sg() {
    // TODO: shouldn't be all_sigs.gate, should be start of envelope cycle
    bool gate = (all_sigs_raw.gate_sel == GATESEL_ENV)? env_gate : sg_state.high;

    uint16_t len_int = all_sigs_raw.sg_len;
    len_int = CLAMP(len_int, MIN_SG_LENGTH, MAX_SG_LENGTH);
    if (gate && !sg_state.prev_gate) {
        sg_state.idx = (sg_state.idx + 1) % len_int;

        // to make sure it can reliably hit 100%
        float recall = CLAMP(fb_dests.rcl * 1.01f, 0.0f, 1.0f);
        
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

bool os1_retrigger = false;
bool os2_retrigger = false;

void compute_env() {
    bool gate_in = (all_sigs_raw.gate_sel == GATESEL_SG)? sg_state.high : all_sigs_raw.gate;
    bool trig = gate_in && !env_state.prev_gate || retrigger;
    env_state.prev_gate = gate_in;

    if (trig || (env_state.stage == ENV_OFF && all_sigs_raw.env_mode == ENVMODE_ARL)) {
        env_state.stage = ENV_ATT;
        os1_retrigger = os2_retrigger = all_sigs_raw.os_retrig;
    }

    float bias = 0.5f * (all_sigs.env_crv + 1.0f);
    bias = bias_clip(1.0f - bias);
    const size_t env_control_curve = 4;

    switch (env_state.stage) {
        case ENV_OFF: {
            env_state.val = 0.0f;
            break;
        }
        case ENV_ATT: {
            float a2 = fb_dests.att * fb_dests.att;
            // float a4 = a2 * a2;
            float att_samples = a2 * MAX_ENV_STAGE_LEN_SAMPLES;
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
            float r2 = fb_dests.rel * fb_dests.rel;
            // float r4 = r2 * r2;
            float rel_samples = r2 * MAX_ENV_STAGE_LEN_SAMPLES;
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
    env_gate = env_state.stage == ENV_ATT;
}

void compute_os2() {
    // parameters that affect os1: 
    // slow/fast mode, key tracking on/off, 
    // tune (fine/course), waveform

    // intervals determine number of octaves range
    const float fast_mode_course_max_interval = 4.0f;
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
        freq_hz = ((all_sigs_raw.os2_kt)? midi_freq * 0.0009765f : slow_mode_base_freq_hz) * multiplier;
    } else {
        float total_interval = fb_dests.os2 * fast_mode_course_max_interval;
        float multiplier = pow2f(total_interval);
        freq_hz = ((all_sigs_raw.os2_kt)? midi_freq * 0.125f : fast_mode_base_freq_hz) * multiplier;
    }

    os2freq = freq_hz;

    float freq_normalized = freq_hz * SR_INV;

    float this_tri = os2_state.next_tri;
    float this_saw = os2_state.next_saw;
    float this_sqr = os2_state.next_sqr;
    os2_state.next_tri = 0.0f;
    os2_state.next_saw = 0.0f;
    os2_state.next_sqr = 0.0f;

    if (os2_retrigger) {
        os2_state.phase = 0.0f;
        os2_state.high = false;
    }

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

    os2_state.next_tri += os2_state.high? 2.0f * os2_state.phase : 2.0f * (1.0f - os2_state.phase);
    os2_state.next_saw += os2_state.phase;
    os2_state.next_sqr += os2_state.high? 1.0f : 0.0f;

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
    const float root = scale->root_freq_hz;

    float rel_octave = 1.0f;
    float norm_ratio = freq_hz / root;

    while (norm_ratio >= 2.0f) {
        norm_ratio *= 0.5f;
        rel_octave *= 2.0f;
    }

    while (norm_ratio < 1.0f) {
        norm_ratio *= 2.0f;
        rel_octave *= 0.5f;
    }

    const float* intervals = scale->intervals;
    uint32_t n = scale->num_intervals;

    uint32_t lo = 0;
    uint32_t hi = n - 1;
    while (lo < hi) {
        uint32_t mid = (lo + hi) >> 1;
        if (norm_ratio < intervals[mid]) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }

    // lo is now the first interval >= norm_ratio (upper bound candidate)
    uint32_t idx_hi = lo;
    uint32_t idx_lo = (lo == 0) ? 0 : (lo - 1);

    // choose closest of the two neighbors
    float diff_hi = absf(norm_ratio - intervals[idx_hi]);
    float diff_lo = absf(norm_ratio - intervals[idx_lo]);

    float quantized_ratio = (diff_lo <= diff_hi)
        ? intervals[idx_lo]
        : intervals[idx_hi];

    return root * rel_octave * quantized_ratio;
}

const Scale chromatic = {.root_freq_hz = 0.0f, .num_intervals = 12, .intervals = {
    1.000000f, 1.059463f, 1.122462f, 1.189207f, 1.259921f, 1.334840f,
    1.414214f, 1.498307f, 1.587401f, 1.681793f, 1.781797f, 1.887749f
}};

const Scale major = {.root_freq_hz = 0.0f, .num_intervals = 7, .intervals = {
    1.000000f, 1.122462f, 1.259921f, 1.334840f,
    1.498307f, 1.681793f, 1.887749f
}};

const Scale natural_minor = {.root_freq_hz = 0.0f, .num_intervals = 7, .intervals = {
    1.000000f, 1.122462f, 1.189207f, 1.334840f,
    1.498307f, 1.587401f, 1.781797f
}};

const Scale harmonic_minor = {.root_freq_hz = 0.0f, .num_intervals = 7, .intervals = {
    1.000000f, 1.122462f, 1.189207f, 1.334840f,
    1.498307f, 1.587401f, 1.887749f
}};

const Scale major_pentatonic = {.root_freq_hz = 0.0f, .num_intervals = 5, .intervals = {
    1.000000f, 1.122462f, 1.259921f, 1.498307f, 1.681793f
}};

const Scale minor_pentatonic = {.root_freq_hz = 0.0f, .num_intervals = 5, .intervals = {
    1.000000f, 1.189207f, 1.334840f, 1.498307f, 1.781797f
}};

const Scale blues = {.root_freq_hz = 0.0f, .num_intervals = 6, .intervals = {
    1.000000f, 1.189207f, 1.259921f, 1.334840f,
    1.414214f, 1.781797f
}};

const Scale whole_tone = {.root_freq_hz = 0.0f, .num_intervals = 6, .intervals = {
    1.000000f, 1.122462f, 1.259921f,
    1.414214f, 1.587401f, 1.781797f
}};

const Scale octatonic = {.root_freq_hz = 0.0f, .num_intervals = 8, .intervals = {
    1.000000f, 1.059463f, 1.189207f, 1.259921f,
    1.414214f, 1.498307f, 1.681793f, 1.781797f
}};

const Scale just_major = {.root_freq_hz = 0.0f, .num_intervals = 7, .intervals = {
    1.000000f, 1.125000f, 1.250000f, 1.333333f,
    1.500000f, 1.666667f, 1.875000f
}};

const Scale slendro = {.root_freq_hz = 0.0f, .num_intervals = 5, .intervals = {
    1.000000f, 1.148698f, 1.319508f,
    1.515717f, 1.741101f
}};

Scale quantizer_scales[] = {chromatic, major, natural_minor, harmonic_minor, 
    major_pentatonic, minor_pentatonic, blues, whole_tone, octatonic, just_major, slendro};

float compute_os1_rate() {
    // care about midi frequency, tuning, and mod bus
    // also about quantization and glide

    // midi_freq is already computed with glide and pitch bend accounted for
    // so the tuning slider, modulations, and FM need to be added.
    // tuning slider and modulations get quantized, then FM is added.

    float freq_multiplier = pow2f(fb_dests.os1 * 2.0f); // maximum two octave range for normalized value
    if (all_sigs_raw.os1_oct == OS1_OCT_n1) freq_multiplier *= 0.5f;
    else if (all_sigs_raw.os1_oct == OS1_OCT_n2) freq_multiplier *= 0.25f;


    if (all_sigs_raw.os1_qtz != 0) {
        Scale scale = quantizer_scales[(size_t)all_sigs_raw.os1_qtz - 1];
        scale.root_freq_hz = midi_note_to_freq_hz(all_sigs_raw.pitch);
        freq_multiplier = quantize(freq_multiplier, &scale);
    }

    float freq_hz = freq_multiplier * midi_note_to_freq_hz(midi_note);
    if (all_sigs_raw.bus_sel == BUS_SEL_FM) {
        float max_fm_index = 8.0f;
        float fm_idx = x3(fb_dests.idx);
        freq_hz *= 1.0f + max_fm_index*fm_idx*os2_state.val;
    }
    if (all_sigs_raw.os2_hilo) freq_hz *= -1.0f;

    // prevent zero frequency
    float sign = (freq_hz > 0.0f) - (freq_hz < 0.0f);
    if (absf(freq_hz) < 1e-6f) freq_hz = 1e-6f * sign;

    return freq_hz;
}

void compute_os1() {
    float freq_normalized = compute_os1_rate() * SR_INV;

    float this_tri = os1_state.next_tri;
    os1_state.next_tri = 0.0f;

    if (os1_retrigger) {
        os1_state.phase = 0.0f;
    }

    os1_state.phase += freq_normalized;

    if (os1_state.high ^ (os1_state.phase < 0.5f)) {
        float t = (os1_state.phase - 0.5f) / freq_normalized;
        float discontinuity = 4.0f * freq_normalized;
        if (freq_normalized < 0.0f) discontinuity = -discontinuity;

        this_tri -= this_iblep(t) * discontinuity;
        os1_state.next_tri -= next_iblep(t) * discontinuity;

        os1_state.high = os1_state.phase < 0.5f;
    }

    if (os1_state.phase >= 1.0f) {
        os1_state.phase -= 1.0f;
        float t = os1_state.phase / freq_normalized;
        float discontinuity = 4.0f * freq_normalized;

        this_tri += this_iblep(t) * discontinuity;
        os1_state.next_tri += next_iblep(t) * discontinuity;

        os1_state.high = true;
    }

    if (freq_normalized < 0.0f && os1_state.phase < 0.0f) {
        float t = os1_state.phase / freq_normalized;
        os1_state.phase += 1.0f;
        float discontinuity = 4.0f * freq_normalized;

        this_tri -= this_iblep(t) * discontinuity;
        os1_state.next_tri -= next_iblep(t) * discontinuity;

        os1_state.high = false;
    }


    os2_state.next_tri += 2.0f * (os1_state.high? 2.0f * os1_state.phase : 2.0f * (1.0f - os1_state.phase)) - 1.0f;
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
        selected_signal *= 1.0f - fb_dests.idx*(1.0f - os2_state.val);
    }
    os1_state.val = selected_signal;
}

float compute_mixer() {
    float bipolar_sg = 2.0f*sg_state.val - 1.0f;
    float bipolar_env = 2.0f*env_state.val - 1.0f;
    float power_law_sg =  x3(all_sigs.mx_sg);
    float power_law_env = x3(all_sigs.mx_env);
    float power_law_os1 = x3(all_sigs.mx_os1);
    float power_law_os2 = x3(all_sigs.mx_os2);
    float sum = bipolar_sg * power_law_sg + bipolar_env * power_law_env \
        + os1_state.val * power_law_os1 + os2_state.val * power_law_os2;
    return sum;  // 3.0f * soft_clip(sum * 0.33f);
}

float wavefolder_tone_lowpass(float x) {
    const float ctf_hz = 1330.0f;
    const float a = 0.615431144f;
    static float y1 = 0.0f;

    y1 += a * (x - y1);
    return y1;
}

float compute_wavefolder(float in) {    
    in += all_sigs.fold_bias;
    in *= 0.5f;  // gainstaging for wavefolder
    float timbre = fb_dests.timb * fb_dests.timb;
    float t = timbre * 19.0f;
    if (all_sigs_raw.bus_sel == BUS_SEL_TM) {
        const float max_timbre_modulation = 1.0f;
        t += max_timbre_modulation * fb_dests.idx * os2_state.val * 13.0f;
    }
    t += 1.0f;  // baseline unity gain
    in *= t;

    static float prev_in = 0.0f;
    float os_outputs[OVERSAMPLE_RATIO];
    for (uint16_t i = 0; i < OVERSAMPLE_RATIO; i++) {
        float frac = (float)i * 0.25f;
        float interpolated_sample = prev_in + (in - prev_in) * frac;

        float folded_sample = naive_buchla_wf_compute(interpolated_sample);

        os_outputs[i] = process_biquad(&dec_filt3, process_biquad(&dec_filt2, process_biquad(&dec_filt1, folded_sample)));
    }
    prev_in = in;
    float filtered_output = wavefolder_tone_lowpass(os_outputs[OVERSAMPLE_RATIO - 1] * 0.25f);
    return filtered_output;
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
    float gain = fb_dests.amp;

    if (all_sigs_raw.amp_sel == AMP_VCA) return in * gain;

    // LPG mode, uses vactrol simulation
    compute_vactrol_params(gain, 0.01f, 0.005f, 0.05f);
    float freq_coeff = 1.0f - expf(-2.0f * PI * vact.normalized_freq);
    in *= vact.gain;

    vact.lpf_state += freq_coeff*(in - vact.lpf_state);

    return vact.lpf_state + (in - vact.lpf_state) * vact.hf_bleed;
}

float delay_filter(float x) {
    static float prev_x, prev_y;

    float ctrl = all_sigs.dly_flt;  // bipolar, -1 to 1
    int sign = (ctrl > 0) - (ctrl < 0);
    if (sign == -1) ctrl += 1; // now it's in the 0 to 1 range
    ctrl *= ctrl; // add nonlinearity to get more frequency control

    float y = 0.0f;
    if (sign == 0) {
        y = x;
    } else if (sign == 1) {
        // hipass mode
        float ctf_freq = linear_map(ctrl, 0, 1, 20, 3000);
        float tau = 1.0f / (2.0f * PI * ctf_freq);
        float alpha = tau / (tau + SR_INV);

        y = alpha * (prev_y + x - prev_x);
    } else {
        // lopass mode;
        float ctf_freq = linear_map(ctrl, 0, 1, 100, 5000);
        float tau = 1.0f / (2.0f * PI * ctf_freq);
        float alpha = SR_INV / (tau + SR_INV);

        y = prev_y + alpha * (x - prev_y);
    }
    prev_y = y;
    prev_x = x;
    return y;
}

float compute_delay(float in) {
    // compute parameters
    float time = x3(all_sigs.dly_tm);
    float delay_time = linear_map(time, 0.0f, 1.0f, DELAY_LINE_MIN, DELAY_LINE_LEN - 1);
    float feedback = normalized_linear_to_logexp(all_sigs.dly_fb, -3) * 1.05f;
    float mix = all_sigs.dly_mx;

    // get read pointer
    float read_ptr = (float)dly.ptr - delay_time;
    while (read_ptr < 0) read_ptr += dly.buffer_size;
    while (read_ptr >= dly.buffer_size) read_ptr -= dly.buffer_size;

    // find new sample (interpolating)
    MAKE_INTEGRAL_FRACTIONAL(read_ptr);
    float sample1 = dly.buffer[read_ptr_integral];
    float sample2 = dly.buffer[(read_ptr_integral + 1) % dly.buffer_size];
    float delayed_sample = sample1 + read_ptr_fractional*(sample2 - sample1);
    if (absf(delayed_sample) < 1e-15f) delayed_sample = 0.0f;

    // handle feedback
    dly.buffer[dly.ptr] = soft_clip(in + delay_filter(feedback*delayed_sample));

    // update pointer
    dly.ptr = (dly.ptr + 1) % dly.buffer_size;

    // output value
    return in*(1.0f - mix) + delayed_sample*mix;
}