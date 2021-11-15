file(GLOB SRC_FILES *.c)

add_library(ptf ${SRC_FILES} C:/cygwin/lib/w32api/libimagehlp.a)