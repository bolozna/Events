"""Module for calculating various features of temporal networks.
"""
import networkx

def get_static_network(elist):
    """Returns unweighted aggregated network network
    """
    g=networkx.Graph()
    for event in elist:
        g.add_edge(event.source,event.dest)

    return g

