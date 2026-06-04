#include "bflb_i2s.h"
#include "bflb_gpio.h"
#include "bflb_dma.h"
#include "bl616_glb_gpio.h"
#include "bflb_l1c.h"
#include "bl616_irq.h"
#include "bflb_irq.h"
#include "bflb_mtimer.h"
#include "board.h"
#include "rgb_led.h"
#include "midi.h"
#include "flash.h"
#include "spi_rgb.h"
#include "gpio.h"

extern const int param_matrix[3][10];

const uint8_t display_bar_leds[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
#define MAT_NUM_ROWS 3
#define MAT_NUM_COLS 10
const uint8_t matrix_leds[MAT_NUM_ROWS][MAT_NUM_COLS] = {
    {40, 39, 38, 37, 36, 35, 34, 33, 32, 31},
    {21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
    {20, 19, 18, 17, 16, 15, 14, 13, 12, 11}
};

// numbers displayed on matrix take up a 3x3 grid
const uint16_t matrix_number_bit_patterns[10] = {0xF780, 0x4900, 0xE780, 0xEF80, 0xBC80, 0xF380, 0x9F80, 0xE480, 0xFF80, 0xFC80};

#define PIXEL_SIZE 3 // each colour takes 4 bytes
#define NUM_LED 64
#define FLASH_INTERVAL (ONE_SECOND / 6)

void test_lights(void);

extern int learn_mode_state;
extern int alt_switch_state;
extern uint64_t reset_mode_started;
extern int save_load_cursor_pos;
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

void matrix_display_number(uint8_t number) {
    number %= 100;  // can only display two digits
    uint8_t ones_place = number % 10;
    uint8_t tens_place = (number - ones_place) / 10;
    for (uint i = 0; i < MAT_NUM_ROWS; i++) {
        for (uint j = 0; j < MAT_NUM_COLS; j++) {
            pixel_buf[matrix_leds[i][j]] = pixel_off;
            
            // tens place digit
            if (j >= 1 && j <= 3) {
                uint8_t shift = (j - 1) + i*3;
                uint16_t mask = 0x8000 >> shift;
                if (matrix_number_bit_patterns[tens_place] & mask) {
                    pixel_buf[matrix_leds[i][j]] = pixel_on;
                }
            }

            // ones place digit
            if (j >= 6 && j <= 8) {
                uint8_t shift = (j - 6) + i*3;
                uint16_t mask = 0x8000 >> shift;
                if (matrix_number_bit_patterns[ones_place] & mask) {
                    pixel_buf[matrix_leds[i][j]] = pixel_on;
                }
            }
        }
    }
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
static bool led_selection_is_numeric;

void set_led_display(float *val, float min, float max, bool numeric){
    led_disp_val_p = val;
    led_disp_min = min;
    led_disp_max = max;
    led_selection_is_numeric = numeric;
}

void update_led_display(void){
    if(led_disp_val_p == NULL){
        return;
    }

    float val = *led_disp_val_p;
    float max = led_disp_max;

    if (!led_selection_is_numeric) {
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
    } else {
        matrix_display_number((uint8_t)val);
        for (uint i = 0; i < 11; i++) {
            led_display[i] = 0.0f;
        }
    }

}

void render_lights(void){
    // for (uint i = 0; i < NUM_LED; i++) {
    //     bool offset_led = i <= 20 && i >= 11;
    //     offset_led = offset_led || (i <= 40 && i >= 31);
    //     offset_led = offset_led || (i == 55);
    //     bool extra_dim = (i <= 40 && i >= 38) || (i <= 20 && i >= 11);
    //     bool matrix_led = i <= 40;

    //     float brightness = 0;
    //     if (matrix_led) {
    //         if (offset_led) {
    //             if (extra_dim) {
    //                 brightness = 0.4;
    //             } else {
    //                 brightness = 0.2;
    //             }
    //         } else {
    //             brightness = 0.1;
    //         }
    //     } else {
    //         brightness = 0.08;
    //     }
    //     pixel_buf[i] = pixel_fade(brightness);
    // }
    // bflb_mtimer_delay_ms(100);
    // return;

    int current_slot = get_current_slot();
    struct CC *ccs = get_ccs();
    struct Sel *sels = get_sels();
    struct Mod *mods = get_mods();
    struct Sigs *sigs = get_signals();
    struct AltSel *altSels = get_alt_sels();
    struct Config *config = get_config();

    bool os1_hi = *sels[4].p == 0;

    float sg = *sigs->sg;
    float mo = (*sigs->mo + 1) * 0.5;
    // float mo_freq = *sigs->mo_freq / 3000; // the mosc range is about 3000
    float mo_freq = (*mods[17].p + 2) * 0.25;// posc range is [-2..2]

    struct pixel_t  display_toggle_on[11] = {pixel_off, pixel_off, pixel_off, pixel_off, pixel_off, pixel_on, pixel_on, pixel_on, pixel_on, pixel_on, pixel_on};
    struct pixel_t display_toggle_off[11] = {pixel_on, pixel_on, pixel_on, pixel_on, pixel_on, pixel_on, pixel_off, pixel_off, pixel_off, pixel_off, pixel_off};

    switch (ui_mode) {
        case UI_MODE_PLAY: {
             // number led bar
            if(alt_switch_state == ALT_SWITCH_STATE_NONE){
                update_led_display();
                for (uint i = 0; i < 11; i++) {
                    struct pixel_t colour;

                    if (led_display[i] == 0) colour = pixel_off;
                    else if (led_display[i] == 1) colour = pixel_on;
                    else colour = pixel_fade(led_display[i]);

                    pixel_buf[i] = colour;
                }
            } else if(alt_switch_state == ALT_SWITCH_STATE_ON) {
                for (uint i = 0; i < 11; i++) pixel_buf[i] = display_toggle_on[i];
            } else {
                for (uint i = 0; i < 11; i++) pixel_buf[i] = display_toggle_off[i];
            }

            // matrix
            if (!led_selection_is_numeric) {
                for (uint i = 0; i < MAT_NUM_ROWS; i++) {
                    for (uint j = 0; j < MAT_NUM_COLS; j++) {
                        size_t pixel_idx = matrix_leds[i][j];
                        size_t cc_num = param_matrix[i][j];
                        pixel_buf[pixel_idx] = pixel_fade(*(ccs[cc_num].p));
                    }
                }
            }
        

            pixel_buf[62] = *(sels[2].p) == 0 ? pixel_on : pixel_off; // amp-lpg
            pixel_buf[63] = *(sels[2].p) == 1 ? pixel_on : pixel_off; // amp-vca
            pixel_buf[61] = pixel_fade(*sigs->amp); // AMP
            pixel_buf[41] = *(sels[22].p) == 1 || *(sels[22].p) == 2 ? pixel_on : pixel_off; // os1-tri
            pixel_buf[42] = *(sels[22].p) == 0 || *(sels[22].p) == 2 ? pixel_on : pixel_off; // os1-sin
            pixel_buf[46] = *(sels[23].p) == 0 ? pixel_on : pixel_off; // bus-am
            pixel_buf[47] = *(sels[23].p) == 1 ? pixel_on : pixel_off; // bus-fm
            pixel_buf[48] = *(sels[23].p) == 2 ? pixel_on : pixel_off; // bus-tm
            pixel_buf[49] = *(sels[7].p) == 0 ? pixel_on : pixel_off; // os2-tri
            pixel_buf[50] = *(sels[7].p) == 1 ? pixel_on : pixel_off; // os2-sqr
            pixel_buf[51] = *(sels[7].p) == 2 ? pixel_on : pixel_off; // os2-saw
            pixel_buf[54] = os1_hi ? pixel_fade_lfo(mo_freq, 1) : pixel_fade(mo); // OS1
            pixel_buf[53] = pixel_fade(sg); // SG
            pixel_buf[52] = pixel_fade(*(sigs->le)); // ENV
            pixel_buf[55] = *(sels[1].p) == 0 ? pixel_on : pixel_off; // env-ar
            pixel_buf[56] = *(sels[1].p) == 1 ? pixel_on : pixel_off; // env-asr
            pixel_buf[57] = *(sels[1].p) == 2 ? pixel_on : pixel_off; // env-arl
            pixel_buf[58] = *(sels[0].p) == 0 ? pixel_on : pixel_off; // gate-int/midi
            pixel_buf[59] = *(sels[0].p) == 1 ? pixel_on : pixel_off; // gate-int/sg
            pixel_buf[60] = *(sels[0].p) == 2 ? pixel_on : pixel_off; // gate-env-midi
            pixel_buf[43] = *(sels[16].p) == 0 ? pixel_on : pixel_off; // os1-oct 0
            pixel_buf[44] = *(sels[16].p) == 1 ? pixel_on : pixel_off; // os1-oct -1
            pixel_buf[45] = *(sels[16].p) == 2 ? pixel_on : pixel_off; // os2-oct -2
            break;
        }
        case UI_MODE_LEARN: {
            for(int i=0; i<NUM_LED; i++)
            {
                pixel_buf[i] = learn_mode_state == LEARN_MODE_READY ? light_pink : 
                            learn_mode_state == LEARN_MODE_SELECTED ? light_green : 
                            light_blue; 
            }
            break;
        }
        case UI_MODE_LOAD: {
            for (int i = 0; i < NUM_LED; i++) {
                pixel_buf[i] = pixel_off;
            }

            for (int i = LED_NUM_MATRIX_START; i < LED_NUM_MATRIX_END; i++) {
                struct pixel_t pixel_value = pixel_off;
                int current_slot_num = i - LED_NUM_MATRIX_START;
                int slot_is_empty = get_config()->empty[current_slot_num];

                // blue for current slot, green for non-empty slot, red for cursor slot
                if (!slot_is_empty) pixel_value = light_green;
                if (current_slot_num == get_current_slot()) pixel_value = light_blue;
                if (current_slot_num == save_load_cursor_pos) pixel_value = pixel_red;

                pixel_buf[i] = pixel_value;
            }
            break;
        }
        case UI_MODE_SAVE: {
            for (int i = 0; i < NUM_LED; i++) {
                pixel_buf[i] = pixel_off;
            }

            for (int i = LED_NUM_MATRIX_START; i < LED_NUM_MATRIX_END; i++) {
                struct pixel_t pixel_value = pixel_off;
                int current_slot_num = i - LED_NUM_MATRIX_START;
                int slot_is_empty = get_config()->empty[current_slot_num];

                // blue for current slot, green for non-empty slot, red for cursor slot
                pixel_value = slot_is_empty? light_pink : light_green;
                if (current_slot_num == get_current_slot()) pixel_value = light_blue;
                if (current_slot_num == save_load_cursor_pos) pixel_value = pixel_red;

                pixel_buf[i] = pixel_value;
            }
            break;
        }
        case UI_MODE_RST: {
            uint64_t now = bflb_mtimer_get_time_us();
            uint64_t elapsed = now - reset_mode_started;
            uint8_t flash = elapsed / FLASH_INTERVAL;
            for(int i=0; i<NUM_LED; i++)
            {
                pixel_buf[i] = flash % 2 ? pixel_off : pixel_on; 
            }
            break;
        }
        case UI_MODE_FACTORY_RST:
        {
            uint64_t now = bflb_mtimer_get_time_us();
            uint64_t elapsed = now - reset_mode_started;
            uint8_t flash = elapsed / FLASH_INTERVAL;
            for(int i=0; i<NUM_LED; i++)
            {
                pixel_buf[i] = pixel_red; 
            }
            break;
        }
        default: break;
    }
   

    // // tint row
    // int start = matrix_row_selected == 0 ? 35 : matrix_row_selected == 1 ? 23 : 11; 
    // if(matrix_row_selected != -1)
    // {
    //     for(int i=0; i<12; i++)
    //     {
    //         pixel_buf[start + i].red++;
    //         pixel_buf[start + i].green++;
    //     }
    // }

    // // tint column
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