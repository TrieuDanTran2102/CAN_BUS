#ifndef COMMON_CAN_PROTOCOL_H
#define COMMON_CAN_PROTOCOL_H

#include <stdint.h>

/* ============================================================================
 * Định nghĩa message layout dùng chung cho cả 3 node trên bus.
 * Dùng standard 11-bit CAN ID: {base_id} + node_id để mỗi node có ID riêng,
 * tránh đụng độ khi nhiều node cùng broadcast 1 loại message.
 * ==========================================================================*/

/* --- Base CAN IDs (mỗi node cộng thêm NODE_ID của mình vào base) -------- */
#define CAN_ID_TIME_SYNC_BASE     0x100   /* 0x100..0x102: mỗi node báo local time */
#define CAN_ID_INPUT_STATUS_BASE  0x200   /* 0x200..0x202: mỗi node báo input state */

#define CAN_ID_TIME_SYNC(node_id)     (CAN_ID_TIME_SYNC_BASE + (node_id))
#define CAN_ID_INPUT_STATUS(node_id)  (CAN_ID_INPUT_STATUS_BASE + (node_id))

/* --- Payload: TIME_SYNC (8 bytes) ---------------------------------------
 * timestamp_ms: mốc thời gian nội bộ (ms) của node gửi, little-endian.
 * Node nhận dùng giá trị này để tính offset đồng bộ (xem middleware/time_sync).
 * -------------------------------------------------------------------------*/
typedef struct __attribute__((packed)) {
    uint64_t timestamp_ms;
} can_msg_time_sync_t;

/* --- Payload: INPUT_STATUS (3 bytes) ------------------------------------
 * input1 / input2: trạng thái digital input (0/1)
 * seq: số thứ tự tăng dần, giúp node khác phát hiện mất gói / node reset.
 * -------------------------------------------------------------------------*/
typedef struct __attribute__((packed)) {
    uint8_t input1;
    uint8_t input2;
    uint8_t seq;
} can_msg_input_status_t;

#endif /* COMMON_CAN_PROTOCOL_H */
