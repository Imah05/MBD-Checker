#ifndef FILTER_H
#define FILTER_H

#include<bits/stdc++.h>
#include "graph.h"
#include "core_game_state.h"
using namespace std;
#define NIL 0
#define INF INT_MAX


//Makes a single check if the core can be thrown out
bool matchingfilter(const int B, const int W,const int right3,const int surplus,const int* degreelist,const int* wantdegree,const Graph& gpaul);

//Applies filter number 2 to one g6-file containing a core
bool filter2(const string& g6);

//Applies filter and outdated filter
bool filter1old(const string& g6);

//Applies filter number 1 to one g6-file containing a core (or a graph?)
bool filter1(const string& g6);

// A class to represent Bipartite graph for Hopcroft
// Karp implementation
class BipGraph
{
    // m and n are number of vertices on left
    // and right sides of Bipartite Graph
    int m, n;
 
         // adj[u] stores adjacents of left side
    // vertex 'u'. The value of u ranges from 1 to m.
    // 0 is used for dummy vertex
    list<int> *adj;

 
    // These are basically pointers to arrays needed
    // for hopcroftKarp()
    int *pairU, *pairV, *dist;
 
public:
    BipGraph(int m, int n); // Constructor
    void addEdge(int u, int v); // To add edge
 
    // Returns true if there is an augmenting path
    bool bfs();
 
    // Adds augmenting path if there is one beginning
    // with u
    bool dfs(int u);
 
    // Returns size of maximum matching
    int hopcroftKarp();
    

    
};

#endif // FILTER_H
