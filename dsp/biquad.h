#ifndef BIQUAD_H_
#define BIQUAD_H_

typedef struct {
    float b0, b1, b2; // Numerator coefficients
    float a1, a2;     // Denominator coefficients (a0 is normalized to 1)
    float x1, x2;     // Input delay line (x[n-1], x[n-2])
    float y1, y2;     // Output delay line (y[n-1], y[n-2])
} Biquad_t;

float process_biquad(Biquad_t* f, float input);
Biquad_t generate_lowpass(float sample_freq, float cutoff_freq, float Q);

#endif  // BIQUAD_H_