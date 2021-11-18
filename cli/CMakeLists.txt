file(GLOB SRC_FILES *.c)
add_executable(cli ${SRC_FILES})
target_link_libraries(cli gclados)