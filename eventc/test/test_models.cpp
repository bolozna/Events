#include "gtest/gtest.h"
#include "../src/eventList.h"
#include "../src/models.h"


TEST(TestSuite, UniformlyRandomEventsModel) {
  // This is just to test that everything compiles and runs.
  // Results are not checked.

  size_t nodes= 10;
  size_t events=30;
  timestamp startTime=0;
  timestamp endTime=10;

  EventList<Event_T> testList = EventList<Event_T>();
  Model_UniformlyRandomEvents(testList,nodes,events,startTime,endTime,1234);

  EXPECT_EQ(testList.size,30);

  for (size_t i=0;i<events;i++){
    EXPECT_GT(testList[i].getTime(),startTime);
    EXPECT_GT(endTime,testList[i].getTime());

    EXPECT_GT(testList[i].source+1,0);
    EXPECT_GT(nodes,testList[i].source);

    EXPECT_GT(testList[i].dest+1,0);
    EXPECT_GT(nodes,testList[i].dest);
  }
}

