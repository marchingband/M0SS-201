#ifndef DSP_UTILS_H_
#define DSP_UTILS_H_

#include <stdint.h>
#include "pow2f_lut.h"

static inline int32_t clipi32(int32_t x, int32_t m, int32_t M) {
	return x < m ? m : (x > M ? M : x);
}

static inline float copysignf(float x, float y) {
	union { float f; uint32_t u; } v, s;
	v.f = x;
	s.f = y;
	v.u = (v.u & 0x7fffffffu) | (s.u & 0x80000000u);
	return v.f;
}

static inline float absf(float x) {
	union { float f; uint32_t u; } v;
	v.f = x;
	v.u = v.u & 0x7fffffffu;
	return v.f;
}

static inline int32_t signfilli32(int32_t x) {
	return x < 0 ? ~0 : 0;
}

static inline float truncf(float x) {
	union { float f; uint32_t u; } v;
	v.f = x;
	const int32_t ex = (v.u & 0x7f800000u) >> 23;
	uint32_t m = (~0u) << clipi32(150 - ex, 0, 23);
	m &= (uint32_t)signfilli32(126 - ex) | 0x80000000;
	v.u &= m;
	return v.f;
}

static inline float floorf(float x) {
	union { float f; int32_t i; } t, y, s;
	t.f = truncf(x); // first bit set when t < 0
	y.f = x - t.f; // first bit set when t > x
	s.f = 1.f;
	s.i &= signfilli32(t.i & y.i);
	const float r = t.f - s.f;
	return r;
}

static inline float sin2pif(float x) {
	x = x - floorf(x);
	float xp1 = x + x - 1.f;
	float xp2 = absf(xp1);
	float xp = 1.570796326794897f - 1.570796326794897f * absf(xp2 + xp2 - 1.f);
	const float y = -copysignf(1.f, xp1) * (xp + xp * xp * (-0.05738534102710938f - 0.1107398163618408f * xp));
	return y;
}


static inline float log2f(float x) {
	union { float f; int32_t i; } v;
	v.f = x;
	int e = v.i >> 23;
	v.i = (v.i & 0x007fffff) | 0x3f800000;
	const float y = (float)e - 129.213475204444817f + v.f * (3.148297929334117f + v.f * (-1.098865286222744f + v.f * 0.1640425613334452f));
	return y;
}

#define CLAMP(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define MAX(a, b) (((a) > (b))? (a) : (b))
#define MIN(a, b) (((a) < (b))? (a) : (b))

// courtesy of Emilie Gillet
#define MAKE_INTEGRAL_FRACTIONAL(x) \
    int32_t x ## _integral = (int32_t)(x); \
    float x ## _fractional = x - (float)(x ## _integral);

static inline float interpolate(const float* table, float index, float size) {
    index *= size - 1;
    MAKE_INTEGRAL_FRACTIONAL(index)
    float a = table[index_integral];
    float b = table[index_integral + 1];
    return a + (b - a) * index_fractional;
}

static inline float pow2f(float x) {
    uint8_t neg = x < 0;
    if (neg) x = -x;
    MAKE_INTEGRAL_FRACTIONAL(x);
    float lut_val = interpolate(pow2f_lut, x_fractional, 1024.0);
    lut_val *= (float)(1 << x_integral);
    return neg? 1.0f/lut_val : lut_val;
}

static inline float expf(float x) {
    const float log2e = 1.44269804089f;
    return pow2f(x * log2e);
}

static inline float tanhf(float x) {
	const float xm = CLAMP(x, -2.115287308554551f, 2.115287308554551f);
	const float axm = absf(xm);
	const float y = xm * axm * (0.01218073260037716f * axm - 0.2750231331124371f) + xm;
	return y;
}

// template <int c>
// constexpr float normalized_linear_to_logexp(float x) {
//     constexpr float coeff = 1.0f / (pow2f(c) - 1.0f);
//     return (pow2f(c*x) - 1.0f) * coeff;
// }

// if we're running out of CPU we can inline the coefficient calculation
static inline float normalized_linear_to_logexp(float x, int c) {
    float coeff = 1.0f / (pow2f(c) - 1.0f);
    return (pow2f(c*x) - 1.0f) * coeff;
}

static inline uint32_t rand_u32() {
    static uint64_t val = 1234;  // seed
    val = (val*134775813 + 1) % (1 << 31);
    return (uint32_t)(val);
}

#define UINT32_MAX_INV 2.328306e-10f;
static inline float randf() {
    uint32_t rand_int = rand_u32();
    return (float)rand_int * UINT32_MAX_INV;
}

inline float this_blep(float t) {
    return 0.5f * t * t;
}

inline float next_blep(float t) {
    t = 1.0f - t;
    return -0.5f * t * t;
}

inline float next_iblep(float t) {
    const float t1 = 0.5f * t;
    const float t2 = t1 * t1;
    const float t4 = t2 * t2;
    return 0.1875f - t1 + 1.5f * t2 - t4;
}

inline float this_iblep(float t) {
    return next_iblep(1.0f - t);
}

inline float linear_map(float x, float a, float b, float A, float B) {
	float normalized_x = (x - a) / (b - a);
	return normalized_x * (B - A) + A;
}

static inline float midi_note_to_freq_hz(float note) {
	return 440.0f * pow2f((note - 69.0f) * 0.08333f);
}

inline float InterpolateHermite(const float* table, float index, float size) {
	index *= size;
	MAKE_INTEGRAL_FRACTIONAL(index)
	const float xm1 = table[index_integral - 1];
	const float x0 = table[index_integral + 0];
	const float x1 = table[index_integral + 1];
	const float x2 = table[index_integral + 2];
	const float c = (x1 - xm1) * 0.5f;
	const float v = x0 - x1;
	const float w = c + v;
	const float a = w + v + (x2 - x0) * 0.5f;
	const float b_neg = w + a;
	const float f = index_fractional;
	return (((a * f) - b_neg) * f + c) * f + x0;
}

inline float soft_limit(float x) {
  	return x * (27.0f + x * x) / (27.0f + 9.0f * x * x);
}

inline float soft_clip(float x) {
	if (x < -3.0f) {
		return -1.0f;
	} else if (x > 3.0f) {
		return 1.0f;
	} else {
		return soft_limit(x);
	}
}

#endif  // DSP_UTILS_H_