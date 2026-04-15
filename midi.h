#ifndef MIDI_H
#define MIDI_H

#define NUM_CC 68 // must be the highest CC value + 1, CC's are 1 indexed.

#define LEARN_MODE_READY 0
#define LEARN_MODE_SELECTED 1
#define LEARN_MODE_LEARNED 2

#define ALT_SWITCH_STATE_NONE 0
#define ALT_SWITCH_STATE_ON 1
#define ALT_SWITCH_STATE_OFF 2

#define MIDI_CHANNEL_OMNI 17

#define BUTTON_NUM_MATRIX_ROW_OS1 16
#define BUTTON_NUM_MATRIX_ROW_SG 25
#define BUTTON_NUM_MATRIX_ROW_ENV 24
#define BUTTON_NUM_MATRIX_COLUMN_LEN 1
#define BUTTON_NUM_MATRIX_COLUMN_RATE 5
#define BUTTON_NUM_MATRIX_COLUMN_RCL 7
#define BUTTON_NUM_MATRIX_COLUMN_SMTH 3
#define BUTTON_NUM_MATRIX_COLUMN_ATT 10
#define BUTTON_NUM_MATRIX_COLUMN_REL 14
#define BUTTON_NUM_MATRIX_COLUMN_OS1 2
#define BUTTON_NUM_MATRIX_COLUMN_OS2 17
#define BUTTON_NUM_MATRIX_COLUMN_BUS -1
#define BUTTON_NUM_MATRIX_COLUMN_IDX 8
#define BUTTON_NUM_MATRIX_COLUMN_TIMB 23
#define BUTTON_NUM_MATRIX_COLUMN_AMP 19

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
    char name[20];
};

struct Mod {
    float *p;
    float min;
    float max;
    float init;
    float step;
    float mul;
    char name[20];
};

struct Alt {
    float *p;
    float min;
    float max;
    float init;
    float step;
    float mul;
    char name[20];
};

struct Sel {
    float *p;
    float min;
    float max;
    float init;
    char name[20];
};

struct AltSel {
    float *p;
    float min;
    float max;
    float init;
    char name[20];
};
struct Asb {
    float *p;
    float min;
    float max;
    float init;
    float step;
    float mul;
    char name[20];
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