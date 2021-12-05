file(GLOB SRC_FILES *.c)

add_executable(miscellaneous_example ${SRC_FILES})
target_link_libraries(miscellaneous_example gclados)