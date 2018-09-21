#ifndef SPREADING
#define SPREADING

#include <vector>
#include "../eventList.h"

template<class EventType>
class InfectionProcess{
 public:
  vector<bool> infected;
  size_t size;
  size_t numberOfInfected;
  //unsigned long time;
  //unsigned long timeInEvents;
  size_t infectingEventIndex;
  size_t lastInfectedNode;
  unsigned rounds;
  size_t startTime; //in number of events

  EventList<EventType> &eventList;

  InfectionProcess(size_t size,EventList<EventType>& eventList);
  void reset(size_t startNode, size_t startTime);
  bool infectNext();
  void infectAll();
  void infectUntilNodesInfected(size_t numberOfNodes);
  unsigned long getTimeInEvents();
  timestamp getTime();

  vector<size_t> rEvents;
  size_t rIndex;
  size_t rCut;
  size_t rSize;
};

template<class EventType>
InfectionProcess<EventType>::InfectionProcess(size_t size, EventList<EventType>& eventList): eventList(eventList){
  this->size=size;
  this->infected= *(new vector<bool>(size));
  this->rEvents= *(new vector<size_t>());
  this->rCut=floor(0.01*size);
  if (this->rCut<=2) this->rCut=0;
}

template<class EventType>
void InfectionProcess<EventType>::reset(size_t startNode, size_t startTime){
  this->startTime=startTime;
  this->rounds=0;
  //this->time=0;
  //this->timeInEvents=0;
  this->numberOfInfected=0;
  this->infectingEventIndex=startTime-1;

  //set all nodes to susceptible
  for(int i=0;i<size;i++){
    this->infected[i]=false;
  }

  //set the first node infected
  size_t candidate;
  if (startNode<0){
	candidate=rand()%this->size;
      } else{
	candidate=startNode;
      }
      this->infected[candidate]=true;
      this->numberOfInfected++;
}

template<class EventType>
bool InfectionProcess<EventType>::infectNext(){
  if (this->numberOfInfected>=this->size)
    return false;

  bool noInfection=true;
  if (this->size - this->numberOfInfected>this->rCut){
    size_t startIndex = this->infectingEventIndex;
  while(noInfection){
    //this->timeInEvents++;
    this->infectingEventIndex++;
    if (this->eventList.size<=this->infectingEventIndex){
      this->infectingEventIndex=0;
      this->rounds++;
    }    

    size_t source=this->eventList[this->infectingEventIndex].source;
    size_t dest=this->eventList[this->infectingEventIndex].dest;

    if(this->infected[source]==true){
      if (this->infected[dest]==false){
	  this->infected[dest]=true;
	  noInfection=false;
	  this->lastInfectedNode=dest;	
      }
    } else if (this->infected[dest]==true){
      this->infected[source]=true;
      noInfection=false;
      this->lastInfectedNode=source;      
    }
    if (startIndex==this->infectingEventIndex) return false; //Looped through everything without infection
  }
  } else{
    if (this->size - this->numberOfInfected==this->rCut){
      /*
      this->rEvents.clear();
      this->rIndex=0;
      for(size_t i=this->infectingEventIndex;i!=this->infectingEventIndex-1;i++){
	if (i==this->eventList.size){
	  i=0;
	}
	size_t source=this->eventList[i].source;
	size_t dest=this->eventList[i].dest;
	if ((!this->infected[source]) || (!this->infected[dest]) ){
	  this->rEvents.push_back(i);
	}
      }      
      this->rSize=rEvents.size();
      */
      this->rEvents.clear();
      bool rIndexNotSet=true;
      for(size_t i=0;i<=this->eventList.size;i++){
	size_t source=this->eventList[i].source;
	size_t dest=this->eventList[i].dest;
	if ((!this->infected[source]) || (!this->infected[dest]) ){
	  this->rEvents.push_back(i);
	  if (rIndexNotSet  && this->infectingEventIndex<i){
	    this->rIndex=this->rEvents.size()-1;
	    rIndexNotSet=false;
	  }
	}
      }      
      this->rSize=rEvents.size();
    }

  size_t startRIndex = this->rIndex;
  while(noInfection){
    this->infectingEventIndex=this->rEvents[this->rIndex];
    this->rIndex++;

    if (this->rEvents.size()<=this->rIndex){
      this->rIndex=0;
      this->rounds++;
    }    

    size_t source=this->eventList[this->infectingEventIndex].source;
    size_t dest=this->eventList[this->infectingEventIndex].dest;


    if(this->infected[source]==true){
      if (this->infected[dest]==false){
	  this->infected[dest]=true;
	  noInfection=false;
	  this->lastInfectedNode=dest;	
      }
    } else if (this->infected[dest]==true){
      this->infected[source]=true;
      noInfection=false;
      this->lastInfectedNode=source;      
    }

  if (startRIndex==this->rIndex) return false; //Looped through everything without infection
  }

  }
  this->numberOfInfected++;

  return true;

}


template<class EventType>
void InfectionProcess<EventType>::infectAll(){
  while(this->infectNext());
}


template<class EventType>
void InfectionProcess<EventType>::infectUntilNodesInfected(size_t numberOfNodes){
  while(this->infectNext() &&  this->numberOfInfected < numberOfNodes);
}

template<class EventType>
unsigned long InfectionProcess<EventType>::getTimeInEvents(){
  return this->infectingEventIndex+this->rounds*this->eventList.size-this->startTime;
}

template<class EventType>
timestamp InfectionProcess<EventType>::getTime(){
  return this->eventList[this->infectingEventIndex].getTime()-this->eventList[this->startTime].getTime()+this->eventList.GetTimeWindowSize()*(this->rounds);
}

template<class EventType>
void getAvgInfectionCurves(EventList<EventType> & e, size_t nRounds, vector<float> & avgInfTime, vector<float> & avgInfCount,timestamp timeResolution,timestamp recordingTime,int randomSeed=1234,bool verbose=true){

  if(!e.isTimeSorted()){
    if (verbose) cerr << "Sorting the event list."<<endl; 
    e.sort_time();
  }

  InfectionProcess<EventWithTime> inf(e.getNumberOfNodes(),e);
  
  if (verbose) cerr << "Initializing the infcetion time vectors." <<endl;
  if (verbose) cerr << "Avg infection time array has: " << e.getNumberOfNodes() << " elements."  << endl;
  avgInfTime.resize(e.getNumberOfNodes());
  for(int i=0;i<e.getNumberOfNodes();i++) avgInfTime[i]=0;

  timestamp timeArraySize=round(recordingTime/float(timeResolution));
  if (verbose) cerr << "Avg infection count array has: " << timeArraySize << " elements."  << endl;
  avgInfCount.resize(timeArraySize);
  for(int i=0;i<timeArraySize;i++) avgInfCount[i]=0;

  RandNumGen<> rands(randomSeed);

  for (int round=0;round<nRounds;round++){
    if (verbose) cerr << "Entering round: " << round+1 <<endl;
    nodeindex startEvent=rands.next(e.size);
    timestamp startTime=e.events[startEvent].time;
    nodeindex startNode;
    if (rands.next(1)==1)
      startNode=e.events[startEvent].source;
    else
      startNode=e.events[startEvent].dest;
    inf.reset(startNode,startEvent);

    timestamp nextTime=0;

    while (inf.infectNext()){
      timestamp infTime=e.events[inf.infectingEventIndex].time-startTime+(e.totalTime-e.startTime)*(inf.rounds);
      avgInfTime[inf.numberOfInfected]=avgInfTime[inf.numberOfInfected]+infTime;
      while(nextTime<=infTime && nextTime< recordingTime ){
	int index=nextTime/timeResolution;
	avgInfCount[index]=avgInfCount[index]+inf.numberOfInfected;
	nextTime=nextTime+timeResolution;
      }
    }
    
    while(nextTime<recordingTime){
      int index=nextTime/timeResolution;
      avgInfCount[index]=avgInfCount[index]+inf.numberOfInfected;
      nextTime=nextTime+timeResolution;
    }



  }

  if (verbose) cerr << "Normalizing the results." << endl;
  for(int i=0;i<e.getNumberOfNodes();i++)
    avgInfTime[i]=avgInfTime[i]/float(nRounds);
  for(int i=0;i<timeArraySize;i++)
    avgInfCount[i]=avgInfCount[i]/float(nRounds);
 
}


#endif //SPREADING
