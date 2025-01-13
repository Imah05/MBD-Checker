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

int mindeg = 3;

using namespace std;

class Core {
public:
    Core(Graph *g);
    void update();
    bool couldBeSWin();  
    char lowerBoundOutcome(char firstPlayer, int remainingGameTreeDepth);
            
private:
    Graph *core;       // pointer avoids expensive copying
    vector<bool> DVtx;  // DVtx[i] is true iff i is occupied by Dominator.
    vector<bool> SVtx;  // SVtx[i] is true iff i is occupied by Staller.
    vector<bool> highDegVtx;    // highDegVtx[i] is true iff i is a high degree vtx in the core.
    vector<int> worstDegSeq;    
    double upperBoundTotalPot;  // upper bound on total ES potential in any completion of core. 
    vector<int> remainingVtx;   // Vertices not claimed by Dominator nor Staller ordered by the corresponding
                                // value stored in potSeq.
};

#endif // CORE_H
