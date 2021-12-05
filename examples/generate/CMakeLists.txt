file(GLOB SRC_FILES *.c)

add_executable(generate_example ${SRC_FILES})
target_link_libraries(generate_example gclados)