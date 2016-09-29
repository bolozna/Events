//
// Created by Rainer Kujala on 29/09/16.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedImportStatement"

#include "test_event.cpp"
#include "test_eventlist.cpp"
#include "test_user_iterator.cpp"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#pragma clang diagnostic pop