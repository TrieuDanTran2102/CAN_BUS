add_library(time_sync STATIC
    time_sync.c
)

target_include_directories(time_sync PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Common
)

# time_sync.c dùng NODE_ID trực tiếp (so sánh với TIME_SYNC_MASTER_NODE_ID),
# nên lib này cũng cần được compile với đúng NODE_ID của node đang build,
# giống như target chính.
target_compile_definitions(time_sync PRIVATE
    NODE_ID=${NODE_ID}
)

target_link_libraries(time_sync PUBLIC
    can_manager
    pico_stdlib
)
