#include <math.h>
#include <stdlib.h>

#include "board.h"

#include "bflb_audac.h"
#include "bflb_gpio.h"
#include "bflb_dma.h"
#include "bl616_irq.h"
#include "bl616_glb.h"
#include "bl616_glb_gpio.h"
#include "bflb_core.h"

#include "audac.h"
#include "dsp.h"
#include "rgb_led.h"
#include "encoder.h"

int min(int a, int b)
{
    return (a < b ? a : b);
}

int max(int a, int b)
{
    return (a > b ? a : b);
}


static uint16_t tic[BUF_SIZE];
static uint16_t toc[BUF_SIZE];

struct bflb_device_s *audac_dma_hd;
struct bflb_device_s *audac_hd;

uint32_t dma_lli_cnt_tic;
static struct bflb_dma_channel_lli_pool_s lli_pool_tic[10];
struct bflb_dma_channel_lli_transfer_s transfers_tic[1];

uint32_t dma_lli_cnt_toc;
static struct bflb_dma_channel_lli_pool_s lli_pool_toc[10];
struct bflb_dma_channel_lli_transfer_s transfers_toc[1];

static void compute(int tictoc)
{
    uint16_t *dest = tictoc == 0 ? tic : toc;
    dsp_run(dest);
}

static void refresh_dma(int tictoc)
{
    if (tictoc)
    {
        transfers_toc[0].src_addr = (uint32_t)toc;
        transfers_toc[0].dst_addr = (uint32_t)DMA_ADDR_AUDAC_TDR;
        transfers_toc[0].nbytes = sizeof(toc);
        bflb_l1c_dcache_clean_range(toc, sizeof(toc));
        dma_lli_cnt_toc = bflb_dma_channel_lli_reload(audac_dma_hd, lli_pool_toc, 10, transfers_toc, 1);
        bflb_dma_channel_start(audac_dma_hd);
    }
    else
    {
        transfers_tic[0].src_addr = (uint32_t)tic;
        transfers_tic[0].dst_addr = (uint32_t)DMA_ADDR_AUDAC_TDR;
        transfers_tic[0].nbytes = sizeof(tic);
        bflb_l1c_dcache_clean_range(tic, sizeof(tic));
        dma_lli_cnt_tic = bflb_dma_channel_lli_reload(audac_dma_hd, lli_pool_tic, 10, transfers_tic, 1);
        bflb_dma_channel_start(audac_dma_hd);
    }
    // update_leds();
}

static uint64_t times[100] = {0};
static size_t time_p = 0;

void audio_dma_callback(void *arg)
{

    // times[time_p++] = bflb_mtimer_get_time_us();

    static int tictoc = 1;
    process_encoder();
    refresh_dma(tictoc);
    compute(tictoc == 0 ? 1 : 0);
    tictoc = tictoc == 0 ? 1 : 0;
    update_leds();
    // times[time_p++] = bflb_mtimer_get_time_us();

    // if(time_p >= 100){
    //     time_p = 0;
    //     for(int i=0; i<100; i+=2){
    //         uint64_t time = times[i+1] - times[i];
    //         printf("%20llu -> %20llu = %20llu\n", times[i], times[i+1], time);
    //     }
    // }
}

/* audio gpio init*/
void audac_gpio_init(void)
{
    struct bflb_device_s *gpio;

    gpio = bflb_device_get_by_name("gpio");

    /* audac gpadc output mode */
    // bflb_gpio_init(gpio, GPIO_PIN_2, GPIO_ANALOG | GPIO_SMT_EN | GPIO_DRV_0);
    // bflb_gpio_init(gpio, GPIO_PIN_3, GPIO_ANALOG | GPIO_SMT_EN | GPIO_DRV_0);

    
    /* audac pwm output mode */
    bflb_gpio_init(gpio, GPIO_PIN_14, GPIO_FUNC_AUDAC_PWM | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
}

void audac_dma_init(void)
{
    struct bflb_dma_channel_config_s audac_dma_cfg;

    audac_dma_cfg.direction = DMA_MEMORY_TO_PERIPH;
    audac_dma_cfg.src_req = DMA_REQUEST_NONE;
    audac_dma_cfg.dst_req = DMA_REQUEST_AUDAC_TX;
    audac_dma_cfg.src_addr_inc = DMA_ADDR_INCREMENT_ENABLE;
    audac_dma_cfg.dst_addr_inc = DMA_ADDR_INCREMENT_DISABLE;
    audac_dma_cfg.src_burst_count = DMA_BURST_INCR4;
    audac_dma_cfg.dst_burst_count = DMA_BURST_INCR4;
    // audac_dma_cfg.src_burst_count = DMA_BURST_INCR8;
    // audac_dma_cfg.dst_burst_count = DMA_BURST_INCR8;
    audac_dma_cfg.src_width = DMA_DATA_WIDTH_16BIT;
    audac_dma_cfg.dst_width = DMA_DATA_WIDTH_16BIT;

    audac_dma_hd = bflb_device_get_by_name("dma0_ch0");
    bflb_dma_channel_init(audac_dma_hd, &audac_dma_cfg);
    bflb_dma_channel_irq_attach(audac_dma_hd, audio_dma_callback, NULL);

    transfers_tic[0].src_addr = (uint32_t)tic;
    transfers_tic[0].dst_addr = (uint32_t)DMA_ADDR_AUDAC_TDR;
    transfers_tic[0].nbytes = sizeof(tic);

    bflb_l1c_dcache_clean_range(tic, sizeof(tic));
    dma_lli_cnt_tic = bflb_dma_channel_lli_reload(audac_dma_hd, lli_pool_tic, 10, transfers_tic, 1);
    bflb_dma_channel_start(audac_dma_hd);
}

/* audio dac init */
static void audac_init(void)
{
    struct bflb_audac_init_config_s audac_init_cfg = {
        .sampling_rate = AUDAC_SAMPLING_RATE_48K,
        // .sampling_rate = AUDAC_SAMPLING_RATE_32K,
        .output_mode = AUDAC_OUTPUT_MODE_PWM,
        // .output_mode = AUDAC_OUTPUT_MODE_GPDAC_CH_A,
        // .output_mode = AUDAC_OUTPUT_MODE_GPDAC_CH_A_B,
        // .source_channels_num = AUDAC_SOURCE_CHANNEL_DUAL,
        .source_channels_num = AUDAC_SOURCE_CHANNEL_SINGLE,
        .mixer_mode = AUDAC_MIXER_MODE_ONLY_L,
        .data_format = AUDAC_DATA_FORMAT_16BIT,
        .fifo_threshold = 3,
        // .fifo_threshold = 7,
    };

    struct bflb_audac_volume_config_s audac_volume_cfg = {
        .mute_ramp_en = true,
        .mute_up_ramp_rate = AUDAC_RAMP_RATE_FS_32,
        .mute_down_ramp_rate = AUDAC_RAMP_RATE_FS_8,
        .volume_update_mode = AUDAC_VOLUME_UPDATE_MODE_RAMP,
        .volume_ramp_rate = AUDAC_RAMP_RATE_FS_128,
        .volume_zero_cross_timeout = AUDAC_RAMP_RATE_FS_128,
    };

    /* clock cfg */
    GLB_Config_AUDIO_PLL_To_491P52M();
    GLB_PER_Clock_UnGate(GLB_AHB_CLOCK_AUDIO);

    /* audac init */
    audac_hd = bflb_device_get_by_name("audac");
    bflb_audac_init(audac_hd, &audac_init_cfg);
    // bflb_audac_feature_control(audac_hd, AUDAC_CMD_SET_VOLUME_VAL, (size_t)(-15 * 2));
    bflb_audac_feature_control(audac_hd, AUDAC_CMD_SET_VOLUME_VAL, 0);
    bflb_audac_volume_init(audac_hd, &audac_volume_cfg);
    /* audac enable dma */
    bflb_audac_link_rxdma(audac_hd, true);
}

void audio_dac_init(){
    audac_gpio_init();
    audac_init();
    audac_dma_init();
    bflb_dma_channel_start(audac_dma_hd);
    bflb_audac_feature_control(audac_hd, AUDAC_CMD_PLAY_START, 0);
}