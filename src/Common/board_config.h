#ifndef COMMON_BOARD_CONFIG_H
#define COMMON_BOARD_CONFIG_H

/* ============================================================================
 * Waveshare RP2350-CAN — pin map lấy đúng theo sơ đồ chân chính hãng.
 *
 * QUAN TRỌNG: XL2515 (CAN controller) trên board này được hàn cứng vào SPI1,
 * KHÔNG PHẢI SPI0. Nếu bạn dùng GP16-19 (SPI0) sẽ không giao tiếp được với
 * chip CAN onboard vì phần cứng không nối tới đó.
 * ==========================================================================*/

/* ---------------- XL2515 CAN controller (onboard, fixed wiring) --------- */
#define BOARD_XL2515_SPI_PORT   spi1
#define BOARD_XL2515_PIN_SCK    10   /* SPI1 SCK  */
#define BOARD_XL2515_PIN_MOSI   11   /* SPI1 TX   */
#define BOARD_XL2515_PIN_MISO   12   /* SPI1 RX   */
#define BOARD_XL2515_PIN_CS     9    /* SPI1 CSn  */
#define BOARD_XL2515_PIN_INT    8    /* GPIO input, active-low interrupt */

#define BOARD_XL2515_SPI_BAUDRATE  (10 * 1000 * 1000)  /* 10 MHz SPI clock */

/* ---------------- Onboard LED -------------------------------------------- */
#define BOARD_LED_PIN            25

/* ---------------- Debug UART (UART1, matches silkscreen GP4/GP5) -------- */
#define BOARD_DEBUG_UART_PORT     uart1
#define BOARD_DEBUG_UART_TX_PIN   4
#define BOARD_DEBUG_UART_RX_PIN   5
#define BOARD_DEBUG_UART_BAUDRATE 115200

/* ---------------- Digital inputs (2 inputs theo yêu cầu) -----------------
 * Chọn 2 GPIO còn trống, không đụng tới các chân đã dùng cho XL2515/LED.
 * Đổi lại nếu bạn có phần cứng nối vào chân khác.
 * -------------------------------------------------------------------------*/
#define BOARD_INPUT1_PIN   20
#define BOARD_INPUT2_PIN   21

#endif /* COMMON_BOARD_CONFIG_H */
