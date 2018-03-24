# BUILD OPTIONS:
# --------------
#
# Default prefix with multihead support:
#   $ cd build
#   $ cmake ..
#   $ sudo make install
#
# Custom prefix without multihead support:
#   $ cd build
#   $ cmake -DCMAKE_INSTALL_PREFIX:PATH=/custom/prefix -DWITH_MULTIHEAD:BOOL=FALSE ..
#   $ make install

cmake_minimum_required (VERSION 3.5)

project(neurowm C)

# Variables
set(MAJOR_VERSION 0)
set(MINOR_VERSION 35)

# Directories
set(WORKSPACE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(SRC_DIR "${WORKSPACE_ROOT}/src")

# Files
file(GLOB_RECURSE ALL_C_SOURCES "${SRC_DIR}/neuro/*.c")
file(GLOB_RECURSE ALL_C_HEADERS "${SRC_DIR}/neuro/*.h")

# Defines
add_definitions(-DPKG_NAME="neurowm")
add_definitions(-DPKG_MYNAME="myneurowm")
add_definitions(-DPKG_VERSION="${MAJOR_VERSION}.${MINOR_VERSION}")
if(WITH_MULTIHEAD)
  add_definitions(-DXRANDR)
endif()

# Library
add_library(lib_neuro SHARED "${ALL_C_SOURCES}")
set_target_properties(lib_neuro PROPERTIES
  OUTPUT_NAME neurowm
  PUBLIC_HEADER "${ALL_C_HEADERS}"
  SOVERSION "${MAJOR_VERSION}.${MINOR_VERSION}")
if(WITH_MULTIHEAD)
  target_link_libraries(lib_neuro X11 pthread Xrandr)
else()
  target_link_libraries(lib_neuro X11 pthread)
endif()

# Executable
add_executable(bin_neuro "${SRC_DIR}/main.c")
target_link_libraries(bin_neuro lib_neuro)
set_target_properties(bin_neuro PROPERTIES
  OUTPUT_NAME neurowm
  INSTALL_RPATH_USE_LINK_PATH TRUE)

# Install
install(TARGETS bin_neuro lib_neuro
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/neuro
  PUBLIC_HEADER DESTINATION include/neuro)
