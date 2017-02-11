cmake_minimum_required(VERSION 3.0)

include_directories("${CMAKE_BINARY_DIR}/common/")


# Set project sources
set(SOURCES
   main.cpp
   mainwindow.cpp
   serial/serialhandler.cpp

   threadarray.cpp

   miniboardupdater.cpp
  
   inputs/controllerhandler.cpp
   inputs/abstractcontroller.cpp
   inputs/xboxcontroller.cpp
   inputs/frsky.cpp
)

set(HEADERS
    mainwindow.h
    messagehandler.h
    serial/serialhandler.h
    serial/call_once.h
    serial/singleton.h

    threadarray.h

    miniboardupdater.h

    inputs/controllerhandler.h
    inputs/abstractcontroller.h
    inputs/xboxcontroller.h
    inputs/frsky.h
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
add_executable(main ${GENERATED_MOC} ${SOURCES} ${HEADERS} ${FORMS} ${RESOURCES})
target_link_libraries(main packets)

# Link libraries from Qt5
target_link_libraries(main Qt5::Widgets)
target_link_libraries(main Qt5::Qml)
target_link_libraries(main Qt5::Quick)
target_link_libraries(main Qt5::Gamepad)
target_link_libraries(main Qt5::SerialPort)
target_link_libraries(main Qt5::QuickWidgets)
target_link_libraries(main ${SFML_LIBRARIES})

