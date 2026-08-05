#ifndef RGB_LED_H
#define REG_LED_H

#define UI_MODE_PLAY      0
#define UI_MODE_LOAD      1
#define UI_MODE_SAVE      2
#define UI_MODE_LEARN     3
#define UI_MODE_DISP_NUM  4
#define UI_MODE_RST       5
#define UI_MODE_FACTORY_RST 6

#define LED_NUM_MATRIX_START 11
#define LED_NUM_MATRIX_END 41

#define LED_NUM_OS1_WAV_SIN 42
#define LED_NUM_OS1_WAV_TRI 41

#define LED_NUM_OS1_OCT_0 43
#define LED_NUM_OS1_OCT_1 44
#define LED_NUM_OS1_OCT_2 45

#define LED_NUM_MOD_AM 46
#define LED_NUM_MOD_FM 47
#define LED_NUM_MOD_TM 48

#define LED_NUM_OS2_WAV_TRI 49
#define LED_NUM_OS2_WAV_SQR 50
#define LED_NUM_OS2_WAV_SAW 51

#define LED_NUM_IND_ENV 52
#define LED_NUM_IND_SG 53
#define LED_NUM_IND_OS2 54

#define LED_NUM_ENV_MODE_AR 55
#define LED_NUM_ENV_MODE_ASR 56
#define LED_NUM_ENV_MODE_ARL 57
#define LED_NUM_GATES_SEPERATE 58
#define LED_NUM_GATES_SG 59
#define LED_NUM_GATES_ENV 60

#define LED_NUM_IND_OUT 61

#define LED_NUM_AMP_SEL_LPG 62
#define LED_NUM_AMP_SEL_VCA 63

struct pixel_t {
  uint8_t green;
  uint8_t red;
  uint8_t blue;
};

void rgb_led_init();
void rgb_set_color(struct pixel_t *pixels);
void render_lights(void);
void rgb_task(void *pvParameters);
void set_ui_mode(int mode);
int get_ui_mode(void);
void test_rgb_lights(void);
void rgb_set_color(struct pixel_t *pixels);
void set_led_display(float *val, float min, float max, bool numeric);
void update_led_display(void);
void update_leds(void);

#endif