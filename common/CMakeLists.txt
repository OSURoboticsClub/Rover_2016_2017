project(packets)

set("${CMAKE_CXX_FLAGS} -Wall -Werror -O3 -g")

include_directories("${CMAKE_SOURCE_DIR}/../base/serial/")

add_custom_command(OUTPUT serialhandler.h serialhandler.cpp
    COMMAND python basepackets.py
    ARGS --dest="${CMAKE_BINARY_DIR}/base/serial/"
    ARGS --src="${CMAKE_SOURCE_DIR}/base/serial/"
    DEPENDS basepackets.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/base/serial/)

add_custom_target(generation
    DEPENDS packethandler.h packethandler.cpp)

set(SOURCES
    packethandler.cpp
)

set(HEADERS
    packethandler.h
)


add_library(packets SHARED ${SOURCES} ${HEADERS})
add_dependencies(packets generation)

target_link_libraries(packets Qt5::Widgets)
target_link_libraries(packets Qt5::SerialPort)
