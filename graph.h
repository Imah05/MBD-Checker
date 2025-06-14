#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <stdexcept>
#include <iostream>
#include <string>
#include <list>
#include <set>
#include <vector>
#include <fstream>  
#include <sstream>  
#include <cstdlib>


using namespace std;

// forward declaration for outcome, which has yet to be defined.
class GameState;

class Graph {
public:
    Graph(const string& graph6);
    bool hasEdge(int u, int v) const;
    Graph& addEdge(int a, int b) const;
    int getNumVertices() const;
    const set<int>& neighborhood(int vtx) const;
    string toGraph6() const;
    // TODO:
    string toCanonicalGraph6() const;
    void printGraph() const;
    // char outcome() const;

private:
    vector<set<int>> graph; // graph[i] contains all the vertices
                            // adjacent to vertex i.
};

#endif // GRAPH_H
