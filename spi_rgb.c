#include "bflb_mtimer.h"
#include "bflb_gpio.h"
#include "bflb_spi.h"
#include "bflb_dma.h"
#include "board.h"

#define BUFF_LEN 128 * 4

uint32_t rx_buff[128];
static ATTR_NOCACHE_NOINIT_RAM_SECTION uint32_t rx_buffer[256];

struct bflb_device_s *spi0;
struct bflb_device_s *dma0_ch1;

static volatile uint8_t dma_tc_flag0 = 0;
struct bflb_dma_channel_lli_pool_s tx_llipool[1];
struct bflb_dma_channel_lli_transfer_s tx_transfers[1];

void spi_rgb_init(){
    struct bflb_device_s *gpio;
    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, GPIO_PIN_16, GPIO_FUNC_SPI0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);/* spi mosi */
    bflb_gpio_init(gpio, GPIO_PIN_17, GPIO_FUNC_SPI0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);/* spi miso */
    bflb_gpio_init(gpio, GPIO_PIN_10, GPIO_FUNC_SPI0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);/* spi cs */
    bflb_gpio_init(gpio, GPIO_PIN_11, GPIO_FUNC_SPI0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1); /* spi clk */


    struct bflb_dma_channel_config_s tx_config = {
        .direction = DMA_MEMORY_TO_PERIPH,
        .src_req = DMA_REQUEST_NONE,
        .dst_req = DMA_REQUEST_SPI0_TX,
        .src_addr_inc = DMA_ADDR_INCREMENT_ENABLE,
        .dst_addr_inc = DMA_ADDR_INCREMENT_DISABLE,
        .src_burst_count = DMA_BURST_INCR1,
        .dst_burst_count = DMA_BURST_INCR1,
        .src_width = DMA_DATA_WIDTH_32BIT,
        .dst_width = DMA_DATA_WIDTH_32BIT,
    };

    struct bflb_spi_config_s spi_cfg = {
        // .freq = 1 * 1000 * 1000,
        .freq = 93750 * 16 * 2,
        .role = SPI_ROLE_MASTER,
        .mode = SPI_MODE3,
        .data_width = SPI_DATA_WIDTH_32BIT,
        .bit_order = SPI_BIT_MSB,
        .byte_order = SPI_BYTE_MSB,
        // .byte_order = SPI_BYTE_LSB,
        .tx_fifo_threshold = 0,
        .rx_fifo_threshold = 0,
    };

    spi0 = bflb_device_get_by_name("spi0");
    bflb_spi_init(spi0, &spi_cfg);
    bflb_spi_link_txdma(spi0, true);

    tx_transfers[0].dst_addr = (uint32_t)DMA_ADDR_SPI0_TDR;
    // tx_transfers[0].nbytes = 256 * 4; // 201
    tx_transfers[0].nbytes = 128 * 4;

    dma0_ch1 = bflb_device_get_by_name("dma0_ch1");
    bflb_dma_channel_init(dma0_ch1, &tx_config);

    bflb_spi_feature_control(spi0, SPI_CMD_SET_CS_INTERVAL, 0);
}

// void spi_rgb_send(uint32_t *buf, size_t len){
//     for(int i=0; i<len; i++){
//         rx_buff[i] = bflb_spi_poll_send(spi0, buf[i]);
//     }
// }

void spi_rgb_send(uint32_t *buf, size_t len){
    tx_transfers[0].src_addr = buf;
    bflb_dma_channel_lli_reload(dma0_ch1, tx_llipool, 1, tx_transfers, 1);
    bflb_dma_channel_start(dma0_ch1);
}