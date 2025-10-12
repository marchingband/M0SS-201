#include "bflb_i2s.h"
#include "bflb_gpio.h"
#include "bflb_dma.h"
#include "bl616_glb_gpio.h"
#include "bflb_l1c.h"
#include "bl616_irq.h"
#include "bflb_irq.h"
#include "board.h"
#include "rgb_led.h"
#include "midi.h"
#include "flash.h"
#include "spi_rgb.h"

#define PIXEL_SIZE 3 // each colour takes 4 bytes
#define NUM_LED 67

void test_lights(void);

extern int learn_mode_state;
extern int alt_switch_state;
int cur_disp_num = 0;

static ATTR_NOCACHE_NOINIT_RAM_SECTION uint32_t data_buf[256] __ALIGNED(4);
static const uint16_t bit_patterns[4] = {0x88, 0x8e, 0xe8, 0xee}; // original
struct pixel_t pixel_buf[NUM_LED] = {0};

void encode_rgb(uint32_t *buf){
    for (int i = 0; i < NUM_LED; i++) {
        int loc = i * PIXEL_SIZE;

        buf[loc]     = (bit_patterns[pixel_buf[i].green >> 6 & 0x03]  << 24) | 
                        (bit_patterns[pixel_buf[i].green >> 4 & 0x03] << 16) | 
                        (bit_patterns[pixel_buf[i].green >> 2 & 0x03] << 8)  | 
                         bit_patterns[pixel_buf[i].green & 0x03];

        buf[loc + 1] = (bit_patterns[pixel_buf[i].red >> 6 & 0x03]    << 24) | 
                        (bit_patterns[pixel_buf[i].red >> 4 & 0x03]   << 16) |
                        (bit_patterns[pixel_buf[i].red   >> 2 & 0x03] << 8)  | 
                         bit_patterns[pixel_buf[i].red & 0x03];
                         
        buf[loc + 2] = (bit_patterns[pixel_buf[i].blue >> 6 & 0x03]   << 24) | 
                        (bit_patterns[pixel_buf[i].blue >> 4 & 0x03]  << 16) |
                        (bit_patterns[pixel_buf[i].blue  >> 2 & 0x03] << 8)  |
                         bit_patterns[pixel_buf[i].blue & 0x03];
    }
}

static uint64_t times[100] = {0};
static size_t time_p = 0;

void update_leds(void)
{
    // // test_lights(); // 201
    render_lights();
    encode_rgb(data_buf);
    spi_rgb_send(data_buf, 128);
}

const struct pixel_t pixel_red = {
    .red = 1,
    .green = 0,
    .blue = 0
};

const struct pixel_t pixel_green = {
    .red = 0,
    .green = 1,
    .blue = 0
};

const struct pixel_t pixel_blue = {
    .red = 0,
    .green = 0,
    .blue = 1
};

const struct pixel_t pixel_on = {
    .red = 4,
    .green = 3,
    .blue = 1
};

const struct pixel_t pixel_off = {
    .red = 0,
    .green = 0,
    .blue = 0
};

const struct pixel_t light_blue = {
    .red = 0,
    .green = 1,
    .blue = 1
};
const struct pixel_t light_pink = {
    .red = 2,
    .green = 1,
    .blue = 1
};
const struct pixel_t light_green = {
    .red = 1,
    .green = 2,
    .blue = 0
};

const uint8_t number_select_lights[NUM_PRESETS] = { 0, 2, 7, 6, 17, 10, 13, 14, 15, 16, 12, 23, 19, 20, 21, 22, 18 };
int ui_mode = UI_MODE_PLAY;

void set_ui_mode(int mode)
{
    ui_mode = mode;
}

int get_ui_mode(void)
{
    return ui_mode;
}

struct pixel_t pixel_fade(float val){
    struct pixel_t pixel = {
        .red = (uint8_t)(10.0 * val),
        .green = (uint8_t)(150.0 * val),
        .blue = (uint8_t)(50.0 * val)
    };
    return pixel;
}

struct pixel_t pixel_fade_lfo(float temp, float val){
    bool some = val > 0;
    uint8_t green = (uint8_t)(20.0 * val * temp);
    uint8_t red = (uint8_t)(80.0 * val);
    uint8_t blue = (uint8_t)(20.0 * val * temp);
    struct pixel_t pixel = {0};
    if(some){
        pixel.green = green > 0 ? green : 1;
        pixel.red = red > 0 ? red : 1;
        pixel.blue = blue > 0 ? blue : 1;
    }
    return pixel;
}

float led_display[11] = {0};

static float *led_disp_val_p = NULL;
static float led_disp_min = 0;
static float led_disp_max = 0;

void set_led_display(float *val, float min, float max){
    led_disp_val_p = val;
    led_disp_min = min;
    led_disp_max = max;
}

void update_led_display(void){
    if(led_disp_val_p == NULL){
        return;
    }

    float val = *led_disp_val_p;
    float max = led_disp_max;

    val -= led_disp_min;
    max -= led_disp_min;
    float out = val / max * 10;
    for(int i=0; i<11; i++)
    {
        if(out > (i + 1))
        {
            led_display[i] = 1;
        }
        else if(out < i)
        {
            led_display[i] = 0;
        }
        else
        {
            led_display[i] = out - i; // just the fractional part
        }
    }
}

void render_lights(void){

    int current_slot = get_current_slot();
    struct CC *ccs = get_ccs();
    struct Sel *sels = get_sels();
    struct Mod *mods = get_mods();
    struct Sigs *sigs = get_signals();
    struct AltSel *altSels = get_alt_sels();
    struct Config *config = get_config();

    bool os1_hi = *sels[4].p == 0;

    float sg = (*sigs->sg + 1) * 0.5;
    float mo = (*sigs->mo + 1) * 0.5;
    // float mo_freq = *sigs->mo_freq / 3000; // the mosc range is about 3000
    float mo_freq = (*mods[17].p + 2) * 0.25;// posc range is [-2..2]

    // number led bar
    if(alt_switch_state == ALT_SWITCH_STATE_NONE){
        update_led_display();
        pixel_buf[0]  =  led_display[0]  == 1 ? pixel_on : led_display[0]  == 0 ? pixel_off : pixel_fade(led_display[0]);  // 0
        pixel_buf[1]  =  led_display[1]  == 1 ? pixel_on : led_display[1]  == 0 ? pixel_off : pixel_fade(led_display[1]);  // 1
        pixel_buf[2]  =  led_display[2]  == 1 ? pixel_on : led_display[2]  == 0 ? pixel_off : pixel_fade(led_display[2]);  // 2
        pixel_buf[3]  =  led_display[3]  == 1 ? pixel_on : led_display[3]  == 0 ? pixel_off : pixel_fade(led_display[3]);  // 3
        pixel_buf[4]  =  led_display[4]  == 1 ? pixel_on : led_display[4]  == 0 ? pixel_off : pixel_fade(led_display[4]);  // 4
        pixel_buf[5]  =  led_display[5]  == 1 ? pixel_on : led_display[5]  == 0 ? pixel_off : pixel_fade(led_display[5]);  // 5
        pixel_buf[6]  =  led_display[6]  == 1 ? pixel_on : led_display[6]  == 0 ? pixel_off : pixel_fade(led_display[6]);  // 6
        pixel_buf[7]  =  led_display[7]  == 1 ? pixel_on : led_display[7]  == 0 ? pixel_off : pixel_fade(led_display[7]);  // 7
        pixel_buf[8]  =  led_display[8]  == 1 ? pixel_on : led_display[8]  == 0 ? pixel_off : pixel_fade(led_display[8]);  // 8
        pixel_buf[9]  =  led_display[9]  == 1 ? pixel_on : led_display[9]  == 0 ? pixel_off : pixel_fade(led_display[9]);  // 9
        pixel_buf[10] = led_display[10] == 1 ? pixel_on : led_display[10] == 0 ? pixel_off : pixel_fade(led_display[10]); // 10
    } else if(alt_switch_state == ALT_SWITCH_STATE_ON) {
        pixel_buf[0]  = pixel_off;// 0
        pixel_buf[1]  = pixel_off;// 1
        pixel_buf[2]  = pixel_off;// 2
        pixel_buf[3]  = pixel_off;// 3
        pixel_buf[4]  = pixel_off;// 4
        pixel_buf[5]  = pixel_on;// 5
        pixel_buf[6]  = pixel_on;// 6
        pixel_buf[7]  = pixel_on;// 7
        pixel_buf[8]  = pixel_on;// 8
        pixel_buf[9]  = pixel_on;// 9
        pixel_buf[10] = pixel_on;// 10
    } else {
        pixel_buf[0]  = pixel_on;// 0
        pixel_buf[1]  = pixel_on;// 1
        pixel_buf[2]  = pixel_on;// 2
        pixel_buf[3]  = pixel_on;// 3
        pixel_buf[4]  = pixel_on;// 4
        pixel_buf[5]  = pixel_on;// 5
        pixel_buf[6]  = pixel_off;// 6
        pixel_buf[7]  = pixel_off;// 7
        pixel_buf[8]  = pixel_off;// 8
        pixel_buf[9]  = pixel_off;// 9
        pixel_buf[10] = pixel_off;// 10
    }

    // matrix
    pixel_buf[11] = pixel_fade((*(sigs->le)) * (*(ccs[41].p))); // env-amp
    pixel_buf[12] = pixel_fade((*(sigs->le)) * (*(ccs[40].p))); // env-timb
    pixel_buf[13] = pixel_fade((*(sigs->le)) * (*(ccs[39].p))); // env-idx
    pixel_buf[14] = pixel_fade((*(sigs->le)) * (*(ccs[38].p))); // env-bus
    pixel_buf[15] = pixel_fade((*(sigs->le)) * (*(ccs[37].p))); // env-os2
    pixel_buf[16] = pixel_fade((*(sigs->le)) * (*(ccs[36].p))); // env-os1
    pixel_buf[17] = pixel_fade((*(sigs->le)) * (*(ccs[35].p))); // env-rel
    pixel_buf[18] = pixel_fade((*(sigs->le)) * (*(ccs[34].p))); // env-att
    pixel_buf[19] = pixel_fade((*(sigs->le)) * (*(ccs[33].p))); // env-smth
    pixel_buf[20] = pixel_fade((*(sigs->le)) * (*(ccs[32].p))); // env-rcl
    pixel_buf[21] = pixel_fade((*(sigs->le)) * (*(ccs[31].p))); // env-rate
    pixel_buf[22] = pixel_fade((*(sigs->le)) * (*(ccs[30].p))); // env-len
    pixel_buf[23] = pixel_fade(sg * (*(ccs[42].p))); // sg-len
    pixel_buf[24] = pixel_fade(sg * (*(ccs[43].p))); // sg-rate
    pixel_buf[25] = pixel_fade(sg * (*(ccs[44].p))); // sg-rcl
    pixel_buf[26] = pixel_fade(sg * (*(ccs[45].p))); // sg-smth
    pixel_buf[27] = pixel_fade(sg * (*(ccs[46].p))); // sg-att
    pixel_buf[28] = pixel_fade(sg * (*(ccs[47].p))); // sg-rel
    pixel_buf[29] = pixel_fade(sg * (*(ccs[48].p))); // sg-os1
    pixel_buf[30] = pixel_fade(sg * (*(ccs[49].p))); // sg-os2
    pixel_buf[31] = pixel_fade(sg * (*(ccs[50].p))); // sg-bus
    pixel_buf[32] = pixel_fade(sg * (*(ccs[51].p))); // sg-idx
    pixel_buf[33] = pixel_fade(sg * (*(ccs[52].p))); // sg-timb
    pixel_buf[34] = pixel_fade(sg * (*(ccs[53].p))); // sg-amp
    if(os1_hi){
        pixel_buf[35] = pixel_fade_lfo(mo_freq, (*(ccs[65].p))); // os1-amp
        pixel_buf[36] = pixel_fade_lfo(mo_freq, (*(ccs[64].p))); // os1-timb
        pixel_buf[37] = pixel_fade_lfo(mo_freq, (*(ccs[63].p))); // os1-idx
        pixel_buf[38] = pixel_fade_lfo(mo_freq, (*(ccs[62].p))); // os1-bus
        pixel_buf[39] = pixel_fade_lfo(mo_freq, (*(ccs[61].p))); // os1-os2
        pixel_buf[40] = pixel_fade_lfo(mo_freq, (*(ccs[60].p))); // os1-os1
        pixel_buf[41] = pixel_fade_lfo(mo_freq, (*(ccs[59].p))); // os1-rel
        pixel_buf[42] = pixel_fade_lfo(mo_freq, (*(ccs[58].p))); // os1-att
        pixel_buf[43] = pixel_fade_lfo(mo_freq, (*(ccs[57].p))); // os1-smth
        pixel_buf[44] = pixel_fade_lfo(mo_freq, (*(ccs[56].p))); // os1-rcl
        pixel_buf[45] = pixel_fade_lfo(mo_freq, (*(ccs[55].p))); // os1-rate
        pixel_buf[46] = pixel_fade_lfo(mo_freq, (*(ccs[54].p))); // os1-len
    } else {
        pixel_buf[35] = pixel_fade(mo * (*(ccs[65].p))); // os1-amp
        pixel_buf[36] = pixel_fade(mo * (*(ccs[64].p))); // os1-timb
        pixel_buf[37] = pixel_fade(mo * (*(ccs[63].p))); // os1-idx
        pixel_buf[38] = pixel_fade(mo * (*(ccs[62].p))); // os1-bus
        pixel_buf[39] = pixel_fade(mo * (*(ccs[61].p))); // os1-os2
        pixel_buf[40] = pixel_fade(mo * (*(ccs[60].p))); // os1-os1
        pixel_buf[41] = pixel_fade(mo * (*(ccs[59].p))); // os1-rel
        pixel_buf[42] = pixel_fade(mo * (*(ccs[58].p))); // os1-att
        pixel_buf[43] = pixel_fade(mo * (*(ccs[57].p))); // os1-smth
        pixel_buf[44] = pixel_fade(mo * (*(ccs[56].p))); // os1-rcl
        pixel_buf[45] = pixel_fade(mo * (*(ccs[55].p))); // os1-rate
        pixel_buf[46] = pixel_fade(mo * (*(ccs[54].p))); // os1-len
    }

    pixel_buf[47] = *(altSels[19].p) == 1 ? pixel_on : pixel_off; // amp-vca
    pixel_buf[48] = *(altSels[19].p) == 0 ? pixel_on : pixel_off; // amp-lpg
    pixel_buf[49] = pixel_fade(*sigs->amp); // AMP
    pixel_buf[50] = *(sels[17].p) == 1 || *(sels[17].p) == 2 ? pixel_on : pixel_off; // os2-tri
    pixel_buf[51] = *(sels[17].p) == 0 || *(sels[17].p) == 2 ? pixel_on : pixel_off; // os2-sin
    pixel_buf[52] = *(altSels[8].p) == 0 ? pixel_on : pixel_off; // bus-am
    pixel_buf[53] = *(altSels[8].p) == 1 ? pixel_on : pixel_off; // bus-fm
    pixel_buf[54] = *(altSels[8].p) == 2 ? pixel_on : pixel_off; // bus-tm
    pixel_buf[55] = *(sels[6].p) == 0 ? pixel_on : pixel_off; // os1-tri
    pixel_buf[56] = *(sels[6].p) == 1 ? pixel_on : pixel_off; // os1-sqr
    pixel_buf[57] = *(sels[6].p) == 2 ? pixel_on : pixel_off; // os1-saw
    pixel_buf[58] = os1_hi ? pixel_fade_lfo(mo_freq, 1) : pixel_fade(mo); // OS1
    pixel_buf[59] = pixel_fade(sg); // SG
    pixel_buf[60] = pixel_fade(*(sigs->le)); // ENV
    pixel_buf[61] = *(sels[12].p) == 2 ? pixel_on : pixel_off; // env-arl
    pixel_buf[62] = *(sels[12].p) == 1 ? pixel_on : pixel_off; // env-asr
    pixel_buf[63] = *(sels[12].p) == 0 ? pixel_on : pixel_off; // env-ar
    pixel_buf[64] = *(sels[11].p) == 0 ? pixel_on : pixel_off; // gate-int/midi
    pixel_buf[65] = *(sels[11].p) == 1 ? pixel_on : pixel_off; // gate-int/sg
    pixel_buf[66] = *(sels[11].p) == 2 ? pixel_on : pixel_off; // gate-env-midi

    // tint row
    // int start = matrix_row_selected == 0 ? 35 : matrix_row_selected == 1 ? 23 : 11; 
    // if(matrix_row_selected != -1)
    // {
    //     for(int i=0; i<12; i++)
    //     {
    //         pixel_buf[start + i].red++;
    //         pixel_buf[start + i].green++;
    //     }
    // }

    // tint column
    // if(matrix_column_selected != -1)
    // {
    //     int led_1 = 11 + (11 - matrix_column_selected);
    //     int led_2 = 11 + matrix_column_selected + 12;
    //     int led_3 = 11 + (11 - matrix_column_selected) + 24;
    //     pixel_buf[led_1].red++;
    //     pixel_buf[led_1].green++;
    //     pixel_buf[led_2].red++;
    //     pixel_buf[led_2].green++;
    //     pixel_buf[led_3].red++;
    //     pixel_buf[led_3].green++;
    // }
}