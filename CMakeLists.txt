cmake_minimum_required (VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(neurowm C)

set(WORKSPACE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE ALL_C_SOURCES "${WORKSPACE_ROOT}/src/neuro/*.c")
file(GLOB_RECURSE ALL_C_HEADERS "${WORKSPACE_ROOT}/src/neuro/*.h")

add_definitions(-DPKG_NAME="neurowm")
add_definitions(-DPKG_MYNAME="myneurowm")
add_definitions(-DPKG_VERSION="0.1")

add_executable(neurowm ${ALL_C_SOURCES} ${ALL_C_HEADERS} "${WORKSPACE_ROOT}/src/main.c")

target_link_libraries(neurowm X11 Xrandr pthread)
