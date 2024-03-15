cmake_minimum_required(VERSION 3.16)

set(DPDKINIT_SOURCES
  dpdkinit/dpdkinit_eal.cpp
)

add_library(dpdkinit_opt ${DPDKINIT_SOURCES})
target_compile_options(dpdkinit_opt PUBLIC -O2)
target_compile_definitions(dpdkinit_opt PUBLIC NDEBUG)
target_include_directories(dpdkinit_opt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dpdkinit)
target_include_directories(dpdkinit_opt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../cfg/cfgutil)
target_include_directories(dpdkinit_opt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../cfg/cfgparse)

add_library(dpdkinit_dbg ${DPDKINIT_SOURCES})
target_compile_options(dpdkinit_dbg PUBLIC -O0 -g)
target_include_directories(dpdkinit_dbg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dpdkinit)
target_include_directories(dpdkinit_dbg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../cfg/cfgutil)
target_include_directories(dpdkinit_dbg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../cfg/cfgparse)
