cmake_minimum_required(VERSION 2.8.11)

project(testproject)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)


# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Qml)
find_package(Qt5Quick)

# Tell CMake to create the helloworld executable
add_executable(main main.cpp qml.qrc)

# Use the Widgets module from Qt 5.
target_link_libraries(main Qt5::Widgets)
target_link_libraries(main Qt5::Qml)
target_link_libraries(main Qt5::Quick)


