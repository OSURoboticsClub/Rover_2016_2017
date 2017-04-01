cmake_minimum_required(VERSION 3.0)
project(Rover2017)

set(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install)


########## Qt Configuration ##########

# Point CMake to the Qt 5.7 location
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}
    "/opt/Qt/5.7/gcc_64"
    "/opt/Qt5.7.0/5.7/gcc_64"
    "~/Qt5.7.0/5.7/gcc_64"
    "~/Qt/5.7/gcc_64"
    "~/Qt5.7.0/5.7/gcc_64"
)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH}
    "/opt/Qt/5.7/gcc_64/qml"
    "/opt/Qt5.7.0/5.7/gcc_64/qml"
    "~/Qt5.7.0/5.7/gcc_64/qml"
    "~/Qt/5.7/gcc_64/qml"
    "~/Qt5.7.0/5.7/gcc_64/qml"
)

# Find the necessary Qt libraries
find_package(Qt5Widgets 5.7 REQUIRED)
find_package(Qt5Qml 5.7 REQUIRED)
find_package(Qt5Quick 5.7 REQUIRED)
find_package(Qt5SerialPort 5.7 REQUIRED)
find_package(Qt5QuickWidgets 5.7 REQUIRED)
find_package(Qt5WebEngine 5.7 REQUIRED)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Automatically handle resource files
set(CMAKE_AUTORCC ON)

add_definitions(${Qt5Widgets_DEFINITIONS})


########## Subdirectories ##########
add_subdirectory(common)
add_subdirectory(base)
