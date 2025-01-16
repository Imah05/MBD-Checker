#ifndef CORE_H
#define CORE_H

#include <set>
#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include "graph.h"


using namespace std;

class Core {
public:
    Core(Graph *core);
    bool filter(char firstPlayer);
    bool filterAfterLowDegMove(int vertex);
    void update();
            
private:
    Graph *core;        // pointer avoids expensive copying
    vector<bool> DVtx;  // DVtx[i] is true iff i is occupied by Dominator.
    vector<bool> SVtx;  // SVtx[i] is true iff i is occupied by Staller.
    vector<int> gameStateDegSeq;
    vector<int> lowDegVtx;
    double totalPot;  // upper bound on total ES potential in any completion of core. 
    vector<int> remainingVtx;   // Vertices not claimed by Dominator nor Staller ordered by the corresponding
                                // value stored in potSeq.
};

#endif // CORE_H
