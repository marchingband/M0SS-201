#include "wavefolder.h"

#define NUM_CELLS 5

typedef struct {
    float gain;
    float bias;
    float threshold;
    float mix;
} FolderCell;

static const FolderCell cells[NUM_CELLS] = {
    {0.8333f, 0.5f,   0.6f,   -12.0f},
    {0.3768f, 1.1281f, 2.994f, -27.777f},
    {0.2829f, 1.5446f, 5.46f,  -21.428f},
    {0.5743f, 1.0338f, 1.8f,    17.647f},
    {0.2673f, 1.0907f, 4.08f,   36.363f},
};

static inline float sgnf(float x) {
    if (x >= 0.0f) return 1.0f;
    return -1.0f;
}

static inline float absf(float x) {
    return sgnf(x) * x;
}

float compute_cell(FolderCell* cell, float x) {
    if (absf(x) < cell->threshold) return 0.0f;
    return cell->mix * (cell->gain*x - cell->bias*sgnf(x));
}

float naive_buchla_wf_compute(float x) {
    float out = 5.0f * x;
    for (uint32_t i = 0; i < NUM_CELLS; i++) {
        out += compute_cell(&cells[i], x);
    }
    return out * 0.2f;
}
