#ifndef EVENTLIST
#define EVENTLIST

/*
 * EventList class. This class is used for storing lists of events and doing basic operations for them,
 * such as sorting, shuffling, writing and reading.
 */


#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <stdint.h> //For the type sizes
#include <vector>
#include <map>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include "event.h"
#include "../../lcelib/Randgens.H"
#include "../../lcelib/Nets.H"
#include "../../lcelib/nets/NetExtras.H"
#include "../../lcelib/nets/Randomizer.H"
#include "event_policy_classes/timeStampPolicies.h"


// We want to hash pairs. Why is this not part of the STD? 
template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}
namespace std{
  template<typename S, typename T> struct hash<pair<S, T> >
  {
    inline size_t operator()(const pair<S, T> & v) const
    {
      size_t seed = 0;
      ::hash_combine(seed, v.first);
      ::hash_combine(seed, v.second);
      return seed;
    }
  };
}




template<class ElementType>
void shuffle_vector(vector<ElementType> &v, RandNumGen<> &r){
  if (v.size()!=0){
    for(size_t i = 0; i < v.size()-1;i++){
      size_t j=i+r.next(v.size()-i);
      ElementType tempValue;
      tempValue=v[i];
      v[i]=v[j];
      v[j]=tempValue;
    }
  }
}


template<class ElementType>
void sample_without_replacement(vector<ElementType> &v,size_t n, size_t k,RandNumGen<> &r){ 
  v.reserve(k);
  if(n<2*k || n<100){ //dense case
    vector<size_t> all;
    all.reserve(n);
    for (size_t i=0;i<n;i++){
      all.push_back(i);
    }

    for(size_t i = 0; i < k;i++){
      size_t j=i+r.next(all.size()-i);
      v.push_back(all[j]);
      all[j]=all[i];
    }
  }else{ //sparse case
    unordered_set<size_t> selected;
    size_t i=0;
    while(i<k){      
      size_t candidate=r.next(n);
      if (selected.count(candidate)==0){
	v.push_back(candidate);
	i++;
	selected.insert(candidate);
      }
    }
    }
}


template<class EventType> 
class EventList{
 private:
  int numberOfNodes; //-1 if not known  
  bool stdSorted; //Source, time, dest
  bool sdtSorted; // ...
  bool stSorted;
  bool sdSorted;
  bool tSorted;
  bool sSorted;
  void SwapLinkSequences(size_t index1,size_t index2);
  void SetNewTopology(vector<pair<nodeindex,nodeindex> > & edgeList);

 public:
  void SetSizeByFile(char* fileName); //Counts the number of rows in a file and sets the size of this list to match it
  vector<EventType> events; // The vector where all of the evets are stored (be careful when modifying this directly)
  size_t size; // Number of events in this event list.
  bool normalized; // True if the events are normalizes (numbering of nodes goes from 0 to n-1)
  timestamp startTime; // The time window starts at this time
  timestamp totalTime; // The time window stops at this time

  //Constructors
  EventList(size_t size=0,timestamp startTime=0,timestamp totalTime=0); //constructor

  //Sorting
  bool IsSorted_SourceDestTime();//is sorted according to source, then destination, then time
  bool IsSorted_SourceTimeDest();//is sorted according to source, then time, then destination
  bool IsSorted_SourceDest();//is sorted according to source, then destination
  bool IsSorted_SourceTime(); //is sorted according to source, then time
  bool IsSorted_Source();//is sorted according to source
  bool IsSorted_Time();//is time sorted

  void Sort_SourceDestTime();//sorts the EventList according to source, then destination, then time
  void Sort_SourceTimeDest();//sorts the EventList according to source, then time, then destination
  void Sort_SourceDest();//sorts the EventList according to source, then destination
  void Sort_SourceTime();//sorts the EventList according to source, then time, then destination
  void Sort_Time();//sorts the EventList according to time
  void Sort_Source();//sorts the EventList according to source


  //Shuffling - purely temporal
  void Shuffle_AddOffset(int seed);
  void Shuffle_AllButTime(int seed);
  void Shuffle_Time(int seed);
  void Shuffle_LinkIETsKeepFirst(int seed);
  void Shuffle_UniformlyRandomTimes(int seed);
  void Shuffle_UniformlyRandomTimesKeepLinkEnds(int seed);
  void Shuffle_EqualWeightLinkSequence(int seed); // Swaps full event sequences between links that have same number of events

  //Shuffling - keeps connections, but changes weights
  void Shuffle_LinkSequence(int seed);
  void Shuffle_EventsToRandomLinks(int seed); //For each event randomly selects new source and destination pair from all links. (Keeps the aggregated network)

  //Shuffling - topological
  void Shuffle_RandomNodes(int seed, bool allowSelfEdges=false); //Uniformly randomly selects new nodes (between 0 and maxnode - 1) for each event
  void Shuffle_UndirectedConfigurationModel(int seed); 
  void Shuffle_NodeIdentitiesInWindows(int seed,timestamp windowWidth=1,bool onlyActive=true);

  //Shuffle interval limited by indices
  void ShuffleBetween_UniformlyRandomTimesKeepLinkEnds(size_t index1, size_t index2,RandNumGen<> &rands);
  void ShuffleBetween_NodeIdentities(size_t index1, size_t index2,RandNumGen<> &rands,bool onlyActive=true);

  //Printing
  void PrintEvents();
  string GetLongEventListInformation();
  string GetShortEventListInformation();

  //Reading and writing
  int ReadFile_SourceDestTime(char* fileName);//create the eventList from the file fileName, which has the following form : source destination time
  int ReadFile_SourceDestTimeDuration(char* fileName);//create the eventList from the file fileName, which has the following form : source destination time duration
  int ReadFile_TimeSourceDest(char* fileName);//create the eventList from the file fileName, which has the following form : time source destination
  int ReadFile_All(char* fileName);//Read EventList from a file. Depending on the EventType the file is assumed to be in the following form: source dest [time] [reverse] [duration] [index]
  int ReadFile_Binary(char* fileName);//Read EventList from a file written with WriteFile_Binary in the same platform.

  int WriteFile_SourceDestTime(char* fileName);//write the eventList to the file fileName. Each line corresponds to a single event, with following columns: source destination time
  int WriteFile_SourceDestTimeDuration(char* fileName);//write the eventList to the file fileName. Each line corresponds to a single event, with following columns: source destination time duration
  int WriteFile_TimeSourceDest(char* fileName);//write the eventList to the file fileName. Each line corresponds to a single event, with following columns: time source destination
  int WriteFile_All(char* fileName);//write the eventList to the file fileName. Each line corresponds to a single event, with following columns (depending on the event type): source dest [time] [reverse] [duration] [index]
  int WriteFile_Binary(char* fileName); // Writes the eventlist to the disk with each event writting in a way they are represented in memory. Note that this format might depend on the platform and this format is not suitable for distributing event lists

  //Property getters
  int GetNumberOfNodes(); //Returns the number of nodes (max node index - 1). Goes thorugh the event list to find the max index if needed
  timestamp GetTimeWindowSize(); // Returns the time window size (totalTime - startTime)

  //Operators
  EventType& operator[](size_t index); // Returns a reference to the even that is in the event list at index
  bool operator==(const EventList<EventType> & otherlist) const;

  //Misc 
  void SetSize(size_t size); //Sets the size of the event list
  void NormalizeNodeIndices(size_t* oldIndices); // Find new labels for nodes such that they go from 0 to n-1. Fills the oldIndices array with map from the new index to old index.
  void NormalizeNodeIndices(); // Find new labels for nodes such that they go from 0 to n-1.
  void AddEvent(EventType eve); // Adds a new event to the end of the event list.
  void AddReversed();// Adds a reversed event in the event list for each event currently in the list. The event list size will be doubled after this operation is complete. 
  void AddIndex();// Add an index to each event (original event and reversed one share the same event index)
  void FindTimeWindowSize(); // Goes through the event list and finds the min and max time stamp. Uses those times as time window starting time and ending time.

  // -> Iterators

  typedef typename vector<EventType>::iterator eventIteratorType;

  class userIterator;

  class userEventIterator {//iterator over the user's events in an eventList
  public:
    userIterator* currentUserIterator;       
    EventType current;
    userEventIterator(userIterator* currentUserIterator) {      
      this->currentUserIterator=currentUserIterator;
    }
    userEventIterator operator++(){ this->current=*(this->currentUserIterator->eventIterator); (this->currentUserIterator->eventIterator)++;  return *this;}
    userEventIterator operator++(int dummy){return ++(*this);} //copy of previous
    EventType& operator*(){return *(this->currentUserIterator->eventIterator);}
    bool operator!=(eventIteratorType other) {return (this->currentUserIterator->eventIterator)!=other; }
  };

  class userIterator{//iterator over the users of an eventList
  public:
    eventIteratorType eventIteratorEnd;
    eventIteratorType eventIterator;           
    nodeindex user;    
    userIterator(eventIteratorType begin, eventIteratorType end) : eventIterator(begin), eventIteratorEnd(end){this->user=(*(this->eventIterator)).source;}
    userEventIterator operator*(){return userEventIterator(this); }
    userIterator operator++() {
      while(this->eventIterator !=  this->eventIteratorEnd && (*this->eventIterator).source==user && this->eventIterator != this->eventIteratorEnd) {
        this->eventIterator++;
      } //This should be optimized away with branch prediction? Or should we have additional if statement?
      if (this->eventIterator !=  this->eventIteratorEnd){ 
	this->user=(*this->eventIterator).source;
      }
      return *this;
    }
    userIterator operator++(int dummy){
      ++(*this);
      return *this;
    }
    bool finished(){return this->eventIteratorEnd == this->eventIterator;}
    bool operator!=(eventIteratorType other) { return this->eventIterator!=other; }
    eventIteratorType& end(){
      if (this->eventIterator==this->eventIteratorEnd){
	return this->eventIteratorEnd;
      }
      
      if ( (*(this->eventIterator)).source!=this->user  ){
	return this->eventIterator; }
      else{
	return this->eventIteratorEnd;
      }
    }
  };

  eventIteratorType end(){return this->events.end();}
  userIterator iterUsers(){return userIterator(events.begin(),events.end()); }
  // <- Iterators
  
  typedef typename vector<EventType>::iterator evtite;
  
};


//--------- Shuffling methods


template<class EventType> 
void EventList<EventType>::Shuffle_AddOffset(int seed){
  //Check that the eventlist is properly sorted.
  if (!this->IsSorted_SourceDestTime()) this->Sort_SourceDestTime();
  
  RandNumGen<> rands(seed);
  timestamp timeWindow=this->GetTimeWindowSize();

  //iterate through all the events in all edges.
  int i=0;
  while(i<events.size()){
    timestamp offset = rands.next(timeWindow);
    EventType currentEdge=this->events[i];
    while(this->events[i].source==currentEdge.source && this->events[i].dest==currentEdge.dest){
      timestamp newTime=events[i].getTime()+offset;
      if (newTime>this->totalTime) newTime=newTime-timeWindow; //periodic bc
      this->events[i].setTime(newTime);
      i++;
    }
  }
}


template<class EventType> 
void EventList<EventType>::Shuffle_LinkSequence(int seed){
  //Check that the eventlist is properly sorted.
  if (!this->IsSorted_SourceDestTime()) this->Sort_SourceDestTime();
  this->stdSorted=false; //after the shuffling it is no longer sorted

  RandNumGen<> rands(seed);

  vector<size_t> edgeEvents; //index of first event in each edge
  size_t i=0;
  while(i<events.size()){
    edgeEvents.push_back(i);
    EventType currentEdge=this->events[i];
    while(this->events[i].source==currentEdge.source && this->events[i].dest==currentEdge.dest){
      i++;
    }
  }  

  //shuffle edgeEvents list
  for (size_t edgeIndex=0;edgeIndex<edgeEvents.size();edgeIndex++){
    size_t newEdgeIndex=edgeIndex+rands.next(edgeEvents.size()-edgeIndex);
    this->SwapLinkSequences(edgeEvents[edgeIndex],edgeEvents[newEdgeIndex]);
    edgeEvents[newEdgeIndex]=edgeEvents[edgeIndex];
  }

}

template<class EventType> 
void EventList<EventType>::SwapLinkSequences(size_t index1,size_t index2){

   nodeindex source1=this->events[index1].source;
   nodeindex dest1=this->events[index1].dest;
   nodeindex source2=this->events[index2].source;
   nodeindex dest2=this->events[index2].dest;

   size_t i=index1;
   while(this->events[i].source==source1 && this->events[i].dest==dest1){
     this->events[i].source=source2;
     this->events[i].dest=dest2;
     i++;
   }

   i=index2;
   while(this->events[i].source==source2 && this->events[i].dest==dest2){
     this->events[i].source=source1;
     this->events[i].dest=dest1;
     i++;
   }
     
}

template<class EventType>
void EventList<EventType>::SetNewTopology(vector<pair<nodeindex,nodeindex> > & edgeList){
  size_t i=0;
  size_t edgeIndex=0;
  while(i<events.size()){
    EventType currentEdge=this->events[i];
    while(this->events[i].source==currentEdge.source && this->events[i].dest==currentEdge.dest){
      this->events[i].source=edgeList[edgeIndex].first;
      this->events[i].dest=edgeList[edgeIndex].second;
      i++;
    }
    edgeIndex++;
  }  

}



template<class EventType> 
void EventList<EventType>::Shuffle_LinkIETsKeepFirst(int seed){
  //Check that the eventlist is properly sorted.
  if (!this->IsSorted_SourceDestTime()) this->Sort_SourceDestTime();
  
  RandNumGen<> rands(seed);
  timestamp timeWindow=this->GetTimeWindowSize();

  //iterate through all the events in all edges.
  int i=0;
  while(i<events.size()){

    //Build IET vector for each link
    EventType currentEdge=this->events[i];
    vector<timestamp> iets;
    iets.clear();
    timestamp prevTime;
    int j=i;
    while(this->events[j].source==currentEdge.source && this->events[j].dest==currentEdge.dest && j<events.size()){
      timestamp newTime=this->events[j].getTime();
      if (j!=i){
	      iets.push_back(newTime-prevTime);
      }
      prevTime=newTime;
      j++;
    }

    //Randomize the IET vector order
    shuffle_vector<timestamp>(iets, rands);

    //Change the times to follow the randomized IET vector
    prevTime=this->events[i].getTime();
    i++; //for the first event we keep the time

    int ietIndex=0;
    while(this->events[i].source==currentEdge.source && this->events[i].dest==currentEdge.dest && i<events.size()){
      prevTime+=iets[ietIndex];
      this->events[i].setTime(prevTime);
      ietIndex++;
      i++;
    }
  }
}


template<class EventType> 
void EventList<EventType>::Shuffle_AllButTime(int seed){
  /*
    Time shuffling of events. 
    In practice this is done by shuffling everything else but the times. This way
    if the times are sorted, they are also sorted after this shuffling.
   */

  RandNumGen<> rands(seed);
  timestamp timeWindow=this->GetTimeWindowSize();
  

  for(size_t i = 0; i < this->size-1;i++){
    size_t j=i+rands.next(this->size-i);
    EventType tempEvent = EventType();
    this->events[i].copyAllButTime(tempEvent);
    this->events[j].copyAllButTime(this->events[i]);
    tempEvent.copyAllButTime(this->events[j]);
  }
 
}


template<class EventType> 
void EventList<EventType>::Shuffle_Time(int seed){
  /*
    Time shuffling of events. 
   */

  RandNumGen<> rands(seed);
  timestamp timeWindow=this->GetTimeWindowSize();
  

  for(size_t i = 0; i < this->size;i++){
    size_t j=i+rands.next(this->size-i);
    timestamp tempTime=this->events[i].getTime();
    this->events[i].setTime(this->events[j].getTime());
    this->events[j].setTime(tempTime);
  }
 
}


template<class EventType> 
void EventList<EventType>::Shuffle_UniformlyRandomTimes(int seed){
  RandNumGen<> rands((float) seed);

  for(size_t i = 0; i < this->size;i++){
    timestamp newtime=this->startTime+rands.next(this->totalTime-this->startTime);
    this->events[i].setTime(newtime);
  }
}

 // Swaps full event sequences between links that have same number of events
 // This algorithm works for source-destination sorted event lists
 template<class EventType>
void EventList<EventType>::Shuffle_EqualWeightLinkSequence(int seed){
  // We need to construct vectors of indices such that links corresponding 
  // to those indices can be swapped inside each vector. These vectors are
  // then shuffled and swapping done based on the shuffled order.
  
  //Check that the eventlist is properly sorted.
  if (!this->IsSorted_SourceDest()) this->Sort_SourceDest();
  
  // Go through the event list, find start index for each link, find the
  // number of events in it, and add the index to correct vector
  unordered_map<uint, vector<size_t>* >  weight_to_index;
  EventType & currentEvent = this->events[0];
  size_t currentEventIndex=0;
  for (size_t i=1; i<=this->size; i++){
    if (i==this->size || currentEvent.source!=this->events[i].source || currentEvent.dest != this->events[i].dest){
      uint weight = i - currentEventIndex; // Weight of the edge
      
      // Create vector for this weight if it doesn't exist
      if(weight_to_index.count(weight)==0){
        weight_to_index[weight]=new vector<size_t>;
      }

      // Add the index to the correct vector
      weight_to_index[weight]->push_back(currentEventIndex);

      // Update the current event and its index if not at the end
      if(i!=this->size){
        currentEvent=this->events[i];
        currentEventIndex=i;
      }
    }
  }

  // Next we need to go through each index vector, and shuffle the links in them.
  RandNumGen<> rands(seed);
  for(typename unordered_map<uint,vector<size_t>* >::iterator i=weight_to_index.begin(); i!=weight_to_index.end();++i){
    vector<size_t> & edgeIndices=*(i->second);
    //shuffle edgeEvents list
    for (size_t indexIndex=0;indexIndex<edgeIndices.size();indexIndex++){
      size_t newIndexIndex=indexIndex+rands.next(edgeIndices.size()-indexIndex);
      this->SwapLinkSequences(edgeIndices[indexIndex],edgeIndices[newIndexIndex]);
      edgeIndices[newIndexIndex]=edgeIndices[indexIndex];
    }

    // Finally we free the memory we allocated in this function.
    delete i->second;
  }

}

template<class EventType> 
void EventList<EventType>::Shuffle_EventsToRandomLinks(int seed){
  // This version doesn't assume anything about the order. Can also be implemented
  // slightly faster if source-destination sorted.
  unordered_set<pair<nodeindex,nodeindex> > edges;
  for(size_t i = 0; i < this->size;i++){
    edges.insert(make_pair(this->events[i].getSource(),this->events[i].getDest()));
  }

  //copy the content of the set to a vector
  vector<pair<nodeindex,nodeindex> > edge_list(edges.begin(),edges.end());

  RandNumGen<> rands(seed);

  for(size_t i = 0; i < this->size;i++){
    size_t linkIndex=rands.next(edge_list.size());
    this->events[i].source=edge_list[linkIndex].first;
    this->events[i].dest=edge_list[linkIndex].second;
  }
  
  // Note that this might leave some edges empty and some of them might be lost.
  // One could define this shuffling in a way that each edges gets at least one
  // event.

}


template<class EventType> 
void EventList<EventType>::Shuffle_RandomNodes(int seed, bool allowSelfEdges){
  RandNumGen<> rands(seed);
  int maxnode= this->GetNumberOfNodes();
  if (maxnode==0) return; //There is only one node -> nothing can be done.
  
  for(size_t i = 0; i < this->size-1;i++){
    size_t source,destination;
    do{
      source=rands.next(maxnode);
      destination=rands.next(maxnode);
    } while((!allowSelfEdges) && source==destination); //We might not want self edges.

    this->events[i].source=source;
    this->events[i].dest=destination;
  }  
}

template<class EventType> 
void EventList<EventType>::Shuffle_UndirectedConfigurationModel(int seed){
  // Needs to be source-dest sorted and node-normalized
  SymmNet<float> net;
  for(size_t i = 0; i < this->size;i++){
    net[this->events[i].getSource()][this->events[i].getDest()]=1.0;
  }

  RandNumGen<> rands(seed);
  
  int limit=15;
  int rounds=10;
  if (net.size()>limit){
    limit=net.size();
  }
    
  randomize(net,rands,rounds,limit);

  vector<pair<nodeindex,nodeindex> > edgeList;
  for (nodeindex i=0;i<net.size();++i){
    for (SymmNet<float>::edge_iterator j=net[i].begin(); !j.finished(); ++j) {
      nodeindex node2=(nodeindex) *j;
      if (i<node2){
	edgeList.push_back(make_pair(i,node2));
      }
    }
  }

  shuffle_vector(edgeList,rands);

  this->SetNewTopology(edgeList);
} 


template<class EventType> 
void EventList<EventType>::Shuffle_UniformlyRandomTimesKeepLinkEnds(int seed){
  RandNumGen<> rands((timestamp) seed);
  size_t i=0;
  while(i<events.size()){
    //find the index where this edge ends
    EventType currentEdge=this->events[i];
    size_t j=i;
    while(this->events[j].source==currentEdge.source && this->events[j].dest==currentEdge.dest && j<events.size()){
      j++;
    }

    this->ShuffleBetween_UniformlyRandomTimesKeepLinkEnds(i,j,rands);
    i=j;
  }
}

template<class EventType>
void EventList<EventType>::Shuffle_NodeIdentitiesInWindows(int seed,timestamp windowWidth,bool onlyActive){
  this->Sort_Time(); //Needs to be time sorted. Remains in this order after shuffling.

  RandNumGen<> rands(seed);

  size_t i=0;
  while(i<events.size()){
    //find the index where this edge ends
    EventType currentEdge=this->events[i];
    size_t j=i;
    while(this->events[j].getTime()<currentEdge.getTime()+windowWidth && j<events.size()){
      j++;
    }
    this->ShuffleBetween_NodeIdentities(i,j,rands,onlyActive);
    i=j;
  }
}



//--------- Shuffling intervals between two indices

// The sequence between index1 and index2 needs to be sorted by time.
template<class EventType> 
void EventList<EventType>::ShuffleBetween_UniformlyRandomTimesKeepLinkEnds(size_t index1, size_t index2,RandNumGen<> &rands){
  timestamp startTime=this->events[index1].getTime();
  timestamp endTime=this->events[index2].getTime();

  if (index1+1<index2){ //otherwise there is nothing to randomize
    for(size_t i = index1+1; i < index2+1;i++){
      timestamp newtime=startTime+rands.next(endTime-startTime);
      this->events[i].setTime(newtime);
    }
  }
}


template<class EventType> 
void EventList<EventType>::ShuffleBetween_NodeIdentities(size_t index1, size_t index2,RandNumGen<> &rands,bool onlyActive){
  
  //First get the active nodes
  unordered_set<nodeindex> activeNodes;
  for(size_t i = index1; i < index2;i++){
    activeNodes.insert(this->events[i].getSource());
    activeNodes.insert(this->events[i].getDest());
  }

  vector<size_t> newNodeNameList;
  if (onlyActive){
    //Make them into a list and shuffle
    newNodeNameList.assign(activeNodes.begin(),activeNodes.end());
    shuffle_vector(newNodeNameList,rands);
  }else{
    size_t nnodes=this->GetNumberOfNodes();
    size_t sample=activeNodes.size();
    sample_without_replacement(newNodeNameList,nnodes,sample,rands);
  }

  //Iterate over the set, and create a renaming map
  size_t nodeIndex=0;
  map<nodeindex,nodeindex> namePermutation;
  
  for (unordered_set<nodeindex>::iterator nodeIterator=activeNodes.begin();nodeIterator!=activeNodes.end();++nodeIterator){
    namePermutation[*nodeIterator]=newNodeNameList[nodeIndex];
    nodeIndex++;
  }
  
  //Do the mapping
  for(size_t i = index1; i < index2;i++){
    this->events[i].source=namePermutation[this->events[i].getSource()];
    this->events[i].dest=namePermutation[this->events[i].getDest()];
  }  

}




//--------- Constructor


template<class EventType> 
EventList<EventType>::EventList(size_t size,timestamp startTime,timestamp totalTime) : events(size)
{ 
 this->size = size;
 this->numberOfNodes=-1;
 this->normalized=false;
 this->startTime=startTime;
 this->totalTime=totalTime;

 this->stdSorted=false;
 this->sdtSorted=false;
 this->stSorted=false;
 this->sdSorted=false;
 this->tSorted=false;
 this->sSorted=false;
}

template<class EventType> 
timestamp EventList<EventType>::GetTimeWindowSize(){
  return this->totalTime - this->startTime;
}

template<class EventType> 
void EventList<EventType>::FindTimeWindowSize(){
  if (this->events.size()==0) return;
  timestamp minTime=this->events[0].getTime();
  timestamp maxTime=this->events[0].getTime();

  for (int i=0;i<this->events.size();++i){
    timestamp time = this->events[i].getTime();
    if(time>maxTime) maxTime=time;
    if(time<minTime) minTime=time;
  }
  this->startTime=minTime;
  this->totalTime=maxTime;
  
}


template<class EventType> 
bool EventList<EventType>::IsSorted_SourceDestTime(){
  return this->sdtSorted;
}

template<class EventType> 
bool EventList<EventType>::IsSorted_SourceTimeDest(){
  return this->stdSorted;
}

template<class EventType> 
bool EventList<EventType>::IsSorted_SourceDest(){
  return this->sdSorted;
}

template<class EventType> 
bool EventList<EventType>::IsSorted_SourceTime(){
  return this->stSorted;
}

template<class EventType> 
bool EventList<EventType>::IsSorted_Time(){
  return this->tSorted;
}

template<class EventType> 
bool EventList<EventType>::IsSorted_Source(){
  return this->sSorted;
}

template<typename EventType>
void EventList<EventType>::AddReversed(){
  size_t oldsize=this->size;
  this->size=oldsize*2;
  this->events.resize(this->size);//resize the eventList
  typename vector<EventType>::iterator reversed=this->events.begin()+oldsize;
  for(typename vector<EventType>::iterator i=this->events.begin(); i!=this->events.end()-oldsize;i++){
    *reversed=i->getReversedEvent();//add the reversed event to the eventList
    reversed++;
  }
}

template<typename EventType>
void EventList<EventType>::AddIndex(){//add an index to each event of the eventList
  int index=0;
  while(index < this->size){
    events[index].setIndex(index);
    index++;
  }
}


template<class EventType> 
void EventList<EventType>::SetSize(size_t size){
  this->events.resize(size);
  this->size=size;
}

template<class EventType> 
void EventList<EventType>::SetSizeByFile(char* fileName){
  ifstream the_file(fileName);
  int size=0;
  string line;
  while ( !the_file.eof() ){
    getline(the_file, line);
    size++;
  }
  if (line.size()==0){
    size=size-1;
  }
  
  the_file.close();
  this->events.clear();
  this->SetSize(size);
}

//Helper function to read from file with different reading methods from the Event class
template<typename EventType, void (EventType::*ReadFromFile)(ifstream & the_file)>
  int ReadFile_TextFile(char* fileName, EventList<EventType> * eventlist){


  ifstream the_file(fileName);

  if ( !the_file.is_open() ){
    cerr << "Could not open file.\n";
    return 0;
  } else {
    if (eventlist->size==0) {
        eventlist->SetSizeByFile(fileName);
    }
  }

  size_t eventIndex=0;

  while (!the_file.eof() && eventIndex<eventlist->size){
    (eventlist->events[eventIndex].*ReadFromFile)(the_file);
    eventIndex++;
  }
  the_file.close();
  return 1;
}

template<class EventType>
int EventList<EventType>::ReadFile_TimeSourceDest(char* fileName){
  return ReadFile_TextFile<EventType,&EventType::readFromFile_tsd>(fileName,this);
}

template<class EventType>
int EventList<EventType>::ReadFile_SourceDestTime(char* fileName){
  return ReadFile_TextFile<EventType, &EventType::readFromFile_sdt>(fileName,this);
}

template<class EventType>
int EventList<EventType>::ReadFile_SourceDestTimeDuration(char* fileName){
  return ReadFile_TextFile<EventType,&EventType::readFromFile_sdtd>(fileName,this);
}

template<class EventType>
int EventList<EventType>::ReadFile_All(char* fileName){
  return ReadFile_TextFile<EventType,&EventType::readFromFile_all>(fileName,this);
}

template<class EventType> 
int EventList<EventType>::ReadFile_Binary(char* fileName){
  //Get the file size in bytes.
  struct stat results;
  size_t fileSize;
  if (stat(fileName, &results) == 0)
    fileSize=results.st_size;
  else return 0;

  //Check that the file size is multiple of EventType size.
  if(fileSize%sizeof(EventType)!=0){ 
    cerr << "Size of the file doesn't match the EventType size." << endl;
    return 0;
  }
  size_t size=fileSize/sizeof(EventType);

  fstream the_file(fileName, ios::in | ios::binary);
  if ( !the_file.is_open() ){
    cerr<<"Could not open file\n";
    return 0;
  }

  this->size=size;
  this->events.clear();
  this->events.resize(size);
  for(int i=0;i<this->size;i++)
    the_file.read((char*) &this->events[i], sizeof(EventType));

  the_file.close();


  this->numberOfNodes=-1;
  return 1;
}


//Helper function to write to file with different writing methods from the Event class
template<typename EventType, void (EventType::*WriteToFile)(ofstream & the_file)>
  int WriteFile_TextFile(char* fileName,EventList<EventType> * eventlist){
  ofstream the_file (fileName);

  if ( !the_file.is_open() ){
    cerr<<"Could not open file\n";
    return 0;
  }
  for(int i=0;i<eventlist->size;i++){
    (eventlist->events[i].*WriteToFile)(the_file);
    the_file << endl;
  }
  the_file.close();
  return 1;
}

template<class EventType>
int EventList<EventType>::WriteFile_TimeSourceDest(char* fileName){
  return WriteFile_TextFile<EventType,&EventType::writeToFile_tsd>(fileName,this);
}

template<class EventType>
int EventList<EventType>::WriteFile_SourceDestTime(char* fileName){
  return WriteFile_TextFile<EventType,&EventType::writeToFile_sdt>(fileName,this);
}

template<class EventType>
int EventList<EventType>::WriteFile_SourceDestTimeDuration(char* fileName){
  return WriteFile_TextFile<EventType,&EventType::writeToFile_sdtd>(fileName,this);
}

template<class EventType>
int EventList<EventType>::WriteFile_All(char* fileName){
  return WriteFile_TextFile<EventType,&EventType::writeToFile_all>(fileName,this);
}


template<class EventType> 
int EventList<EventType>::WriteFile_Binary(char* fileName){
  ofstream the_file (fileName,ofstream::binary);


  if ( !the_file.is_open() ){
    cerr<<"Could not open file\n";
    return 0;
  }

  for(int i=0;i<this->size;i++)
    the_file.write ((char*)&this->events[i],sizeof(EventType));


  the_file.close();
  return 1;
}



template<class EventType> 
EventType& EventList<EventType>::operator[](size_t index){
  return this->events[index];
}

template<class EventType> 
bool EventList<EventType>::operator==(const EventList<EventType> & otherlist) const{
  //First check time window and size
  if (this->size !=otherlist.size) {return false;}
  if (this->startTime !=otherlist.startTime) {return false;}
  if (this->totalTime !=otherlist.totalTime) {return false;}

  //Then check the events
  for (int i=0;i<this->size;i++){
    if (this->events[i]!=otherlist.events[i]){return false;}
  }

  //Everything checks out, they are the same list
  return true;
}


template<class EventType> 
void EventList<EventType>::NormalizeNodeIndices(size_t* oldIndices){
  map<unsigned long, unsigned long> nodeMap;

  for (int i=0;i<this->size;i++){
    unsigned source=this->events[i].source;
    unsigned dest=this->events[i].dest;
    unsigned newSource;
    unsigned newDest;

    if (nodeMap.find(source) == nodeMap.end()){
      newSource=nodeMap.size();
      oldIndices[newSource]=source;
      nodeMap.insert(make_pair(source, newSource));
      this->events[i].source = newSource;
    } else{
      this->events[i].source=nodeMap[source];
    }


    if (nodeMap.find(dest) == nodeMap.end()){
      newDest=nodeMap.size();
      oldIndices[newDest]=dest;
      nodeMap.insert(make_pair(dest, newDest));
      this->events[i].dest = newDest;
    } else{
      this->events[i].dest=nodeMap[dest];
    }    
  }
  this->normalized=true;
  this->numberOfNodes=nodeMap.size();
}

template<class EventType> 
void EventList<EventType>::NormalizeNodeIndices(){
  map<unsigned long, unsigned long> nodeMap;

  for (int i=0;i<this->size;i++){
    unsigned source=this->events[i].source;
    unsigned dest=this->events[i].dest;
    unsigned newSource;
    unsigned newDest;

    if (nodeMap.find(source) == nodeMap.end()){
      newSource=nodeMap.size();
      nodeMap.insert(make_pair(source, newSource));
      this->events[i].source = newSource;
    } else{
      this->events[i].source=nodeMap[source];
    }


    if (nodeMap.find(dest) == nodeMap.end()){
      newDest=nodeMap.size();
      nodeMap.insert(make_pair(dest, newDest));
      this->events[i].dest = newDest;
    } else{
      this->events[i].dest=nodeMap[dest];
    }    
  }
  this->normalized=true;
  this->numberOfNodes=nodeMap.size();
}


template<class EventType> 
int EventList<EventType>::GetNumberOfNodes(){
  if (this->numberOfNodes==-1){
    unsigned maxNode=0;
    for (int i=0;i<this->size;i++){
      if (maxNode<this->events[i].source) maxNode=this->events[i].source;
      if (maxNode<this->events[i].dest) maxNode=this->events[i].dest;
    }

    if (this->size>0)
      this->numberOfNodes=maxNode+1;
    else
      return 0;
  }
  return this->numberOfNodes;

}


template<class EventType> 
void EventList<EventType>::Sort_SourceDestTime(){
  sort(this->events.begin(), this->events.end(), compare_sdt<EventType>);
  this->sdtSorted=true;
  this->sdSorted=true;
  this->sSorted=true;

  this->stdSorted=false;
  this->stSorted=false;
  this->tSorted=false;

}

template<class EventType> 
void EventList<EventType>::Sort_SourceTimeDest(){
  sort(this->events.begin(), this->events.end(), compare_std<EventType>);
  this->stdSorted=true;
  this->stSorted=true;
  this->sSorted=true;

  this->sdtSorted=false;
  this->sdSorted=false;
  this->tSorted=false;

}


template<class EventType> 
void EventList<EventType>::Sort_SourceTime(){
  sort(this->events.begin(), this->events.end(), compare_st<EventType>);
  this->stSorted=true;
  this->sSorted=true;

  this->stdSorted=false;
  this->sdtSorted=false;
  this->sdSorted=false;
  this->tSorted=false;
}

template<class EventType> 
void EventList<EventType>::Sort_SourceDest(){
  sort(this->events.begin(), this->events.end(), compare_sd<EventType>);
  this->sdSorted=true;
  this->sSorted=true;

  this->stdSorted=false;
  this->sdtSorted=false;
  this->stSorted=false;
  this->tSorted=false;
}

template<class EventType> 
void EventList<EventType>::Sort_Time(){
  sort(this->events.begin(), this->events.end(), compare_t<EventType>);
  this->tSorted=true;

  this->stdSorted=false;
  this->sdtSorted=false;
  this->sdSorted=false;
  this->stSorted=false;
  this->sSorted=false;
}

template<class EventType> 
void EventList<EventType>::Sort_Source(){
  sort(this->events.begin(), this->events.end(), compare_s<EventType>);
  this->sSorted=true;

  this->stdSorted=false;
  this->sdtSorted=false;
  this->sdSorted=false;
  this->stSorted=false;
  this->tSorted=false;
}


template<class EventType> 
void EventList<EventType>::PrintEvents(){
  for(typename vector<EventType>::iterator i=this->events.begin(); i!=this->events.end();i++){
    cout << (*i).getLongEventInformation() << "\n";
  }
}

template<class EventType> 
void EventList<EventType>::AddEvent(EventType eve){
  events.push_back(eve);
  this->size+=1;
}

template<class EventType> 
string EventList<EventType>::GetShortEventListInformation(){
  stringstream s;
  s << "EventList<Event type: " << EventType::getEventTypeStr() << ", size: "<< this->size  << ">";
  return s.str();
}

template<class EventType> 
string EventList<EventType>::GetLongEventListInformation(){
  stringstream s;
    s << "EventList[ ";
  if (this->size<3){
    for(typename vector<EventType>::iterator i=this->events.begin(); i!=this->events.end();i++){
      s << (*i).getShortEventInformation();
      if (i!=this->events.end()){
	s << ", ";
      }
    }    
  }else if (this->size<11){
    for(typename vector<EventType>::iterator i=this->events.begin(); i!=this->events.end();i++){
      s << (*i).getShortEventInformation() << ",\n";
    }    
  } else {
    int count=0;
    for(typename vector<EventType>::iterator i=this->events.begin(); count < 10;i++){
      s << (*i).getShortEventInformation() << ",\n";
      count++;
    } 
    s << "...\n";
  }
  s << "]";
  return s.str();
}


#endif //EVENTLIST
