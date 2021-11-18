file(GLOB SRC_FILES *.c)

add_executable(example ${SRC_FILES})
target_link_libraries(example gclados)