from __future__ import print_function

import unittest
import eventpy
import os
import tempfile


class TestEvents(unittest.TestCase):

    def get_small_eventlist1(self):
        l = eventpy.EventList(5, startTime=0, totalTime=10, time=True)
        l[0] = eventpy.Event(0, 1, 1)
        l[1] = eventpy.Event(0, 2, 2)
        l[2] = eventpy.Event(0, 1, 4)
        l[3] = eventpy.Event(2, 0, 5)
        l[4] = eventpy.Event(0, 1, 7)
        return l

    def test_event_basics(self):
        e = eventpy.Event(source=0, dest=1, time=1, duration=2)
        self.assertEqual(e.source, 0)
        self.assertEqual(e.dest, 1)
        self.assertEqual(e.getTime(), 1)
        self.assertEqual(e.getDuration(), 2)

        e = eventpy.Event(source=0, dest=1, time=1, duration=2, reversed=True, index=0)
        self.assertEqual(e.source, 0)
        self.assertEqual(e.dest, 1)
        self.assertEqual(e.getTime(), 1)
        self.assertEqual(e.getDuration(), 2)
        self.assertEqual(e.isReversed(), True)
        self.assertEqual(e.getIndex(), 0)

    def test_eventlist_basics(self):
        l = eventpy.EventList(2, startTime=0, totalTime=10, duration=True)
        self.assertEqual(l.size, 2)
        self.assertEqual(len(l), 2)

        l[0].source = 0
        l[0].dest = 1
        l[0].setTime(1)
        l[0].setDuration(2)

        self.assertEqual(l[0].source, 0)
        self.assertEqual(l[0].dest, 1)
        self.assertEqual(l[0].getTime(), 1)
        self.assertEqual(l[0].getDuration(), 2)

        e = eventpy.Event(source=0, dest=1, time=1, duration=2)
        l[1] = e
        self.assertEqual(l[1].source, 0)
        self.assertEqual(l[1].dest, 1)
        self.assertEqual(l[1].getTime(), 1)
        self.assertEqual(l[1].getDuration(), 2)

        e2 = eventpy.Event(source=1, dest=2, time=2, duration=2)
        l.AddEvent(e2)

        self.assertEqual(l.size, 3)
        self.assertEqual(len(l), 3)

        self.assertEqual(l[2].source, 1)
        self.assertEqual(l[2].dest, 2)
        self.assertEqual(l[2].getTime(), 2)
        self.assertEqual(l[2].getDuration(), 2)

        # test that events are copied, not referenced
        e.source = 3
        self.assertEqual(l[1].source, 0)
        e2.source = 3
        self.assertEqual(l[2].source, 1)

    def test_eventlist_exceptions(self):
        l = eventpy.EventList(2, startTime=0, totalTime=10, duration=True)
        e = eventpy.Event(source=0, dest=1, time=1, duration=2)

        self.assertEqual(len(l), 1)
        self.assertRaises(Exception, lambda _: l[2])

        def sete(ind):
            l[ind] = e

        sete(1)
        self.assertRaises(Exception, lambda _: sete(2))
        sete(1)

    def test_eventlist_iterators(self):
        l = self.get_small_eventlist1()
        self.assertEqual(len(list(l)), 5)

        self.assertRaises(AssertionError, lambda: l.iterUsers().next())

        l.Sort_SourceDestTime()

        self.assertEqual(len(list(l.iterUsers())), 2)
        self.assertEqual(len(l.iterUsers().next()), 4)

    def test_eventlist_sorting(self):
        pass

    def test_eventnet_basics(self):
        l = self.get_small_eventlist1()
        self.assertRaises(AssertionError, lambda: eventpy.EventNet(l))

        l.Sort_SourceDestTime()
        n = eventpy.EventNet(l)

        n.GetAvgTauZero(0, 1)

    def test_eventlist_compare(self):
        l1 = self.get_small_eventlist1()
        l2 = self.get_small_eventlist1()

        self.assertEqual(l1, l2)

        l2_startTime = l2.startTime
        l2.startTime = 1
        self.assertNotEqual(l1, l2)

        l2.startTime = l2_startTime
        self.assertEqual(l1, l2)

        l1[2] = eventpy.Event(0, 1, 5)
        self.assertNotEqual(l1, l2)

    def test_eventlist_io(self):
        datadir = os.path.join(os.path.dirname(os.path.realpath(eventpy.__file__)), "data")
        small1_fn = os.path.join(datadir, "eventlist_small1_sdt.txt")
        small1 = eventpy.EventList(time=True)
        small1.ReadFile_SourceDestTime(small1_fn)

        small1_exp = self.get_small_eventlist1()
        self.assertNotEqual(small1, small1_exp)
        small1_exp.totalTime = 0
        small1_exp.startTime = 0

        self.assertEqual(small1, small1_exp)

        # write -> read -> see that the result is the same.
        tempdir = tempfile.mkdtemp()
        print(tempdir)

        small1_sdt_fn = os.path.join(tempdir, "small1_sdt.txt")
        small1.WriteFile_SourceDestTime(small1_sdt_fn)
        small1_rw = eventpy.EventList(time=True)
        small1_rw.ReadFile_SourceDestTime(small1_sdt_fn)
        self.assertEqual(small1_rw, small1)

        small1_tsd_fn = os.path.join(tempdir, "small1_tsd.txt")
        small1.WriteFile_TimeSourceDest(small1_tsd_fn)
        small1_rw = eventpy.EventList(time=True)
        small1_rw.ReadFile_TimeSourceDest(small1_tsd_fn)
        self.assertEqual(small1_rw, small1)

        small1_all_fn = os.path.join(tempdir, "small1_all.txt")
        small1.WriteFile_All(small1_all_fn)
        small1_rw = eventpy.EventList(time=True)
        small1_rw.ReadFile_All(small1_all_fn)
        self.assertEqual(small1_rw, small1)


def test_events():
    suite = unittest.TestSuite()
    suite.addTest(TestEvents("test_event_basics"))
    suite.addTest(TestEvents("test_eventlist_basics"))
    suite.addTest(TestEvents("test_eventlist_exceptions"))
    suite.addTest(TestEvents("test_eventlist_iterators"))
    suite.addTest(TestEvents("test_eventlist_compare"))
    suite.addTest(TestEvents("test_eventlist_io"))
    suite.addTest(TestEvents("test_eventnet_basics"))

    unittest.TextTestRunner().run(suite)


if __name__ == '__main__':
    test_events()
