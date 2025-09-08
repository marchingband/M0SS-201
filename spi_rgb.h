#ifndef SPI_RGB_H
#define SPI_RGB_H

void spi_rgb_init();
void spi_rgb_send(uint32_t *buf, size_t len);
void spi_rgb_start(uint32_t *buf, size_t len);

#endif