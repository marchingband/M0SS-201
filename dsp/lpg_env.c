
class LPGEnvelope {
    public:
    LPGEnvelope() { }
    ~LPGEnvelope() { }

    inline void Init() {
        vactrol_state_ = 0.0f;
        gain_ = 1.0f;
        frequency_ = 0.5f;
        hf_bleed_ = 0.0f;
        ramp_up_ = false;
    }

    inline void Trigger() {
        ramp_up_ = true;
    }

    inline void ProcessPing(float attack, float short_decay, float decay_tail, float hf) {
        if (ramp_up_) {
            vactrol_state_ += attack;
            if (vactrol_state_ >= 1.0f) {
                vactrol_state_ = 1.0f;
                ramp_up_ = false;
            }
        }
        ProcessLP(ramp_up_ ? vactrol_state_ : 0.0f, short_decay, decay_tail, hf);
    }

    inline void ProcessLP(float level, float short_decay, float decay_tail, float hf) {
        float vactrol_input = level;
        float vactrol_error = (vactrol_input - vactrol_state_);
        float vactrol_state_2 = vactrol_state_ * vactrol_state_;
        float vactrol_state_4 = vactrol_state_2 * vactrol_state_2;
        float tail = 1.0f - vactrol_state_;
        float tail_2 = tail * tail;
        float vactrol_coefficient = (vactrol_error > 0.0f)? 0.6f \
            : short_decay + (1.0f - vactrol_state_4) * decay_tail;

        vactrol_state_ += vactrol_coefficient * vactrol_error;
        gain_ = vactrol_state_;
        frequency_ = 0.003f + 0.3f * vactrol_state_4 + hf * 0.04f;
        hf_bleed_ = (tail_2 + (1.0f - tail_2) * hf) * hf * hf;
    }


    inline float gain() const { return gain_; }
    inline float frequency() const { return frequency_; }
    inline float hf_bleed() const { return hf_bleed_; }
private:
    float vactrol_state_;
    float gain_;
    float frequency_;
    float hf_bleed_;
    bool ramp_up_;
};

 float compressed_level = 1.3f * modulations.level / (0.3f + fabsf(modulations.level));
CONSTRAIN(compressed_level, 0.0f, 1.0f);
 const float short_decay = (200.0f * kBlockSize) / kSampleRate *
SemitonesToRatio(-96.0f * patch.decay);

const float hf = patch.lpg_colour;
const float decay_tail = (20.0f * kBlockSize) / kSampleRate * SemitonesToRatio(-72.0f * patch.decay + 12.0f * hf) - short_decay;

lpg_envelope_.ProcessLP(compressed_level, short_decay, decay_tail, hf);
