#include "biquad.h"

#include <math.h>


float process_biquad(Biquad_t *f, float input) {
    // Calculate the current output sample
    float out = (f->b0 * input) + (f->b1 * f->x1) + (f->b2 * f->x2) 
                - (f->a1 * f->y1) - (f->a2 * f->y2);

    // Update delay lines for next iteration
    f->x2 = f->x1;
    f->x1 = input;
    f->y2 = f->y1;
    f->y1 = out;

    return out;
}

Biquad_t generate_lowpass(float sample_freq, float cutoff_freq, float Q) {
    Biquad_t f = {0}; // Initialize delay lines to zero
    
    float omega = 2.0f * M_PI * cutoff_freq / sample_freq;
    float sn = sinf(omega);
    float cs = cosf(omega);
    float alpha = sn / (2.0f * Q);

    // a0 is used to normalize all other coefficients
    float a0 = 1.0f + alpha;

    f.b0 = ((1.0f - cs) / 2.0f) / a0;
    f.b1 = (1.0f - cs) / a0;
    f.b2 = ((1.0f - cs) / 2.0f) / a0;
    f.a1 = (-2.0f * cs) / a0;
    f.a2 = (1.0f - alpha) / a0;

    return f;
}