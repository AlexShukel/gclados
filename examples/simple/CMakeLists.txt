file(GLOB SRC_FILES *.c)

add_executable(simple_example ${SRC_FILES})
target_link_libraries(simple_example gclados)