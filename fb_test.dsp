import("stdfaust.lib");
import("source_of_uncertainty.dsp")

fb11 = vslider("fb11", 0, 0, 1, 0.001);
fb12 = vslider("fb12", 0, 0, 1, 0.001);
fb13 = vslider("fb13", 0, 0, 1, 0.001);
fb21 = vslider("fb21", 0, 0, 1, 0.001);
fb22 = vslider("fb22", 0, 0, 1, 0.001);
fb23 = vslider("fb23", 0, 0, 1, 0.001);
fb31 = vslider("fb31", 0, 0, 1, 0.001);
fb32 = vslider("fb32", 0, 0, 1, 0.001);
fb33 = vslider("fb33", 0, 0, 1, 0.001);

fb_block(a, b, c) = a_f, a_a, b_f, b_a, c_f, c_a
with {
    a_f = fb11*a + fb21*b + fb31*c : *(200) + 200;
    b_f = fb22*b + fb12*a + fb32*c : *(200) + 200;
    c_f = fb33*c + fb13*a + fb23*b : *(200) + 200;
    b_a = fb11*a + fb21*b + fb31*c;
    c_a = fb22*b + fb12*a + fb32*c;
    a_a = fb33*c + fb13*a + fb23*b;
};

myosc(f, a) = os.osc(f) * a;

process = ((myosc, myosc, myosc) ~ fb_block) :> *(0.1) <: _,_;