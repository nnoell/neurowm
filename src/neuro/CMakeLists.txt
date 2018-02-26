cmake_minimum_required (VERSION 3.5)
set(NEUROLIB_MAJOR_VERSION 0)
set(NEUROLIB_MINOR_VERSION 35)
set(NEUROLIB_PATCH_VERSION 0)
set(NEUROLIB_VERSION "${NEUROLIB_MAJOR_VERSION}.${NEUROLIB_MINOR_VERSION}.${NEUROLIB_PATCH_VERSION}")

add_definitions(-DPKG_NAME="neurowm")
add_definitions(-DPKG_MYNAME="myneurowm")
add_definitions(-DPKG_VERSION="0.35")

file(GLOB_RECURSE ALL_C_SOURCES "*.c")
file(GLOB_RECURSE ALL_C_HEADERS "*.h")

add_library(neuro SHARED ${ALL_C_SOURCES} ${ALL_C_HEADERS})
add_library(neuroStatic STATIC ${ALL_C_SOURCES} ${ALL_C_HEADERS})

set_target_properties(neuro PROPERTIES PREFIX "lib")
set_target_properties(neuro PROPERTIES OUTPUT_NAME "neurowm")
set_target_properties(neuro PROPERTIES SUFFIX ".so.${NEUROLIB_VERSION}")

set_target_properties(neuroStatic PROPERTIES PREFIX "lib")
set_target_properties(neuroStatic PROPERTIES OUTPUT_NAME "neurowm")

install (TARGETS neuro DESTINATION lib)
install (TARGETS neuroStatic DESTINATION lib)
install (FILES ${ALL_C_HEADERS} DESTINATION include/neuro)
