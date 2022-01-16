file(GLOB SRC_FILES *.c)

set(CMAKE_C_FLAGS_RELEASE "-O3")

add_executable(cli ${SRC_FILES})
set_target_properties(cli PROPERTIES OUTPUT_NAME "gclados")
target_link_libraries(cli gclados)