
set(PACKETS_SOURCES
    "${CMAKE_BINARY_DIR}/common/packets.cpp"
)

set(PACKETS_HEADERS
    "${CMAKE_BINARY_DIR}/common/packets.h"
)


add_custom_command(
    OUTPUT ${PACKETS_SOURCES} ${PACKETS_HEADERS}
    COMMAND python3 basepackets.py
    ARGS --dest="${CMAKE_BINARY_DIR}/common/"
    ARGS --src="${CMAKE_SOURCE_DIR}/base/serial/"
    DEPENDS "${CMAKE_SOURCE_DIR}/common/basepackets.py"
    DEPENDS "${CMAKE_SOURCE_DIR}/base/serial/packets.h" "${CMAKE_SOURCE_DIR}/base/serial/packets.cpp"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/common/"
)

qt5_wrap_cpp(GENERATED_MOC ${PACKETS_HEADERS})

add_library(packets STATIC ${GENERATED_MOC} ${PACKETS_SOURCES})

target_link_libraries(packets Qt5::Widgets)
