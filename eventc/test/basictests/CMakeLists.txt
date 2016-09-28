cmake_minimum_required(VERSION 2.8)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runTests test_example.cpp)

target_link_libraries(runTests gtest gtest_main)
add_test(runTests runTests )
