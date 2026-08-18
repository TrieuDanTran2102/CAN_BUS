#ifndef DRIVER_XL2515_XL2515_REGS_H
#define DRIVER_XL2515_XL2515_REGS_H

/* ============================================================================
 * XL2515 là chip clone pin-to-pin & register-compatible với Microchip MCP2515,
 * nên toàn bộ SPI command set và register map dưới đây theo chuẩn MCP2515.
 * ==========================================================================*/

/* --- SPI instruction set -------------------------------------------------*/
#define XL2515_CMD_RESET        0xC0
#define XL2515_CMD_READ         0x03
#define XL2515_CMD_WRITE        0x02
#define XL2515_CMD_RTS(n)       (0x80 | (1 << (n)))  /* n = 0,1,2 (TXB0..2) */
#define XL2515_CMD_READ_STATUS  0xA0
#define XL2515_CMD_RX_STATUS    0xB0
#define XL2515_CMD_BIT_MODIFY   0x05

/* --- Control registers ---------------------------------------------------*/
#define XL2515_REG_CANSTAT   0x0E
#define XL2515_REG_CANCTRL   0x0F
#define XL2515_REG_CNF3      0x28
#define XL2515_REG_CNF2      0x29
#define XL2515_REG_CNF1      0x2A
#define XL2515_REG_CANINTE   0x2B
#define XL2515_REG_CANINTF   0x2C
#define XL2515_REG_EFLG      0x2D

/* --- TX buffer 0 (dùng cho bản basic, chỉ cần 1 TX buffer) --------------*/
#define XL2515_REG_TXB0CTRL  0x30
#define XL2515_REG_TXB0SIDH  0x31
#define XL2515_REG_TXB0SIDL  0x32
#define XL2515_REG_TXB0DLC   0x35
#define XL2515_REG_TXB0D0    0x36

/* --- RX buffer 0 -----------------------------------------------------------*/
#define XL2515_REG_RXB0CTRL  0x60
#define XL2515_REG_RXB0SIDH  0x61
#define XL2515_REG_RXB0SIDL  0x62
#define XL2515_REG_RXB0DLC   0x65
#define XL2515_REG_RXB0D0    0x66

/* --- RX buffer 1 -----------------------------------------------------------*/
#define XL2515_REG_RXB1CTRL  0x70
#define XL2515_REG_RXB1SIDH  0x71
#define XL2515_REG_RXB1SIDL  0x72
#define XL2515_REG_RXB1DLC   0x75
#define XL2515_REG_RXB1D0    0x76

/* --- RX filter (dùng RXM0 = mask 0 => nhận tất cả ID, đơn giản cho basic) */
#define XL2515_REG_RXM0SIDH  0x20
#define XL2515_REG_RXM0SIDL  0x21
#define XL2515_REG_RXM1SIDH  0x24
#define XL2515_REG_RXM1SIDL  0x25

/* --- CANCTRL / CANSTAT mode bits -----------------------------------------*/
#define XL2515_MODE_NORMAL      0x00
#define XL2515_MODE_SLEEP       0x20
#define XL2515_MODE_LOOPBACK    0x40
#define XL2515_MODE_LISTENONLY  0x60
#define XL2515_MODE_CONFIG      0x80
#define XL2515_MODE_MASK        0xE0

/* --- CANINTF flags --------------------------------------------------------*/
#define XL2515_CANINTF_RX0IF  0x01
#define XL2515_CANINTF_RX1IF  0x02
#define XL2515_CANINTF_TX0IF  0x04

/* --- RXB0CTRL: nhận mọi standard/extended frame, không lọc (basic mode) -*/
#define XL2515_RXB0CTRL_RXM_ANY  0x60

#endif /* DRIVER_XL2515_XL2515_REGS_H */
