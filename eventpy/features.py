"""Module for calculating various features of temporal networks.
"""
import networkx


class SequenceFeature(object):
    """Class for representing feature sequences and 
    calculating the statistics related to the feature
    sequence motifs.
    """
    def __init__(self,feature):
        self.feature=feature
    def avg(self):
        return sum(self.feature)/float(len(feature))
    def dist(self):
        d={}
        for element in self.feature:
            #some checks for immutability
            if isinstance(element,list):
                element=tuple(element)
            if isinstance(element,dict):
                element=tuple(sorted(element.items()))
            d[element]=d.get(element,0)+1
        return d
            
class SequenceOfSequencesFeature(object):
    def __init__(self,feature):
        self.feature=feature
    def _flat_seq():
        """Flatten (concatenate) all sequences."""
        pass
    def seq_of_dist(self):
        seqofdist=[]
        for seq in self.feature:
            seqofdist.append(SequenceFeature(seq).dist())
        return seqofdist
    def seq_of_avg(self):
        seqofavg=[]
        for seq in self.feature:
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
    

def get_static_network(elist):
    """Returns unweighted aggregated network network
    """
    g=networkx.Graph()
    for event in elist:
        g.add_edge(event.source,event.dest)

    return g

def get_degrees(elist):
    g=get_static_network(elist)
    return netowrkx.degree(g)

