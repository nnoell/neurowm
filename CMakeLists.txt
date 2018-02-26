cmake_minimum_required (VERSION 3.5)
# set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

project(neurowm C)
set(neurowm_MAJOR_VERSION 0)
set(neurowm_MINOR_VERSION 35)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(WORKSPACE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC_DIR "${WORKSPACE_ROOT}/src")
set(NEURO_DIR "${SRC_DIR}/neuro")

include_directories("${SRC_DIR}/neuro")
add_subdirectory("${SRC_DIR}/neuro")

add_executable(neurowm "${SRC_DIR}/main.c")
set_target_properties(neurowm PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(neurowm neuro X11 Xrandr pthread)

install (TARGETS neurowm DESTINATION bin)
