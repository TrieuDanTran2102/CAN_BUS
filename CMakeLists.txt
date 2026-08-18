# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.0)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Rp2350 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add subdirectories for drivers and middleware
add_subdirectory(src/Driver/gpio)
add_subdirectory(src/Driver/spi)
add_subdirectory(src/Driver/uart)
add_subdirectory(src/Driver/xl2515)
add_subdirectory(src/Middleware/can)
# add_subdirectory(src/Middleware/time_sync)

# Add executable. Default name is the project name, version 0.1

add_executable(Rp2350 
        src/App/Rp2350.c
        src/App/input/app_input.c
        src/App/can_sync/app_can_sync.c
)

pico_set_program_name(Rp2350 "Rp2350")
pico_set_program_version(Rp2350 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Rp2350 1)
pico_enable_stdio_usb(Rp2350 0)

# Add the standard include files to the build
target_include_directories(Rp2350 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/src
        ${CMAKE_CURRENT_LIST_DIR}/src/Common
        ${CMAKE_CURRENT_LIST_DIR}/src/Driver
        ${CMAKE_CURRENT_LIST_DIR}/src/Middleware
        ${CMAKE_CURRENT_LIST_DIR}/src/App
)

# Add the standard library to the build
target_link_libraries(Rp2350
        pico_stdlib
        hardware_spi
        hardware_uart
        hardware_gpio
        drv_gpio
        drv_spi
        drv_uart
        xl2515
        can_manager
        time_sync
)

pico_add_extra_outputs(Rp2350)

