#include "bflb_flash.h"
#include "board.h"
#include "midi.h"
#include "gpio.h"
#include "flash.h"

#define CONFIG_BASE 0x40000
#define PRESET_BASE 0x20000

#define SECTOR_SIZE 0x1000 // 4096

#define NUM_CONFIG_VARS 1

#define NUM_FLOATS (NUM_CC + NUM_BUTTONS + NUM_BUTTONS + NUM_BUTTONS + NUM_BUTTONS + NUM_CONFIG_VARS)
#define NUM_BYTES_PER_PRESET (NUM_FLOATS * sizeof(float)) // 612
#define TOTAL_BYTES (NUM_BYTES_PER_PRESET * NUM_PRESETS) // 10,414

extern int cur_midi_chan;

static float presets[NUM_PRESETS][NUM_FLOATS];

static int current_slot = -1;

static struct Config default_config = {
    .empty = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true },
    .cc_map = {
        1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
        17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
        33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,
        49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
        65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,
        81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,
        97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,
        113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128
    },
    .magic = 129,
};

static struct Config current_config;

static uint8_t buf[NUM_BYTES_PER_PRESET];
static float *floats_buf;

int get_current_slot(void)
{
    return current_slot;
}

struct Config *get_config(void)
{
    return &current_config;
}

void flash_init(void)
{
    // printf("initializing flash\n");
    struct Config conf;
    bflb_flash_read(CONFIG_BASE, &conf, sizeof(struct Config));
    if(conf.magic == default_config.magic)
    {
        // printf("the magic number in flash was correct\n");
        current_config = conf;
        // for(int i=0; i< 127; i++)
        // {
        //     printf("%d : %d\n", i, current_config.cc_map[i]);
        // }
    }
    else
    {
        // printf("the magic number in flash was NOT correct\n");
        // printf("initializing flash storage!\n");
        bflb_flash_erase(CONFIG_BASE, sizeof(struct Config));
        bflb_flash_erase(PRESET_BASE, TOTAL_BYTES);
        bflb_flash_write(CONFIG_BASE, &default_config, sizeof(struct Config));
        current_config = default_config;
    }
    load_all_presets_from_flash();
}

void save(int slot)
{
    // printf("saving slot %d\n", slot);
    current_slot = slot;
    int ret;
    uint32_t base = PRESET_BASE + (SECTOR_SIZE * slot);

    floats_buf = (float *)&buf[0];

    struct CC *ccs   = get_ccs();
    struct Mod *mods = get_mods();
    struct Alt *alts = get_alts();
    struct Sel *sels = get_sels();
    struct Asb *asbs = get_asbs();

    int idx = 0;

    for(int i=0; i< NUM_CC; i++)
    {
        float val = ccs[i].p ? *ccs[i].p : 0.0;
        // printf("writing %f to cc %d\n", val, i);
        floats_buf[idx++] = val;
    }

    for(int i=0; i< NUM_BUTTONS; i++)
    {
        float val = mods[i].p ? *mods[i].p : 0.0;
        // printf("writing %f to mods %d\n", val, i);
        floats_buf[idx++] = val;
    }

    for(int i=0; i< NUM_BUTTONS; i++)
    {
        float val = alts[i].p ? *alts[i].p : 0.0;
        // printf("writing %f to alts %d\n", val, i);
        floats_buf[idx++] = val;
    }

    for(int i=0; i< NUM_BUTTONS; i++)
    {
        float val = asbs[i].p ? *asbs[i].p : 0.0;
        // printf("writing %f to alts %d\n", val, i);
        floats_buf[idx++] = val;
    }

    for(int i=0; i< NUM_BUTTONS; i++)
    {
        float val = sels[i].p ? *sels[i].p : 0.0;
        // printf("writing %f to sels %d\n", val, i);
        floats_buf[idx++] = val;
    }

    // add config vars
    floats_buf[idx++] = (float)cur_midi_chan;


    // printf("erase slot\n");
    ret = bflb_flash_erase(base, sizeof(buf));
    if(ret < 0)
    {
        // printf("bflb_flash_erase returned %d\n", ret);
        return;
    }
    // printf("write buf\n");
    ret = bflb_flash_write(base, buf, sizeof(buf));
    if(ret < 0)
    {
        // printf("bflb_flash_write returned %d\n", ret);
        return;
    }

    // printf("erase config\n");
    ret = bflb_flash_erase(CONFIG_BASE, sizeof(struct Config));
    if(ret < 0)
    {
        // printf("bflb_flash_erase returned %d\n", ret);
        return;
    }
    // printf("write config\n");
    current_config.empty[slot] = false;
    ret = bflb_flash_write(CONFIG_BASE, &current_config, sizeof(struct Config));
    if(ret < 0)
    {
        // printf("bflb_flash_write returned %d\n", ret);
        return;
    }
}

void load(int slot)
{
    if(current_config.empty[slot] == true)
    {
        // printf("memory slot %d is empty\n", slot);
        return;
    }

    current_slot = slot;

    // printf("loading slot %d\n", slot);
    int ret;
    uint32_t base = PRESET_BASE + (SECTOR_SIZE * slot);

    struct CC *ccs = get_ccs();
    struct Mod *mods = get_mods();
    struct Alt *alts = get_alts();
    struct Sel *sels = get_sels();
    struct Asb *asbs = get_asbs();

    ret = bflb_flash_read(base, buf, sizeof(buf));

    if(ret < 0)
    {
        // printf("bflb_flash_read returned %d\n", ret);
        return;
    }

    floats_buf = (float *) &buf[0];

    int idx = 0;

    for(int i=0; i< NUM_CC; i++)
    {
        if(ccs[i].p == NULL)
        {
            // printf("cc %d is null\n", i);
            idx++;
            continue;
        }
        float val = floats_buf[idx++];
        // printf("reading %f to cc %d\n", val, i);
        *ccs[i].p = val;
    }

    for(int i=0; i< NUM_BUTTONS; i++)
    {
        if(mods[i].p == 0)
        {
            // printf("mod %d is null\n", i);
            idx++;
            continue;
        }
        float val = floats_buf[idx++];
        // printf("reading %f to mod %d\n", val, i);
        *mods[i].p = val;
    }

    for(int i=0; i< NUM_BUTTONS; i++)
    {
        if(alts[i].p == 0)
        {
            // printf("alt %d is null\n", i);
            idx++;
            continue;
        }
        float val = floats_buf[idx++];
        // printf("reading %f to alts %d\n", val, i);
        *alts[i].p = val;
    }

    for(int i=0; i< NUM_BUTTONS; i++)
    {
        if(asbs[i].p == 0)
        {
            // printf("alt %d is null\n", i);
            idx++;
            continue;
        }
        float val = floats_buf[idx++];
        // printf("reading %f to alts %d\n", val, i);
        *asbs[i].p = val;
    }

    for(int i=0; i< NUM_BUTTONS; i++)
    {
        if(sels[i].p == 0)
        {
            // printf("sel %d is null\n", i);
            idx++;
            continue;
        }
        float val = floats_buf[idx++];
        // printf("reading %f to sels %d\n", val, i);
        *sels[i].p = val;
    }

    cur_midi_chan = (int)floats_buf[idx++];
}

void load_all_presets_from_flash(void){
    for(int slot=0; slot<NUM_PRESETS; slot++){
        if(current_config.empty[slot] == true)
        {
            printf("memory slot %d is empty\n", slot);
            continue;
        }
    
        uint32_t flash_addr = PRESET_BASE + (SECTOR_SIZE * slot);
        float *preset_start = &presets[slot][0];
        uint8_t *dest = (uint8_t *)preset_start;
        int ret = bflb_flash_read(flash_addr, dest, NUM_BYTES_PER_PRESET);
        if(ret < 0)
        {
            printf("bflb_flash_read returned %d\n", ret);
            continue;
        }
    
    }
}

void load_preset(int slot){
    if(current_config.empty[slot] == true)
    {
        printf("memory slot %d is empty\n", slot);
        return;
    }

    current_slot = slot;

    // printf("loading slot %d\n", slot);
    // int ret;
    // uint32_t base = PRESET_BASE + (SECTOR_SIZE * slot);

    struct CC *ccs = get_ccs();
    struct Mod *mods = get_mods();
    struct Alt *alts = get_alts();
    struct Sel *sels = get_sels();
    struct Asb *asbs = get_asbs();

    // ret = bflb_flash_read(base, buf, sizeof(buf));

    // if(ret < 0)
    // {
    //     // printf("bflb_flash_read returned %d\n", ret);
    //     return;
    // }

    // floats_buf = (float *) &buf[0];
    floats_buf = &presets[slot][0];

    int idx = 0;

    for(int i=0; i< NUM_CC; i++)
    {
        if(ccs[i].p == NULL)
        {
            // printf("cc %d is null\n", i);
            idx++;
            continue;
        }
        float val = floats_buf[idx++];
        // printf("reading %f to cc %d\n", val, i);
        *ccs[i].p = val;
    }

    for(int i=0; i< NUM_BUTTONS; i++)
    {
        if(mods[i].p == 0)
        {
            // printf("mod %d is null\n", i);
            idx++;
            continue;
        }
        float val = floats_buf[idx++];
        // printf("reading %f to mod %d\n", val, i);
        *mods[i].p = val;
    }

    for(int i=0; i< NUM_BUTTONS; i++)
    {
        if(alts[i].p == 0)
        {
            // printf("alt %d is null\n", i);
            idx++;
            continue;
        }
        float val = floats_buf[idx++];
        // printf("reading %f to alts %d\n", val, i);
        *alts[i].p = val;
    }

    for(int i=0; i< NUM_BUTTONS; i++)
    {
        if(asbs[i].p == 0)
        {
            // printf("alt %d is null\n", i);
            idx++;
            continue;
        }
        float val = floats_buf[idx++];
        // printf("reading %f to alts %d\n", val, i);
        *asbs[i].p = val;
    }

    for(int i=0; i< NUM_BUTTONS; i++)
    {
        if(sels[i].p == 0)
        {
            // printf("sel %d is null\n", i);
            idx++;
            continue;
        }
        float val = floats_buf[idx++];
        // printf("reading %f to sels %d\n", val, i);
        *sels[i].p = val;
    }

    cur_midi_chan = (int)floats_buf[idx++];
}