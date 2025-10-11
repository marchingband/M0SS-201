import("stdfaust.lib");
import("peripherals.lib");
// declare options "[midi:on][nvoices:mono]";

ui(x) = tgroup("ui", x);
main_ui(x) = ui(vgroup("[0]main", x));
top_ui(x) = main_ui(hgroup("[0]signal path", x));
bot_ui(x) = main_ui(hgroup("[1]modulators", x));

osc_ui(x) = top_ui(hgroup("[0]oscillator", x));
oscmod_ui(x) = osc_ui(vgroup("[1]bus", x));
posc_ui(x) = osc_ui(vgroup("[2]principal", x));
mix_ui(x) = top_ui(hgroup("[1]mixer", x));
wf_ui(x) = top_ui(hgroup("[2]wavefolder", x));
amp_ui(x) = top_ui(hgroup("[3]amplifier", x));
dly_ui(x) = top_ui(hgroup("[4]delay", x));

mosc_ui(x) = bot_ui(vgroup("[0]mod oscillator", x));
sg_ui(x) = bot_ui(hgroup("[2]stochastic generator", x));
le_ui(x) = bot_ui(hgroup("[3]envelope", x));

// midi entry
// midi_freq = posc_ui(nentry("freq[hidden:1][pitch]",200,50,1000,0.01)) : log : si.smooth(ba.tau2pole(glide)) : exp;
midi_freq = posc_ui(nentry("freq[hidden:1][pitch]",40,0,127,1)) : ba.midikey2hz : si.smooth(ba.tau2pole(glide));
midi_gate = posc_ui(button("gate[hidden:1][gate]"));

fbmod_ui(x) = ui(vgroup("[1]fb mod", x));
fbsg_ui(x) = fbmod_ui(hgroup("[0]from SG", x));
fble_ui(x) = fbmod_ui(hgroup("[1]from LE", x));
fbmo_ui(x) = fbmod_ui(hgroup("[2]from MO", x));

le2sg_len     = fble_ui(vslider("[0]le2sg_len[midi:30]    ", 0, 0, 1, 0.001));
le2sg_rate    = fble_ui(vslider("[1]le2sg_rate[midi:31]   ", 0, 0, 1, 0.001));
le2sg_recall  = fble_ui(vslider("[2]le2sg_recall[midi:32] ", 0, 0, 1, 0.001));
le2sg_smooth  = fble_ui(vslider("[3]le2sg_smooth[midi:33] ", 0, 0, 1, 0.001));
le2le_attack  = fble_ui(vslider("[4]le2le_attack[midi:34] ", 0, 0, 1, 0.001));
le2le_release = fble_ui(vslider("[5]le2le_release[midi:35]", 0, 0, 1, 0.001));
le2mo_freq    = fble_ui(vslider("[6]le2mo_freq[midi:36]   ", 0, 0, 1, 0.001));
le2po_freq    = fble_ui(vslider("[7]le2po_freq[midi:37]   ", 0, 0, 1, 0.001));
le2vca_ctrl   = fble_ui(vslider("[8]le2vca_ctrl[midi:38]  ", 0, 0, 1, 0.001));
le2vca_input  = fble_ui(vslider("[9]le2vca_input[midi:39] ", 0, 0, 1, 0.001));
le2fold       = fble_ui(vslider("[10]le2fold[midi:40]     ", 0, 0, 1, 0.001));
le2amp        = fble_ui(vslider("[11]le2amp[midi:41]      ", 1, 0, 1, 0.001));
sg2sg_len     = fbsg_ui(vslider("[0]sg2sg_len[midi:42]    ", 0, 0, 1, 0.001));
sg2sg_rate    = fbsg_ui(vslider("[1]sg2sg_rate[midi:43]   ", 0, 0, 1, 0.001));
sg2sg_recall  = fbsg_ui(vslider("[2]sg2sg_recall[midi:44] ", 0, 0, 1, 0.001));
sg2sg_smooth  = fbsg_ui(vslider("[3]sg2sg_smooth[midi:45] ", 0, 0, 1, 0.001));
sg2le_attack  = fbsg_ui(vslider("[4]sg2le_attack[midi:46] ", 0, 0, 1, 0.001));
sg2le_release = fbsg_ui(vslider("[5]sg2le_release[midi:47]", 0, 0, 1, 0.001));
sg2mo_freq    = fbsg_ui(vslider("[6]sg2mo_freq[midi:48]   ", 0, 0, 1, 0.001));
sg2po_freq    = fbsg_ui(vslider("[7]sg2po_freq[midi:49]   ", 0, 0, 1, 0.001));
sg2vca_ctrl   = fbsg_ui(vslider("[8]sg2vca_ctrl[midi:50]  ", 0, 0, 1, 0.001));
sg2vca_input  = fbsg_ui(vslider("[9]sg2vca_input[midi:51] ", 0, 0, 1, 0.001));
sg2fold       = fbsg_ui(vslider("[10]sg2fold[midi:52]     ", 0, 0, 1, 0.001));
sg2amp        = fbsg_ui(vslider("[11]sg2amp[midi:53]      ", 0, 0, 1, 0.001));
mo2sg_len     = fbmo_ui(vslider("[0]mo2sg_len[midi:54]    ", 0, 0, 1, 0.001));
mo2sg_rate    = fbmo_ui(vslider("[1]mo2sg_rate[midi:55]   ", 0, 0, 1, 0.001));
mo2sg_recall  = fbmo_ui(vslider("[2]mo2sg_recall[midi:56] ", 0, 0, 1, 0.001));
mo2sg_smooth  = fbmo_ui(vslider("[3]mo2sg_smooth[midi:57] ", 0, 0, 1, 0.001));
mo2le_attack  = fbmo_ui(vslider("[4]mo2le_attack[midi:58] ", 0, 0, 1, 0.001));
mo2le_release = fbmo_ui(vslider("[5]mo2le_release[midi:59]", 0, 0, 1, 0.001));
mo2mo_freq    = fbmo_ui(vslider("[6]mo2mo_freq[midi:60]   ", 0, 0, 1, 0.001));
mo2po_freq    = fbmo_ui(vslider("[7]mo2po_freq[midi:61]   ", 0, 0, 1, 0.001));
mo2vca_ctrl   = fbmo_ui(vslider("[8]mo2vca_ctrl[midi:62]  ", 0, 0, 1, 0.001));
mo2vca_input  = fbmo_ui(vslider("[9]mo2vca_input[midi:63] ", 1, 0, 1, 0.001));
mo2fold       = fbmo_ui(vslider("[10]mo2fold[midi:64]     ", 0, 0, 1, 0.001));
mo2amp        = fbmo_ui(vslider("[11]mo2amp[midi:65]      ", 0, 0, 1, 0.001));


// STOCHASTIC GENERATOR
sg_length_slider = sg_ui(vslider("[0]length[mod:1][midi:1]", 8, 4, mp.max_buffer_length, 1)) : int;
sg_rate_slider = sg_ui(vslider("[1]rate[mod:5][midi:2]", 0.1, 0.01, 1, 0.001));
sg_recall_slider = sg_ui(vslider("[3]recall[mod:7][midi:3]", 0, 0, 1, 0.01));
sg_smooth_slider = sg_ui(vslider("[4]smooth[mod:3][midi:4]", 0.01, 0.01, 0.25, 0.001));

gate_sel = bot_ui(vslider("[1]gate routing[style:radio{'SEPERATE':0;'SG':1;'ENV':2}][sel:11][midi:5]", 0, 0, 2, 1));

// calculations
sg_rate = sg_rate_slider : ba.lin2LogGain : it.remap(0, 1, 0.01, 200);
// actual signal
fb_sg(len_mod, rec_mod, rate_mod, smoo_mod, gate_in) = mp.rand_gen(len_, rec_, rate_, smoo_, gate_in, gate_sel == 2)
with {
    len_ = len_mod + sg_length_slider : aa.clip(1, mp.max_buffer_length) : int;
    rec_ = rec_mod + sg_recall_slider : aa.clip(0., 1.);
    rate_ = rate_mod + sg_rate;
    smoo_ = smoo_mod + sg_smooth_slider : aa.clip(0., 1.);
};
// modulation out
sg2pfreq_ = sg_sig * sg2po_freq * 2 : (2^_);
sg2vca_ctrl_ = sg_sig * sg2vca_ctrl * 0.5;
sg2vca_input_ = sg_sig * sg2vca_input;
sg2fold_ = (sg_sig/2 + 0.5) * sg2fold;
sg2amp_ = (sg_sig/2 + 0.5) * sg2amp;

// LOOPING ENVELOPE
le_attack_slider = le_ui(vslider("[0]attack[mod:10][midi:6]", 0.01, 0.01, 1, 0.001));
le_release_slider = le_ui(vslider("[1]release[mod:14][midi:7]", 0.5, 0.01, 1, 0.001));
le_mode_sel = le_ui(vslider("[2]mode[style:radio{'AR':0;'ASR':1;'LOOP':2}][sel:12][midi:8]", 0, 0, 2, 1));

// actual signal
fb_le(att_mod, rel_mod, sg_gate_) = (le_ar, le_asr, le_loop_ar : select3(le_mode_sel)), le_gate_out
with {
    gate = midi_gate, sg_gate_ : select2(gate_sel == 1);
    a = le_attack_slider + att_mod;
    r = le_release_slider + rel_mod;
    b = 0.3;
    le_ar = gate : en.adsr_bias(a, r, 0, r, 1-b, b, b, 1);
    le_asr = gate : en.adsr_bias(a, 0, 1, r, 1-b, b, b, 1);
    le_loop_ar_sig = gate : mp.looping_ar_bias(a, r, 1-b, b);
    le_loop_ar = le_loop_ar_sig : _, !;
    le_loop_gate = le_loop_ar_sig : !, _;
    le_gate_out = gate, le_loop_gate : select2(le_mode_sel == 2);
};
// modulations
le2pfreq_ = le_sig * le2po_freq * 2 : (2^_);
le2vca_ctrl_ = le_sig * le2vca_ctrl * 0.5;
le2vca_input_ = le_sig * le2vca_input;
le2fold_ = le_sig * le2fold;
le2amp_ = le_sig * le2amp;

fb_block(sg, sg_gate, le, le_gate_out, mo) = len, recall, rate, smooth, sg_gate_in, attack, release, sg_gate, mo_freq
with {
    len_ = le2sg_len*le + sg2sg_len*sg + mo2sg_len*mo;
    len = len_ * mp.max_buffer_length : int;
    recall = le2sg_recall*le + sg2sg_recall*sg + mo2sg_recall*mo;
    rate = le2sg_rate*le + sg2sg_rate*sg + mo2sg_rate*mo : ba.lin2LogGain : it.remap(0, 1, 0.1, 200);
    smooth = le2sg_smooth*le + sg2sg_smooth*sg + mo2sg_smooth*mo;
    attack  = sg2le_attack*sg + le2le_attack*le + mo2le_attack*mo;
    release = sg2le_release*sg + le2le_release*le + mo2le_release*mo;
    mo_freq = le2mo_freq*le + sg2mo_freq*sg + mo2mo_freq*mo : *(2) : (2^_);
    sg_gate_in = 0, le_gate_out : select2(gate_sel == 2);
};
mod_block = (fb_sg, fb_le, fb_mosc) ~ fb_block;
sg_gate_out = mod_block : !, _, !, !, !;
sg_sig = mod_block : _, !, !, !, ! <: attach(sg_ui(vbargraph("[9]SG[SG]", -1, 1)));
le_sig = mod_block : !, !, _, !, ! <: attach(le_ui(vbargraph("[9]LE[LE]", 0, 1)));
mosc_sig = mod_block : !, !, !, !, _ <: attach(mosc_ui(vbargraph("[9]MO[MO]", -1, 1)));

// COMPLEX OSCILLATOR

// modulation oscillator
mosc_freq_course_slider = mosc_ui(hslider("[0]course[mod:2][midi:9]", 0.5, 0, 1, 0.001));
mosc_freq_fine_slider = mosc_ui(hslider("[1]fine[alt:2][midi:10]", 0, -0.03, 0.03, 0.0001));
mosc_wave_sel = mosc_ui(hslider("[2]waveform[style:radio{'TRI':0;'SQR':1;'SAW':2}][sel:6][midi:11]", 0, 0, 2, 1));
mosc_hi_lo_sel = mosc_ui(checkbox("[3]LFO[sel:4][midi:12]"));
mosc_track_sel = mosc_ui(checkbox("[4]TRACK[alt-sel:4]"));

fb_mosc(freq_in) = mosc_freq : mosc_osc
with {
    mosc_freq = mosc_base_freq * freq_in
    with {
        mosc_freq_input = mosc_freq_course_slider + mosc_freq_fine_slider : aa.clip(0, 1);

        mosc_freq_track = mosc_untracked_, mosc_tracked_ : select2(mosc_track_sel)
        with {
            mosc_tracked_ = mosc_freq_input : it.remap(0, 1, -2, 2) : (2^_) : *(midi_freq);
            mosc_untracked_ = mosc_freq_input : ba.lin2LogGain : it.remap(0., 1., 20., 3000.);
        };
            
        mosc_base_freq = mosc_freq_track <: *(1), /(10) : select2(mosc_hi_lo_sel);
    };
    mosc_tri = _ <: ba.if(mosc_hi_lo_sel == 1, os.lf_triangle, os.triangle);
    mosc_sqr = _ <: ba.if(mosc_hi_lo_sel == 1, os.lf_squarewave, os.square);
    mosc_saw = _ <: ba.if(mosc_hi_lo_sel == 1, os.lf_saw, os.sawtooth);
    mosc_osc = _ <: mosc_tri, mosc_sqr, mosc_saw : select3(mosc_wave_sel);
};
mo2pfreq_ = mosc_sig * mo2po_freq * 2 : (2^_);
mo2vca_ctrl_ = mosc_sig * mo2vca_ctrl * 0.5;
mo2vca_input_ = mosc_sig * mo2vca_input;
mo2fold_ = mosc_sig * mo2fold;
mo2amp_ = mosc_sig * mo2amp;

// mod bus
mod_index_slider = oscmod_ui(hslider("[0]index[mod:8][midi:13]", 0, 0, 1, 0.001));
mod_type_sel = oscmod_ui(hslider("[1]type[style:radio{'AM':0;'FM':1;'TM':2}][alt-sel:8][midi:14]", 1, 0, 2, 1));
mod_index = mod_index_slider + sg2vca_ctrl_ + le2vca_ctrl_ + mo2vca_ctrl_: aa.clip(0, 1);
index_input_sig = (mo2vca_input_ + le2vca_input_ + sg2vca_input_);
idx2amp = 1 + mod_index * index_input_sig;
idx2freq = mod_index * index_input_sig * 2 : (2^_);
idx2timbre = mod_index * (index_input_sig/2 + 1);

// primary oscillator
// posc_octave_sel = posc_ui(hslider("[0]octave[style:radio{'-2':0;'0':1;'+2':2}]", 1, 0, 2, 1));
posc_tune_slider = posc_ui(hslider("[0]tune[alt:17][midi:15]", 0, -2, 2, 0.001));
posc_wave_sel = posc_ui(hslider("[1]waveform[style:radio{'SINE':0;'TRI':1;'BOTH':2}][sel:17][midi:16]", 0, 0, 2, 1));
posc_quantize = posc_ui(checkbox("[2]quantize[alt-sel:20][midi:17]"));
posc_sig = posc_freq : posc_osc : *(posc_amp)
with {
    posc_base_freq = midi_freq * (2^posc_tune_slider);
    posc_freq_modulated = posc_base_freq * ba.if(mod_type_sel == 1, idx2freq, 1) * le2pfreq_ * sg2pfreq_ * mo2pfreq_;
    posc_freq = posc_freq_modulated <: select2(posc_quantize, *(1), qu.quantize(440, qu.penta));
    posc_osc = _ <: os.osc, os.triangle, (os.osc + os.triangle) / 2 : select3(posc_wave_sel);
    posc_amp = ba.if(mod_type_sel == 0, idx2amp, 1);
};


// wavefolder
wf_fold_slider = wf_ui(vslider("timbre[mod:23][midi:18]", 0.7, 0.7, 2.5, 0.001)) : si.smoo;
wf_bias_slider = wf_ui(vslider("bias[mod:20][midi:19]", 0, -0.5, 0.5, 0.01)) : si.smoo;
folder = +(wf_bias_slider) : *(wf_freq) : sin : *(wf_gain) : fi.dcblocker
with {
    base_fold_amt = wf_fold_slider + ba.if(mod_type_sel == 2, idx2timbre, 0);
    fold = base_fold_amt + le2fold_ + sg2fold_ + mo2fold_: aa.clip(0, 3) : ^(2);
    wf_freq = ma.PI * fold / 2;
    wf_gain = ba.if(fold < 1, 1/sin(wf_freq), 1);
};

// mixer
mix_posc_slider = mix_ui(vslider("[0]principal[mod:18][midi:20]", 1, 0, 1, 0.001));
mix_mosc_slider = mix_ui(vslider("[1]modulator[mod:9][midi:21]", 0, 0, 1, 0.001));
mix_sg_slider = mix_ui(vslider("[2]stochastic[alt:9][midi:22]", 0, 0, 1, 0.001));
mix_le_slider = mix_ui(vslider("[3]envelope[alt:18][midi:23]", 0, 0, 1, 0.001));
mixed_sig = mixed_posc + mixed_mosc + mixed_sg + mixed_le
with {
    mixed_posc = mix_posc_slider * posc_sig;
    mixed_mosc = mix_mosc_slider * mosc_sig;
    mixed_sg = mix_sg_slider * sg_sig;
    mixed_le = mix_le_slider * le_sig;
};

// amplifier
amp_mode_sel = amp_ui(vslider("[0]mode[style:radio{'LPG':0;'VCA':1}][alt-sel:19][midi:24]", 0, 0, 1, 1));
vol = amp_ui(vslider("[1]volume[alt:19][midi:25]", 0.4, 0, 1, 0.001)) : si.smoo;
glide = amp_ui(vslider("[2]glide[alt:23][midi:26]", 0.001, 0.001, 0.99, 0.001));
amp = _ <: amp_lpg, amp_vca : select2(amp_mode_sel)
with {
    amp_ctrl = le2amp_ + sg2amp_ + mo2amp_, 1.0 : select2(le2amp == 0);
    amp_vca = *(amp_ctrl);
    amp_ctf = (amp_ctrl * 90)^2 + 0.01;
    amp_lpg = fi.lowpass(1, amp_ctf) : amp_vca;
};

delay_time_slider = dly_ui(vslider("time[alt:16][midi:27]", 2^10, 2^10, 2^13, 0.001));
delay_fb_slider = dly_ui(vslider("feedback[alt:25][midi:28]", 0, 0, 1, 0.001));
delay_wet_slider = dly_ui(vslider("mix[alt:24][midi:29]", 0, 0, 0.5, 0.001));
delay(x) = wet * wet_amt + dry * dry_amt
with {
    dry = x;
    delay_intrp = 100 * ma.SR/1000.0;
    wet = (+(x) : de.sdelay(int(2^13), delay_intrp, delay_time_slider)) ~ *(delay_fb_slider);
    wet_amt = delay_wet_slider;
    dry_amt = 1 - wet_amt;
}; 

process = mixed_sig : folder : amp : delay : *(vol) <: _,_;
// process = os.square(1000);