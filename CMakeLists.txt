cmake_minimum_required(VERSION 3.20)
project(gclados C)

set(CMAKE_C_STANDARD 11)

include_directories(lib)
add_subdirectory(lib)
add_subdirectory(cli)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(examples)
endif ()
