#ifndef RGB_LED_H
#define REG_LED_H

#define UI_MODE_PLAY      0
#define UI_MODE_LOAD      1
#define UI_MODE_SAVE      2
#define UI_MODE_LEARN     3
#define UI_MODE_DISP_NUM  4

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
void set_led_display(float *val, float min, float max);
void update_led_display(void);
void update_leds(void);

#endif