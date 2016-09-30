from __future__ import print_function
from eventpy import _module


def _get_type_str(time, reversed, duration, index, kind):
    s = ""
    if time:
        s += "T"
    if reversed:
        s += "R"
    if duration:
        s += "D"
    if index:
        s += "I"
    if kind:
        s += "K"
    return s


def Event(source=0, dest=0, time=0, reversed=None, duration=None, index=None, kind=None):
    event = eval("_module.Event_" + _get_type_str(time is not None,
                                                  reversed is not None,
                                                  duration is not None,
                                                  index is not None,
                                                  kind is not None))()
    event.source = source
    event.dest = dest
    if time is not None:
        event.setTime(time)
    if reversed is not None:
        event.setReversed(reversed)
    if duration is not None:
        event.setDuration(duration)
    if index is not None:
        event.setIndex(index)
    if kind is not None:
        event.setKind(index)
    return event


def EventList(size=0, startTime=0, totalTime=0, time=True, reversed=False, duration=False, index=False, kind=False):
    return eval("_module.EventList_" + _get_type_str(time, reversed, duration, index, kind))(size, startTime, totalTime)


def EventNet(eventlist):
    assert eventlist.IsSorted_SourceDest(), "Event list must be sorted by user. Use, for example, Sort_SourceDest."
    cs = str(eventlist.__class__)
    typestr = cs[cs.find("EventList_"):].strip("EventList_").strip("'>")
    return eval("_module.EventNet_" + typestr + "(eventlist)")


# Adding methods to make the classes more Pythonic
def _EventList_iterusers(s):
    assert s.IsSorted_Source(), "Event list must be sorted by user. Use, for example, Sort_Source."
    thisUser = None
    userEvents = []
    for event in s:
        if thisUser is None:
            thisUser = event.source

        if event.source == thisUser:
            userEvents.append(event)
        else:
            yield userEvents
            userEvents = [event]
            thisUser = event.source

    if len(userEvents) != 0:
        yield userEvents


def _patch_module():
    import itertools
    for typeargs in itertools.product(*(5 * [(True, False)])):
        typestr = _get_type_str(*typeargs)
        exec("_module.EventList_" + typestr + ".iterUsers=_EventList_iterusers")


_patch_module()
