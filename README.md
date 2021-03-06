Events library for analysing event-based networks
=================================================

This is a library for analysing large-scale event-based networks. Main parts of  the library are impelemented with C++, which are then wrapped (using Swig) to Python for convenient usage.

Installing and Requirements
---------------------------

This library relies on LCELib for static network analysis. It is included as a subrepository of Events, and to fetch it while cloning Events you should make sure that your Git client also fetches the subrepositories. You can do this by adding the "recursive" tag in the clone command:
```
git clone git@github.com:bolozna/Events.git --recursive
``` 

If you want to use the Python version of this library (called eventpy), then you should install it using the setup.py script provided:

```
python setup.py install
```

or

```
python setup.py install --user
```

if you don't have the rights to install packages in your computer for all users.

Currently only Python 2 is supported.

Basic concepts
--------------
This library consist of	three basic classes: `Event`, `EventList` and `EventNet`.

* Events work as basic units of temporal networks and always contain source and destination nodes in addition to optional information about the time of the event, duration of the event, index of the event and reverse status of the event. The index and reverse status are not actual data, but auxiliary information that is necessary for some types of analysis. 
* EventLists are just arrays of	Events with some additional information about the time window at which the event list takes place. EventList objects have some basic methods for analysing, sorting, shuffling, reading and writing.
* EventNets are aggregated network data structures. Each EventNet is associate to EventList and	each edge in an	EventNet points to an index in the EventList such that Events associated to that edge can be easily accessed. 


Using the library
-----------------

See documentation in the eventc and eventpy directories for the C++ and Python versions of this library.


