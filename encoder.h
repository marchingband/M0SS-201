#ifndef ENCODER_H
#define ENCODER_H

struct encoder_change_event_t {
    uint64_t time;
    bool up;
};

void encoder_init(void);
void process_encoder(void);

#endif