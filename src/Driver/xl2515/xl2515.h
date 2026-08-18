#ifndef DRIVER_XL2515_XL2515_H
#define DRIVER_XL2515_XL2515_H

#include "common/types.h"
#include "driver/spi/drv_spi.h"

#define XL2515_MAX_DLC 8

typedef struct {
    uint32_t id;                    /* standard 11-bit CAN ID */
    uint8_t  dlc;                   /* data length, 0..8 */
    uint8_t  data[XL2515_MAX_DLC];
} xl2515_can_frame_t;

typedef struct {
    drv_spi_config_t spi;
    uint32_t          int_pin;
    uint32_t          osc_hz;
} xl2515_t;

/* Khởi tạo SPI, reset chip, cấu hình bit-timing cho 500kbps, chuyển Normal mode */
status_t xl2515_init(xl2515_t *dev);

/* Gửi 1 frame qua TXB0. Blocking đơn giản, không dùng queue nhiều buffer. */
status_t xl2515_send(xl2515_t *dev, const xl2515_can_frame_t *frame);

/* Kiểm tra có frame mới trong RX buffer 0/1 không (poll, không cần ngắt) */
bool xl2515_frame_available(xl2515_t *dev);

/* Đọc 1 frame đã nhận. Trả STATUS_NOT_READY nếu không có frame nào. */
status_t xl2515_receive(xl2515_t *dev, xl2515_can_frame_t *frame);

#endif /* DRIVER_XL2515_XL2515_H */
