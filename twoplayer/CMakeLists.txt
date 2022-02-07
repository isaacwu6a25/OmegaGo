add_executable(twoplayer twoplayer.cpp ../include/board_gui.h)

target_link_libraries(twoplayer PUBLIC 
    engine
    Glad
    glfw
)

target_include_directories(
    twoplayer PUBLIC
    ${PROJECT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)