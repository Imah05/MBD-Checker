#include "graph.h"

// constructor for the class Graph. Converts a string 
// (graph6 format) into a graph and stores it in this.
Graph::Graph(const string& graph6) {
    int n = graph6[0] - 63;

    graph = vector<set<int>>(n);

    int u = 0;
    int v = 1;
    int curChar;
    int powOfTwo = 32;
    for (int i = 1; i <= (n*(n-1)/2+5)/6; ++i) {
        powOfTwo = 32;
        curChar = graph6[i] - 63;
        for (int j = 0; j < 6; ++j) {
            if (curChar >= powOfTwo) {
                graph[u].insert(v);
                graph[v].insert(u);
                curChar -= powOfTwo;
            }
            powOfTwo /= 2;
            if (u < v - 1) {
                ++u;
            }
            else {
                ++v;
                u=0;
            }
        }
    }
}

// returns wheter the two vertices u and v are adjacent
bool Graph::hasEdge(int u, int v) const {
    return graph[u].count(v) > 0;
}

Graph& Graph::addEdge(int a, int b) const {
    Graph* newGraph = new Graph(*this);
    newGraph->graph[a].insert(b);
    newGraph->graph[b].insert(a);
    return *newGraph;
}

int Graph::getNumVertices() const {
    return graph.size();
}

const set<int>& Graph::neighborhood(int vtx) const {
    if (vtx >= 0 && vtx < graph.size()) {
        return graph[vtx];
    } else {
        throw out_of_range("Vertex index out of range.");
    }
}

// Graph to graph6 converter for testing
string Graph::toGraph6() const {
    int n = graph.size();
    string out = string();
    out += static_cast<char>(n + 63);
    int u = 0;
    int v = 1;
    int powOfTwo, curChar;
    for (int i = 1; i <= (n*(n-1)/2 + 5)/6; ++i) {
        powOfTwo = 32;
        curChar = 63;
        for (int j = 0; j < 6; ++j) {
            if (u < n && hasEdge(u,v)) {
                curChar += powOfTwo;
            }
            powOfTwo /= 2;
            if (u < v - 1) {
                ++u;
            }
            else {
                ++v;
                u=0;
            }
        }
        out += static_cast<char>(curChar);
    }
    return out;
}

// Prints n lines of integers, such that in the ith line
// all the vertices adjacent to vertex i-1 are listed.
// Here n is the number of vertices of the graph.
void Graph::printGraph() const {
    for (int i = 0; i < graph.size(); i++) {
        for (auto elem : graph[i]) {
            cout << elem << " ";
        }
        cout << endl;
    }
}
