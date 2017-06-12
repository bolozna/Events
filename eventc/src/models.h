#ifndef MODELS
#define MODELS

/* Function for creating instances of temporal network models.
 * 
 */

#include "event.h"
#include "../../../lcelib/Randgens.H"
#include "../../../lcelib/Nets.H"
#include "../../../lcelib/nets/NetExtras.H"
#include "../../../lcelib/nets/Randomizer.H"
#include "event_policy_classes/timeStampPolicies.h"

//Model where the size of the network is constrained but everything otherwise maximally random.
template<class EventType>
void Model_UniformlyRandomEvents(EventList<EventType> & elist,size_t nodes, size_t events,timestamp startTime, timestamp totalTime,int seed){

  elist.SetSize(0);
  elist.startTime=startTime;
  elist.totalTime=totalTime;

  RandNumGen<> rands(seed);

  for (size_t i=0;i<events;++i){
    timestamp offset = rands.next(totalTime-startTime);
    nodeindex source=0;
    nodeindex dest=0;
    while(source==dest){
      source = rands.next(nodes);
      dest = rands.next(nodes);
    }

    EventType event;
    event.source=source;
    event.dest=dest;
    event.setTime(startTime+offset);

    elist.AddEvent(event);
  }

}

#endif //MODELS
