project(tests)

add_subdirectory(lib/gtest-1.8.0)

include_directories(lib/gtest-1.8.0/include lib/gtest-1.8.0/src)

add_executable(testEvents test_event.cpp)
target_link_libraries(testEvents gtest gtest_main)

add_executable(testEventList test_eventlist.cpp)
target_link_libraries(testEventList gtest gtest_main)

add_executable(testUserIterator test_user_iterator.cpp)
target_link_libraries(testUserIterator gtest gtest_main)

add_executable(testAll testAll.cpp)
target_link_libraries(testAll gtest)
