#include "app_can_sync.h"
#include "common/can_protocol.h"
#include "common/project_config.h"
#include "middleware/can/can_manager.h"
#include "middleware/time_sync/time_sync.h"
#include <string.h>

static void on_can_rx(const xl2515_can_frame_t *frame, void *ctx)
{
    (void)ctx;

    /* --- Time sync message từ master ------------------------------------ */
    for (int node = 0; node < NODE_COUNT; node++) {
        if (frame->id == CAN_ID_TIME_SYNC(node) &&
            node == TIME_SYNC_MASTER_NODE_ID &&
            frame->dlc == sizeof(can_msg_time_sync_t)) {

            can_msg_time_sync_t msg;
            memcpy(&msg, frame->data, sizeof(msg));
            time_sync_on_master_timestamp_received(msg.timestamp_ms);
            return;
        }
    }

    /* --- Input status từ node khác ---------------------------------------
     * Bản basic: chỉ log/consume ở đây. Nếu cần xử lý logic dựa trên input
     * của node khác, thêm callback riêng hoặc mở rộng tại đây. */
    for (int node = 0; node < NODE_COUNT; node++) {
        if (frame->id == CAN_ID_INPUT_STATUS(node) &&
            frame->dlc == sizeof(can_msg_input_status_t)) {
            /* TODO: xử lý input status từ node khác nếu cần */
            return;
        }
    }
}


void app_can_sync_init(void)
{
    time_sync_init();
    can_manager_init();
    can_manager_register_rx_callback(on_can_rx, NULL);
}

void app_can_sync_task(void)
{
    can_manager_poll();
    time_sync_task();
}
