#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <set>

using namespace std;

/* Supports basic graph operations and conversions to and from 
 * the graph6 format, as well as canonical labeling by envoking
 * nauty's labelg.
 */
class Graph {
public:
    Graph(const string& graph6);
    bool hasEdge(int u, int v) const;

    // Getter for the number of vertices
    int getN() const;
    const set<int>& neighborhood(int u) const;
    int deg(int u) const;

    string toGraph6() const;
    
    // Envokes nauty's labelg to generate a canoncial graph6 string
    // not depending on the labelling of the graph. Returns the same
    // string for isomorphic graphs. 
    string toCanonicalGraph6() const;

protected:
    // graph[i] contains all the vertices adjacent to vertex i.
    vector<set<int>> graph; 
};

#endif // GRAPH_H
