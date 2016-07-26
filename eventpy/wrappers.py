from eventpy import _module

def _get_type_str(time,reversed,duration,index):
    s=""
    if time: s+="T"
    if reversed: s+="R"
    if duration: s+="D"
    if index: s+="I"
    return s

def Event(source=0,dest=0,time=0,reversed=None,duration=None,index=None):
    event=eval("_module.Event_"+_get_type_str(time!=None,reversed!=None,duration!=None,index!=None))()
    event.source=source
    event.dest=dest
    if time!=None:
        event.setTime(time)
    if reversed!=None:
        event.setReversed(reversed)
    if duration!=None:
        event.setDuration(duration)
    if index!=None:
        event.setIndex(index)
    return event

def EventList(size=0,startTime=0,totalTime=0,time=True,reversed=False,duration=False,index=False):
    return eval("_module.EventList_"+_get_type_str(time,reversed,duration,index))(size,startTime,totalTime)

def EventNet(eventlist):
    assert eventlist.IsSorted_SourceDest(),"Event list must be sorted by user. Use, for example, Sort_SourceDest."
    cs=str(eventlist.__class__)
    typestr=cs[cs.find("EventList_"):].strip("EventList_").strip("'>")
    return eval("_module.EventNet_"+typestr+"(eventlist)")

#Adding methods to make the classes more Pythonic
def _EventList_iterusers(s):
    assert s.IsSorted_Source(),"Event list must be sorted by user. Use, for example, Sort_Source."
    thisUser=None
    userEvents=[]
    for event in s:
        if thisUser==None:
            thisUser=event.source

        if event.source==thisUser:
            userEvents.append(event)
        else:
            yield userEvents
            userEvents=[event]
            thisUser=event.source

    if len(userEvents)!=0:
        yield userEvents

def _patch_module():
    import itertools
    for typeargs in itertools.product(*(4*[(True,False)])):
        typestr=_get_type_str(*typeargs)
        exec("_module.EventList_"+typestr+".iterUsers=_EventList_iterusers")

_patch_module()

