#include "app_input.h"
#include "common/board_config.h"
#include "common/project_config.h"
#include "driver/gpio/drv_gpio.h"
#include "pico/time.h"

/*
conmmit one 
******************
*****************
******************
*****************
******************
*****************
******************
*****************
*/

static bool s_input1 = false;
static bool s_input2 = false;
static uint32_t s_last_sample_ms = 0;

void app_input_init(void)
{
    /* Input dùng pull-up: mặc định mức cao (1) khi hở mạch, kéo xuống mức
     * thấp (0) khi có tín hiệu active. Đổi pull-up/down tùy phần cứng thật. */
    drv_gpio_init(BOARD_INPUT1_PIN, DRV_GPIO_DIR_INPUT, DRV_GPIO_PULL_UP);
    drv_gpio_init(BOARD_INPUT2_PIN, DRV_GPIO_DIR_INPUT, DRV_GPIO_PULL_UP);
}

bool app_input_get(uint8_t index)
{
    return (index == 0) ? s_input1 : s_input2;
}

void app_input_task(void)
{
    uint32_t now = to_ms_since_boot(get_absolute_time());

    /* Sample định kỳ (basic debounce: chỉ đơn giản đọc theo chu kỳ cố định,
     * chưa lọc nhiễu cạnh -- nâng cấp sau nếu input thực tế bị dội) */
    if (now - s_last_sample_ms >= INPUT_SAMPLE_PERIOD_MS) {
        s_last_sample_ms = now;
        s_input1 = drv_gpio_read(BOARD_INPUT1_PIN);
        s_input2 = drv_gpio_read(BOARD_INPUT2_PIN);
    }
}
