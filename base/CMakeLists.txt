cmake_minimum_required(VERSION 3.0.0)

# Point CMake to the Qt 5.7 location
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} 
    "/opt/Qt/5.7/gcc_64"
)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Automatically handle resource files
set(CMAKE_AUTORCC ON)
# Automatically handle ui forms
set(CMAKE_AUTOUIC ON)

add_definitions(${Qt5Widgets_DEFINITIONS})

# Find the necessary Qt libraries
find_package(Qt5Widgets 5.7 REQUIRED)
find_package(Qt5Qml 5.7 REQUIRED)
find_package(Qt5Quick 5.7 REQUIRED)
find_package(Qt5Gamepad 5.7 REQUIRED)
find_package(Qt5SerialPort 5.7 REQUIRED)

# Set project sources
set(SOURCES
   main.cpp
   mainwindow.cpp
   gamepadmonitor.cpp
)

# Set forms
set(FORMS
    mainwindow.ui
)

# Set any resource files
set(RESOURCES
    qml.qrc
)

include_directories(${NANOPB_INCLUDE_DIRS})
include_directories(${PROJECT_BINARY_DIR}/common)


# Tell CMake to create the main executable
add_executable(main ${SOURCES} ${FORMS} ${RESOURCES})

target_link_libraries(main packets)

# Link libraries from Qt5
target_link_libraries(main Qt5::Widgets)
target_link_libraries(main Qt5::Qml)
target_link_libraries(main Qt5::Quick)
target_link_libraries(main Qt5::Gamepad)
target_link_libraries(main Qt5::SerialPort)
