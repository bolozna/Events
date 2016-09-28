cmake_minimum_required(VERSION 2.8)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(../../src/ ../../build})

add_executable(runAllTests test_example.cpp)


target_link_libraries(runAllTests gtest gtest_main)
