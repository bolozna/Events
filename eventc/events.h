#ifndef EVENTS
#define EVENTS

/* The Event class. Objects of this class are used to store single events. 
 * The implemenation of event class makes a heavy use of the policy class pattern.
 * Each data element (time stamp, duration, index, reversed status) has two policy classes:
 * for exmaple, TimeStamp and NoTimeStamp for time stamps. Each event class is then constructed
 * by selecting one of these policy classes depending on which data members one wants to use.
 */


#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>

#define nodeindex unsigned
#define eventindex unsigned
#define timestamp float

#define NOTIME 0
#define DEFAULTDURATION 0
#define DEFAULTDELAY 0
#define NOINDEX 0

using namespace std;

/** Policy classes for storing (or not storing) time stamps. **/
class TimeStamp{
 private:
  timestamp time;
 protected:
  void readTime(ifstream &the_file);
  void writeTime(ofstream &the_file);
  void readTimeConditional(ifstream &the_file);
  void writeTimeConditional(ofstream &the_file);
  bool isEqualTime(const TimeStamp & other) const {return this->time==other.time;}
 public:
  timestamp getTime() const;
  void setTime(timestamp time);
  TimeStamp();
  TimeStamp( const TimeStamp &event);
  string getLongTimeStr();
  string getShortTimeStr();
  static const string timeTypeStr;
};
const string TimeStamp::timeTypeStr = string("time");

timestamp TimeStamp::getTime() const{
  return this->time;
}
void TimeStamp::setTime(timestamp time){
  this->time=time;
}
void TimeStamp::readTime(ifstream &the_file){
  the_file >> this->time;
}
void TimeStamp::writeTime(ofstream &the_file){
  the_file << this->time;
}
void TimeStamp::readTimeConditional(ifstream &the_file){
  the_file >> this->time;
}
void TimeStamp::writeTimeConditional(ofstream &the_file){
  the_file<< " " << this->time;
}
TimeStamp::TimeStamp(){
}
TimeStamp::TimeStamp(const TimeStamp &event){
  this->time=event.time;
}
string TimeStamp::getLongTimeStr(){//convert TimeStamp time into string to print it
  stringstream s; 
  s << this->time;
  return string("time: ") + s.str();
}

string TimeStamp::getShortTimeStr(){
  stringstream s; 
  s << this->time;
  return s.str();
}

 

class NoTimeStamp{
 public:
  timestamp getTime() const;
  void setTime(timestamp time);
  NoTimeStamp();
  NoTimeStamp( const NoTimeStamp &event);
  string getLongTimeStr();
  string getShortTimeStr();
  static const string timeTypeStr;
 protected:
  void readTime(ifstream &the_file);
  void writeTime(ofstream &the_file);
  void readTimeConditional(ifstream &the_file){}
  void writeTimeConditional(ofstream &the_file){}
  static bool isEqualTime(const NoTimeStamp & other) {return true;}
};

const string NoTimeStamp::timeTypeStr=string("");

timestamp NoTimeStamp::getTime() const{
  return NOTIME;
}
void NoTimeStamp::setTime(timestamp time){
}
void NoTimeStamp::readTime(ifstream &the_file){
  timestamp dummy;
  the_file >> dummy;
}
void NoTimeStamp::writeTime(ofstream &the_file){
  the_file << "NaN";
}
NoTimeStamp::NoTimeStamp(){
}
NoTimeStamp::NoTimeStamp(const NoTimeStamp &event){
}
string NoTimeStamp::getLongTimeStr(){
  return string("no time");
}
string NoTimeStamp::getShortTimeStr(){
  return string("");
}

//--


/** Policy classes for storing reversed information. **/
class Reverse{
 private:
  bool reversed; 
 protected:
  void readReversed(ifstream &the_file);
  void writeReversed(ofstream &the_file);
  void readReversedConditional(ifstream &the_file);
  void writeReversedConditional(ofstream &the_file);
  bool isEqualReversed(const Reverse & other) const {return this->reversed==other.reversed;}
 public:
  void toggleReversed(); 
  void setReversed(const bool reversed);
  bool isReversed();
  Reverse();
  Reverse( const Reverse &event);
  string getLongReverseStr();
  string getShortReverseStr();
  static const string reverseTypeStr;
};
const string Reverse::reverseTypeStr=string("reverse");

void Reverse::readReversed(ifstream &the_file){
  the_file >> this->reversed;
}
void Reverse::writeReversed(ofstream &the_file){
  the_file << this->reversed;
}
void Reverse::readReversedConditional(ifstream &the_file){
  the_file >> this->reversed;
}
void Reverse::writeReversedConditional(ofstream &the_file){
  the_file<< " " << this->reversed;
}

void Reverse::toggleReversed(){
  this->reversed = !this->reversed;
}

void Reverse::setReversed(const bool reversed){
  this->reversed = reversed;
}
bool Reverse::isReversed(){
  return this->reversed;
}
Reverse::Reverse(){
  this->reversed=false;
}
Reverse::Reverse(const Reverse &event){
  this->reversed=event.reversed;
}
string Reverse::getLongReverseStr(){
  if(this->reversed){
    return string("reversed: true");
      }else{
    return string("reversed: false");
  }
}
string Reverse::getShortReverseStr(){
  if(this->reversed){
    return string("true");
      }else{
    return string("false");
  }
}


class NoReverse{
 protected:
  void toggleReversed();
  void readReversed(ifstream &the_file);
  void writeReversed(ofstream &the_file);
  void readReversedConditional(ifstream &the_file){}
  void writeReversedConditional(ofstream &the_file){}
  static bool isEqualReversed(const NoReverse & other) {return true;}
 public:
  bool isReversed();
  NoReverse();
  NoReverse( const NoReverse &event);
  string getLongReverseStr();
  string getShortReverseStr();
  static const string reverseTypeStr;
};
const string NoReverse::reverseTypeStr=string("");

void NoReverse::readReversed(ifstream &the_file){
  timestamp dummy;
  the_file >> dummy;
}
void NoReverse::writeReversed(ofstream &the_file){
  the_file << "0"; //Always false
}

void NoReverse::toggleReversed(){
  cout << "Warning: trying to toggle reversed status of an event that cannot be reversed!" <<endl;
} //this should not be called
bool NoReverse::isReversed(){
  return false;
}
NoReverse::NoReverse(){
}
NoReverse::NoReverse(const NoReverse &event){
}
string NoReverse::getLongReverseStr(){
  return string("no reversing");
}
string NoReverse::getShortReverseStr(){
  return string("");
}

//--

/** Policy classes for storing durations. **/
class Duration{
 private:
  timestamp duration;
 protected:
  void readDuration(ifstream &the_file);
  void writeDuration(ofstream &the_file);
  void readDurationConditional(ifstream &the_file);
  void writeDurationConditional(ofstream &the_file);
  bool isEqualDuration(const Duration & other) const {return this->duration==other.duration;}
 public:
  timestamp getDuration() const;
  void setDuration(timestamp duration);
  Duration();
  Duration( const Duration &event);
  string getLongDurationStr();
  string getShortDurationStr();
  static const string durationTypeStr;
};
const string Duration::durationTypeStr=string("duration");

void Duration::readDuration(ifstream &the_file){
  the_file >> this->duration;
}
void Duration::writeDuration(ofstream &the_file){
  the_file << this->duration;
}
void Duration::readDurationConditional(ifstream &the_file){
  the_file >> this->duration;
}
void Duration::writeDurationConditional(ofstream &the_file){
  the_file<< " " << this->duration;
}

timestamp Duration::getDuration() const{
  return this->duration;
}
Duration::Duration(){
}
Duration::Duration(const Duration &event){
  this->duration=event.duration;
}
void Duration::setDuration(timestamp duration){
  this->duration=duration;
}
string Duration::getLongDurationStr(){
  stringstream s;
  s << this->duration;
  return string("duration: ")+s.str();  
}
string Duration::getShortDurationStr(){
  stringstream s;
  s << this->duration;
  return s.str();  
}


class NoDuration{
 public:
  timestamp getDuration() const;
  void setDuration(timestamp duration);
  NoDuration();
  NoDuration( const NoDuration &event);
  string getLongDurationStr();
  string getShortDurationStr();
  static const string durationTypeStr;
 protected:
  void readDuration(ifstream &the_file);
  void writeDuration(ofstream &the_file);
  void readDurationConditional(ifstream &the_file){}
  void writeDurationConditional(ofstream &the_file){}
  static bool isEqualDuration(const NoDuration & other) {return true;}
};
const string NoDuration::durationTypeStr=string("");

timestamp NoDuration::getDuration() const{
  return DEFAULTDURATION;
}
void NoDuration::readDuration(ifstream &the_file){
  timestamp dummy;
  the_file >> dummy;
}
void NoDuration::writeDuration(ofstream &the_file){
  the_file << "NaN";
}
NoDuration::NoDuration(){
}
NoDuration::NoDuration(const NoDuration &event){
}
void NoDuration::setDuration(timestamp duration){
}
string NoDuration::getLongDurationStr(){
  return string("no duration");
}
string NoDuration::getShortDurationStr(){
  return string("");
}


/** Policy classes for storing index. **/
class Index{
 private:
  eventindex index;
 public:
  eventindex getIndex();
  eventindex setIndex(eventindex index);
  Index();
  Index( const Index &event);
  string getLongIndexStr();
  string getShortIndexStr();
  static const string indexTypeStr;
 protected:
  void readIndex(ifstream &the_file);
  void writeIndex(ofstream &the_file);
  void readIndexConditional(ifstream &the_file);
  void writeIndexConditional(ofstream &the_file);
  bool isEqualIndex(const Index & other) const {return this->index==other.index;}
};
const string Index::indexTypeStr=string("index");



eventindex Index::getIndex(){
  return this->index;
}
void Index::readIndex(ifstream &the_file){
  the_file >> this->index;
}
void Index::writeIndex(ofstream &the_file){
  the_file << this->index;
}
void Index::readIndexConditional(ifstream &the_file){
  the_file >> this->index;
}
void Index::writeIndexConditional(ofstream &the_file){
  the_file<< " " << this->index;
}
Index::Index(){
}
Index::Index( const Index &event){
  this->index=event.index;
}
eventindex Index::setIndex(eventindex index){
  this->index=index;
}
string Index::getLongIndexStr(){
  stringstream s;
  s << this->index;
  return string("index: ")+s.str();  
}
string Index::getShortIndexStr(){
  stringstream s;
  s << this->index;
  return s.str();  
}


class NoIndex{
 public:
  eventindex getIndex() const;
  string getLongIndexStr() const;
  string getShortIndexStr() const;
  static const string indexTypeStr;
  void setIndex(int index);
  NoIndex();
  NoIndex( const Index &event);
 protected:
  void readIndex(ifstream &the_file);
  void writeIndex(ofstream &the_file);
  void readIndexConditional(ifstream &the_file){}
  void writeIndexConditional(ofstream &the_file){}
  static bool isEqualIndex(const NoIndex & other) {return true;}
};
const string NoIndex::indexTypeStr=string("");

void NoIndex::readIndex(ifstream &the_file){
  eventindex dummy;
  the_file >> dummy;
}
void NoIndex::writeIndex(ofstream &the_file){
  the_file << "NaN";
}
eventindex NoIndex::getIndex() const{
  return NOINDEX;
}
string NoIndex::getLongIndexStr() const{
  return "no index";
}
string NoIndex::getShortIndexStr() const{
  return "";
}
NoIndex::NoIndex(){
}
NoIndex::NoIndex(const Index &event){
}
void NoIndex::setIndex(int index){
}


//--




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
  if(time.length()!=0){s<<","<<time;}
  const string reverse=Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::reverseTypeStr;
  if(reverse.length()!=0){s<<","<<reverse;}
  const string duration=Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::durationTypeStr;
  if(duration.length()!=0){s<<","<<duration;}
  const string index=Event<TimeStampPolicy,ReversedPolicy,DurationPolicy,IndexPolicy>::indexTypeStr;
  if(index.length()!=0){s<<","<<index;}
  s<<")";
  return s.str();
}



//Systematic names for event types
typedef Event<NoTimeStamp,NoReverse,NoDuration,NoIndex> Event_;
typedef Event<TimeStamp,NoReverse,NoDuration,NoIndex> Event_T;
typedef Event<TimeStamp,Reverse,NoDuration,NoIndex> Event_TR;
typedef Event<TimeStamp,Reverse,Duration,NoIndex> Event_TRD;
typedef Event<TimeStamp,Reverse,NoDuration,Index> Event_TRI;
typedef Event<TimeStamp,Reverse,Duration,Index> Event_TRDI;
typedef Event<TimeStamp,NoReverse,Duration,NoIndex> Event_TD;
typedef Event<TimeStamp,NoReverse,Duration,Index> Event_TDI;
typedef Event<TimeStamp,NoReverse,NoDuration,Index> Event_TI;
typedef Event<NoTimeStamp,Reverse,NoDuration,NoIndex> Event_R;
typedef Event<NoTimeStamp,Reverse,Duration,NoIndex> Event_RD;
typedef Event<NoTimeStamp,Reverse,NoDuration,Index> Event_RI;
typedef Event<NoTimeStamp,Reverse,Duration,Index> Event_RDI;
typedef Event<NoTimeStamp,NoReverse,Duration,NoIndex> Event_D;
typedef Event<NoTimeStamp,NoReverse,Duration,Index> Event_DI;
typedef Event<NoTimeStamp,NoReverse,NoDuration,Index> Event_I;


//Some simple names for event types
typedef Event<NoTimeStamp,NoReverse,NoDuration,NoIndex> SimpleEvent;
typedef Event<TimeStamp,NoReverse,NoDuration,NoIndex> EventWithTime;
typedef Event<TimeStamp,Reverse,NoDuration,Index> EventWithTimeRev;
typedef Event<TimeStamp,NoReverse,Duration,NoIndex> EventWithDuration;



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
