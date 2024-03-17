cmake_minimum_required(VERSION 3.16)

set(SOURCES
  main.cpp
  init.cpp
)

add_executable(test_dpdkinit_eal.tsk ${SOURCES})
target_compile_options(test_dpdkinit_eal.tsk PUBLIC -O0 -g)
target_link_libraries(test_dpdkinit_eal.tsk PUBLIC fmt)
target_link_libraries(test_dpdkinit_eal.tsk PUBLIC dpdkinit_dbg)
target_link_libraries(test_dpdkinit_eal.tsk PUBLIC cfgparse_dbg)
target_link_libraries(test_dpdkinit_eal.tsk PUBLIC cfgutil_dbg)
target_link_libraries(test_dpdkinit_eal.tsk PUBLIC ${DPDK_SELECTED_LIBRARIES})
target_link_libraries(test_dpdkinit_eal.tsk PUBLIC gtest)
target_link_libraries(test_dpdkinit_eal.tsk PUBLIC pthread)
