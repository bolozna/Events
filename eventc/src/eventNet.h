#ifndef EVENTNET
#define EVENTNET

/* The EventNet class. This is network data structure where payload on each edge corresponds to an index in an event list which
 * is sorted in firstly with source and then with destination. This data structure can be used to do various operations that
 * require using the structure of the underlying aggregated network corresponding to the event list.
 */

#include <utility>
#include <vector>
#include <algorithm>
#include "eventList.h"
#include "../../../lcelib/Nets.H"
#include "event_policy_classes/timeStampPolicies.h"

using namespace std;

template<typename T>
T modulo(T n,T p){
  return n%p;
}

template<>
float modulo(float n,float p){
  int num=floor(n/p);
  return n-num*p;
}



template<class elementType>
class pVector{ //a periodic vector
 public:
  vector<elementType> & baseVector;
  elementType range;
 pVector(vector<elementType> & baseVector, elementType range) : baseVector(baseVector), range(range){}
  elementType operator[](int index){
    int multiples,returnIndex;
      multiples = 0;
      returnIndex=index;
      while (returnIndex<0){
	returnIndex=returnIndex+this->baseVector.size();
	multiples--;
      }
    return this->baseVector[returnIndex]+multiples * this->range;
  }  
};




template<class EventType> 
class EventNet{
 private:


 public:
  EventList<EventType> eventList;
  DirNet<unsigned> *indexNet;

  EventNet(EventList<EventType> & eventList);  
  ~EventNet();
  
  void GetLinkEventsDirected(nodeindex a, nodeindex b, vector<EventType> & linkEvents);
  void GetLinkEventsDirected(nodeindex a, nodeindex b, vector<pair<EventType,size_t> > & linkEvents);
  void GetLinkEventsUndirected(nodeindex a, nodeindex b, vector<EventType> & linkEvents);
  void GetLinkEventsUndirected(nodeindex a, nodeindex b, vector<pair<EventType,size_t> > & linkEvents);

  void GetLinkEventTimesDirected(nodeindex a, nodeindex b, vector<timestamp> & linkTimes);
  void GetLinkEventTimesDirected(nodeindex a, nodeindex b, vector<pair<timestamp,size_t> > & linkEvents);

  void GetLinkEventTimesUndirected(nodeindex a, nodeindex b, vector<pair<timestamp,size_t> > & linkEvents);
  void GetLinkEventTimesUndirected(nodeindex a, nodeindex b, vector<timestamp> & linkTimes);


  void GetInterEventTimes(nodeindex a, nodeindex b, vector<timestamp> & linkIets, vector<size_t> & eventIndices);
  void GetInterEventTimes(long * linkIets);
  void GetInterEventTimes(vector<timestamp> & times, vector<timestamp> & iets);

  void GetNeighborEvents(nodeindex a, nodeindex b, vector<timestamp> & neighborEvents);
  void GetNeighborEvents(nodeindex a, nodeindex b, vector<pair<timestamp,size_t> > & neighborEvents);
  void GetNeighborEvents(nodeindex a, nodeindex b, vector<pair<EventType,size_t> > & neighborEvents);

  float GetAvgTauZero(nodeindex a, nodeindex b);

  void GetTauOnes(nodeindex a, nodeindex b, vector<timestamp> & tauOnes);
  float GetAvgTauOne(nodeindex a, nodeindex b);

  float GetAvgTauInfApprox(nodeindex a, nodeindex b, long * iets);
  float GetAvgTauInfApprox(nodeindex a, nodeindex b, long * iets, SymmNet<unsigned> & aggNet);
  float GetAvgTauInfApprox_neighborhoodIet(nodeindex a, nodeindex b);

  void MakeLinkIetPreservingList(EventList<EventType> & outList,int seed);

};

template<class EventType> 
void EventNet<EventType>::MakeLinkIetPreservingList(EventList<EventType> & outList,int seed){
  EventNet<EventType> & inNet=*this;
  EventList<EventType> & inList=inNet.eventList;
  
  if (!inList.IsSorted_SourceDestTime()){
    cerr << "Error: the eventList needs to be sorted with Sort_SourceDestTime.\n" ;
    return;
  }
  

  timestamp timeWindowSize=inList.totalTime- inList.startTime;
  timestamp startTime=inList.startTime;
  outList.startTime=inList.startTime;
  outList.totalTime=inList.totalTime;

  outList.events.clear();

  RandNumGen<> rands(seed);
  
  vector<timestamp> iets;
  vector<size_t> dummy;

  int nLinks=0;
 
  for (int i=0;i<inList.GetNumberOfNodes();i++) { 
    for (DirNet<unsigned>::const_edge_iterator j=(*(inNet.indexNet))(i).begin();!j.finished();++j) {
      if (i<*j){
	timestamp currentTime=rands.next(timeWindowSize);
	EventType event;
	event.source=i;
	event.dest=*j;
	
	iets.clear();
	inNet.GetInterEventTimes(i,*j,iets,dummy);
	//shuffle the iets.
	std::random_shuffle(iets.begin(),iets.end());

	nLinks++;

	for(int index=0;index<iets.size();++index){
	  currentTime = currentTime+iets[index];
	  event.setTime(modulo(currentTime,timeWindowSize)+startTime);
	  outList.events.push_back(event);
	}
      }
    }
  }

  outList.size=outList.events.size();

}


template<class EventType> 
EventNet<EventType>::EventNet(EventList<EventType> & eventList) : eventList(eventList){
  //Create an aggregated network of indices refering to the first element of the edge in the event list.
  this->indexNet=new DirNet<unsigned>(this->eventList.GetNumberOfNodes());
  DirNet<unsigned> &indexNet=*this->indexNet;
  for(size_t i = 0; i < this->eventList.size;++i){
    nodeindex source,dest;
    source=this->eventList[i].source;
    dest=this->eventList[i].dest;
    if (indexNet[source][dest]==0){
      indexNet[source][dest]=i+1;
      }
  }
}


template<class EventType> 
EventNet<EventType>::~EventNet(){
  //delete (this->indexNet);
}


template<class EventType>
void EventNet<EventType>::GetLinkEventsDirected(nodeindex a, nodeindex b, vector<EventType> & linkEvents){
  size_t eventIndex=(*this->indexNet)[a][b];
  if (eventIndex!=0){
    eventIndex--;
  while(this->eventList[eventIndex].getSource()==a  && this->eventList[eventIndex].getDest()==b){
    linkEvents.push_back(this->eventList[eventIndex]);
    eventIndex++;
  }
  }
}

template<class EventType>
void EventNet<EventType>::GetLinkEventsDirected(nodeindex a, nodeindex b, vector<pair<EventType,size_t> > & linkEvents){
  size_t eventIndex=(*this->indexNet)[a][b];
  if (eventIndex!=0){
    eventIndex--;
  while(this->eventList[eventIndex].getSource()==a  && this->eventList[eventIndex].getDest()==b){
    linkEvents.push_back(pair<EventType,size_t>(this->eventList[eventIndex],eventIndex));
    eventIndex++;
  }
  }
}


template<class EventType>
void EventNet<EventType>::GetLinkEventTimesDirected(nodeindex a, nodeindex b, vector<timestamp> & linkTimes){
  size_t eventIndex=(*this->indexNet)[a][b];
  if (eventIndex!=0){
    eventIndex--;
    while(this->eventList[eventIndex].getSource()==a  && this->eventList[eventIndex].getDest()==b){
      linkTimes.push_back(this->eventList[eventIndex].getTime());
      eventIndex++;
    }
  }
}

template<class EventType>
void EventNet<EventType>::GetLinkEventTimesDirected(nodeindex a, nodeindex b, vector<pair<timestamp,size_t> > & linkEvents){
  size_t eventIndex=(*this->indexNet)[a][b];
  if (eventIndex!=0){
    eventIndex--;
    while(this->eventList[eventIndex].getSource()==a  && this->eventList[eventIndex].getDest()==b){
      linkEvents.push_back(pair<timestamp,size_t>(this->eventList[eventIndex].getTime(),eventIndex));
      eventIndex++;
    }
  }
}

template<class EventType>
void EventNet<EventType>::GetLinkEventTimesUndirected(nodeindex a, nodeindex b, vector<timestamp> & linkTimes){
  this->GetLinkEventTimesDirected(a,b,linkTimes);
  this->GetLinkEventTimesDirected(b,a,linkTimes);
  sort(linkTimes.begin(), linkTimes.end());
}

template<class EventType>
void EventNet<EventType>::GetLinkEventTimesUndirected(nodeindex a, nodeindex b, vector<pair<timestamp,size_t> > & linkEvents){
  this->GetLinkEventTimesDirected(a,b,linkEvents);
  this->GetLinkEventTimesDirected(b,a,linkEvents);
  sort(linkEvents.begin(), linkEvents.end());
}

template<class EventType>
void EventNet<EventType>::GetLinkEventsUndirected(nodeindex a, nodeindex b, vector<EventType> & linkEvents){
  this->GetLinkEventsDirected(a,b,linkEvents);
  this->GetLinkEventsDirected(b,a,linkEvents);
}

template<class EventType>
void EventNet<EventType>::GetLinkEventsUndirected(nodeindex a, nodeindex b, vector<pair<EventType,size_t> > & linkEvents){
  this->GetLinkEventsDirected(a,b,linkEvents);
  this->GetLinkEventsDirected(b,a,linkEvents);
}

template<class EventType>
void EventNet<EventType>::GetInterEventTimes(vector<timestamp> & times, vector<timestamp> & iets){
  pVector<timestamp> pTimes(times,this->eventList.totalTime-this->eventList.startTime);
  for (int i=0;i<times.size();++i){
    iets.push_back(pTimes[i]-pTimes[i-1]);
  } 
}

template<class EventType>
void EventNet<EventType>::GetInterEventTimes(nodeindex a, nodeindex b, vector<timestamp> & linkIets, vector<size_t> & eventIndices){
  /*
    Writes the inter-event times of the edge sequences to the interEventTimes array.
   */  
  vector<pair<timestamp,size_t> > eventsAB;//=vector<pair<timestamp,size_t> >();
  eventsAB.clear();
  GetLinkEventTimesUndirected(a,b,eventsAB);

  bool first=true;
  timestamp previous;
  vector<pair<timestamp,size_t> >::const_iterator i=eventsAB.begin();
  for (;i<eventsAB.end();++i){
    timestamp time=(*i).first;
    size_t index=(*i).second;
    if (first){ 
      first=false;
    } else {
      linkIets.push_back(time-previous);
      eventIndices.push_back(index);
    } 
    previous=time;
  }
  i=eventsAB.begin();
  linkIets.push_back(this->eventList.totalTime+(*i).first-previous-this->eventList.startTime);
  eventIndices.push_back((*i).second);

}

template<class EventType>
void EventNet<EventType>::GetInterEventTimes(long * linkIets){
  //Iterate all the edges in the network
  for (int i=0;i<this->eventList.GetNumberOfNodes();i++) {
    for (DirNet<unsigned>::const_edge_iterator j=(*indexNet)(i).begin();!j.finished();++j) {
      if (i<*j){
	vector<timestamp> abIets;vector<size_t> eventIndices;
	this->GetInterEventTimes(i,*j,abIets,eventIndices);
	for (int k = 0;k<abIets.size();k++){
	  linkIets[eventIndices[k]]=abIets[k];
	}
      }
    } 
  }
}


template<class EventType>
void EventNet<EventType>::GetNeighborEvents(nodeindex a, nodeindex b, vector<pair<timestamp,size_t> > & neighborEvents){
  for (DirNet<unsigned>::const_edge_iterator j=(*this->indexNet)(a).begin();!j.finished();++j) {
    if (*j!=b)
      this->GetLinkEventTimesUndirected(a,*j,neighborEvents);
  }
  for (DirNet<unsigned>::const_edge_iterator j=(*this->indexNet)(b).begin();!j.finished();++j) {
    if (*j!=a)
      this->GetLinkEventTimesUndirected(b,*j,neighborEvents);
  }
  sort(neighborEvents.begin(), neighborEvents.end());
 
}


template<class EventType>
void EventNet<EventType>::GetNeighborEvents(nodeindex a, nodeindex b, vector<timestamp> & neighborEvents){
  for (DirNet<unsigned>::const_edge_iterator j=(*this->indexNet)(a).begin();!j.finished();++j) {
    if (*j!=b)
      this->GetLinkEventTimesUndirected(a,*j,neighborEvents);
  }
  for (DirNet<unsigned>::const_edge_iterator j=(*this->indexNet)(b).begin();!j.finished();++j) {
    if (*j!=a)
      this->GetLinkEventTimesUndirected(b,*j,neighborEvents);
  }
  sort(neighborEvents.begin(), neighborEvents.end());
 
}


template<class EventType>
bool compare_event_time(const pair<EventType,size_t> & event1,const pair<EventType,size_t> & event2){
  return event1.first.getTime()<event2.first.getTime();
}

template<class EventType>
void EventNet<EventType>::GetNeighborEvents(nodeindex a, nodeindex b, vector<pair<EventType,size_t> > & neighborEvents){
  for (DirNet<unsigned>::const_edge_iterator j=(*this->indexNet)(a).begin();!j.finished();++j) {
    if (*j!=b)
      this->GetLinkEventsUndirected(a,*j,neighborEvents);
  }
  for (DirNet<unsigned>::const_edge_iterator j=(*this->indexNet)(b).begin();!j.finished();++j) {
    if (*j!=a)
      this->GetLinkEventsUndirected(b,*j,neighborEvents);
  }
  sort(neighborEvents.begin(), neighborEvents.end(),compare_event_time<EventType>);
 
}


template<class EventType>
void EventNet<EventType>::GetTauOnes(nodeindex a, nodeindex b, vector<timestamp> & tauOnes){
  vector<pair<timestamp,size_t> > neighborEvents;
  neighborEvents.clear();
  this->GetNeighborEvents(a,b,neighborEvents);

  vector<timestamp> abTimes;
  this->GetLinkEventTimesUndirected(a, b, abTimes);
  pVector<timestamp> abTimesPeriodic(abTimes,this->eventList.totalTime-this->eventList.startTime);

  int nextEventIndex=0;
  for (int i=0;i<neighborEvents.size();i++){
    timestamp neighborTime=neighborEvents[i].first;
    while(abTimesPeriodic[nextEventIndex]<neighborTime){
      nextEventIndex++;
    }
    tauOnes.push_back(abTimesPeriodic[nextEventIndex]-neighborTime);
  }
}


template<class EventType>
float EventNet<EventType>::GetAvgTauOne(nodeindex a, nodeindex b){
  float sum=0;
  vector<timestamp> tauOnes;
  this->GetTauOnes(a,b,tauOnes);
  for (int i=0;i<tauOnes.size();i++)
    sum+=tauOnes[i];
  return sum/float(tauOnes.size());
}
 

template<class EventType>
float EventNet<EventType>::GetAvgTauZero(nodeindex a, nodeindex b){
  vector<timestamp> iets;
  vector<size_t> eventIndices; //we dont need this
  this->GetInterEventTimes(a, b, iets, eventIndices);

  float tsum=0.0;
  float ttsum=0.0;
  for (vector<timestamp>::const_iterator i=iets.begin();i!=iets.end();++i){
    float iet=*i;
    tsum=tsum+iet;
    ttsum=ttsum+iet*iet;
  }
  return ttsum/2.0/tsum;
}



template<class EventType>
float EventNet<EventType>::GetAvgTauInfApprox(nodeindex a, nodeindex b, long * iets){
  float weighedSum=0;
  float ietSum=0;

  vector<pair<timestamp,size_t> > neighborEvents;
  neighborEvents.clear();
  this->GetNeighborEvents(a,b,neighborEvents);

  vector<timestamp> abTimes;
  this->GetLinkEventTimesUndirected(a, b, abTimes);
  pVector<timestamp> abTimesPeriodic(abTimes,this->eventList.totalTime-this->eventList.startTime);

  int nextEventIndex=0;
  for (int i=0;i<neighborEvents.size();i++){
    timestamp neighborTime=neighborEvents[i].first;
    while(abTimesPeriodic[nextEventIndex]<neighborTime){
      nextEventIndex++;
    }
    timestamp tauOne=abTimesPeriodic[nextEventIndex]-neighborTime;
    long iet=iets[neighborEvents[i].second];
    weighedSum+=tauOne*iet;
    ietSum+=iet;
  }

  return weighedSum/ietSum;
}


template<class EventType>
float EventNet<EventType>::GetAvgTauInfApprox(nodeindex a, nodeindex b, long * iets, SymmNet<unsigned> & aggNet){
  float weighedSum=0;
  float ietSum=0;

  vector<pair<EventType,size_t> > neighborEvents;
  neighborEvents.clear();
  this->GetNeighborEvents(a,b,neighborEvents);

  vector<timestamp> abTimes;
  this->GetLinkEventTimesUndirected(a, b, abTimes);
  pVector<timestamp> abTimesPeriodic(abTimes,this->eventList.totalTime-this->eventList.startTime);

  int nextEventIndex=0;
  for (int i=0;i<neighborEvents.size();i++){
    timestamp neighborTime=neighborEvents[i].first.getTime();
    while(abTimesPeriodic[nextEventIndex]<neighborTime){
      nextEventIndex++;
    }
    timestamp tauOne=abTimesPeriodic[nextEventIndex]-neighborTime;
    long iet=iets[neighborEvents[i].second];
    float weight=aggNet[neighborEvents[i].first.getSource()][neighborEvents[i].first.getDest()]; //wrong!
    weighedSum+=tauOne*iet*weight;
    ietSum+=iet*weight;
  }

  return weighedSum/ietSum;
}


template<class EventType>
float EventNet<EventType>::GetAvgTauInfApprox_neighborhoodIet(nodeindex a, nodeindex b){
  float weighedSum=0;
  float ietSum=0;

  vector<timestamp> neighborEvents;
  neighborEvents.clear();
  this->GetNeighborEvents(a,b,neighborEvents);

  vector<timestamp> neighborIets;
  this->GetInterEventTimes(neighborEvents,neighborIets);

  vector<timestamp> abTimes;
  this->GetLinkEventTimesUndirected(a, b, abTimes);
  pVector<timestamp> abTimesPeriodic(abTimes,this->eventList.totalTime-this->eventList.startTime);

  int nextEventIndex=0;
  for (int i=0;i<neighborEvents.size();i++){
    timestamp neighborTime=neighborEvents[i];
    while(abTimesPeriodic[nextEventIndex]<neighborTime){
      nextEventIndex++;
    }
    timestamp tauOne=abTimesPeriodic[nextEventIndex]-neighborTime;
    long iet=neighborIets[i];
    weighedSum+=tauOne*iet;
    ietSum+=iet;
  }

  return weighedSum/ietSum;
}



#endif //EVENTNET
