add_library(can_manager STATIC
    can_manager.c
)

target_include_directories(can_manager PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Common
)

target_link_libraries(can_manager PUBLIC
    xl2515
)
