add_library(drv_spi STATIC
    drv_spi.c
)

target_include_directories(drv_spi PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Common
)

target_link_libraries(drv_spi PUBLIC
    drv_gpio
    hardware_spi
)
