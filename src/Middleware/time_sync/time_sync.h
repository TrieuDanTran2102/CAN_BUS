#ifndef MIDDLEWARE_TIME_SYNC_TIME_SYNC_H
#define MIDDLEWARE_TIME_SYNC_TIME_SYNC_H

#include "common/types.h"

/* ============================================================================
 * Mô hình đồng bộ thời gian "basic" cho 3 node:
 *
 * - Node có NODE_ID == TIME_SYNC_MASTER_NODE_ID (xem project_config.h) định kỳ
 *   broadcast timestamp nội bộ của nó (local monotonic ms, từ boot) lên bus.
 * - Các node còn lại khi nhận được, tính offset = master_ts - local_ts_lúc_nhận,
 *   rồi cộng offset đó vào local time mỗi khi cần lấy "synced time".
 *
 * Đây KHÔNG phải là giao thức PTP chuẩn (chưa bù trễ truyền trên bus), chỉ đủ
 * cho bản basic. Có thể nâng cấp sau bằng cách đo round-trip delay.
 * ==========================================================================*/

void time_sync_init(void);

/* Lấy timestamp hiện tại của node này (ms kể từ khi boot) */
timestamp_ms_t time_sync_get_local_ms(void);

/* Lấy timestamp đã đồng bộ theo master (= local + offset).
 * Trên node master, hàm này trả về chính local time (offset = 0). */
timestamp_ms_t time_sync_get_synced_ms(void);

/* Gọi định kỳ trong main loop:
 * - Nếu là master: broadcast timestamp theo chu kỳ TIME_SYNC_PERIOD_MS.
 * - Nếu là slave: không làm gì (cập nhật offset xảy ra trong callback RX). */
void time_sync_task(void);

/* Được can_manager gọi khi nhận được frame CAN_ID_TIME_SYNC từ master */
void time_sync_on_master_timestamp_received(timestamp_ms_t master_ts_ms);

#endif /* MIDDLEWARE_TIME_SYNC_TIME_SYNC_H */
