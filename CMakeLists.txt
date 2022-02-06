cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(OmegaGo VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(include/glad)

add_executable(twoplayer twoplayer.cpp)

target_link_libraries(twoplayer PUBLIC Glad)

target_include_directories(
    twoplayer PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/include/**"
)
