#ifndef MIDDLEWARE_CAN_CAN_MANAGER_H
#define MIDDLEWARE_CAN_CAN_MANAGER_H

#include "common/types.h"
#include "driver/xl2515/xl2515.h"

/* Callback được gọi mỗi khi nhận được 1 frame CAN, để app layer xử lý
 * (app_can_sync, app_input, ...) tự lọc theo ID mình quan tâm. */
typedef void (*can_manager_rx_cb_t)(const xl2515_can_frame_t *frame, void *user_ctx);

status_t can_manager_init(void);

/* Gửi 1 frame ra bus */
status_t can_manager_send(uint32_t id, const uint8_t *data, uint8_t len);

/* Đăng ký callback nhận frame. Bản basic chỉ hỗ trợ 1 callback duy nhất,
 * đủ dùng vì hiện tại chỉ có app_can_sync xử lý toàn bộ RX rồi tự dispatch. */
void can_manager_register_rx_callback(can_manager_rx_cb_t cb, void *user_ctx);

/* Gọi liên tục trong main loop (poll mode, không dùng ngắt cho bản basic) */
void can_manager_poll(void);

#endif /* MIDDLEWARE_CAN_CAN_MANAGER_H */
