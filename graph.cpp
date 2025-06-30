#include "graph.h"
#include <iostream>
#include <fstream>
#include <cstring>

Graph::Graph(const string& graph6) {
    int n = graph6[0] - 63;
    if (n < 0 || n > 62) {
        throw invalid_argument("Graph Constructor: only graph6 strings with "
            "n < 63 are supported");
    }
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
                u = 0;
            }
        }
    }
}

bool Graph::hasEdge(int u, int v) const {
    int n = getN();
    if (u < 0 || v < 0 || u >= n || v >= n || u == v) {
        return false;
    }
    return graph[u].count(v) > 0;
}


int Graph::getN() const {
    return graph.size();
}

const set<int>& Graph::neighborhood(int u) const {
    int n = getN();
    if (u < 0 || u >= n) {
        throw out_of_range("neighborhood: vertex index out of bounds");
    }
    return graph[u];
}

int Graph::deg(int u) const {
    return neighborhood(u).size();
}

string Graph::toGraph6() const {
    int n = getN();
    string out = string();
    out += static_cast<char>(n + 63);
    int u = 0;
    int v = 1;
    int powOfTwo, curChar;
    for (int i = 1; i <= (n*(n-1)/2 + 5)/6; ++i) {
        powOfTwo = 32;
        curChar = 63;
        for (int j = 0; j < 6; ++j) {
            if (v < n && hasEdge(u,v)) {
                curChar += powOfTwo;
            }
            powOfTwo /= 2;
            if (u < v - 1) {
                ++u;
            }
            else {
                ++v;
                u = 0;
            }
        }
        out += static_cast<char>(curChar);
    }
    return out;
}

string Graph::toCanonicalGraph6() const {
    string graph6 = toGraph6();

    ofstream tempIn("temp_input.g6");
    tempIn << graph6;
    tempIn.close();

    FILE* fp = popen("nauty-labelg -q temp_input.g6", "r");
    if (!fp) {
        throw runtime_error("toCanonicalGraph6: popen failed");
    }

    char buff[100];
    if (!fgets(buff, sizeof(buff), fp)) {
        pclose(fp);
        throw runtime_error("toCanonicalGraph6: fgets failed");
    }
    pclose(fp);

    buff[strcspn(buff, "\n")] = 0;
    return string(buff);
}
