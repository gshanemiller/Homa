cmake_minimum_required(VERSION 3.16)

set(CFGUTIL_SOURCES
  cfgutil/cfgutil_validate.cpp
)

add_library(cfgutil_opt ${CFGUTIL_SOURCES})
target_compile_options(cfgutil_opt PUBLIC -O2)
target_compile_definitions(cfgutil_opt PUBLIC NDEBUG)
target_include_directories(cfgutil_opt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cfgutil)

add_library(cfgutil_dbg ${CFGUTIL_SOURCES})
target_compile_options(cfgutil_dbg PUBLIC -O0 -g)
target_include_directories(cfgutil_dbg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cfgutil)

set(CFGPARSE_SOURCES
  cfgparse/cfgparse_dpdk.cpp
  cfgparse/cfgparse_nic.cpp
  cfgparse/cfgparse_memzone.cpp
  cfgparse/cfgparse_mempool.cpp
  cfgparse/cfgparse_rxq.cpp
  cfgparse/cfgparse_txq.cpp
  cfgparse/cfgparse_thread.cpp
)

add_library(cfgparse_opt ${CFGPARSE_SOURCES})
target_compile_options(cfgparse_opt PUBLIC -O2)
target_compile_definitions(cfgparse_opt PUBLIC NDEBUG)
target_include_directories(cfgparse_opt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cfgparse)
target_include_directories(cfgparse_opt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cfgutil)

add_library(cfgparse_dbg ${CFGPARSE_SOURCES})
target_compile_options(cfgparse_dbg PUBLIC -O0 -g)
target_include_directories(cfgparse_dbg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cfgparse)
target_include_directories(cfgparse_dbg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cfgutil)
