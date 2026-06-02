#ifndef MIDI_H
#define MIDI_H

#define NUM_CC 63 // must be the highest CC value + 1, CC's are 1 indexed.

#define LEARN_MODE_READY 0
#define LEARN_MODE_SELECTED 1
#define LEARN_MODE_LEARNED 2

#define ALT_SWITCH_STATE_NONE 0
#define ALT_SWITCH_STATE_ON 1
#define ALT_SWITCH_STATE_OFF 2

#define MIDI_CHANNEL_OMNI 17

#define BUTTON_NUM_MATRIX_ROW_OS2 10
#define BUTTON_NUM_MATRIX_ROW_SG 12
#define BUTTON_NUM_MATRIX_ROW_ENV 11

#define BUTTON_NUM_MATRIX_COLUMN_OS1 16
#define BUTTON_NUM_MATRIX_COLUMN_IDX 23
#define BUTTON_NUM_MATRIX_COLUMN_OS2 7
#define BUTTON_NUM_MATRIX_COLUMN_SMTH 17
#define BUTTON_NUM_MATRIX_COLUMN_RATE 21
#define BUTTON_NUM_MATRIX_COLUMN_RCL 20
#define BUTTON_NUM_MATRIX_COLUMN_ATT 3
#define BUTTON_NUM_MATRIX_COLUMN_REL 4
#define BUTTON_NUM_MATRIX_COLUMN_TIMB 6
#define BUTTON_NUM_MATRIX_COLUMN_AMP 2

#define CC_MAP_VELOCITY_IDX     128
#define CC_MAP_AFTERTOUCH_IDX   129

extern int matrix_row_selected;
extern int matrix_column_selected;

struct CC {
    float *p;
    float min;
    float max;
    float init;
    float step;
    float mul;
    bool numeric;
};

struct Mod {
    float *p;
    float min;
    float max;
    float init;
    float step;
    float mul;
    bool numeric;
};

struct Alt {
    float *p;
    float min;
    float max;
    float init;
    float step;
    float mul;
    bool numeric;
};

struct Sel {
    float *p;
    float min;
    float max;
    float init;
    bool numeric;
};

struct AltSel {
    float *p;
    float min;
    float max;
    float init;
    bool numeric;
};
struct Asb {
    float *p;
    float min;
    float max;
    float init;
    float step;
    float mul;
    bool numeric;
};

struct Sigs {
    float *pitch;
    float *gate;
    float *sg;
    float *le;
    float *mo;
    float *mo_freq;
    float *amp;
    // float *lfo;
    // float *env;
    float *pitch_bend;
    // float *vca;
    float *fine_tune;
    float *transpose;
};

struct Sel *get_sels(void);
struct AltSel *get_alt_sels(void);
struct Asb *get_asbs(void);
struct Mod *get_mods(void);
struct Alt *get_alts(void);
struct CC *get_ccs(void);
struct Sigs *get_signals(void);
uint8_t *get_cc_map(void);
int get_mode_learn_current_cc(void);
void reset_params(void);

void uart_receive_init(void);

void link_ui();

#endif