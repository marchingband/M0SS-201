#ifndef FLASH_H
#define FLASH_H

#define NUM_PRESETS 17
#define NUM_CC_MAP 128

struct Config {
    uint8_t empty[NUM_PRESETS];
    uint8_t cc_map[NUM_CC_MAP];
    uint8_t magic;
};

void flash_init(void);
void save(int slot);
void load(int slot);
int get_current_slot(void);
struct Config *get_config(void);

void load_preset(int slot);
void load_all_presets_from_flash(void);

#endif