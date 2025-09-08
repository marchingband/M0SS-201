#include "bflb_uart.h"
#include <math.h>
#include <stdlib.h>

#include "bflb_mtimer.h"
#include "bflb_adc.h"
#include "bflb_audac.h"
#include "bflb_gpio.h"
#include "bflb_dma.h"
#include "bflb_uart.h"
#include "bflb_flash.h"
#include "bflb_clock.h"
#include "mem.h"

#include "bl616_glb.h"
#include "bflb_i2s.h"
#include "bl616_glb_gpio.h"
#include "bl616_irq.h"
#include "board.h"

#include "bflb_core.h"

#include "usbh_core.h"
#include <FreeRTOS.h>

#include "rgb_led.h"
#include "gpio.h"
#include "flash.h"
#include "encoder.h"
#include "audac.h"
#include "spi_rgb.h"
#include "midi.h"
#include "dsp.h"

extern uint32_t __HeapBase;
extern uint32_t __HeapLimit;

static void uart_init(bool set_console){
    struct bflb_device_s *gpio;
    struct bflb_device_s *uart0;

    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_uart_init(gpio, GPIO_PIN_21, GPIO_UART_FUNC_UART0_TX);
    bflb_gpio_uart_init(gpio, GPIO_PIN_22, GPIO_UART_FUNC_UART0_RX);

    struct bflb_uart_config_s cfg = {0};
    cfg.baudrate = 31250;
    cfg.data_bits = UART_DATA_BITS_8;
    cfg.stop_bits = UART_STOP_BITS_1;
    cfg.parity = UART_PARITY_NONE;
    cfg.flow_ctrl = 0;
    cfg.tx_fifo_threshold = 7;
    cfg.rx_fifo_threshold = 7;
    cfg.bit_order = UART_LSB_FIRST;
    uart0 = bflb_device_get_by_name("uart0");
    bflb_uart_init(uart0, &cfg);

    if(set_console){
        bflb_uart_set_console(uart0);
    }
}

static void peripheral_clock_init(void)
{
    PERIPHERAL_CLOCK_ADC_DAC_ENABLE();
    PERIPHERAL_CLOCK_SEC_ENABLE();
    PERIPHERAL_CLOCK_DMA0_ENABLE();
    PERIPHERAL_CLOCK_UART0_ENABLE();
    PERIPHERAL_CLOCK_UART1_ENABLE();
    PERIPHERAL_CLOCK_SPI0_ENABLE();
    PERIPHERAL_CLOCK_I2C0_ENABLE();
    PERIPHERAL_CLOCK_PWM0_ENABLE();
    PERIPHERAL_CLOCK_TIMER0_1_WDG_ENABLE();
    PERIPHERAL_CLOCK_IR_ENABLE();
    PERIPHERAL_CLOCK_I2S_ENABLE();
    PERIPHERAL_CLOCK_USB_ENABLE();
    PERIPHERAL_CLOCK_CAN_ENABLE();
    PERIPHERAL_CLOCK_AUDIO_ENABLE();
    PERIPHERAL_CLOCK_CKS_ENABLE();

    GLB_Set_UART_CLK(ENABLE, HBN_UART_CLK_XCLK, 0);
    GLB_Set_SPI_CLK(ENABLE, GLB_SPI_CLK_MCU_MUXPLL_160M, 0);
    GLB_Set_DBI_CLK(ENABLE, GLB_SPI_CLK_MCU_MUXPLL_160M, 0);
    GLB_Set_I2C_CLK(ENABLE, GLB_I2C_CLK_XCLK, 0);
    GLB_Set_ADC_CLK(ENABLE, GLB_ADC_CLK_XCLK, 1);
    GLB_Set_DIG_CLK_Sel(GLB_DIG_CLK_XCLK);
    GLB_Set_DIG_512K_CLK(ENABLE, ENABLE, 0x4E);
    GLB_Set_PWM1_IO_Sel(GLB_PWM1_IO_SINGLE_END);
    GLB_Set_IR_CLK(ENABLE, GLB_IR_CLK_SRC_XCLK, 19);
    GLB_Set_CAM_CLK(ENABLE, GLB_CAM_CLK_WIFIPLL_96M, 3);

    GLB_Set_PKA_CLK_Sel(GLB_PKA_CLK_MCU_MUXPLL_160M);

    GLB_Set_USB_CLK_From_WIFIPLL(1);
    GLB_Swap_MCU_SPI_0_MOSI_With_MISO(0);
}

static void system_clock_init(void)
{
    /* wifipll/audiopll */
    GLB_Power_On_XTAL_And_PLL_CLK(GLB_XTAL_40M, GLB_PLL_WIFIPLL | GLB_PLL_AUPLL);
    GLB_Set_MCU_System_CLK(GLB_MCU_SYS_CLK_TOP_WIFIPLL_320M);
    CPU_Set_MTimer_CLK(ENABLE, BL_MTIMER_SOURCE_CLOCK_MCU_XCLK, Clock_System_Clock_Get(BL_SYSTEM_CLOCK_XCLK) / 1000000 - 1);
}

static void m0ss_board_init(bool set_console){
    int ret = -1;
    uintptr_t flag;
    size_t heap_len;

    flag = bflb_irq_save();
    ret = bflb_flash_init();
    system_clock_init();
    peripheral_clock_init();
    bflb_irq_initialize();
    uart_init(set_console);

    heap_len = ((size_t)&__HeapLimit - (size_t)&__HeapBase);
    kmem_init((void *)&__HeapBase, heap_len);

    bflb_irq_restore(flag);
}

int main(void)
{
    /* board init */
    board_init();
    // m0ss_board_init( /*set console*/ false );

    /* load the config struct or initialize it */
    // printf("flash_init\r\n");
    // flash_init();

    /* setup button scanners */
    printf("gpio_init\r\n");
    buttons_init();

    /* start usb */
    // printf("usbh_initialize\r\n");
    // usbh_initialize();

    /* dsp init */
    printf("dsp_init\r\n");
    dsp_init(BUF_SIZE);

    /* audio dac init */
    printf("audio_dac_init\r\n");
    audio_dac_init();

    /* uart midi */
    // printf("uart_receive_init\r\n");
    // uart_receive_init();

    /* start the LEDs */
    // printf("rgb_led_init\r\n");
    // spi_rgb_init();

    /* setup encoder */
    printf("encoder_init\r\n");
    encoder_init();

    // while(1){
    //     read_buttons();
    //     bflb_mtimer_delay_ms(1000);
    // }

    vTaskStartScheduler();

    /***************************/
    /*                         */
    // no code past here will run
    /*                         */
    /***************************/

    while (1){
        bflb_mtimer_delay_ms(1000);
    }
}

