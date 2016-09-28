cmake_minimum_required(VERSION 2.8)
project(eventc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        test/basictests/test_eventlist.cpp
        test/basictests/test_user_iterator.cpp
        src/eventList.h
        src/eventNet.h
        src/event.h src/event_policy_classes/reversePolicies.h src/event_policy_classes/indexPolicies.h)

add_executable(eventc ${SOURCE_FILES})

add_subdirectory(src)
add_subdirectory(test)
