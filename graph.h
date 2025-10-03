#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <set>


class Graph {
public:
    Graph(const std::string& graph6);
    bool hasEdge(int u, int v) const;

    int getN() const;
    const std::set<int>& neighborhood(int u) const;
    int deg(int u) const;

    std::string toGraph6() const;

protected:
    // graph[i] contains all the vertices adjacent to vertex i.
    std::vector<std::set<int>> graph; 
};

#endif // GRAPH_H
