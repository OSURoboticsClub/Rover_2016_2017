cmake_minimum_required(VERSION 3.0)
project(Rover2017)

set(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#set(SFML_STATIC_LIBRARIES TRUE)

include_directories("${CMAKE_BINARY_DIR}/common/")
include_directories("${CMAKE_SOURCE_DIR}/common/")


# Point CMake to the Qt 5.7 location
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}
    "/opt/Qt/5.7/gcc_64"
)

# Find the necessary Qt libraries
find_package(Qt5Widgets 5.7 REQUIRED)
find_package(Qt5Qml 5.7 REQUIRED)
find_package(Qt5Quick 5.7 REQUIRED)
find_package(Qt5Gamepad 5.7 REQUIRED)
find_package(Qt5SerialPort 5.7 REQUIRED)
find_package(Qt5QuickWidgets 5.7 REQUIRED)

find_package(SFML 2 COMPONENTS window REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Automatically handle resource files
set(CMAKE_AUTORCC ON)
# Automatically handle ui forms
set(CMAKE_AUTOUIC ON)

add_definitions(${Qt5Widgets_DEFINITIONS})

add_subdirectory(base)
add_subdirectory(common)
add_subdirectory(test)
