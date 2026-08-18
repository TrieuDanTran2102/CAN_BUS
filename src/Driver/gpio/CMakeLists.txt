add_library(drv_gpio STATIC
    drv_gpio.c
)

target_include_directories(drv_gpio PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Common
)

target_link_libraries(drv_gpio PUBLIC
    hardware_gpio
)
