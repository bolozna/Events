cmake_minimum_required(VERSION 2.8)

project(eventc C CXX)



# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        src/eventList.h
        src/eventNet.h
        src/event.h
        src/event_policy_classes/durationPolicies.h
        src/event_policy_classes/indexPolicies.h
        src/event_policy_classes/kindPolicies.h
        src/event_policy_classes/reversePolicies.h
        src/event_policy_classes/timeStampPolicies.h)

add_executable(eventc ${SOURCE_FILES})
set_target_properties(eventc PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(src)
add_subdirectory(test)
