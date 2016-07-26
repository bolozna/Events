%module eventpy_module

%include "documentation.i"

%include "exception.i"

#define nodeindex unsigned
#define eventindex unsigned
%include "carrays.i"
%include <std_string.i>
%{
#include "../eventc/events.h"
#include "../eventc/eventList.h"
#include "../eventc/eventNet.h"
static int exception_occured=0; 
%}

%include "../eventc/events.h"
%include "../eventc/eventList.h"
%include "../eventc/eventNet.h"

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


%template(Event_) Event<NoTimeStamp,NoReverse,NoDuration,NoIndex>;
%template(Event_T) Event<TimeStamp,NoReverse,NoDuration,NoIndex>;
%template(Event_TR) Event<TimeStamp,Reverse,NoDuration,NoIndex>;
%template(Event_TRD) Event<TimeStamp,Reverse,Duration,NoIndex>;
%template(Event_TRI) Event<TimeStamp,Reverse,NoDuration,Index>;
%template(Event_TRDI) Event<TimeStamp,Reverse,Duration,Index>;
%template(Event_TD) Event<TimeStamp,NoReverse,Duration,NoIndex>;
%template(Event_TDI) Event<TimeStamp,NoReverse,Duration,Index>;
%template(Event_TI) Event<TimeStamp,NoReverse,NoDuration,Index>;
%template(Event_R) Event<NoTimeStamp,Reverse,NoDuration,NoIndex>;
%template(Event_RD) Event<NoTimeStamp,Reverse,Duration,NoIndex>;
%template(Event_RI) Event<NoTimeStamp,Reverse,NoDuration,Index>;
%template(Event_RDI) Event<NoTimeStamp,Reverse,Duration,Index>;
%template(Event_D) Event<NoTimeStamp,NoReverse,Duration,NoIndex>;
%template(Event_DI) Event<NoTimeStamp,NoReverse,Duration,Index>;
%template(Event_I) Event<NoTimeStamp,NoReverse,NoDuration,Index>;

%define TEMPLATE_WRAP(T,postfix)
%ignore EventList<T>::DAG;
%template(EventList ## postfix) EventList<T>;
%template(EventNet ## postfix) EventNet<T>;


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



%array_class(long, longArray);


