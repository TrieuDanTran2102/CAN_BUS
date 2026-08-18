#ifndef DRIVER_SPI_DRV_SPI_H
#define DRIVER_SPI_DRV_SPI_H

#include "common/types.h"
#include "hardware/spi.h"

typedef struct {
    spi_inst_t *port;
    uint32_t    pin_sck;
    uint32_t    pin_mosi;
    uint32_t    pin_miso;
    uint32_t    pin_cs;
    uint32_t    baudrate_hz;
} drv_spi_config_t;

status_t drv_spi_init(const drv_spi_config_t *cfg);

/* CS được điều khiển thủ công bằng GPIO (không dùng hardware CS của SPI
 * block) để tương thích với cách MCP2515/XL2515 driver thường thao tác. */
void drv_spi_cs_select(const drv_spi_config_t *cfg);
void drv_spi_cs_deselect(const drv_spi_config_t *cfg);

status_t drv_spi_write(const drv_spi_config_t *cfg, const uint8_t *data, size_t len);
status_t drv_spi_read(const drv_spi_config_t *cfg, uint8_t *data, size_t len);
status_t drv_spi_write_read(const drv_spi_config_t *cfg, const uint8_t *tx, uint8_t *rx, size_t len);

#endif /* DRIVER_SPI_DRV_SPI_H */
