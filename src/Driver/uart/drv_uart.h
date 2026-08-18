#ifndef DRIVER_UART_DRV_UART_H
#define DRIVER_UART_DRV_UART_H

#include "common/types.h"
#include "hardware/uart.h"

typedef struct {
    uart_inst_t *port;
    uint32_t     pin_tx;
    uint32_t     pin_rx;
    uint32_t     baudrate;
} drv_uart_config_t;

status_t drv_uart_init(const drv_uart_config_t *cfg);
status_t drv_uart_write(const drv_uart_config_t *cfg, const uint8_t *data, size_t len);
status_t drv_uart_print(const drv_uart_config_t *cfg, const char *str);

#endif /* DRIVER_UART_DRV_UART_H */
