%module eventpy_module

%include "documentation.i"

%include "exception.i"

#define eventindex unsigned
%include "carrays.i"
%include <std_string.i>
%{
#include "../eventc/src/eventc.h"
static int exception_occured=0;
%}

%include "../eventc/src/event.h"
%include "../eventc/src/eventList.h"
%include "../eventc/src/eventNet.h"
%include "../eventc/src/event_policy_classes/durationPolicies.h"
%include "../eventc/src/event_policy_classes/timeStampPolicies.h"
%include "../eventc/src/event_policy_classes/reversePolicies.h"
%include "../eventc/src/event_policy_classes/timeStampPolicies.h"
%include "../eventc/src/event_policy_classes/indexPolicies.h"
%include "../eventc/src/event_policy_classes/kindPolicies.h"
%include "../eventc/src/dynamics/spreading.h"

//%rename(__str__) Event::getLongEventInformation;
%extend Event {
    string __repr__() {
         return $self->getLongEventInformation();
    }
};
%extend Event {
    string __str__() {
         return $self->getShortEventInformation();
    }
};





%template(Event_) Event<NoTimeStamp,NoReverse,NoDuration,NoIndex,NoKind>;
%template(Event_T) Event<TimeStamp,NoReverse,NoDuration,NoIndex,NoKind>;
%template(Event_TR) Event<TimeStamp,Reverse,NoDuration,NoIndex,NoKind>;
%template(Event_TRD) Event<TimeStamp,Reverse,Duration,NoIndex,NoKind>;
%template(Event_TRI) Event<TimeStamp,Reverse,NoDuration,Index,NoKind>;
%template(Event_TRDI) Event<TimeStamp,Reverse,Duration,Index,NoKind>;
%template(Event_TD) Event<TimeStamp,NoReverse,Duration,NoIndex,NoKind>;
%template(Event_TDI) Event<TimeStamp,NoReverse,Duration,Index,NoKind>;
%template(Event_TI) Event<TimeStamp,NoReverse,NoDuration,Index,NoKind>;
%template(Event_R) Event<NoTimeStamp,Reverse,NoDuration,NoIndex,NoKind>;
%template(Event_RD) Event<NoTimeStamp,Reverse,Duration,NoIndex,NoKind>;
%template(Event_RI) Event<NoTimeStamp,Reverse,NoDuration,Index,NoKind>;
%template(Event_RDI) Event<NoTimeStamp,Reverse,Duration,Index,NoKind>;
%template(Event_D) Event<NoTimeStamp,NoReverse,Duration,NoIndex,NoKind>;
%template(Event_DI) Event<NoTimeStamp,NoReverse,Duration,Index,NoKind>;
%template(Event_I) Event<NoTimeStamp,NoReverse,NoDuration,Index,NoKind>;
%template(Event_K) Event<NoTimeStamp,NoReverse,NoDuration,NoIndex,Kind>;
%template(Event_TK) Event<TimeStamp,NoReverse,NoDuration,NoIndex,Kind>;
%template(Event_TRK) Event<TimeStamp,Reverse,NoDuration,NoIndex,Kind>;
%template(Event_TRDK) Event<TimeStamp,Reverse,Duration,NoIndex,Kind>;
%template(Event_TRIK) Event<TimeStamp,Reverse,NoDuration,Index,Kind>;
%template(Event_TRDIK) Event<TimeStamp,Reverse,Duration,Index,Kind>;
%template(Event_TDK) Event<TimeStamp,NoReverse,Duration,NoIndex,Kind>;
%template(Event_TDIK) Event<TimeStamp,NoReverse,Duration,Index,Kind>;
%template(Event_TIK) Event<TimeStamp,NoReverse,NoDuration,Index,Kind>;
%template(Event_RK) Event<NoTimeStamp,Reverse,NoDuration,NoIndex,Kind>;
%template(Event_RDK) Event<NoTimeStamp,Reverse,Duration,NoIndex,Kind>;
%template(Event_RIK) Event<NoTimeStamp,Reverse,NoDuration,Index,Kind>;
%template(Event_RDIK) Event<NoTimeStamp,Reverse,Duration,Index,Kind>;
%template(Event_DK) Event<NoTimeStamp,NoReverse,Duration,NoIndex,Kind>;
%template(Event_DIK) Event<NoTimeStamp,NoReverse,Duration,Index,Kind>;
%template(Event_IK) Event<NoTimeStamp,NoReverse,NoDuration,Index,Kind>;

%define TEMPLATE_WRAP(T,postfix)
%ignore EventList<T>::DAG;
%template(EventList ## postfix) EventList<T>;
%template(EventNet ## postfix) EventNet<T>;
%template(InfectionProcess ## postfix) InfectionProcess<T>;

%exception EventList<T>::__getitem__{
 $action
 if (exception_occured){
  exception_occured=0;
  SWIG_exception(SWIG_IndexError,"event list index out of range.");
 }
}

%exception EventList<T>::__setitem__{
 $action
 if (exception_occured){
  exception_occured=0;
  SWIG_exception(SWIG_IndexError,"event list index out of range.");
 }
}

%extend EventList<T> {
    T &  __getitem__(size_t index) {
      	 if ($self->size<=index){exception_occured=1;}
         return $self->events[index];
    }
};

%extend EventList<T> {
    void  __setitem__(size_t index,T & event) {
      	 if ($self->size<=index){
		exception_occured=1;
	 }else{
		$self->events[index]=event;
	 }
    }
};


%extend EventList<T> {
    int  __len__() {
    	  return $self->size;
    }
};

%extend EventList<T> {
  bool __eq__(EventList<T> & other){
    return (*($self))==other;
  }
};

%extend EventList<T> {
    string  __str__() {
    	  return $self->GetLongEventListInformation();
    }
};

%extend EventList<T> {
    string  __repr__() {
    	  return $self->GetShortEventListInformation();
    }
};

%enddef

TEMPLATE_WRAP(Event_,_)
TEMPLATE_WRAP(Event_T,_T)
TEMPLATE_WRAP(Event_TR,_TR)
TEMPLATE_WRAP(Event_TRD,_TRD)
TEMPLATE_WRAP(Event_TRI,_TRI)
TEMPLATE_WRAP(Event_TRDI,_TRDI)
TEMPLATE_WRAP(Event_TD,_TD)
TEMPLATE_WRAP(Event_TDI,_TDI)
TEMPLATE_WRAP(Event_TI,_TI)
TEMPLATE_WRAP(Event_R,_R)
TEMPLATE_WRAP(Event_RD,_RD)
TEMPLATE_WRAP(Event_RI,_RI)
TEMPLATE_WRAP(Event_RDI,_RDI)
TEMPLATE_WRAP(Event_D,_D)
TEMPLATE_WRAP(Event_DI,_DI)
TEMPLATE_WRAP(Event_I,_I)

TEMPLATE_WRAP(Event_K,_K)
TEMPLATE_WRAP(Event_TK,_TK)
TEMPLATE_WRAP(Event_TRK,_TRK)
TEMPLATE_WRAP(Event_TRDK,_TRDK)
TEMPLATE_WRAP(Event_TRIK,_TRIK)
TEMPLATE_WRAP(Event_TRDIK,_TRDIK)
TEMPLATE_WRAP(Event_TDK,_TDK)
TEMPLATE_WRAP(Event_TDIK,_TDIK)
TEMPLATE_WRAP(Event_TIK,_TIK)
TEMPLATE_WRAP(Event_RK,_RK)
TEMPLATE_WRAP(Event_RDK,_RDK)
TEMPLATE_WRAP(Event_RIK,_RIK)
TEMPLATE_WRAP(Event_RDIK,_RDIK)
TEMPLATE_WRAP(Event_DK,_DK)
TEMPLATE_WRAP(Event_DIK,_DIK)
TEMPLATE_WRAP(Event_IK,_IK)



%array_class(long, longArray);


