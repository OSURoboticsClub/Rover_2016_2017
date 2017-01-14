cmake_minimum_required(VERSION 3.0)
project(Rover2017)

set(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install)


########## Qt Configuration ##########

# Point CMake to the Qt 5.7 location
# TODO: add more paths to account for different install locations
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}
    "/opt/Qt/5.7/gcc_64"
    "~/Qt/5.7/gcc_64"
)

# Find the necessary Qt libraries
find_package(Qt5Widgets 5.7 REQUIRED)
find_package(Qt5Qml 5.7 REQUIRED)
find_package(Qt5Quick 5.7 REQUIRED)
find_package(Qt5Gamepad 5.7 REQUIRED)
find_package(Qt5SerialPort 5.7 REQUIRED)
find_package(Qt5QuickWidgets 5.7 REQUIRED)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Automatically handle resource files
set(CMAKE_AUTORCC ON)
# Automatically handle ui forms
set(CMAKE_AUTOUIC ON)

add_definitions(${Qt5Widgets_DEFINITIONS})

########## SFML Configuration ##########

# Detect and add SFML
set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2.3 REQUIRED COMPONENTS window)

include_directories(${SFML_INCLUDE_DIR})


########## Subdirectories ##########
add_subdirectory(common)
add_subdirectory(base)
add_subdirectory(test)
