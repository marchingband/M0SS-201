import("stdfaust.lib");
declare options "[midi:on][nvoices:mono]";

gate = button("gate[hidden:1]");
ui(x) = hgroup("ui", x);

attack = ui(vslider("[0]attack", 0.01, 0, 1, 0.001));
release = ui(vslider("[1]release", 0.3, 0, 1, 0.001));
env_sel = ui(vslider("[2]type[style:radio{'AR':0;'ASR':1}]", 0, 0, 1, 1));

bias = 0.3;
ar = en.adsr_bias(attack, release, 0, release, 1-bias, bias, bias, 1);
asr = en.adsr_bias(attack, release, 1, release, 1-bias, bias, bias, 1);
env = gate <: ar, asr : select2(env_sel);

process = os.osc(440) * env * 0.4 <: _,_;
