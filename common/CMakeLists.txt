cmake_minimum_required(VERSION 2.8)

include_directories(${NANOPB_INCLUDE_DIRS})
message(STATUS "${NANOPB_INCLUDE_DIRS}")
add_custom_command(OUTPUT io_gen.h io_gen.c
                   COMMAND python3 filegen.py
                   ARGS --dest="${CMAKE_CURRENT_BINARY_DIR}"
                   DEPENDS filegen.py
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
                   
add_custom_target(iogen
                  DEPENDS io_gen.h io_gen.c)


nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS packets.proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(generate_proto_sources DEPENDS ${PROTO_SRCS} ${PROTO_HDRS})
set_source_files_properties(${PROTO_SRCS} ${PROTO_HDRS}
    PROPERTIES GENERATED TRUE)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -g -O0")

set(SOURCES
    packets.pb.c
    io.c
    io_gen.c
)

set(HEADERS
    packets.pb.h
    io.h
    io_gen.h
)

add_library(packets SHARED ${SOURCES} ${HEADERS} ${PROTO_SRCS} ${PROTO_HDRS})
