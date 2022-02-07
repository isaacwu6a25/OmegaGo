add_library(Glad glad.c glad.h KHR/khrplatform.h)

target_include_directories(Glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/KHR
)