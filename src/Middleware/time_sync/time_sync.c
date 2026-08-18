#include "time_sync.h"
#include "common/project_config.h"
#include "common/can_protocol.h"
#include "middleware/can/can_manager.h"
#include "pico/time.h"

static int64_t s_offset_ms = 0;          /* synced_time = local_time + offset (chỉ dùng ở slave) */
static timestamp_ms_t s_last_broadcast_ms = 0;

void time_sync_init(void)
{
    s_offset_ms = 0;
    s_last_broadcast_ms = 0;
}

timestamp_ms_t time_sync_get_local_ms(void)
{
    return (timestamp_ms_t)(time_us_64() / 1000ULL);
}

timestamp_ms_t time_sync_get_synced_ms(void)
{
    if (NODE_ID == TIME_SYNC_MASTER_NODE_ID) {
        return time_sync_get_local_ms();
    }
    return (timestamp_ms_t)((int64_t)time_sync_get_local_ms() + s_offset_ms);
}

void time_sync_task(void)
{
    if (NODE_ID != TIME_SYNC_MASTER_NODE_ID) {
        return; /* slave chỉ cập nhật thụ động khi nhận CAN frame */
    }

    timestamp_ms_t now = time_sync_get_local_ms();
    if (now - s_last_broadcast_ms < TIME_SYNC_PERIOD_MS) {
        return;
    }
    s_last_broadcast_ms = now;

    can_msg_time_sync_t msg = { .timestamp_ms = now };
    can_manager_send(CAN_ID_TIME_SYNC(NODE_ID), (const uint8_t *)&msg, sizeof(msg));
}

void time_sync_on_master_timestamp_received(timestamp_ms_t master_ts_ms)
{
    if (NODE_ID == TIME_SYNC_MASTER_NODE_ID) {
        return; /* master không cần tự đồng bộ với chính nó */
    }
    timestamp_ms_t local_now = time_sync_get_local_ms();
    s_offset_ms = (int64_t)master_ts_ms - (int64_t)local_now;
}
