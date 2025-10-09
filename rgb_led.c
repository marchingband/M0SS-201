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
    // led_disp_val_p = val;
    // led_disp_min = min;
    // led_disp_max = max;
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
    struct Sel *sels = get_sels();
    struct Mod *mods = get_mods();
    struct Sigs *sigs = get_signals();
    struct AltSel *altSels = get_alt_sels();
    struct Config *config = get_config();

    pixel_buf[0] =  led_display[0]  == 1 ? pixel_on : led_display[0]  == 0 ? pixel_off : pixel_fade(led_display[0]);  // 0
    pixel_buf[1] =  led_display[1]  == 1 ? pixel_on : led_display[1]  == 0 ? pixel_off : pixel_fade(led_display[1]);  // 1
    pixel_buf[2] =  led_display[2]  == 1 ? pixel_on : led_display[2]  == 0 ? pixel_off : pixel_fade(led_display[2]);  // 2
    pixel_buf[3] =  led_display[3]  == 1 ? pixel_on : led_display[3]  == 0 ? pixel_off : pixel_fade(led_display[3]);  // 3
    pixel_buf[4] =  led_display[4]  == 1 ? pixel_on : led_display[4]  == 0 ? pixel_off : pixel_fade(led_display[4]);  // 4
    pixel_buf[5] =  led_display[5]  == 1 ? pixel_on : led_display[5]  == 0 ? pixel_off : pixel_fade(led_display[5]);  // 5
    pixel_buf[6] =  led_display[6]  == 1 ? pixel_on : led_display[6]  == 0 ? pixel_off : pixel_fade(led_display[6]);  // 6
    pixel_buf[7] =  led_display[7]  == 1 ? pixel_on : led_display[7]  == 0 ? pixel_off : pixel_fade(led_display[7]);  // 7
    pixel_buf[8] =  led_display[8]  == 1 ? pixel_on : led_display[8]  == 0 ? pixel_off : pixel_fade(led_display[8]);  // 8
    pixel_buf[9] =  led_display[9]  == 1 ? pixel_on : led_display[9]  == 0 ? pixel_off : pixel_fade(led_display[9]);  // 9
    pixel_buf[10] = led_display[10] == 1 ? pixel_on : led_display[10] == 0 ? pixel_off : pixel_fade(led_display[10]); // 10

    pixel_buf[11] = 0; // env-amp
    pixel_buf[12] = 0; // env-timb
    pixel_buf[13] = 0; // env-idx
    pixel_buf[14] = 0; // env-bus
    pixel_buf[15] = 0; // env-os2
    pixel_buf[16] = 0; // env-os1
    pixel_buf[17] = 0; // env-rel
    pixel_buf[18] = 0; // env-att
    pixel_buf[19] = 0; // env-smth
    pixel_buf[20] = 0; // env-rcl
    pixel_buf[21] = 0; // env-rate
    pixel_buf[22] = 0; // env-len
    pixel_buf[23] = 0; // sg-len
    pixel_buf[24] = 0; // sg-rate
    pixel_buf[25] = 0; // sg-rcl
    pixel_buf[26] = 0; // sg-smth
    pixel_buf[27] = 0; // sg-att
    pixel_buf[28] = 0; // sg-rel
    pixel_buf[29] = 0; // sg-os1
    pixel_buf[30] = 0; // sg-os2
    pixel_buf[31] = 0; // sg-bus
    pixel_buf[32] = 0; // sg-idx
    pixel_buf[33] = 0; // sg-timb
    pixel_buf[34] = 0; // sg-amp
    pixel_buf[35] = 0; // os1-amp
    pixel_buf[36] = 0; // os1-timb
    pixel_buf[37] = 0; // os1-idx
    pixel_buf[38] = 0; // os1-bus
    pixel_buf[39] = 0; // os1-os2
    pixel_buf[40] = 0; // os1-os1
    pixel_buf[41] = 0; // os1-rel
    pixel_buf[42] = 0; // os1-att
    pixel_buf[43] = 0; // os1-smth
    pixel_buf[44] = 0; // os1-rcl
    pixel_buf[45] = 0; // os1-rate
    pixel_buf[46] = 0; // os1-len

    pixel_buf[47] = altSels[19] == 1 ? pixel_on : pixel_off; // amp-vca
    pixel_buf[48] = altSels[19] == 0 ? pixel_on : pixel_off; // amp-lpg
    pixel_buf[49] = 0; // AMP
    pixel_buf[50] = sels[17] == 1 || sels[17] == 2 ? pixel_on : pixel_off; // os2-tri
    pixel_buf[51] = sels[17] == 0 || sels[17] == 2 ? pixel_on : pixel_off; // os2-sin
    pixel_buf[52] = altSels[8] == 0 ? pixel_on : pixel_off; // bus-am
    pixel_buf[53] = altSels[8] == 1 ? pixel_on : pixel_off; // bus-fm
    pixel_buf[54] = altSels[8] == 2 ? pixel_on : pixel_off; // bus-tm
    pixel_buf[55] = sels[6] == 0 ? pixel_on : pixel_off; // os1-tri
    pixel_buf[56] = sels[6] == 1 ? pixel_on : pixel_off; // os1-sqr
    pixel_buf[57] = sels[6] == 2 ? pixel_on : pixel_off; // os1-saw
    pixel_buf[58] = 0; // OS1
    pixel_buf[59] = 0; // SG
    pixel_buf[60] = 0; // ENV
    pixel_buf[61] = sels[12] == 2 ? pixel_on : pixel_off; // env-arl
    pixel_buf[62] = sels[12] == 1 ? pixel_on : pixel_off; // env-asr
    pixel_buf[63] = sels[12] == 0 ? pixel_on : pixel_off; // env-ar
    pixel_buf[64] = sels[11] == 0 ? pixel_on : pixel_off; // gate-int/midi
    pixel_buf[65] = sels[11] == 1 ? pixel_on : pixel_off; // gate-int/sg
    pixel_buf[66] = sels[11] == 2 ? pixel_on : pixel_off; // gate-env-midi
}

// TEST 201 (also modify spi_rgb.c for larger xfer)

// static ATTR_NOCACHE_NOINIT_RAM_SECTION uint32_t test_data_buf[256] __ALIGNED(4);
// #define NUM 67

// void test_lights(void){
//     struct pixel_t test_pixel_buf[NUM];
//     for (int i = 0; i < NUM; i++) {
//         test_pixel_buf[i] = pixel_on;
//         int loc = i * PIXEL_SIZE;

//         test_data_buf[loc]     = (bit_patterns[test_pixel_buf[i].green >> 6 & 0x03]  << 24) | 
//                         (bit_patterns[test_pixel_buf[i].green >> 4 & 0x03] << 16) | 
//                         (bit_patterns[test_pixel_buf[i].green >> 2 & 0x03] << 8)  | 
//                          bit_patterns[test_pixel_buf[i].green & 0x03];

//         test_data_buf[loc + 1] = (bit_patterns[test_pixel_buf[i].red >> 6 & 0x03]    << 24) | 
//                         (bit_patterns[test_pixel_buf[i].red >> 4 & 0x03]   << 16) |
//                         (bit_patterns[test_pixel_buf[i].red   >> 2 & 0x03] << 8)  | 
//                          bit_patterns[test_pixel_buf[i].red & 0x03];
                         
//         test_data_buf[loc + 2] = (bit_patterns[test_pixel_buf[i].blue >> 6 & 0x03]   << 24) | 
//                         (bit_patterns[test_pixel_buf[i].blue >> 4 & 0x03]  << 16) |
//                         (bit_patterns[test_pixel_buf[i].blue  >> 2 & 0x03] << 8)  |
//                          bit_patterns[test_pixel_buf[i].blue & 0x03];
//     }
//     spi_rgb_send(test_data_buf, 256);
// }