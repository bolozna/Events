#include "gtest/gtest.h"
#include "../src/eventList.h"


TEST(TestSuite, RunningAllShufflingMethods) {
  // This is just to test that everything compiles and runs.
  // Results are not checked.

  EventList<Event_T> testList = EventList<Event_T>(5,0,7);
    testList[0].source = 0;
    testList[0].dest = 1;
    testList[0].setTime(1);
    testList[1].source = 0;
    testList[1].dest = 2;
    testList[1].setTime(2);
    testList[2].source = 0;
    testList[2].dest = 1;
    testList[2].setTime(4);
    testList[3].source = 2;
    testList[3].dest = 0;
    testList[3].setTime(5);
    testList[4].source = 0;
    testList[4].dest = 1;
    testList[4].setTime(7);

    testList.Shuffle_AddOffset(1);
    testList.Shuffle_AllButTime(1);
    testList.Shuffle_Time(1);
    testList.Shuffle_LinkIETsKeepFirst(1);
    testList.Shuffle_UniformlyRandomTimes(1);
    testList.Shuffle_UniformlyRandomTimesKeepLinkEnds(1);

    testList.Shuffle_LinkSequence(1);
    testList.Shuffle_EventsToRandomLinks(1);

    testList.Shuffle_RandomNodes(1,false);
    testList.Shuffle_RandomNodes(1,true);
    
    //testList.Shuffle_UndirectedConfigurationModel(1,true);

}

