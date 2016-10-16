cmake_minimum_required(VERSION 3.0)
project(Rover2017)

set(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install)

include_directories("${CMAKE_BINARY_DIR}/common")
include_directories("${CMAKE_SOURCE_DIR}/common")

add_subdirectory(base)
add_subdirectory(common)
add_subdirectory(test)
