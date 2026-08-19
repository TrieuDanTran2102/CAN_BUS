#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

// Optional includes - only if files exist
// #include "common/board_config.h"
// #include "common/project_config.h"
// #include "driver/gpio/drv_gpio.h"
// #include "driver/uart/drv_uart.h"
#include "app/can_sync/app_can_sync.h"
#include "app/input/app_input.h"
// #include "middleware/time_sync/time_sync.h"
// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart1
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5

#define TICK_INTERVAL_MS 250

static uint32_t ctrl_trip_count = 0;
static uint32_t clean_windows   = 0;
static uint8_t  node_state      = 0;


int main()
{
    stdio_init_all();
    sleep_ms(1000); 
    printf("RP2350 CAN Project Starting...\n");

    
    // Initialize 2 digital inputs
    app_can_sync_init();
    app_input_init();
    


    // printf("RP2350 CAN Project Started\n");
    // printf("Reading 2 digital inputs...\n");
    printf("Tick_MS, CTRL_IN, STATUS_IN, Trip_Count, State, Clean_Win\n");
    uint32_t last_tick_ms = to_ms_since_boot(get_absolute_time());
    while (true) {
        // Process input reading and broadcasting
        app_can_sync_task();
        app_input_task();

        uint32_t now_ms = to_ms_since_boot(get_absolute_time());

        /*process charactic even 250 ms*/
        if(now_ms - last_tick_ms >= TICK_INTERVAL_MS){
            bool ctrl_in = app_input_get(0);
            
        }

        // Read current input states
        // bool input1 = app_input_get(0);
        // bool input2 = app_input_get(1);

        // Print input status every 500ms
        static uint32_t last_print_ms = 0;
        if (now_ms - last_tick_ms >= TICK_INTERVAL_MS) {
            last_tick_ms = now_ms;

            // 1. Lấy mẫu tại tick hiện tại (Sampled every tick)
            bool ctrl_in   = app_input_get(0); // Input 1
            bool status_in = app_input_get(1); // Input 2

            // 2. Đếm số lần CTRL_IN nhảy lên mức High
            if (ctrl_in == true) {
                ctrl_trip_count++;
            }

            // 3. LOGIC CHUYỂN TRẠNG THÁI & BỘ ĐẾM (STATE MACHINE)
            // LƯU Ý QUAN TRỌNG: Biến `ctrl_in` TUYỆT ĐỐI KHÔNG xuất hiện ở đoạn code này!
            if (node_state == 0 /* REATTEST */) {
                if (status_in == false) {
                    clean_windows++; // Tăng liên tục bất chấp CTRL_IN đang là gì
                }
            }
            printf("%u, %d, %d, %u, %u, %u\n", 
                   now_ms, 
                   ctrl_in, 
                   status_in, 
                   ctrl_trip_count, 
                   node_state,
                   clean_windows);
        }


        // if (now_ms - last_print_ms >= 500) {
        //     last_print_ms = now_ms;
        //     printf("[%u ms] Input1: %s, Input2: %s\n", 
        //            now_ms, 
        //            input1 ? "HIGH" : "LOW",
        //            input2 ? "HIGH" : "LOW");
        // }

        sleep_ms(10);  // Small delay to prevent CPU spinning
    }
}
