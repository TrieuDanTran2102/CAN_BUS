#include "drv_spi.h"
#include "driver/gpio/drv_gpio.h"
#include "hardware/gpio.h"

status_t drv_spi_init(const drv_spi_config_t *cfg)
{
    if (cfg == NULL) {
        return STATUS_INVALID_PARAM;
    }

    spi_init(cfg->port, cfg->baudrate_hz);

    gpio_set_function(cfg->pin_sck, GPIO_FUNC_SPI);
    gpio_set_function(cfg->pin_mosi, GPIO_FUNC_SPI);
    gpio_set_function(cfg->pin_miso, GPIO_FUNC_SPI);

    /* CS điều khiển tay bằng GPIO thường, không dùng SPI hardware CS */
    drv_gpio_init(cfg->pin_cs, DRV_GPIO_DIR_OUTPUT, DRV_GPIO_PULL_NONE);
    drv_gpio_write(cfg->pin_cs, true); /* idle = high */

    return STATUS_OK;
}

void drv_spi_cs_select(const drv_spi_config_t *cfg)
{
    drv_gpio_write(cfg->pin_cs, false);
}

void drv_spi_cs_deselect(const drv_spi_config_t *cfg)
{
    drv_gpio_write(cfg->pin_cs, true);
}

status_t drv_spi_write(const drv_spi_config_t *cfg, const uint8_t *data, size_t len)
{
    int written = spi_write_blocking(cfg->port, data, len);
    return (written == (int)len) ? STATUS_OK : STATUS_ERROR;
}

status_t drv_spi_read(const drv_spi_config_t *cfg, uint8_t *data, size_t len)
{
    int read_bytes = spi_read_blocking(cfg->port, 0x00, data, len);
    return (read_bytes == (int)len) ? STATUS_OK : STATUS_ERROR;
}

status_t drv_spi_write_read(const drv_spi_config_t *cfg, const uint8_t *tx, uint8_t *rx, size_t len)
{
    int n = spi_write_read_blocking(cfg->port, tx, rx, len);
    return (n == (int)len) ? STATUS_OK : STATUS_ERROR;
}
