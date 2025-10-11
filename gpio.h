#ifndef MUX_H
#define MUX_H

#define NUM_BUTTONS 27

#define BUTTON_NUM_SAVE 3
#define BUTTON_NUM_LOAD 1
#define BUTTON_NUM_ALT 22
#define BUTTON_NUM_OS1 16
#define BUTTON_NUM_SG 25
#define BUTTON_NUM_ENV 24
#define BUTTON_NUM_LEARN 5
#define BUTTON_NUM_ENCODER 26
#define BUTTON_NUM_MIDI_CHAN 0 // ?

struct Param {
    int button;
    bool alt;
    bool cc;
};

void buttons_init();
void poll_gpio();

uint8_t *get_button_states(void);
uint8_t *get_button_pushes(void);
void clear_button_pushes(void);
bool alt_held(void);
bool OS1_held(void);
bool SG_held(void);
bool ENV_held(void);
int button_label(int num);
void set_param_selected(int num, bool alt, bool cc);
struct Param get_param_selected(void);
void read_buttons();

#endif