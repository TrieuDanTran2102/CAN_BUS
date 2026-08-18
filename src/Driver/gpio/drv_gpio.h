#ifndef DRIVER_GPIO_DRV_GPIO_H
#define DRIVER_GPIO_DRV_GPIO_H

#include "common/types.h"

typedef enum {
    DRV_GPIO_DIR_INPUT = 0,
    DRV_GPIO_DIR_OUTPUT,
} drv_gpio_dir_t;

typedef enum {
    DRV_GPIO_PULL_NONE = 0,
    DRV_GPIO_PULL_UP,
    DRV_GPIO_PULL_DOWN,
} drv_gpio_pull_t;

/* Khởi tạo 1 pin GPIO thuần (không phải SPI/UART func) */
status_t drv_gpio_init(uint32_t pin, drv_gpio_dir_t dir, drv_gpio_pull_t pull);

status_t drv_gpio_write(uint32_t pin, bool level);
bool     drv_gpio_read(uint32_t pin);

#endif /* DRIVER_GPIO_DRV_GPIO_H */
