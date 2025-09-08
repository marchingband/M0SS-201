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
#define NUM_LED 37

void test_lights(void);

extern int learn_mode_state;
extern int alt_switch_state;
int cur_disp_num = 0;

static ATTR_NOCACHE_NOINIT_RAM_SECTION uint32_t data_buf[128] __ALIGNED(4);
static const uint16_t bit_patterns[4] = {0x88, 0x8e, 0xe8, 0xee}; // original
struct pixel_t pixel_buf[NUM_LED] = {0};

void encode_rgb(uint32_t *buf){
    // for (int i = 0; i < NUM_LED; i++) {
    //     int loc = i * PIXEL_SIZE;

    //     buf[loc]     = (bit_patterns[pixel_buf[i].green >> 6 & 0x03]  << 24) | 
    //                     (bit_patterns[pixel_buf[i].green >> 4 & 0x03] << 16) | 
    //                     (bit_patterns[pixel_buf[i].green >> 2 & 0x03] << 8)  | 
    //                      bit_patterns[pixel_buf[i].green & 0x03];

    //     buf[loc + 1] = (bit_patterns[pixel_buf[i].red >> 6 & 0x03]    << 24) | 
    //                     (bit_patterns[pixel_buf[i].red >> 4 & 0x03]   << 16) |
    //                     (bit_patterns[pixel_buf[i].red   >> 2 & 0x03] << 8)  | 
    //                      bit_patterns[pixel_buf[i].red & 0x03];
                         
    //     buf[loc + 2] = (bit_patterns[pixel_buf[i].blue >> 6 & 0x03]   << 24) | 
    //                     (bit_patterns[pixel_buf[i].blue >> 4 & 0x03]  << 16) |
    //                     (bit_patterns[pixel_buf[i].blue  >> 2 & 0x03] << 8)  |
    //                      bit_patterns[pixel_buf[i].blue & 0x03];
    // }
}

static uint64_t times[100] = {0};
static size_t time_p = 0;

void update_leds(void)
{
    // // test_lights(); // 201
    // render_lights();
    // encode_rgb(data_buf);
    // spi_rgb_send(data_buf, 128);
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
    // .red = 5,
    // .green = 30,
    // .blue = 0
    // .red = 2,
    // .green = 1,
    // .blue = 0
};

const struct pixel_t pixel_off = {
    .red = 0,
    .green = 0,
    .blue = 0
};

const struct pixel_t p1 = {
    .red = 1,
    .green = 1,
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
    // bool some = val > 0;
    // uint8_t green = (uint8_t)(20.0 * val * temp);
    // uint8_t red = (uint8_t)(80.0 * val);
    // uint8_t blue = (uint8_t)(20.0 * val * temp);
    // struct pixel_t pixel = {0};
    // if(some){
    //     pixel.green = green > 0 ? green : 1;
    //     pixel.red = red > 0 ? red : 1;
    //     pixel.blue = blue > 0 ? blue : 1;
    // }
    // return pixel;
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
    // if(led_disp_val_p == NULL){
    //     return;
    // }

    // float val = *led_disp_val_p;
    // float max = led_disp_max;

    // val -= led_disp_min;
    // max -= led_disp_min;
    // float out = val / max * 10;
    // for(int i=0; i<11; i++)
    // {
    //     if(out > (i + 1))
    //     {
    //         led_display[i] = 1;
    //     }
    //     else if(out < i)
    //     {
    //         led_display[i] = 0;
    //     }
    //     else
    //     {
    //         led_display[i] = out - i; // just the fractional part
    //     }
    // }
}

void render_lights(void){

    // int current_slot = get_current_slot();
    // struct Sel *sels = get_sels();
    // struct Mod *mods = get_mods();
    // struct Sigs *sigs = get_signals();
    // struct AltSel *altSels = get_alt_sels();
    // struct Config *config = get_config();

    // switch(ui_mode){
    //     case UI_MODE_PLAY:
    //         pixel_buf[0] = ((*(sels[0].p) == 0) || (*(sels[0].p) == 2)) ? pixel_on : pixel_off; // o1sqr
    //         pixel_buf[1] = ((*(sels[0].p) == 1) || (*(sels[0].p) == 2)) ? pixel_on : pixel_off;  // o1saw

    //         pixel_buf[2] = *(sels[1].p)   == 0 ? pixel_on : pixel_off;  // o1oct0
    //         pixel_buf[3] = *(sels[1].p)   == 1 ? pixel_on : pixel_off;  // o1oct-1
    //         pixel_buf[4] = *(sels[1].p)   == 2 ? pixel_on : pixel_off;  // o1oct-2

    //         pixel_buf[7] = *(sels[2].p)   == 0 ? pixel_on : pixel_off;  // o2oct0
    //         pixel_buf[8] = *(sels[2].p)   == 1 ? pixel_on : pixel_off;  // o2oct-1
    //         pixel_buf[9] = *(sels[2].p)   == 2 ? pixel_on : pixel_off;  // o2oct-2

    //         pixel_buf[6] = ((*(sels[3].p) == 0) || (*(sels[3].p) == 2)) ? pixel_on : pixel_off;  // o2sqr
    //         pixel_buf[5] = ((*(sels[3].p) == 1) || (*(sels[3].p) == 2)) ? pixel_on : pixel_off;  // o2saw


    //         pixel_buf[10] = *(sels[7].p)  == 0 ? pixel_on : pixel_off; // lfotri
    //         pixel_buf[11] = *(sels[7].p)  == 1 ? pixel_on : pixel_off; // lfosqr
    //         pixel_buf[12] = *(sels[7].p)  == 2 ? pixel_on : pixel_off; // lfornd
    //         pixel_buf[14] = *(sels[9].p)  == 1 ? pixel_on : pixel_off; // lfornd

    //         pixel_buf[23] = *(sels[13].p)  == 0 ? pixel_on : pixel_off; // vcfhp
    //         pixel_buf[24] = *(sels[13].p)  == 1 ? pixel_on : pixel_off; // vcflp
    //         pixel_buf[25] = *(sels[13].p)  == 2 ? pixel_on : pixel_off; // vcfbp

    //         // pixel_buf[16] = *(sels[11].p) == 0 ? pixel_on : pixel_off; // envasr
    //         // pixel_buf[17] = *(sels[11].p) == 1 ? pixel_on : pixel_off; // envar

    //         if(*(altSels[12].p) == 0){
    //             // low lfo range
    //             pixel_buf[17] = pixel_fade((*sigs->lfo) * (*mods[6].p));     // lfo.pwm
    //             pixel_buf[15] = pixel_fade((*sigs->lfo) * (*mods[10].p));    // lfo.osc
    //             pixel_buf[16] = pixel_fade((*sigs->lfo) * (*mods[11].p));    // lfo.vcf
    //             pixel_buf[13] = pixel_fade((*sigs->lfo) * (*mods[8].p));     // lfo.vca
    //             pixel_buf[18] = pixel_fade(*sigs->vca);                     // vca
    //         } else {
    //             // high lfo range
    //             pixel_buf[17] = pixel_fade_lfo((*mods[12].p), (*mods[6].p));     // lfo.pwm
    //             pixel_buf[15] = pixel_fade_lfo((*mods[12].p), (*mods[10].p));    // lfo.osc
    //             pixel_buf[16] = pixel_fade_lfo((*mods[12].p), (*mods[11].p));    // lfo.vcf
    //             pixel_buf[13] = pixel_fade_lfo((*mods[12].p), (*mods[8].p));     // lfo.vca
    //             pixel_buf[18] = pixel_fade_lfo((*mods[12].p), (*sigs->env));                     // vca
    //         }

    //         pixel_buf[20] = pixel_fade((*(sigs->env)) * (*mods[18].p)); // env.vcf
    //         pixel_buf[19] = pixel_fade((*(sigs->env)) * (*mods[15].p)); // env.vca
    //         pixel_buf[21] = pixel_fade((*(sigs->env)) * (*mods[20].p)); // env.osc
    //         pixel_buf[22] = pixel_fade((*(sigs->env)) * (*mods[24].p)); // env.pwm

    //         if(alt_switch_state == ALT_SWITCH_STATE_NONE){
    //             update_led_display();
    //             pixel_buf[36] = pixel_on;
    //             pixel_buf[35] = led_display[0] == 1 ? pixel_on : led_display[0] == 0 ? pixel_off : pixel_fade(led_display[0]);
    //             pixel_buf[34] = led_display[1] == 1 ? pixel_on : led_display[1] == 0 ? pixel_off : pixel_fade(led_display[1]); 
    //             pixel_buf[33] = led_display[2] == 1 ? pixel_on : led_display[2] == 0 ? pixel_off : pixel_fade(led_display[2]); 
    //             pixel_buf[32] = led_display[3] == 1 ? pixel_on : led_display[3] == 0 ? pixel_off : pixel_fade(led_display[3]); 
    //             pixel_buf[31] = led_display[4] == 1 ? pixel_on : led_display[4] == 0 ? pixel_off : pixel_fade(led_display[4]); 
    //             pixel_buf[30] = led_display[5] == 1 ? pixel_on : led_display[5] == 0 ? pixel_off : pixel_fade(led_display[5]); 
    //             pixel_buf[29] = led_display[6] == 1 ? pixel_on : led_display[6] == 0 ? pixel_off : pixel_fade(led_display[6]); 
    //             pixel_buf[28] = led_display[7] == 1 ? pixel_on : led_display[7] == 0 ? pixel_off : pixel_fade(led_display[7]); 
    //             pixel_buf[27] = led_display[8] == 1 ? pixel_on : led_display[8] == 0 ? pixel_off : pixel_fade(led_display[8]); 
    //             pixel_buf[26] = led_display[9] == 1 ? pixel_on : led_display[9] == 0 ? pixel_off : pixel_fade(led_display[9]); 
    //         } else if(alt_switch_state == ALT_SWITCH_STATE_ON){
    //             pixel_buf[36] = pixel_off;
    //             pixel_buf[35] = pixel_off;
    //             pixel_buf[34] = pixel_off; 
    //             pixel_buf[33] = pixel_off; 
    //             pixel_buf[32] = pixel_off; 
    //             pixel_buf[31] = pixel_off; 
    //             pixel_buf[30] = pixel_on; 
    //             pixel_buf[29] = pixel_on; 
    //             pixel_buf[28] = pixel_on; 
    //             pixel_buf[27] = pixel_on; 
    //             pixel_buf[26] = pixel_on; 
    //         } else {
    //             pixel_buf[36] = pixel_on;
    //             pixel_buf[35] = pixel_on;
    //             pixel_buf[34] = pixel_on; 
    //             pixel_buf[33] = pixel_on; 
    //             pixel_buf[32] = pixel_on; 
    //             pixel_buf[31] = pixel_on; 
    //             pixel_buf[30] = pixel_off; 
    //             pixel_buf[29] = pixel_off; 
    //             pixel_buf[28] = pixel_off; 
    //             pixel_buf[27] = pixel_off; 
    //             pixel_buf[26] = pixel_off; 

    //         }

    //         break;
    //     case UI_MODE_SAVE:
    //     {
    //         for(int i=0; i<NUM_LED; i++)
    //         {
    //             pixel_buf[i] = pixel_off; 
    //         }

    //         for(int i=0; i<NUM_PRESETS; i++)
    //         {
    //             uint8_t light_num = number_select_lights[i];
    //             if(i == current_slot)
    //             {
    //                 pixel_buf[light_num] = light_blue;
    //             }
    //             else if(config->empty[i])
    //             {
    //                 pixel_buf[light_num] = light_green;
    //             }
    //             else
    //             {
    //                 pixel_buf[light_num] = light_pink;
    //             }
    //         }
    //         break;
    //     }
    //     case UI_MODE_LOAD:
    //     {
    //         for(int i=0; i<NUM_LED; i++)
    //         {
    //             pixel_buf[i] = pixel_off; 
    //         }

    //         for(int i=0; i<NUM_PRESETS; i++)
    //         {
    //             uint8_t light_num = number_select_lights[i];
    //             if(i == current_slot)
    //             {
    //                 pixel_buf[light_num] = light_blue;
    //             }
    //             else if(!config->empty[i])
    //             {
    //                 pixel_buf[light_num] = light_green;
    //             }
    //         }
    //         break;
    //     }
    //     case UI_MODE_LEARN:
    //     {
    //         for(int i=0; i<NUM_LED; i++)
    //         {
    //             pixel_buf[i] = learn_mode_state == LEARN_MODE_READY ? light_pink : 
    //                         learn_mode_state == LEARN_MODE_SELECTED ? light_green : 
    //                         light_blue; 
    //         }
    //         break;
    //     }
    //     case UI_MODE_DISP_NUM:
    //     {
    //         for(int i=0; i<NUM_LED; i++)
    //         {
    //             pixel_buf[i] = pixel_off; 
    //         }

    //         for(int i=0; i<NUM_PRESETS; i++)
    //         {
    //             uint8_t light_num = number_select_lights[i];
    //             if(i + 1 == cur_disp_num) // i==0 is actually number 1
    //             {
    //                 pixel_buf[light_num] = light_green;
    //             }
    //         }
    //         break;
    //     }
    //     default: break;
    // }
}

// TEST 201 (also modify spi_rgb.c for larger xfer)

static ATTR_NOCACHE_NOINIT_RAM_SECTION uint32_t test_data_buf[256] __ALIGNED(4);
#define NUM 67

void test_lights(void){
    struct pixel_t test_pixel_buf[NUM];
    for (int i = 0; i < NUM; i++) {
        test_pixel_buf[i] = pixel_on;
        int loc = i * PIXEL_SIZE;

        test_data_buf[loc]     = (bit_patterns[test_pixel_buf[i].green >> 6 & 0x03]  << 24) | 
                        (bit_patterns[test_pixel_buf[i].green >> 4 & 0x03] << 16) | 
                        (bit_patterns[test_pixel_buf[i].green >> 2 & 0x03] << 8)  | 
                         bit_patterns[test_pixel_buf[i].green & 0x03];

        test_data_buf[loc + 1] = (bit_patterns[test_pixel_buf[i].red >> 6 & 0x03]    << 24) | 
                        (bit_patterns[test_pixel_buf[i].red >> 4 & 0x03]   << 16) |
                        (bit_patterns[test_pixel_buf[i].red   >> 2 & 0x03] << 8)  | 
                         bit_patterns[test_pixel_buf[i].red & 0x03];
                         
        test_data_buf[loc + 2] = (bit_patterns[test_pixel_buf[i].blue >> 6 & 0x03]   << 24) | 
                        (bit_patterns[test_pixel_buf[i].blue >> 4 & 0x03]  << 16) |
                        (bit_patterns[test_pixel_buf[i].blue  >> 2 & 0x03] << 8)  |
                         bit_patterns[test_pixel_buf[i].blue & 0x03];
    }
    spi_rgb_send(test_data_buf, 256);
}