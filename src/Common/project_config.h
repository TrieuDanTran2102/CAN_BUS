#ifndef COMMON_PROJECT_CONFIG_H
#define COMMON_PROJECT_CONFIG_H

/* ------------------------------------------------------------------------
 * NODE IDENTITY
 * Mỗi node trong hệ thống 3-node CAN phải build với 1 NODE_ID khác nhau.
 * Cách đơn giản nhất cho bản "basic": set qua compile definition khi build,
 * ví dụ trong CMakeLists.txt add_subdirectory / target_compile_definitions
 * cho từng target (node0/node1/node2), hoặc build riêng lẻ với:
 *
 *   cmake -DNODE_ID=0 ..
 *
 * Nếu không truyền vào, default là 0 (dùng để build/test 1 node đơn lẻ).
 * ------------------------------------------------------------------------ */
#ifndef NODE_ID
#define NODE_ID 0
#endif

#define NODE_COUNT 3

/* Node 0 đóng vai trò "time master" - phát timestamp gốc cho cả bus.
 * Node 1, 2 nhận và đồng bộ theo. Đây là mô hình đơn giản nhất
 * (không phải PTP thực thụ) phù hợp cho bản basic. */
#define TIME_SYNC_MASTER_NODE_ID   0

/* ------------------------------------------------------------------------
 * TIMING
 * ------------------------------------------------------------------------ */
#define TIME_SYNC_PERIOD_MS        500     /* master broadcast timestamp mỗi 500ms */
#define INPUT_SAMPLE_PERIOD_MS     50      /* đọc input mỗi 50ms (debounce đơn giản) */
#define INPUT_BROADCAST_PERIOD_MS  200     /* broadcast trạng thái input mỗi 200ms,
                                               hoặc ngay khi input thay đổi */

/* ------------------------------------------------------------------------
 * CAN BUS
 * ------------------------------------------------------------------------ */
#define CAN_BITRATE_500KBPS  500000UL

/* XL2515 (MCP2515-compatible) onboard oscillator.
 * NOTE: Waveshare không công bố rõ trị số này trong tài liệu public.
 * 8MHz là giá trị phổ biến nhất cho các module MCP2515-compatible.
 * -> Nếu tốc độ CAN thực tế sai lệch, kiểm tra lại schematic/datasheet
 *    của board và sửa giá trị này cho khớp phần cứng thật.
 */
#define XL2515_OSC_HZ        8000000UL

#endif /* COMMON_PROJECT_CONFIG_H */
