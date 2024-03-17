cmake_minimum_required(VERSION 3.16)

set(SOURCES
  main.cpp
  dpdk.cpp
  nic.cpp
  memzone.cpp
  mempool.cpp
  rxq.cpp
  txq.cpp
  thread.cpp
)

add_executable(test_cfgparse_config.tsk ${SOURCES})
target_compile_options(test_cfgparse_config.tsk PUBLIC -O0 -g)
target_link_libraries(test_cfgparse_config.tsk PUBLIC fmt)
target_link_libraries(test_cfgparse_config.tsk PUBLIC cfgparse_dbg)
target_link_libraries(test_cfgparse_config.tsk PUBLIC cfgutil_dbg)
target_link_libraries(test_cfgparse_config.tsk PUBLIC gtest)
target_link_libraries(test_cfgparse_config.tsk PUBLIC pthread)
