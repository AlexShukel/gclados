cmake_minimum_required(VERSION 3.20)
project(propag_testing_framework C)

set(CMAKE_C_STANDARD 99)

add_executable(cli cli/main.c cli/help.c)
add_executable(example example/sum.c ./lib/ptf.c)