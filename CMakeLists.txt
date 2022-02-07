cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(OmegaGo VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(engine)
add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(twoplayer)

