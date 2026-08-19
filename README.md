# CAN_BUS Project

## 1. Introduction

This project is a firmware running on the Raspberry Pi Pico RP2350 board, designed to communicate over the CAN bus and process digital input signals. The main goals of the project are:

- read the status of 2 digital inputs;
- transmit data over the CAN bus using a predefined message structure;
- receive CAN data from other nodes;
- synchronize time between nodes on the CAN bus;
- provide a foundation for a multi-node system in the future.

The project is implemented with a layered architecture, separating driver, middleware, application, and system configuration code.

---

## 2. What is CAN bus?

CAN (Controller Area Network) is a serial communication protocol designed for connecting multiple devices in environments such as automotive systems, robotics, industrial control systems, and real-time sensor networks.

### 2.1 Key characteristics of CAN

- Multi-node bus: multiple devices share the same communication line.
- Message-based communication, not address-based like Ethernet or UART.
- Supports bus access priority through arbitration.
- Reliable transmission in noisy and electromagnetically harsh environments.
- Common transmission speeds include 125 kbps, 500 kbps, and 1 Mbps.

### 2.2 CAN signal structure

CAN uses two signal wires:

- CAN_H: high line
- CAN_L: low line

The logic levels are not simple 0/1 values; they are represented by the voltage difference between CAN_H and CAN_L:

- Dominant: logic 0, indicates the bus is active and a node has priority.
- Recessive: logic 1, indicates the bus is idle.

This mechanism allows multiple nodes to compete for the bus without data collisions.

### 2.3 CAN messages

A basic CAN frame contains the following parts:

- ID: identifies the message type.
- DLC: data length code.
- Data: payload.
- CRC / ACK / EOF: used for error checking and acknowledgment.

In this project, the ID uses the standard 11-bit CAN format, grouped by message type:

- time sync message
- input status message

---

## 3. Current project description

This project is building a CAN node on the RP2350 board connected to the XL2515 CAN controller module. The XL2515 is a CAN interface chip compatible with MCP2515 and is mounted on the Waveshare RP2350-CAN board.

### 3.1 Role of each component

- RP2350: MCU responsible for logic control and application processing.
- XL2515: CAN controller that sends and receives CAN frames over the bus.
- SPI: interface between RP2350 and XL2515.
- GPIO: reads digital input signals.
- UART: used for debug logs over serial output.

### 3.2 Current functional goals

The project currently provides the following features:

1. Initialize the required drivers.
2. Read 2 input states from GPIO.
3. Send input status over the CAN bus.
4. Receive CAN frames from other nodes on the bus.
5. Handle time synchronization using the `time sync` message.
6. Run in the main loop using a polling model.

---

## 4. Software architecture

The project is divided into clearly separated layers:

- App layer: handles application logic.
- Middleware layer: handles CAN and time synchronization logic.
- Driver layer: interfaces with hardware.
- Common layer: shared configuration and protocol definitions.

### 4.1 Application layer

- `src/App/Rp2350.c`
  - main file.
  - initializes the system, runs the main loop, and calls application tasks.
- `src/App/can_sync/app_can_sync.c`
  - handles received CAN messages related to time synchronization and input status.
- `src/App/input/app_input.c`
  - reads the 2 GPIO inputs and stores their local states.

### 4.2 Middleware layer

- `src/Middleware/can/can_manager.c`
  - manages the CAN bus.
  - initializes the XL2515 module, sends frames, receives frames, and invokes callbacks when data arrives.
- `src/Middleware/can/can_manager.h`
  - defines the CAN API used by the application layer.
- `src/Middleware/time_sync/`
  - planned or currently under development for node-to-node time synchronization.

### 4.3 Driver layer

- `src/Driver/xl2515/xl2515.c` and `xl2515.h`
  - direct driver for the CAN controller chip.
  - handles CAN frame send/receive operations.
- `src/Driver/spi/drv_spi.c`
  - SPI bus wrapper.
- `src/Driver/gpio/drv_gpio.c`
  - GPIO wrapper.
- `src/Driver/uart/drv_uart.c`
  - UART driver used for debugging.

### 4.4 Common layer

- `src/Common/project_config.h`
  - configures node ID, sync timing, bitrate, and XL2515 oscillator settings.
- `src/Common/board_config.h`
  - defines GPIO, SPI, UART, and input pin mapping.
- `src/Common/can_protocol.h`
  - defines CAN IDs and message payloads.
- `src/Common/types.h`
  - defines shared data types.

---

## 5. Project structure inside the CAN_BUS folder

Here is the actual folder structure of the project inside the `CAN_BUS` directory:

```text
CAN_BUS/
├── CMakeLists.txt
├── pico_sdk_import.cmake
├── README.md
├── build/
│   ├── build.ninja
│   ├── CMakeCache.txt
│   ├── compile_commands.json
│   ├── Rp2350.uf2
│   ├── generated/
│   └── CMakeFiles/
├── src/
│   ├── App/
│   │   ├── Rp2350.c
│   │   ├── can_sync/
│   │   │   ├── app_can_sync.c
│   │   │   └── app_can_sync.h
│   │   └── input/
│   │       ├── app_input.c
│   │       └── app_input.h
│   ├── Common/
│   │   ├── board_config.h
│   │   ├── can_protocol.h
│   │   ├── project_config.h
│   │   └── types.h
│   ├── Driver/
│   │   ├── gpio/
│   │   │   ├── CMakeLists.txt
│   │   │   ├── drv_gpio.c
│   │   │   └── drv_gpio.h
│   │   ├── spi/
│   │   │   ├── CMakeLists.txt
│   │   │   ├── drv_spi.c
│   │   │   └── drv_spi.h
│   │   ├── uart/
│   │   │   ├── CMakeLists.txt
│   │   │   ├── drv_uart.c
│   │   │   └── drv_uart.h
│   │   └── xl2515/
│   │       ├── CMakeLists.txt
│   │       ├── xl2515.c
│   │       ├── xl2515.h
│   │       └── xl2515_regs.h
│   ├── Middleware/
│   │   ├── can/
│   │   │   ├── CMakeLists.txt
│   │   │   ├── can_manager.c
│   │   │   └── can_manager.h
│   │   └── time_sync/
│   │       ├── CMakeLists.txt
│   │       ├── time_sync.c
│   │       └── time_sync.h
│   └── Service/
│       └── (under development / not yet fully defined)
└── README.md
```

### 5.1 Layered architecture diagram

```text
┌───────────────────────────────┐
│          Application          │
│  app_input / app_can_sync     │
└──────────────┬────────────────┘
               │
┌──────────────▼────────────────┐
│         Middleware            │
│       CAN manager / sync      │
└──────────────┬────────────────┘
               │
┌──────────────▼────────────────┐
│           Driver              │
│  GPIO / SPI / UART / XL2515   │
└──────────────┬────────────────┘
               │
┌──────────────▼────────────────┐
│         Hardware              │
│ RP2350 + CAN controller      │
└───────────────────────────────┘
```

---

## 6. Main execution flow

### 6.1 System initialization

Inside `main()` in `src/App/Rp2350.c`, the program performs the following steps:

1. initialize stdio UART;
2. call `app_can_sync_init()`;
3. call `app_input_init()`;
4. begin the main loop.

### 6.2 Main loop

Each cycle, the program executes:

- `app_can_sync_task()`
  - calls `can_manager_poll()`
  - handles received CAN messages
  - runs time synchronization logic
- `app_input_task()`
  - reads the two GPIO states periodically
- prints the input status to the debug UART log

### 6.3 CAN send and receive flow

- `can_manager_send()` uses `xl2515_send()` to send a frame onto the bus.
- `can_manager_poll()` repeatedly reads all available CAN frames from the RX buffer.
- When a new frame arrives, the callback `on_can_rx()` is invoked.
- The callback analyzes the frame ID and payload. If it is a time synchronization or input status message, it is processed appropriately.

---

## 7. CAN message structure in the project

The project defines 2 main payload types in `src/Common/can_protocol.h`.

### 7.1 `CAN_ID_TIME_SYNC_BASE = 0x100`

Used for time synchronization. The number of nodes is set to `NODE_COUNT = 3`, and the master node is `TIME_SYNC_MASTER_NODE_ID = 0`.

Payload definition:

```c
typedef struct __attribute__((packed)) {
    uint64_t timestamp_ms;
} can_msg_time_sync_t;
```

### 7.2 `CAN_ID_INPUT_STATUS_BASE = 0x200`

Used to transmit the status of 2 digital inputs.

Payload definition:

```c
typedef struct __attribute__((packed)) {
    uint8_t input1;
    uint8_t input2;
    uint8_t seq;
} can_msg_input_status_t;
```

---

## 8. Hardware configuration

The file `src/Common/board_config.h` contains the actual pin mapping of the board:

- XL2515 SPI port: `spi1`
- SCK: GP10
- MOSI: GP11
- MISO: GP12
- CS: GP9
- INT: GP8
- Input 1: GP20
- Input 2: GP21
- UART debug: UART1, TX=GP4, RX=GP5

This is important because the board connects the CAN controller directly to SPI1, not SPI0. This is a key point to remember when debugging CAN communication.

---

## 9. CMake and build

The project uses CMake + Pico SDK. The main files are:

- `CMakeLists.txt`
- `pico_sdk_import.cmake`

CMake builds the project into a `.uf2` file for the Pico, which can then be flashed using Pico tool or OpenOCD.

### Basic build

```bash
cmake -S . -B build
cmake --build build
```

### Build output

- `.uf2` / hex files are generated in the `build/` directory
- project target name: `Rp2350`

---

## 10. Conclusion

The `CAN_BUS` project is a sample firmware demonstrating how to transfer data between nodes on a CAN bus, using the RP2350 board together with the XL2515 CAN controller module. It has a clear layered structure and is suitable for expansion with additional features such as:

- adding more nodes to the bus;
- sending input states periodically or on event-driven triggers;
- improving time synchronization accuracy;
- adding watchdogs, fault detection, and EEPROM configuration;
- developing more advanced control modules such as motors, relays, or sensors.

With this model, each layer has a distinct responsibility, making the project easier to maintain and extend.

---

## 11. Short summary

- CAN bus is a message-based protocol that is ideal for multi-node systems.
- This project uses RP2350 + XL2515 to create a CAN node.
- Input data is read from GPIO and transmitted in CAN message format.
- The project architecture is divided into `App`, `Middleware`, `Driver`, and `Common` layers.
- Each folder has a clear responsibility, helping with integration and long-term maintenance.

---

If you want, I can also create a more polished GitHub-style version of this README with:

1. hardware block diagram;
2. file-by-file explanation for each `.c` and `.h` file;
3. flash and debug instructions for Pico;
4. a version tailored for GitHub, portfolio, or team documentation.
