cmake_minimum_required(VERSION 3.0)


include_directories("${CMAKE_CURRENT_BINARY_DIR}/../common/")

# Set project sources
set(SOURCES
   main.cpp
   mainwindow.cpp
   serialhandler.cpp
   controlhandler.cpp
   abstractcontroller.cpp
   xboxcontroller.cpp
   frsky.cpp
)

set(HEADERS
    mainwindow.h
    serialhandler.h
    controlhandler.h
    abstractcontroller.h
    xboxcontroller.h
    frsky.h
)

# Set forms
set(FORMS
    mainwindow.ui
)

# Set any resource files
set(RESOURCES
    qml.qrc
)


# Tell CMake to create the main executable
add_executable(main ${SOURCES} ${HEADERS} ${FORMS} ${RESOURCES})

target_link_libraries(main packets)
# Link libraries from Qt5
target_link_libraries(main Qt5::Widgets)
target_link_libraries(main Qt5::Qml)
target_link_libraries(main Qt5::Quick)
target_link_libraries(main Qt5::Gamepad)
target_link_libraries(main Qt5::SerialPort)
target_link_libraries(main Qt5::QuickWidgets)
