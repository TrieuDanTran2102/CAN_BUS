add_library(drv_uart STATIC
    drv_uart.c
)

target_include_directories(drv_uart PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Common
)

target_link_libraries(drv_uart PUBLIC
    hardware_uart
)
