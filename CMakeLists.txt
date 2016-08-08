cmake_minimum_required(VERSION 2.8)
project(Rover2017)

set(NANOPB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/install/nanopb-0.3.6-linux-x86/extra)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${NANOPB_PATH})
find_package(Nanopb REQUIRED)

add_subdirectory(common)
