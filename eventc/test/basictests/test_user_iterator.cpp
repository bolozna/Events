#include "../../src/eventList.h"

char * testListFileName= (char *) "../../../eventpy/data/eventlist_small1_sdt.txt";

int main(int argc, char *argv[]){
  EventList<Event_TR> testList=EventList<Event_TR>(0);
  testList.ReadFile_SourceDestTime(testListFileName);
  testList.AddReversed();

  testList.Sort_SourceTime();
  
  for (int i=0;i<testList.size;i++){
    cout << i<< ": " << testList[i].getLongEventInformation()<< endl;
  }

  
  for(EventList<Event_TR>::userIterator uiter=testList.iterUsers();uiter!=testList.end();uiter++){
    cout << uiter.user << endl;
    for(EventList<Event_TR>::userEventIterator ueiter=*uiter;
	ueiter!=uiter.end();
	ueiter++){
      Event_TR event= *ueiter;
      cout << event.getLongEventInformation() << endl;
    }
  }
  
  return 1;
}
