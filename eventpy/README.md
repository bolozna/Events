The EventPy library
===================

This package contains basic classes and functions to analyse event-based temporal networks in Python. Most of this library is simply wrappers for the C++ libary eventc found in the parent directory.

For installation, see the README in the parent directory.


Test suite
----------

After installing the library you should run the test suite to see that everything is workign fine:

```
python -m eventpy.tests
```

You can also take a look at the test scripts to get an idea of how the library works.

Basics
------

Import the library
'''
>>> import eventpy
'''

To create a new event:

'''
>>> myEvent = eventpy.Event(0,1, time = 10)
'''

This creates an event between nodes 0 and 1 at time 10. The event doesn't have other variables, such as the duration, set at all and is internally represented in memory as an object that doesn't have these members. You can always check the type of an Event:

'''
>>> myEvent
Event(source: 1, dest: 2, time: 10, no reversing, no duration, no index)
>>> print myEvent
(1,2,10)
'''

Event lists can be constructed similarly:

'''
>>> myEventList = eventpy.EventList(time=True)
'''

This creates you an empty event list with size set to 0. You can now easily add events

```
>>> myEventList.AddEvent(myEvent)
```

or by using the []-operator, in which case you must make sure to resize the eventlist first:

```
>>> myEventList.SetSize(2)
>>> myEventList[1]=myEvent
```

Note that the Event type and the EventList types much match each other. You can get information about the EventList and its type similarly to the Event objects:

```
>>> myEventList
EventList<Event type: (source,dest,time), size: 2>
>>> print myEventList
EventList[ (0,1,10), (0,1,10), ]
```

Note that the events are always COPIED to the event lists, so changing the event object doesn't change the event list:

'''
>>> myEvent.source=2
>>> print myEvent
(2,1,10)
>>> print myEventList
EventList[ (0,1,10), (0,1,10), ]
'''

You can of course modify the event list later as expected:

'''
>>> myEventList[0].source = 3
>>> eventInList = myEventList[1]
>>> eventInList.time = 10
>>> print myEventList
EventList[ (3,1,10), (0,1,10), ]
'''



