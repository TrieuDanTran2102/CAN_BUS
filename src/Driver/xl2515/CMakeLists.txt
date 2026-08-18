add_library(xl2515 STATIC
    xl2515.c
)

target_include_directories(xl2515 PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Common
)

target_link_libraries(xl2515 PUBLIC
    drv_spi
    drv_gpio
    pico_stdlib
)
