#include "gtest/gtest.h"
#include "../src/eventList.h"


TEST(TestSuite, aTest) {
    // Create a list of events with time, set the size to 5 events

    EventList<Event_T> testList=EventList<Event_T>(5);
    testList[0].source=0;
    testList[0].dest=1;
    testList[0].setTime(1);
    testList[1].source=0;
    testList[1].dest=2;
    testList[1].setTime(2);
    testList[2].source=0;
    testList[2].dest=1;
    testList[2].setTime(4);
    testList[3].source=2;
    testList[3].dest=0;
    testList[3].setTime(5);
    testList[4].source=0;
    testList[4].dest=1;
    testList[4].setTime(7);

    // cout << "Simple event list:" << endl;
    for (int i=0; i<testList.size; i++){
        EXPECT_GT(testList[i].getLongEventInformation().length(), 0);
    }

    // Test writing and reading binary file
    char *testFileName = (char *) "test1.bin";
    testList.WriteFile_Binary(testFileName);

    EventList<Event_T> testList2=EventList<Event_T>(0);
    testList2.ReadFile_Binary(testFileName);

    if (remove(testFileName) != 0 ) {
        cout << "Error while deleting file " << testFileName << endl;
    }


    // cout << "Simple event list after writing and reading binary file:" << endl;
    for (int i=0; i < testList2.size; i++) {
        Event_T e1 = testList[i];
        Event_T e2 = testList2[i];

        EXPECT_GT(testList2[i].getLongEventInformation().length(), 0);

        EXPECT_EQ(e1.getDest(), e2.getDest());
        EXPECT_EQ(e1.getSource(), e2.getSource());
        EXPECT_EQ(e1.getTime(), e2.getTime());
    }

    // Test reading a file, compare result to explicit event list
    EventList<Event_T> testList_file = EventList<Event_T>(0);

    char* testListFileName = (char *) "../eventpy/data/eventlist_small1_sdt.txt";

    // cout << "Reading in some SourceDestTime data" << endl;
    testList_file.ReadFile_SourceDestTime(testListFileName);
    EXPECT_EQ(testList_file.size, testList.size);


}