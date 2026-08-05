#include "bflb_flash.h"
#include "board.h"
#include "midi.h"
#include "gpio.h"
#include "flash.h"
#include "dsp/synth.h"
#include "bl616_glb.h"

#define CONFIG_BASE 0x40000
#define PRESET_BASE 0x20000

#define SECTOR_SIZE 0x1000 // 4096

#define NUM_CONFIG_VARS (1 + NUM_CC_MAP) // midi_chan + CC map

#define NUM_MODS NUM_BUTTONS
#define NUM_ALTS NUM_BUTTONS
#define NUM_SELS NUM_BUTTONS
#define NUM_ALTSELS NUM_BUTTONS
#define NUM_ASBS NUM_BUTTONS
#define SG_STATE_LENGTH 32

#define NUM_FLOATS (NUM_CC + NUM_MODS + NUM_ALTS + NUM_SELS + NUM_ALTSELS + NUM_ASBS + NUM_CONFIG_VARS + SG_STATE_LENGTH)
#define NUM_BYTES_PER_PRESET (NUM_FLOATS * sizeof(float)) // 612
#define TOTAL_BYTES (NUM_BYTES_PER_PRESET * NUM_PRESETS) // 10,414

extern int cur_midi_chan;
extern bool is_initial_boot;

extern SGState_t sg_state;

static float presets[NUM_PRESETS][NUM_FLOATS];

static int current_slot = -1;

static struct Config default_config = {
    // 30 presets
    .empty = { true, true, true, true, true, true, 
               true, true, true, true, true, true, 
               true, true, true, true, true, true, 
               true, true, true, true, true, true, 
               true, true, true, true, true, true},
    .cc_map = {
        1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
        17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
        33,34,35,36,37,38,39,40,41,42,43,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0 // 130
    },
    .magic = 19,
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
        is_initial_boot = true;
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
    struct AltSel *altsels = get_alt_sels();
    struct Asb *asbs = get_asbs();

    int idx = 0;

    for(int i=0; i< NUM_CC; i++)
    {
        float val = ccs[i].p ? *ccs[i].p : 0.0;
        // printf("writing %f to cc %d\n", val, i);
        floats_buf[idx++] = val;
    }

    for(int i=0; i< NUM_MODS; i++)
    {
        float val = mods[i].p ? *mods[i].p : 0.0;
        // printf("writing %f to mods %d\n", val, i);
        floats_buf[idx++] = val;
    }

    for(int i=0; i< NUM_ALTS; i++)
    {
        float val = alts[i].p ? *alts[i].p : 0.0;
        // printf("writing %f to alts %d\n", val, i);
        floats_buf[idx++] = val;
    }

    for(int i=0; i< NUM_SELS; i++)
    {
        float val = sels[i].p ? *sels[i].p : 0.0;
        // printf("writing %f to sels %d\n", val, i);
        floats_buf[idx++] = val;
    }

    for(int i=0; i< NUM_ALTSELS; i++)
    {
        float val = altsels[i].p ? *altsels[i].p : 0.0;
        // printf("writing %f to sels %d\n", val, i);
        floats_buf[idx++] = val;
    }

    for(int i=0; i< NUM_ASBS; i++)
    {
        float val = asbs[i].p ? *asbs[i].p : 0.0;
        // printf("writing %f to alts %d\n", val, i);
        floats_buf[idx++] = val;
    }

    // add config vars
    floats_buf[idx++] = (float)cur_midi_chan;
    for(int i=0; i<NUM_CC_MAP; i++){
        floats_buf[idx++] = current_config.cc_map[i];
    }

    for (int i=0; i < SG_STATE_LENGTH; i++) {
        floats_buf[idx++] = sg_state.arr[i];
    }

    // save to ram presets
    memcpy(&presets[slot], buf, NUM_BYTES_PER_PRESET);

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
    struct AltSel *altsels = get_alt_sels();
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

    for(int i=0; i< NUM_MODS; i++)
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

    for(int i=0; i< NUM_ALTS; i++)
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

    for(int i=0; i< NUM_SELS; i++)
    {
        if(sels[i].p == 0)
        {
            // printf("alt %d is null\n", i);
            idx++;
            continue;
        }
        float val = floats_buf[idx++];
        // printf("reading %f to alts %d\n", val, i);
        *sels[i].p = val;
    }

    for(int i=0; i< NUM_ALTSELS; i++)
    {
        if(altsels[i].p == 0)
        {
            // printf("sel %d is null\n", i);
            idx++;
            continue;
        }
        float val = floats_buf[idx++];
        // printf("reading %f to sels %d\n", val, i);
        *altsels[i].p = val;
    }

    for(int i=0; i< NUM_ASBS; i++)
    {
        if(asbs[i].p == 0)
        {
            // printf("asbs %d is null\n", i);
            idx++;
            continue;
        }    
        float val = floats_buf[idx++];
        // printf("reading %f to alts %d\n", val, i);
        *asbs[i].p = val;
    }  

    cur_midi_chan = (int)floats_buf[idx++];
    for(int i=0; i<NUM_CC_MAP; i++){
        current_config.cc_map[i] = floats_buf[idx++];
    }

    for (int i=0; i < SG_STATE_LENGTH; i++) {
        sg_state.arr[i] = floats_buf[idx++];
    }
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
    struct Sel *altsels = get_alt_sels();
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

    for(int i=0; i< NUM_MODS; i++)
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

    for(int i=0; i< NUM_SELS; i++)
    {
        if(sels[i].p == 0)
        {
            // printf("alt %d is null\n", i);
            idx++;
            continue;
        }
        float val = floats_buf[idx++];
        // printf("reading %f to alts %d\n", val, i);
        *sels[i].p = val;
    }

    for(int i=0; i< NUM_ALTSELS; i++)
    {
        if(altsels[i].p == 0)
        {
            // printf("sel %d is null\n", i);
            idx++;
            continue;
        }
        float val = floats_buf[idx++];
        // printf("reading %f to sels %d\n", val, i);
        *altsels[i].p = val;
    }

    for(int i=0; i< NUM_ASBS; i++)
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

    cur_midi_chan = (int)floats_buf[idx++];
    for(int i=0; i<NUM_CC_MAP; i++){
        current_config.cc_map[i] = floats_buf[idx++];
    }

    for (int i=0; i < SG_STATE_LENGTH; i++) {
        sg_state.arr[i] = floats_buf[idx++];
    }
}

void factory_reset(void) {
    bflb_flash_erase(CONFIG_BASE, sizeof(struct Config));
    GLB_SW_System_Reset();
}