#include "xl2515.h"
#include "xl2515_regs.h"
#include "common/project_config.h"
#include "driver/gpio/drv_gpio.h"
#include "pico/time.h"
#include <string.h>

/* ---------------------------------------------------------------------------
 * Low-level SPI helpers
 * -------------------------------------------------------------------------*/
static void xl2515_reg_write(xl2515_t *dev, uint8_t reg, uint8_t value)
{
    uint8_t buf[3] = { XL2515_CMD_WRITE, reg, value };
    drv_spi_cs_select(&dev->spi);
    drv_spi_write(&dev->spi, buf, sizeof(buf));
    drv_spi_cs_deselect(&dev->spi);
}

static uint8_t xl2515_reg_read(xl2515_t *dev, uint8_t reg)
{
    uint8_t tx[3] = { XL2515_CMD_READ, reg, 0x00 };
    uint8_t rx[3] = { 0 };
    drv_spi_cs_select(&dev->spi);
    drv_spi_write_read(&dev->spi, tx, rx, sizeof(tx));
    drv_spi_cs_deselect(&dev->spi);
    return rx[2];
}

static void xl2515_reg_write_burst(xl2515_t *dev, uint8_t start_reg,
                                    const uint8_t *data, size_t len)
{
    uint8_t header[2] = { XL2515_CMD_WRITE, start_reg };
    drv_spi_cs_select(&dev->spi);
    drv_spi_write(&dev->spi, header, sizeof(header));
    drv_spi_write(&dev->spi, data, len);
    drv_spi_cs_deselect(&dev->spi);
}

static void xl2515_reg_read_burst(xl2515_t *dev, uint8_t start_reg,
                                   uint8_t *data, size_t len)
{
    uint8_t header[2] = { XL2515_CMD_READ, start_reg };
    drv_spi_cs_select(&dev->spi);
    drv_spi_write(&dev->spi, header, sizeof(header));
    drv_spi_read(&dev->spi, data, len);
    drv_spi_cs_deselect(&dev->spi);
}

static void xl2515_bit_modify(xl2515_t *dev, uint8_t reg, uint8_t mask, uint8_t value)
{
    uint8_t buf[4] = { XL2515_CMD_BIT_MODIFY, reg, mask, value };
    drv_spi_cs_select(&dev->spi);
    drv_spi_write(&dev->spi, buf, sizeof(buf));
    drv_spi_cs_deselect(&dev->spi);
}

static void xl2515_reset(xl2515_t *dev)
{
    uint8_t cmd = XL2515_CMD_RESET;
    drv_spi_cs_select(&dev->spi);
    drv_spi_write(&dev->spi, &cmd, 1);
    drv_spi_cs_deselect(&dev->spi);
    sleep_ms(10); /* chip cần thời gian ổn định sau reset, theo datasheet MCP2515 */
}

static status_t xl2515_set_mode(xl2515_t *dev, uint8_t mode)
{
    xl2515_bit_modify(dev, XL2515_REG_CANCTRL, XL2515_MODE_MASK, mode);

    /* Đợi CANSTAT phản ánh đúng mode, timeout ~100ms cho an toàn */
    for (int i = 0; i < 100; i++) {
        if ((xl2515_reg_read(dev, XL2515_REG_CANSTAT) & XL2515_MODE_MASK) == mode) {
            return STATUS_OK;
        }
        sleep_ms(1);
    }
    return STATUS_TIMEOUT;
}

/* ---------------------------------------------------------------------------
 * Bit timing: chỉ hỗ trợ sẵn cấu hình 500kbps @ 8MHz oscillator (giá trị
 * chuẩn dùng phổ biến cho MCP2515-compatible ở tốc độ này). Nếu oscillator
 * thực tế trên board khác 8MHz hoặc cần tốc độ khác 500kbps, cần tính lại
 * CNF1/CNF2/CNF3 theo datasheet.
 * -------------------------------------------------------------------------*/
static status_t xl2515_apply_bit_timing(xl2515_t *dev)
{
    if (dev->osc_hz != XL2515_OSC_HZ) {
        /* Bản basic chỉ có bảng cấu hình sẵn cho 8MHz. */
        return STATUS_INVALID_PARAM;
    }

    xl2515_reg_write(dev, XL2515_REG_CNF1, 0x00);
    xl2515_reg_write(dev, XL2515_REG_CNF2, 0x90);
    xl2515_reg_write(dev, XL2515_REG_CNF3, 0x02);
    return STATUS_OK;
}

status_t xl2515_init(xl2515_t *dev)
{
    if (dev == NULL) {
        return STATUS_INVALID_PARAM;
    }

    status_t st = drv_spi_init(&dev->spi);
    if (st != STATUS_OK) {
        return st;
    }

    drv_gpio_init(dev->int_pin, DRV_GPIO_DIR_INPUT, DRV_GPIO_PULL_UP);

    xl2515_reset(dev);

    /* Reset đưa chip về Configuration mode sẵn, set lại cho chắc */
    st = xl2515_set_mode(dev, XL2515_MODE_CONFIG);
    if (st != STATUS_OK) {
        return st;
    }

    st = xl2515_apply_bit_timing(dev);
    if (st != STATUS_OK) {
        return st;
    }

    /* Mask = 0 => nhận mọi ID, không lọc gì cả (đơn giản cho bản basic) */
    xl2515_reg_write(dev, XL2515_REG_RXM0SIDH, 0x00);
    xl2515_reg_write(dev, XL2515_REG_RXM0SIDL, 0x00);
    xl2515_reg_write(dev, XL2515_REG_RXM1SIDH, 0x00);
    xl2515_reg_write(dev, XL2515_REG_RXM1SIDL, 0x00);

    /* RXB0: nhận mọi standard/extended frame, rollover sang RXB1 khi đầy */
    xl2515_reg_write(dev, XL2515_REG_RXB0CTRL, XL2515_RXB0CTRL_RXM_ANY | 0x04);

    xl2515_reg_write(dev, XL2515_REG_CANINTE,
                      XL2515_CANINTF_RX0IF | XL2515_CANINTF_RX1IF);

    return xl2515_set_mode(dev, XL2515_MODE_NORMAL);
}

status_t xl2515_send(xl2515_t *dev, const xl2515_can_frame_t *frame)
{
    if (dev == NULL || frame == NULL || frame->dlc > XL2515_MAX_DLC) {
        return STATUS_INVALID_PARAM;
    }

    /* Đợi TXB0 rảnh (bit TXREQ trong TXB0CTRL = 0), timeout ~50ms */
    for (int i = 0; i < 50; i++) {
        if ((xl2515_reg_read(dev, XL2515_REG_TXB0CTRL) & 0x08) == 0) {
            break;
        }
        sleep_ms(1);
        if (i == 49) {
            return STATUS_BUSY;
        }
    }

    /* Standard 11-bit ID: SIDH = id[10:3], SIDL = id[2:0] << 5 */
    uint8_t sidh = (uint8_t)((frame->id >> 3) & 0xFF);
    uint8_t sidl = (uint8_t)((frame->id & 0x07) << 5);

    uint8_t header[5] = { sidh, sidl, 0x00 /* EID8 */, 0x00 /* EID0 */, frame->dlc };
    xl2515_reg_write_burst(dev, XL2515_REG_TXB0SIDH, header, sizeof(header));

    if (frame->dlc > 0) {
        xl2515_reg_write_burst(dev, XL2515_REG_TXB0D0, frame->data, frame->dlc);
    }

    /* RTS (Request To Send) cho TXB0 */
    uint8_t rts = XL2515_CMD_RTS(0);
    drv_spi_cs_select(&dev->spi);
    drv_spi_write(&dev->spi, &rts, 1);
    drv_spi_cs_deselect(&dev->spi);

    return STATUS_OK;
}

bool xl2515_frame_available(xl2515_t *dev)
{
    uint8_t intf = xl2515_reg_read(dev, XL2515_REG_CANINTF);
    return (intf & (XL2515_CANINTF_RX0IF | XL2515_CANINTF_RX1IF)) != 0;
}

status_t xl2515_receive(xl2515_t *dev, xl2515_can_frame_t *frame)
{
    if (dev == NULL || frame == NULL) {
        return STATUS_INVALID_PARAM;
    }

    uint8_t intf = xl2515_reg_read(dev, XL2515_REG_CANINTF);
    uint8_t buf_sidh_reg, clear_flag;

    if (intf & XL2515_CANINTF_RX0IF) {
        buf_sidh_reg = XL2515_REG_RXB0SIDH;
        clear_flag = XL2515_CANINTF_RX0IF;
    } else if (intf & XL2515_CANINTF_RX1IF) {
        buf_sidh_reg = XL2515_REG_RXB1SIDH;
        clear_flag = XL2515_CANINTF_RX1IF;
    } else {
        return STATUS_NOT_READY;
    }

    uint8_t header[5] = { 0 };
    xl2515_reg_read_burst(dev, buf_sidh_reg, header, sizeof(header));

    uint8_t sidh = header[0];
    uint8_t sidl = header[1];
    uint8_t dlc  = header[4] & 0x0F;

    frame->id  = ((uint32_t)sidh << 3) | (sidl >> 5);
    frame->dlc = (dlc > XL2515_MAX_DLC) ? XL2515_MAX_DLC : dlc;

    if (frame->dlc > 0) {
        /* D0 nằm ngay sau SIDH,SIDL,EID8,EID0,DLC => +5 register */
        xl2515_reg_read_burst(dev, buf_sidh_reg + 5 /* offset tới D0 */,
                               frame->data, frame->dlc);
    }

    /* Clear interrupt flag đã xử lý */
    xl2515_bit_modify(dev, XL2515_REG_CANINTF, clear_flag, 0x00);

    return STATUS_OK;
}
