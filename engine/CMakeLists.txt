add_library(engine engine.h engine.cpp)

target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )