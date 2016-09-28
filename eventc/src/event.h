#ifndef EVENTS
#define EVENTS

/* The Event class. Objects of this class are used to store single events. 
 * The implementation of event class makes a heavy use of the policy class pattern.
 * Each data element (time stamp, duration, index, reversed status) has two policy classes:
 * for example, TimeStamp and NoTimeStamp for time stamps. Each event class is then constructed
 * by selecting one of these policy classes depending on which data members one wants to use.
 */


typedef unsigned nodeindex;
typedef float timestamp;

#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include "event_policy_classes/durationPolicies.h"
#include "event_policy_classes/timeStampPolicies.h"
#include "event_policy_classes/reversePolicies.h"
#include "event_policy_classes/timeStampPolicies.h"
#include "event_policy_classes/indexPolicies.h"



using namespace std;


/** The main event class. **/
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
  class Event : public TimeStampPolicy, public ReversedPolicy, public DurationPolicy, public IndexPolicy
{
 public:
  nodeindex source;
  nodeindex dest;

  void readFromFile_tsd(ifstream &the_file);
  void readFromFile_sdt(ifstream &the_file);
  void readFromFile_sdtd(ifstream &the_file);
  void readFromFile_all(ifstream &the_file);

  void writeToFile_tsd(ofstream &the_file);
  void writeToFile_sdt(ofstream &the_file);
  void writeToFile_sdtd(ofstream &the_file);
  void writeToFile_all(ofstream &the_file);

  void copyAllButTime(Event& targetEvent);
  nodeindex getSource();
  nodeindex getDest();
  Event(); //Constructor
  Event( const Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy> &event);//Copy constructor
  Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy> getReversedEvent();
  string getLongEventInformation();
  string getShortEventInformation();
  static string getEventTypeStr();

  bool operator==(const Event & other) const;
  bool operator!=(const Event & other) const {return !((*this)==other);}
  
  friend ostream& operator << (ostream& os, const Event& event)// overload the operator <<
 {
  os << event.source << " " << event.dest << " ";
  return os ;
 }
  
};

template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
  bool Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::operator==(const Event & other) const{
  return (this->source==other.source && this->dest==other.dest && this->isEqualTime(other) && this->isEqualReversed(other) && this->isEqualDuration(other) && this->isEqualIndex(other) );
}

//function returning the source node of an event
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
  nodeindex Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::getSource(){
  return this->source;
}

//function returning the destination node of an event
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
nodeindex Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::getDest(){
  return this->dest;
}

//a way to read an event from the_file
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
void Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::readFromFile_tsd(ifstream &the_file){
  this->readTime(the_file);
  the_file >> this->source;
  the_file >> this->dest;
}

//another way to read an event from the_file
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
void Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::readFromFile_sdt(ifstream &the_file){
  the_file >> this->source;
  the_file >> this->dest;
  this->readTime(the_file);
}

//another way to read an event from the_file
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
void Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::readFromFile_sdtd(ifstream &the_file){
  the_file >> this->source;
  the_file >> this->dest;
  this->readTime(the_file);
  this->readDuration(the_file);
}

//another way to read an event from the_file
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
void Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::readFromFile_all(ifstream &the_file){
  the_file >> this->source;
  the_file >> this->dest;
  this->readTimeConditional(the_file);
  this->readReversedConditional(the_file);
  this->readDurationConditional(the_file);
  this->readIndexConditional(the_file);
}

// --- Writing --- 

template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
void Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::writeToFile_tsd(ofstream &the_file){
  this->writeTime(the_file);
  the_file << " ";
  the_file << this->source;
  the_file << " ";
  the_file << this->dest;
}

template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
void Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::writeToFile_sdt(ofstream &the_file){
  the_file << this->source;
  the_file << " ";
  the_file << this->dest;
  the_file << " ";
  this->writeTime(the_file);
}

template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
void Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::writeToFile_sdtd(ofstream &the_file){
  the_file << this->source;
  the_file << " ";
  the_file << this->dest;
  the_file << " ";
  this->writeTime(the_file);
  the_file << " ";
  this->writeDuration(the_file);
}

template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
void Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::writeToFile_all(ofstream &the_file){
  the_file << this->source;
  the_file << " ";
  the_file << this->dest;
  this->writeTimeConditional(the_file);
  this->writeReversedConditional(the_file);
  this->writeDurationConditional(the_file);
  this->writeIndexConditional(the_file);
}

// --- Constructors ---

//copy the source and destination of an event to targetEvent
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
void Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::copyAllButTime(Event& targetEvent){
  targetEvent.source=this->source;
  targetEvent.dest=this->dest;
}

//Constructor
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
  Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::Event() : TimeStampPolicy(),ReversedPolicy(),DurationPolicy(),IndexPolicy(){
}

//Copy constructor
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
  Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::Event( const Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy> &event) : TimeStampPolicy(event),ReversedPolicy(event),DurationPolicy(event),IndexPolicy(event){
    this->source=event.source;
    this->dest=event.dest;
}


//create the reversed event of the considered event with same characteristics 
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy> Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::getReversedEvent(){
  Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy> newEvent=Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>(*this);
  newEvent.toggleReversed();
  newEvent.dest=this->source;
  newEvent.source=this->dest;
  return newEvent;
}


//print information about the considered event
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
string Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::getLongEventInformation(){
  stringstream s;
  s << "Event(source: " << this->source << ", dest: " << this->dest <<", " << this->getLongTimeStr()<< ", "<< this->getLongReverseStr() << ", " << this->getLongDurationStr() << ", " << this->getLongIndexStr() << ")";
  return s.str();
}

//print information about the considered event
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
string Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::getShortEventInformation(){
  stringstream s;
  s << "(" << this->source << "," << this->dest;
  string time=this->getShortTimeStr();
  if(time.length()!=0){s<<","<<time;}
  string reverse=this->getShortReverseStr();
  if(reverse.length()!=0){s<<","<<reverse;}
  string duration=this->getShortDurationStr();
  if(duration.length()!=0){s<<","<<duration;}
  string index=this->getShortIndexStr();
  if(index.length()!=0){s<<","<<index;}
  s<<")";
  return s.str();
}

//print information about the considered event type
template <typename TimeStampPolicy, typename ReversedPolicy, typename DurationPolicy, typename IndexPolicy>
string Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::getEventTypeStr(){
  stringstream s;
  s << "(source,dest";
  const string time=Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::timeTypeStr;
  if (time.length()!=0) {
    s<<","<<time;
  }
  const string reverse = Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::reverseTypeStr;
  if( reverse.length()!=0) {
    s<<","<<reverse;
  }
  const string duration = Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::durationTypeStr;
  if(duration.length()!=0){s<<","<<duration;}
  const string index = Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::indexTypeStr;
  if(index.length()!=0){s<<","<<index;}
  s<<")";
  return s.str();
}



//Systematic names for event types
typedef Event<NoTimeStamp, NoReverse, NoDuration, NoIndex> Event_;
typedef Event<TimeStamp, NoReverse, NoDuration, NoIndex> Event_T;
typedef Event<TimeStamp, Reverse, NoDuration, NoIndex> Event_TR;
typedef Event<TimeStamp, Reverse, Duration, NoIndex> Event_TRD;
typedef Event<TimeStamp, Reverse, NoDuration, Index> Event_TRI;
typedef Event<TimeStamp, Reverse, Duration, Index> Event_TRDI;
typedef Event<TimeStamp, NoReverse, Duration, NoIndex> Event_TD;
typedef Event<TimeStamp, NoReverse, Duration, Index> Event_TDI;
typedef Event<TimeStamp, NoReverse, NoDuration, Index> Event_TI;
typedef Event<NoTimeStamp, Reverse, NoDuration, NoIndex> Event_R;
typedef Event<NoTimeStamp, Reverse, Duration, NoIndex> Event_RD;
typedef Event<NoTimeStamp, Reverse, NoDuration, Index> Event_RI;
typedef Event<NoTimeStamp, Reverse, Duration, Index> Event_RDI;
typedef Event<NoTimeStamp, NoReverse, Duration, NoIndex> Event_D;
typedef Event<NoTimeStamp, NoReverse, Duration, Index> Event_DI;
typedef Event<NoTimeStamp, NoReverse, NoDuration, Index> Event_I;


//Some simple names for event types
typedef Event<NoTimeStamp, NoReverse, NoDuration,NoIndex> SimpleEvent;
typedef Event<TimeStamp, NoReverse, NoDuration,NoIndex> EventWithTime;
typedef Event<TimeStamp, Reverse, NoDuration,Index> EventWithTimeRev;
typedef Event<TimeStamp, NoReverse, Duration,NoIndex> EventWithDuration;



//Comparing events

template<class EventType>
bool compare_sdt(const EventType & event1,const EventType & event2){//compare 2 events according to the source, then the destination, then the time
  if (event1.source!=event2.source)
    return event1.source<event2.source;
  else if (event1.dest!=event2.dest)
    return event1.dest<event2.dest;
  else
    return event1.getTime()<event2.getTime();
}

template<class EventType>
bool compare_std(const EventType & event1,const EventType & event2){//compare 2 events according to the source, then the time, then the destination
  if (event1.source!=event2.source)
    return event1.source<event2.source;
  else if (event1.getTime()!=event2.getTime())
    return event1.getTime()<event2.getTime();
  else
    return event1.dest<event2.dest;
}

template<class EventType>
bool compare_st(const EventType & event1,const EventType & event2){//compare 2 events according to the source, then the time 
  if (event1.source!=event2.source)
    return event1.source<event2.source;
  else
    return event1.getTime()<event2.getTime();
}


template<class EventType>
bool compare_sd(const EventType & event1,const EventType & event2){//compare 2 events according to the source, then the destination 
  if (event1.source!=event2.source)
    return event1.source<event2.source;
  else
    return event1.dest<event2.dest;
}


template<class EventType>
bool compare_t(const EventType & event1,const EventType & event2){//compare 2 events according to the time
  return event1.getTime()<event2.getTime();
}

template<class EventType>
bool compare_s(const EventType & event1,const EventType & event2){//compare 2 events according to the source
  return event1.source<event2.source;
}



#endif //EVENTS
