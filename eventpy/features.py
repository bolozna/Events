"""Module for calculating various features of temporal networks.
"""
import networkx
import itertools

class SequenceFeature(object):
    """Class for representing feature sequences and 
    calculating the statistics related to the feature
    sequence motifs.
    """
    def __init__(self,feature):
        """Initialize a new sequence type feature.

        The feature can be either list or a dict.
        """
        assert isinstance(feature,list) or isinstance(feature,dict)
        
        self.feature=feature
        self.islist=isinstance(feature,list)        
    def _get_val_iterator(self):
        if self.islist:
            return self.feature
        else:
            return self.feature.itervalues()
    def avg(self):
        return sum(self._get_val_iterator())/float(len(feature))
    def dist(self):
        d={}
        for element in self._get_val_iterator():
            #some checks for immutability
            if isinstance(element,list):
                element=tuple(element)
            if isinstance(element,dict):
                element=tuple(sorted(element.items()))
            d[element]=d.get(element,0)+1
        return d
            
class SequenceOfSequencesFeature(SequenceFeature):
    def _flat_seq():
        """Flatten (concatenate) all sequences."""
        l=[]
        for seq in self._get_val_iterator():
            for element in seq._get_val_iterator():
                l.append(element)
        return l
    def seq_of_dist(self):
        seqofdist=[]
        for seq in self._get_val_iterator():
            seqofdist.append(SequenceFeature(seq).dist())
        return seqofdist
    def seq_of_avg(self):
        seqofavg=[]
        for seq in self._get_val_iterator():
            seqofavg.append(SequenceFeature(seq).avg())
        return seqofavg
    def dist_of_seq(self):
        return SequenceFeature(self.feature).dist()
    def dist_of_dist(self):
        return SequenceFeature(self.seq_of_dist()).dist()
    def dist(self):
        return SequenceFeature(self._flat_seq()).dist()
    def dist_of_avg(self):
        return SequenceFeature(self.seq_of_avg()).dist()
    def avg(self):
        return SequenceFeature(self._flat_seq()).avg()





# Topological features

def get_unweighted_network(elist):
    """Returns unweighted aggregated network network
    """
    g=networkx.Graph()
    for event in elist:
        g.add_edge(event.source,event.dest)

    return g    

def get_degrees(elist):
    g=get_unweighted_network(elist)
    return networkx.degree(g)

def get_node_ccs(elist):
    """Returns the clustering coefficients of nodes.
    """
    g=get_unweighted_network(elist)
    return networkx.clustering(g)

def get_number_of_links(elist):
    """Returns the number of links in the aggregated graph.
    """
    g=get_unweighted_network(elist)
    return g.number_of_edges()

def get_number_of_active_nodes(elist):
    """Returns the number of active nodes in the aggregated graph.
    """    
    g=get_unweighted_network(elist) #currently this doesn't include empty nodes
    return len(filter(lambda d:d>0,networkx.degree(g).itervalues()))

def is_connected(elist):
    """Checks if the network is connected.
    """
    g=get_unweighted_network(elist)
    return networkx.is_connected(g)


# Aggregated features

def get_weighted_network(elist):
    """Returns weighted aggregated network network.

    There are two weights:
    'number' is the count of events
    'duration' is the sum of durations of events
    """
    g=networkx.Graph()
    for event in elist:
        if (not event.source in g) or (not event.dest in g[event.source]):
            g.add_edge(event.source,event.dest,duration=0,number=0)
        g[event.source][event.dest]["duration"]+=event.getDuration()
        g[event.source][event.dest]["number"]+=1
            
    return g    

def get_number_of_contacts_on_links(elist):
    r={}
    g=get_weighted_network(elist)
    for edge in g.edges():
        edge=tuple(sorted(edge))
        r[edge]=g[edge[0]][edge[1]]["number"]
    return r

def get_total_duration_on_links(elist):
    r={}
    g=get_weighted_network(elist)
    for edge in g.edges():
        edge=tuple(sorted(edge))
        r[edge]=g[edge[0]][edge[1]]["duration"]
    return r

def get_number_of_contacts_on_nodes(elist):
    r={}
    g=get_weighted_network(elist)
    for node in g:
        r[node]=0
        for neigh in g[node]:
            r[node]+=g[node][neigh]["number"]
    return r

def get_total_duration_on_nodes(elist):
    r={}
    g=get_weighted_network(elist)
    for node in g:
        r[node]=0
        for neigh in g[node]:
            r[node]+=g[node][neigh]["duration"]
    return r



# Temporal features

def _get_first_times(seqs):
    r={}
    for seqid,times in seqs.iteritems():
        r[seqid]=times[0]
    return r

def _get_last_times(seqs):
    r={}
    for seqid,times in seqs.iteritems():
        r[seqid]=times[-1]
    return r

def _get_inter_event_times_seq(times,durations):
    r=[]
    last_time=None
    for time,duration in itertools.izip(times,durations):
        if last_time!=None:
            r.append(time-last_time)
        last_time=time+duration
    return r

def _get_inter_event_times_seqs_of_seqs(times,durations):
    r={}
    for seqid in times.iterkeys():
        r[seqid]=_get_inter_event_times_seq(times[seqid],durations[seqid])
    return r

# snapshots

def get_snapshots(elist):
    """Constructs the snapshot graphs.

    Warning! Ignores durations.
    """
    r={}
    for event in elist:
        if event.getTime() not in r:
            r[event.getTime()]=networkx.Graph()
        r[event.getTime()].add_edge(event.source,event.dest)
    return  r

def get_snapshot_degrees(elist):
    r={}
    for time,net in get_snapshots(elist).iteritems():
        r[time]=networkx.degree(net)
    return r

def get_snapshot_number_of_nodes(elist):
    r={}
    for time,net in get_snapshots(elist).iteritems():
        r[time]=net.number_of_nodes()
    return r    

def get_snapshot_number_of_links(elist):
    r={}
    for time,net in get_snapshots(elist).iteritems():
        r[time]=net.number_of_nodes()
    return r

# links
def get_times_links(elist):
    r={}
    for event in elist:
        edge=tuple(sorted([event.source,event.dest]))
        if edge not in r:
            r[edge]=[]
        r[edge].append(event.getTime())
    for val in r.itervalues():
        val.sort()
    return r

def get_durations_links(elist):
    timedur={}
    for event in elist:
        edge=tuple(sorted([event.source,event.dest]))
        if edge not in timedur:
            timedur[edge]=[]
        timedur[edge].append((event.getTime(),event.getDuration()))
    for val in timedur.itervalues():
        val.sort(key=lambda x:x[0])

    r={}
    for val,td in timedur.iteritems():
        r[val]=map(lambda x:x[1],td)
    return r
    
def get_first_times_links(elist):
    l=get_times_links(elist)
    return _get_first_times(l)

def get_last_times_links(elist):
    l=get_times_links(elist)
    return _get_first_times(l)

def get_inter_event_times_links(elist):
    times=get_times_links(elist)
    durations=get_durations_links(elist)
    return _get_inter_event_times_seqs_of_seqs(times,durations)



# nodes

def _get_times_durations_nodes(elist):
    timedur={}
    for event in elist:
        if event.source not in timedur:
            timedur[event.source]=[]
        if event.dest not in timedur:
            timedur[event.dest]=[]
            
        timedur[event.source].append((event.getTime(),event.getDuration()))
        timedur[event.dest].append((event.getTime(),event.getDuration()))

    for val in timedur.itervalues():
        val.sort(key=lambda x:x[0])

    times={}
    durations={}
    for val,td in timedur.iteritems():
        times[val]=map(lambda x:x[0],td)
        durations[val]=map(lambda x:x[1],td)
    return times,durations


def get_times_nodes(elist):
    times,durations=_get_times_durations_nodes(elist)
    return times

def get_durations_nodes(elist):
    times,durations=_get_times_durations_nodes(elist)
    return durations
    
def get_first_times_nodes(elist):
    l=get_times_nodes(elist)
    return _get_first_times(l)

def get_last_times_nodes(elist):
    l=get_times_nodes(elist)
    return _get_first_times(l)

def get_inter_event_times_nodes(elist):
    times,durations=_get_times_durations_nodes(elist)
    return _get_inter_event_times_seqs_of_seqs(times,durations)


features_all={}
feature_symbols={}

#topological
features_topological={"unweighted_network":get_unweighted_network,
                      "degrees":get_degrees,
                      "node_ccs":get_node_ccs,
                      "number_of_links":get_number_of_links,
                      "number_of_active_nodes":get_number_of_active_nodes,
                      "is_connected":is_connected}
features_all.update(features_topological)
feature_symbols.update({"unweighted_network": r"G_{stat}",
                        "degrees":r"k",
                        "node_ccs":r"c",
                        "number_of_links":r"M_{stat}",
                        "number_of_active_nodes":r"N^*_{stat}",
                        "is_connected":r"\lambda_1"})


#aggregated
features_aggregated={"number_of_contacts_on_links":get_number_of_contacts_on_links,
                     "total_duration_on_links":get_total_duration_on_links,
                     "number_of_contacts_on_nodes":get_number_of_contacts_on_nodes,
                     "total_duration_on_nodes":get_total_duration_on_nodes}
features_all.update(features_aggregated)
feature_symbols.update({"number_of_contacts_on_links":{r"n"},
                        "total_duration_on_links":r"w",
                        "number_of_contacts_on_nodes":r"a",
                        "total_duration_on_nodes":r"s"})


#temporal
features_temporal={"snapshots":get_snapshots,
                   "times_links":get_times_links,
                   "first_times_links":get_first_times_links,
                   "last_times_links":get_last_times_links,
                   "durations_links":get_durations_links,
                   "inter_event_times_links":get_inter_event_times_links,
                   "times_nodes":get_times_nodes,
                   "durations_nodes":get_durations_nodes,
                   "inter_event_times_nodes":get_inter_event_times_nodes,
                   "first_times_nodes":get_first_times_nodes,
                   "last_times_nodes":get_last_times_nodes,
                   "snapshot_degrees":get_snapshot_degrees,
                   "snapshot_number_of_links":get_snapshot_number_of_links,
                   "snapshot_number_of_nodes":get_snapshot_number_of_nodes}
features_all.update(features_temporal)
feature_symbols.update({"snapshots":r"G_t",
                        "times_links":r"\Theta",
                        "first_times_links":r"t_1",
                        "last_times_links":r"t_w",
                        "durations_links":r"\tau",
                        "inter_event_times_links":r"\Delta\tau",
                        "times_nodes":r"\Phi",
                        "durations_nodes":r"\alpha",
                        "inter_event_times_nodes":r"\Delta\alpha",
                        "first_times_nodes":r"t^n_1",
                        "last_times_nodes":r"t^n_w",
                        "snapshot_degrees":r"k^t",
                        "snapshot_number_of_links":r"M_t",
                        "snapshot_number_of_nodes":r"N^*_t"})

