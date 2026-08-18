#include "can_manager.h"
#include "common/board_config.h"
#include "common/project_config.h"
#include <string.h>

static xl2515_t s_xl2515;
static can_manager_rx_cb_t s_rx_cb = NULL;
static void *s_rx_cb_ctx = NULL;

status_t can_manager_init(void)
{
    memset(&s_xl2515, 0, sizeof(s_xl2515));

    s_xl2515.spi.port        = BOARD_XL2515_SPI_PORT;
    s_xl2515.spi.pin_sck     = BOARD_XL2515_PIN_SCK;
    s_xl2515.spi.pin_mosi    = BOARD_XL2515_PIN_MOSI;
    s_xl2515.spi.pin_miso    = BOARD_XL2515_PIN_MISO;
    s_xl2515.spi.pin_cs      = BOARD_XL2515_PIN_CS;
    s_xl2515.spi.baudrate_hz = BOARD_XL2515_SPI_BAUDRATE;
    s_xl2515.int_pin         = BOARD_XL2515_PIN_INT;
    s_xl2515.osc_hz          = XL2515_OSC_HZ;

    return xl2515_init(&s_xl2515);
}

status_t can_manager_send(uint32_t id, const uint8_t *data, uint8_t len)
{
    xl2515_can_frame_t frame = { 0 };
    frame.id  = id;
    frame.dlc = len;
    if (len > 0 && data != NULL) {
        memcpy(frame.data, data, len);
    }
    return xl2515_send(&s_xl2515, &frame);
}

void can_manager_register_rx_callback(can_manager_rx_cb_t cb, void *user_ctx)
{
    s_rx_cb = cb;
    s_rx_cb_ctx = user_ctx;
}

void can_manager_poll(void)
{
    xl2515_can_frame_t frame;
    /* Đọc hết tất cả frame đang chờ trong 1 lần poll, tránh bị dồn */
    while (xl2515_frame_available(&s_xl2515)) {
        if (xl2515_receive(&s_xl2515, &frame) == STATUS_OK) {
            if (s_rx_cb != NULL) {
                s_rx_cb(&frame, s_rx_cb_ctx);
            }
        } else {
            break;
        }
    }
}
