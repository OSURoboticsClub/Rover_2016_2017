project(packets)

set("${CMAKE_CXX_FLAGS} -Wall -Werror -O3 -g")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../base/")

add_custom_command(OUTPUT commgen.h commgen.c
    COMMAND python base_packets.py
    ARGS --dest="${CMAKE_CURRENT_BINARY_DIR}"
    ARGS --src="${CMAKE_CURRENT_SOURCE_DIR}"
    DEPENDS base_packets.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(generation
    DEPENDS commgen.h commgen.c)

set(SOURCES
    commgen.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../base/comm.cpp)

set(HEADERS
    commgen.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../base/comm.h)


add_library(packets SHARED ${SOURCES} ${HEADERS})
add_dependencies(packets generation)

target_link_libraries(packets Qt5::Widgets)
target_link_libraries(packets Qt5::SerialPort)
