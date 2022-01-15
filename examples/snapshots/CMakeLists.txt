file(GLOB SRC_FILES *.c)

add_executable(snapshots_example ${SRC_FILES})
target_link_libraries(snapshots_example gclados)