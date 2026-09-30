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


static uint16_t tic[BUF_SIZE * 2];
static uint16_t toc[BUF_SIZE * 2];

static struct bflb_device_s *i2s0;
static struct bflb_device_s *i2s_dma_hd;

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
        transfers_toc[0].dst_addr = (uint32_t)DMA_ADDR_I2S_TDR;
        transfers_toc[0].nbytes = sizeof(toc);
        bflb_l1c_dcache_clean_range(toc, sizeof(toc));
        dma_lli_cnt_toc = bflb_dma_channel_lli_reload(i2s_dma_hd, lli_pool_toc, 10, transfers_toc, 1);
        bflb_dma_channel_start(i2s_dma_hd);
    }
    else
    {
        transfers_tic[0].src_addr = (uint32_t)tic;
        transfers_tic[0].dst_addr = (uint32_t)DMA_ADDR_I2S_TDR;
        transfers_tic[0].nbytes = sizeof(tic);
        bflb_l1c_dcache_clean_range(tic, sizeof(tic));
        dma_lli_cnt_tic = bflb_dma_channel_lli_reload(i2s_dma_hd, lli_pool_tic, 10, transfers_tic, 1);
        bflb_dma_channel_start(i2s_dma_hd);
    }
}

static uint64_t times[100] = {0};
static size_t time_p = 0;

#define DBG_TIMING
#define DBG_TIMING_PIN 15

void audio_dma_callback(void *arg)
{
    #ifdef DBG_TIMING
    struct bflb_device_s* gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_set(gpio, DBG_TIMING_PIN);
    #endif

    static int tictoc = 1;
    process_encoder();
    refresh_dma(tictoc);
    compute(tictoc == 0 ? 1 : 0);
    tictoc = tictoc == 0 ? 1 : 0;
    update_leds();

    #ifdef DBG_TIMING
    bflb_gpio_reset(gpio, DBG_TIMING_PIN);
    #endif
}

static void i2s_gpio_init(void)
{
    struct bflb_device_s *gpio = bflb_device_get_by_name("gpio");

    /* I2S_FS / LRCK */
    bflb_gpio_init(gpio, GPIO_PIN_1, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    /* I2S_DI (not required for TX-only, but harmless) */
    // bflb_gpio_init(gpio, GPIO_PIN_10, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    /* I2S_DO / DIN */
    bflb_gpio_init(gpio, GPIO_PIN_3, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    /* I2S_BCLK / BCK */
    bflb_gpio_init(gpio, GPIO_PIN_0, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);

    #ifdef DBG_TIMING
    bflb_gpio_init(gpio, DBG_TIMING_PIN, GPIO_OUTPUT | GPIO_SMT_EN | GPIO_DRV_0);
    #endif
}

static void i2s_dma_init(void) {
    struct bflb_dma_channel_config_s tx_config = {
        .direction = DMA_MEMORY_TO_PERIPH,
        .src_req = DMA_REQUEST_NONE,
        .dst_req = DMA_REQUEST_I2S_TX,
        .src_addr_inc = DMA_ADDR_INCREMENT_ENABLE,
        .dst_addr_inc = DMA_ADDR_INCREMENT_DISABLE,
        .src_burst_count = DMA_BURST_INCR1,
        .dst_burst_count = DMA_BURST_INCR1,
        .src_width = DMA_DATA_WIDTH_16BIT,
        .dst_width = DMA_DATA_WIDTH_16BIT,
    };

    i2s_dma_hd = bflb_device_get_by_name("dma0_ch0");
    bflb_dma_channel_init(i2s_dma_hd, &tx_config);
    bflb_dma_channel_irq_attach(i2s_dma_hd, audio_dma_callback, NULL);

    transfers_tic[0].src_addr = (uint32_t)tic;
    transfers_tic[0].dst_addr = (uint32_t)DMA_ADDR_I2S_TDR;
    transfers_tic[0].nbytes = sizeof(tic);

    bflb_l1c_dcache_clean_range(tic, sizeof(tic));
    dma_lli_cnt_tic = bflb_dma_channel_lli_reload(i2s_dma_hd, lli_pool_tic, 10, transfers_tic, 1);
}

static void i2s_init(void) {
    struct bflb_i2s_config_s i2s_cfg = {
        .bclk_freq_hz = AUDIO_SAMPLE_RATE_HZ * AUDIO_FRAME_BITS * AUDIO_CHANNELS,
        .role = I2S_ROLE_MASTER,
        .format_mode = I2S_MODE_LEFT_JUSTIFIED,
        .channel_mode = I2S_CHANNEL_MODE_NUM_2,
        .frame_width = I2S_SLOT_WIDTH_16,
        .data_width = I2S_SLOT_WIDTH_16,
        .fs_offset_cycle = 1,
        .tx_fifo_threshold = 0,
        .rx_fifo_threshold = 0,
    };

    i2s0 = bflb_device_get_by_name("i2s0");
    bflb_i2s_init(i2s0, &i2s_cfg);
    bflb_i2s_link_txdma(i2s0, true);
}


void audio_dac_init(){
    // set up pins for outputting the signal
    i2s_gpio_init();
    // set up the peripheral
    i2s_init();
    // set up dma
    i2s_dma_init();
    // start dma
    bflb_dma_channel_start(i2s_dma_hd);
    // start peripheral
    bflb_i2s_feature_control(i2s0, I2S_CMD_DATA_ENABLE, I2S_CMD_DATA_ENABLE_TX);
}