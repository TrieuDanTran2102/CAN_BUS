#include "drv_uart.h"
#include "hardware/gpio.h"
#include <string.h>

status_t drv_uart_init(const drv_uart_config_t *cfg)
{
    if (cfg == NULL) {
        return STATUS_INVALID_PARAM;
    }

    uart_init(cfg->port, cfg->baudrate);
    gpio_set_function(cfg->pin_tx, GPIO_FUNC_UART);
    gpio_set_function(cfg->pin_rx, GPIO_FUNC_UART);

    return STATUS_OK;
}

status_t drv_uart_write(const drv_uart_config_t *cfg, const uint8_t *data, size_t len)
{
    uart_write_blocking(cfg->port, data, len);
    return STATUS_OK;
}

status_t drv_uart_print(const drv_uart_config_t *cfg, const char *str)
{
    return drv_uart_write(cfg, (const uint8_t *)str, strlen(str));
}
