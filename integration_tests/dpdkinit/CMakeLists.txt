cmake_minimum_required(VERSION 3.16)

set(SOURCES
  main.cpp
  dpdkinit.cpp
)

add_executable(test_dpdkinit.tsk ${SOURCES})
target_compile_options(test_dpdkinit.tsk PUBLIC -O0 -g)
target_link_libraries(test_dpdkinit.tsk PUBLIC fmt)
target_link_libraries(test_dpdkinit.tsk PUBLIC dpdkinit_dbg)
target_link_libraries(test_dpdkinit.tsk PUBLIC cfgparse_dbg)
target_link_libraries(test_dpdkinit.tsk PUBLIC cfgutil_dbg)
target_link_libraries(test_dpdkinit.tsk PUBLIC ${DPDK_SELECTED_LIBRARIES})
target_link_libraries(test_dpdkinit.tsk PUBLIC gtest)
target_link_libraries(test_dpdkinit.tsk PUBLIC pthread)
