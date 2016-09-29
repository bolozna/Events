#include "gtest/gtest.h"
#include "../src/event.h"

TEST(EventsTest, testEventWithDuration) {
    EventWithDurationAndType event = EventWithDuration();
    event.setDuration(10);
    event.setTime(10);
    EXPECT_EQ(event.getEndTime(), 20);
}