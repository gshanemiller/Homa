cmake_minimum_required(VERSION 3.16)
project(Homa)

# Define location of build artifacts
#
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PDB_OUTPUT_DIRECTORY         ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY     ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY     ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY     ${CMAKE_SOURCE_DIR}/build)

#
# Set project wide compilation options
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-diagnostics-color")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/local/include")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/root/local/include")

#
# DPDK Link Libraries
#
# Requires pre-exported ENV, for example, as done in ./install:
#   PKG_CONFIG_PATH=<path-to-DPDK-pkgconfig>
#
if (DPDK_STATIC_LINK)
  execute_process(
    COMMAND /usr/bin/pkg-config --static --libs libdpdk
    RESULT_VARIABLE DPDK_LIB_RESULT
    OUTPUT_VARIABLE DPDK_SELECTED_LIBRARIES
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  message(STATUS "HOMA will static link to DPDK: ${DPDK_SELECTED_LIBRARIES}")
else()
  execute_process(
    COMMAND /usr/bin/pkg-config --libs libdpdk
    RESULT_VARIABLE DPDK_LIB_RESULT
    OUTPUT_VARIABLE DPDK_SELECTED_LIBRARIES
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  message(STATUS "HOMA will dynamic link to DPDK: ${DPDK_SELECTED_LIBRARIES}")
endif()

#
# Set project wide linker flags
#
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/local/lib")

#
# Add library code for building library
#
add_subdirectory(src)

#
# Add integration tests
#
add_subdirectory(integration_tests)

#
# Add tests
#
# add_subdirectory(unit_tests)
