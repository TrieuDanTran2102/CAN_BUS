#include "drv_gpio.h"
#include "hardware/gpio.h"

status_t drv_gpio_init(uint32_t pin, drv_gpio_dir_t dir, drv_gpio_pull_t pull)
{
    gpio_init(pin);
    gpio_set_dir(pin, dir == DRV_GPIO_DIR_OUTPUT ? GPIO_OUT : GPIO_IN);

    switch (pull) {
        case DRV_GPIO_PULL_UP:
            gpio_pull_up(pin);
            break;
        case DRV_GPIO_PULL_DOWN:
            gpio_pull_down(pin);
            break;
        case DRV_GPIO_PULL_NONE:
        default:
            gpio_disable_pulls(pin);
            break;
    }

    return STATUS_OK;
}

status_t drv_gpio_write(uint32_t pin, bool level)
{
    gpio_put(pin, level);
    return STATUS_OK;
}

bool drv_gpio_read(uint32_t pin)
{
    return gpio_get(pin);
}
