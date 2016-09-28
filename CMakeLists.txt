cmake_minimum_required(VERSION 2.8)
project(Rover2017)

set(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install)
set(NANOPB_DIR ${INSTALL_DIR}/nanopb-0.3.6-linux-x86)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    ${NANOPB_DIR}/extra
)


include_directories("${PROJECT_SOURCE_DIR}/common")

find_package(Nanopb REQUIRED)

add_subdirectory(common)
add_subdirectory(base)

add_dependencies(packets iogen)
