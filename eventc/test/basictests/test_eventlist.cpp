#include "../../src/eventList.h"


int main(int argc, char *argv[]) {
  char * testListFileName= (char *) "../../eventpy/data/eventlist_small1_sdt.txt";
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

  cout << "Simple event list:" << endl;
  for (int i=0;i<testList.size;i++){
    cout << i << ": " << testList[i].getLongEventInformation()<< endl;
  }

  // Test writing and reading binary file
  testList.WriteFile_Binary("test1.bin");

  EventList<Event_T> testList2=EventList<Event_T>(0);  
  testList2.ReadFile_Binary("test1.bin");

  cout << "Simple event list after writing and reading binary file:" << endl;
  for (int i=0; i < testList2.size; i++) {
    cout << i << ": " << testList[i].getLongEventInformation() << endl;
  }
  
  // Test reading a file, compare result to explicit event list
  EventList<Event_T> testList_file=EventList<Event_T>(0);  
  testList_file.ReadFile_SourceDestTime(testListFileName);
  cout << "The two lists should be the same. This is " << (testList_file == testList) <<"." << endl;

  return 0;
}
