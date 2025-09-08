import("stdfaust.lib");
import("peripherals.lib");

ui(x) = hgroup("ui", x);

// SG params
sg_length_slider = ui(vslider("[0]length", 8, 4, mp.max_buffer_length, 1)) : int;
sg_rate_slider = ui(vslider("[1]rate", 0.1, 0.01, 1, 0.001));
sg_recall_slider = ui(vslider("[3]recall", 0, 0, 1, 0.01));
sg_smooth_slider = ui(vslider("[4]smooth", 0.01, 0.01, 0.25, 0.001));

// LE params
le_attack_slider = ui(vslider("[5]attack", 0.001, 0.001, 1, 0.001));
le_release_slider = ui(vslider("[6]release", 0.5, 0.001, 1, 0.001));

// FB params
le2sg_len     = ui(vslider("[7]le2sg_len    ", 0, 0, 1, 0.001));
le2sg_rate    = ui(vslider("[8]le2sg_rate   ", 0, 0, 1, 0.001));
le2sg_recall  = ui(vslider("[9]le2sg_recall ", 0, 0, 1, 0.001));
le2sg_smooth  = ui(vslider("[10]le2sg_smooth ", 0, 0, 1, 0.001));
sg2le_attack  = ui(vslider("[11]sg2le_attack ", 0, 0, 1, 0.001));
sg2le_release = ui(vslider("[12]sg2le_release", 0, 0, 1, 0.001));

sg_rate = sg_rate_slider : ba.lin2LogGain : it.remap(0, 1, 0.1, 200);

fb_block(sg, sg_gate, le) = len, recall, rate, smooth, attack, release
with {
    len_ = le2sg_len * le * mp.max_buffer_length : int;
    len  = len_ + sg_length_slider;
    recall = le2sg_recall*le + sg_recall_slider : /(2);
    rate_ = le2sg_rate*le : ba.lin2LogGain : it.remap(0, 1, 0.1, 200);
    rate = rate_ + sg_rate;
    smooth = le2sg_smooth*le + sg_smooth_slider : /(2);
    attack  = sg2le_attack*sg + le_attack_slider;
    release = sg2le_release*sg + le_release_slider;
};

fb_sg(len,rec,rat,smoo) = mp.rand_gen(len,rec,rat,smoo,0,0);
fb_lar(att,rel) = mp.looping_ar_bias(att,rel,0.7,0.3,0);

mod_block = (fb_sg, fb_lar) ~ fb_block;
sg = mod_block : _, !, !;
le = mod_block : !, !, _;
// sg_gate_out = sg : !, _;
// sg = (sg_length_slider, sg_recall_slider, sg_rate, sg_smooth_slider) : fb_sg;
// sg = mp.rand_gen(sg_length_slider, sg_recall_slider, sg_rate, sg_smooth_slider,0,0);
process = sg;